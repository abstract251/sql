-- =====================================================
-- 更新存储过程以修复字段引用不明确的问题
-- =====================================================

-- 1. 更新 sp_get_ancestors
CREATE OR REPLACE FUNCTION sp_get_ancestors(p_person_id INT)
RETURNS TABLE (
    person_id INT, name VARCHAR(100), gender CHAR(1), birth_year INT,
    death_year INT, generation INT, level INT, path TEXT
) AS $$
BEGIN
    RETURN QUERY
    WITH RECURSIVE ancestry_cte AS (
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.death_year,
               p.generation, p.birth_family_id, 0 AS ancestor_level, p.person_id::TEXT AS path
        FROM persons p WHERE p.person_id = p_person_id
        UNION ALL
        SELECT parent.person_id, parent.name, parent.gender, parent.birth_year, parent.death_year,
               parent.generation, parent.birth_family_id, ac.ancestor_level + 1,
               ac.path || '->' || parent.person_id::TEXT
        FROM ancestry_cte ac
        JOIN families f ON ac.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE ac.ancestor_level < 50
          AND parent.person_id != ac.person_id
          AND position(parent.person_id::TEXT IN ac.path) = 0
    )
    SELECT 
        ac.person_id, 
        ac.name, 
        ac.gender, 
        ac.birth_year, 
        ac.death_year, 
        ac.generation, 
        ac.ancestor_level AS level, 
        ac.path
    FROM ancestry_cte ac 
    WHERE ac.ancestor_level > 0
    ORDER BY ac.ancestor_level, ac.generation;
END;
$$ LANGUAGE plpgsql;

-- 2. 更新 sp_get_descendants
CREATE OR REPLACE FUNCTION sp_get_descendants(p_person_id INT, max_depth INT DEFAULT 30)
RETURNS TABLE (
    person_id INT, name VARCHAR(100), gender CHAR(1), birth_year INT,
    death_year INT, generation INT, level INT, path TEXT
) AS $$ 
BEGIN 
    RETURN QUERY 
    WITH RECURSIVE descendants_cte AS ( 
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.death_year, 
               p.generation, 0 AS descendant_level, p.person_id::TEXT AS path 
        FROM persons p WHERE p.person_id = p_person_id 
        UNION ALL 
        SELECT child.person_id, child.name, child.gender, child.birth_year, child.death_year, 
               child.generation, dc.descendant_level + 1, 
               dc.path || '->' || child.person_id::TEXT 
        FROM descendants_cte dc 
        JOIN families f ON f.husband_id = dc.person_id OR f.wife_id = dc.person_id 
        JOIN persons child ON child.birth_family_id = f.family_id 
        WHERE dc.descendant_level < max_depth 
          AND position(child.person_id::TEXT IN dc.path) = 0 
    ) 
    SELECT 
        dc.person_id, 
        dc.name, 
        dc.gender, 
        dc.birth_year, 
        dc.death_year, 
        dc.generation, 
        dc.descendant_level AS level, 
        dc.path 
    FROM descendants_cte dc 
    WHERE dc.descendant_level > 0 
    ORDER BY dc.descendant_level, dc.birth_year; 
END; 
$$ LANGUAGE plpgsql;

-- 3. 测试更新后的存储过程
SELECT '存储过程更新完成!' AS status;
SELECT '现在可以测试祖先查询了...' AS info;
