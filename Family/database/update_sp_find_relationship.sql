-- 更新 sp_find_relationship 存储过程
-- 注意：必须先删除旧函数，因为返回类型改变了

-- 先删除旧函数
DROP FUNCTION IF EXISTS sp_find_relationship(INT, INT);

-- 重新创建更新后的函数
CREATE OR REPLACE FUNCTION sp_find_relationship(person1_id INT, person2_id INT)
RETURNS TABLE (
    common_ancestor_id INT, common_ancestor_name VARCHAR(100),
    generation INT, path_to_person1 TEXT, path_to_person2 TEXT,
    relationship_type TEXT
) AS $$
DECLARE
    p1_birth_family INT;
    p2_birth_family INT;
    p1_name VARCHAR(100);
    p2_name VARCHAR(100);
BEGIN
    SELECT name, birth_family_id INTO p1_name, p1_birth_family
    FROM persons WHERE person_id = person1_id;
    
    SELECT name, birth_family_id INTO p2_name, p2_birth_family
    FROM persons WHERE person_id = person2_id;
    
    IF p1_name IS NULL OR p2_name IS NULL THEN
        RETURN;
    END IF;
    
    -- Check for spouse relationship
    IF EXISTS (
        SELECT 1 FROM families 
        WHERE (husband_id = person1_id AND wife_id = person2_id)
           OR (husband_id = person2_id AND wife_id = person1_id)
    ) THEN
        RETURN QUERY SELECT 0, '婚姻关系'::VARCHAR(100), 0, 
                            p1_name::TEXT, p2_name::TEXT, '夫妻'::TEXT;
        RETURN;
    END IF;
    
    -- Check for parent-child relationship (person1 is parent of person2)
    IF EXISTS (
        SELECT 1 FROM families f
        WHERE f.family_id = p2_birth_family
          AND (f.husband_id = person1_id OR f.wife_id = person1_id)
    ) THEN
        RETURN QUERY SELECT person1_id, p1_name::VARCHAR(100), 0,
                            p1_name::TEXT, p1_name::TEXT || '->' || p2_name::TEXT,
                            CASE WHEN (SELECT husband_id FROM families WHERE family_id = p2_birth_family) = person1_id 
                                 THEN '父女/父子'::TEXT 
                                 ELSE '母女/母子'::TEXT END;
        RETURN;
    END IF;
    
    -- Check for parent-child relationship (person2 is parent of person1)
    IF EXISTS (
        SELECT 1 FROM families f
        WHERE f.family_id = p1_birth_family
          AND (f.husband_id = person2_id OR f.wife_id = person2_id)
    ) THEN
        RETURN QUERY SELECT person2_id, p2_name::VARCHAR(100), 0,
                            p2_name::TEXT || '->' || p1_name::TEXT, p2_name::TEXT,
                            CASE WHEN (SELECT husband_id FROM families WHERE family_id = p1_birth_family) = person2_id 
                                 THEN '父女/父子'::TEXT 
                                 ELSE '母女/母子'::TEXT END;
        RETURN;
    END IF;
    
    -- Check for sibling relationship (same birth_family_id)
    IF p1_birth_family IS NOT NULL AND p1_birth_family = p2_birth_family THEN
        RETURN QUERY SELECT 0, '同父母'::VARCHAR(100), 0,
                            p1_name::TEXT, p2_name::TEXT, '兄弟姐妹'::TEXT;
        RETURN;
    END IF;
    
    -- Find common ancestor using recursive CTE
    RETURN QUERY
    WITH RECURSIVE
    anc1 AS (
        SELECT person_id, birth_family_id, generation, person_id::TEXT AS path, 0 AS level
        FROM persons WHERE person_id = person1_id
        UNION ALL
        SELECT parent.person_id, parent.birth_family_id, parent.generation,
               anc1.path || '->' || parent.person_id::TEXT, anc1.level + 1
        FROM anc1
        JOIN families f ON anc1.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE anc1.level < 50
    ),
    anc2 AS (
        SELECT person_id, birth_family_id, generation, person_id::TEXT AS path, 0 AS level
        FROM persons WHERE person_id = person2_id
        UNION ALL
        SELECT parent.person_id, parent.birth_family_id, parent.generation,
               anc2.path || '->' || parent.person_id::TEXT, anc2.level + 1
        FROM anc2
        JOIN families f ON anc2.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE anc2.level < 50
    )
    SELECT a1.person_id, p.name, p.generation, a1.path, a2.path, '共同祖先'::TEXT
    FROM anc1 a1
    JOIN anc2 a2 ON a1.person_id = a2.person_id
    JOIN persons p ON a1.person_id = p.person_id
    ORDER BY p.generation DESC
    LIMIT 1;
END;
$$ LANGUAGE plpgsql;

-- 提示信息
SELECT 'sp_find_relationship 存储过程已成功更新！' AS message;
