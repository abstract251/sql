-- =====================================================
-- "寻根溯源"族谱管理系统 - 核心SQL查询
-- =====================================================

USE family_genealogy;

-- =====================================================
-- 1. 基本查询：给定成员ID，查询配偶及所有子女
-- =====================================================

DELIMITER //

DROP PROCEDURE IF EXISTS sp_get_spouse_and_children//

CREATE PROCEDURE sp_get_spouse_and_children(IN p_person_id INT)
BEGIN
    -- 临时表存储结果
    CREATE TEMPORARY TABLE IF NOT EXISTS temp_relations AS
    SELECT
        '本人' AS relation_type,
        person_id AS related_id,
        name AS related_name,
        gender,
        birth_year,
        generation
    FROM persons
    WHERE person_id = p_person_id;

    -- 查找配偶（作为丈夫或妻子）
    INSERT INTO temp_relations
    SELECT
        '配偶' AS relation_type,
        p.person_id,
        p.name,
        p.gender,
        p.birth_year,
        p.generation
    FROM families f
    JOIN persons p ON (
        (f.husband_id = p_person_id AND f.wife_id = p.person_id) OR
        (f.wife_id = p_person_id AND f.husband_id = p.person_id)
    )
    WHERE f.husband_id = p_person_id OR f.wife_id = p_person_id;

    -- 查找所有子女（通过出生家庭关联）
    INSERT INTO temp_relations
    SELECT DISTINCT
        '子女' AS relation_type,
        p.person_id,
        p.name,
        p.gender,
        p.birth_year,
        p.generation
    FROM persons p
    WHERE p.birth_family_id IN (
        SELECT family_id FROM families
        WHERE husband_id = p_person_id OR wife_id = p_person_id
    );

    -- 返回结果
    SELECT * FROM temp_relations;

    -- 清理
    DROP TEMPORARY TABLE IF EXISTS temp_relations;
END//

DELIMITER ;

-- 调用示例
-- CALL sp_get_spouse_and_children(1);


-- =====================================================
-- 2. 递归查询：向上追溯所有祖先（使用Recursive CTE）
-- 输入成员A的ID，输出其向上追溯的所有历代祖先
-- =====================================================

DELIMITER //

DROP PROCEDURE IF EXISTS sp_get_ancestors//

CREATE PROCEDURE sp_get_ancestors(IN p_person_id INT)
BEGIN
    WITH RECURSIVE ancestry_cte AS (
        -- 基础查询：起始成员
        SELECT
            p.person_id,
            p.name,
            p.gender,
            p.birth_year,
            p.death_year,
            p.generation,
            0 AS ancestor_level,
            CAST(p.person_id AS CHAR(1000)) AS path
        FROM persons p
        WHERE p.person_id = p_person_id

        UNION ALL

        -- 递归：找父母
        SELECT
            parent.person_id,
            parent.name,
            parent.gender,
            parent.birth_year,
            parent.death_year,
            parent.generation,
            ac.ancestor_level + 1,
            CONCAT(ac.path, '->', parent.person_id)
        FROM ancestry_cte ac
        JOIN families f ON ac.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE parent.person_id != ac.person_id
          AND ac.ancestor_level < 50
          AND NOT FIND_IN_SET(parent.person_id, ac.path)
    )
    SELECT
        person_id,
        name,
        gender,
        birth_year,
        death_year,
        generation,
        ancestor_level AS 代际层级,
        path AS 亲缘路径
    FROM ancestry_cte
    WHERE ancestor_level > 0
    ORDER BY ancestor_level, generation;
END//

DELIMITER ;

-- 直接使用CTE的查询语句
/*
WITH RECURSIVE ancestry_cte AS (
    SELECT
        person_id,
        name,
        gender,
        birth_year,
        death_year,
        generation,
        birth_family_id,
        0 AS ancestor_level,
        CAST(person_id AS CHAR(1000)) AS path
    FROM persons
    WHERE person_id = ?

    UNION ALL

    SELECT
        parent.person_id,
        parent.name,
        parent.gender,
        parent.birth_year,
        parent.death_year,
        parent.generation,
        parent.birth_family_id,
        ac.ancestor_level + 1,
        CONCAT(ac.path, '->', parent.person_id)
    FROM ancestry_cte ac
    JOIN families f ON ac.birth_family_id = f.family_id
    JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
    WHERE ac.ancestor_level < 50
      AND NOT FIND_IN_SET(parent.person_id, ac.path)
)
SELECT * FROM ancestry_cte WHERE ancestor_level > 0 ORDER BY ancestor_level;
*/


-- =====================================================
-- 3. 递归查询：向下追溯所有后代
-- =====================================================

DELIMITER //

DROP PROCEDURE IF EXISTS sp_get_descendants//

CREATE PROCEDURE sp_get_descendants(IN p_person_id INT, IN max_depth INT)
BEGIN
    SET max_depth = IFNULL(max_depth, 10);

    WITH RECURSIVE descendants_cte AS (
        -- 基础查询：起始成员
        SELECT
            p.person_id,
            p.name,
            p.gender,
            p.birth_year,
            p.death_year,
            p.generation,
            0 AS descendant_level,
            CAST(p.person_id AS CHAR(1000)) AS path
        FROM persons p
        WHERE p.person_id = p_person_id

        UNION ALL

        -- 递归：找子女
        SELECT
            child.person_id,
            child.name,
            child.gender,
            child.birth_year,
            child.death_year,
            child.generation,
            dc.descendant_level + 1,
            CONCAT(dc.path, '->', child.person_id)
        FROM descendants_cte dc
        JOIN families f ON dc.person_id = f.husband_id OR dc.person_id = f.wife_id
        JOIN parent_child pc ON f.family_id = pc.family_id
        JOIN persons child ON pc.child_id = child.person_id
        WHERE dc.descendant_level < max_depth
          AND NOT FIND_IN_SET(child.person_id, dc.path)
    )
    SELECT
        person_id,
        name,
        gender,
        birth_year,
        death_year,
        generation,
        descendant_level AS 后代层级,
        path AS 亲缘路径
    FROM descendants_cte
    WHERE descendant_level > 0
    ORDER BY descendant_level, birth_year;
END//

DELIMITER ;


-- =====================================================
-- 4. 统计分析：平均寿命最长的一代人（辈分）
-- =====================================================

SELECT
    generation AS 辈分,
    COUNT(*) AS 人数,
    ROUND(AVG(death_year - birth_year), 2) AS 平均寿命,
    MIN(death_year - birth_year) AS 最短寿命,
    MAX(death_year - birth_year) AS 最长寿命
FROM persons
WHERE death_year IS NOT NULL
  AND birth_year IS NOT NULL
  AND death_year >= birth_year
GROUP BY generation
ORDER BY 平均寿命 DESC
LIMIT 5;


-- =====================================================
-- 5. 查询所有年龄超过50岁且没有配偶的男性成员
-- 假设当前日期减去出生日期 > 50岁，且没有在families表中出现
-- =====================================================

SELECT
    p.person_id,
    p.name,
    p.gender,
    p.birth_year,
    p.death_year,
    p.generation,
    p.genealogy_id,
    g.name AS genealogy_name,
    CASE
        WHEN p.death_year IS NULL THEN FLOOR(DATEDIFF(CURDATE(), STR_TO_DATE(CONCAT(p.birth_year, '-01-01'), '%Y-%m-%d')) / 365.25)
        ELSE p.death_year - p.birth_year
    END AS age
FROM persons p
JOIN genealogies g ON p.genealogy_id = g.genealogy_id
WHERE p.gender = 'M'
  AND p.birth_year <= YEAR(CURDATE()) - 50
  AND p.death_year IS NULL
  AND NOT EXISTS (
      SELECT 1 FROM families f
      WHERE f.husband_id = p.person_id
  )
ORDER BY p.birth_year;


-- =====================================================
-- 6. 找出出生年份早于该辈分平均出生年份的所有成员
-- =====================================================

WITH generation_avg_birth AS (
    SELECT
        generation,
        AVG(birth_year) AS avg_birth_year
    FROM persons
    WHERE birth_year IS NOT NULL
    GROUP BY generation
)
SELECT
    p.person_id,
    p.name,
    p.gender,
    p.birth_year,
    p.generation,
    p.genealogy_id,
    g.name AS genealogy_name,
    gab.avg_birth_year AS 该辈平均出生年,
    p.birth_year - gab.avg_birth_year AS 早于平均年份
FROM persons p
JOIN genealogies g ON p.genealogy_id = g.genealogy_id
JOIN generation_avg_birth gab ON p.generation = gab.generation
WHERE p.birth_year < gab.avg_birth_year
ORDER BY p.generation, p.birth_year;


-- =====================================================
-- 7. 亲缘关系查询：查询两个人之间的亲缘路径
-- 使用双向BFS算法思想
-- =====================================================

DELIMITER //

DROP PROCEDURE IF EXISTS sp_find_relationship//

CREATE PROCEDURE sp_find_relationship(IN person1_id INT, IN person2_id INT)
BEGIN
    DECLARE EXIT HANDLER FOR NOT FOUND
    BEGIN
        SELECT '未找到亲缘关系' AS result;
    END;

    -- 临时表用于存储探查路径
    CREATE TEMPORARY TABLE IF NOT EXISTS ancestors1 (
        person_id INT,
        generation INT,
        path VARCHAR(1000)
    );

    CREATE TEMPORARY TABLE IF NOT EXISTS ancestors2 (
        person_id INT,
        generation INT,
        path VARCHAR(1000)
    );

    -- 查找person1的所有祖先
    WITH RECURSIVE anc1_cte AS (
        SELECT
            person_id,
            birth_family_id,
            generation,
            CAST(person_id AS CHAR(1000)) AS path,
            0 AS level
        FROM persons
        WHERE person_id = person1_id

        UNION ALL

        SELECT
            parent.person_id,
            parent.birth_family_id,
            parent.generation,
            CONCAT(ac.path, '->', parent.person_id),
            ac.level + 1
        FROM anc1_cte ac
        JOIN families f ON ac.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE ac.level < 50
          AND NOT FIND_IN_SET(parent.person_id, ac.path)
    )
    INSERT INTO ancestors1
    SELECT person_id, generation, path FROM anc1_cte;

    -- 查找person2的所有祖先
    WITH RECURSIVE anc2_cte AS (
        SELECT
            person_id,
            birth_family_id,
            generation,
            CAST(person_id AS CHAR(1000)) AS path,
            0 AS level
        FROM persons
        WHERE person_id = person2_id

        UNION ALL

        SELECT
            parent.person_id,
            parent.birth_family_id,
            parent.generation,
            CONCAT(ac.path, '->', parent.person_id),
            ac.level + 1
        FROM anc2_cte ac
        JOIN families f ON ac.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE ac.level < 50
          AND NOT FIND_IN_SET(parent.person_id, ac.path)
    )
    INSERT INTO ancestors2
    SELECT person_id, generation, path FROM anc2_cte;

    -- 查找最近公共祖先
    SELECT
        a1.person_id AS common_ancestor_id,
        p.name AS common_ancestor_name,
        p.generation,
        a1.path AS path_to_person1,
        a2.path AS path_to_person2,
        CONCAT(a1.path, ' <-公共祖先-> ', a2.path) AS full_path
    FROM ancestors1 a1
    JOIN ancestors2 a2 ON a1.person_id = a2.person_id
    JOIN persons p ON a1.person_id = p.person_id
    ORDER BY p.generation DESC
    LIMIT 1;

    -- 清理
    DROP TEMPORARY TABLE IF EXISTS ancestors1;
    DROP TEMPORARY TABLE IF EXISTS ancestors2;
END//

DELIMITER ;

-- 直接使用CTE查找亲缘关系
/*
WITH RECURSIVE
anc1 AS (
    SELECT person_id, birth_family_id, generation, CAST(person_id AS CHAR(1000)) AS path, 0 AS level
    FROM persons WHERE person_id = ?

    UNION ALL

    SELECT parent.person_id, parent.birth_family_id, parent.generation,
           CONCAT(anc1.path, '->', parent.person_id), anc1.level + 1
    FROM anc1
    JOIN families f ON anc1.birth_family_id = f.family_id
    JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
    WHERE anc1.level < 50
),
anc2 AS (
    SELECT person_id, birth_family_id, generation, CAST(person_id AS CHAR(1000)) AS path, 0 AS level
    FROM persons WHERE person_id = ?

    UNION ALL

    SELECT parent.person_id, parent.birth_family_id, parent.generation,
           CONCAT(anc2.path, '->', parent.person_id), anc2.level + 1
    FROM anc2
    JOIN families f ON anc2.birth_family_id = f.family_id
    JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
    WHERE anc2.level < 50
)
SELECT
    a1.person_id AS common_ancestor_id,
    p.name AS common_ancestor_name,
    a1.path AS path_to_person1,
    a2.path AS path_to_person2
FROM anc1 a1
JOIN anc2 a2 ON a1.person_id = a2.person_id
JOIN persons p ON a1.person_id = p.person_id
ORDER BY p.generation DESC
LIMIT 1;
*/


-- =====================================================
-- 8. 索引性能测试：查询某曾祖父的所有曾孙（四代查询）
-- =====================================================

-- 有索引版本（正常执行）
EXPLAIN ANALYZE
WITH RECURSIVE ancestors_cte AS (
    SELECT person_id, birth_family_id, generation
    FROM persons
    WHERE person_id = 1

    UNION ALL

    SELECT parent.person_id, parent.birth_family_id, parent.generation
    FROM ancestors_cte ac
    JOIN families f ON ac.birth_family_id = f.family_id
    JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
    WHERE ac.generation > 1
),
great_grandfather AS (
    SELECT person_id FROM ancestors_cte WHERE generation = 1
),
descendants_cte AS (
    SELECT person_id, birth_family_id, generation
    FROM persons p
    WHERE p.birth_family_id IN (
        SELECT f.family_id FROM families f
        WHERE f.husband_id IN (SELECT person_id FROM great_grandfather)
           OR f.wife_id IN (SELECT person_id FROM great_grandfather)
    )

    UNION ALL

    SELECT child.person_id, child.birth_family_id, child.generation
    FROM descendants_cte dc
    JOIN families f ON dc.person_id = f.husband_id OR dc.person_id = f.wife_id
    JOIN parent_child pc ON f.family_id = pc.family_id
    JOIN persons child ON pc.child_id = child.person_id
    WHERE dc.generation < 5
)
SELECT DISTINCT person_id, generation FROM descendants_cte WHERE generation = 4;


-- =====================================================
-- 9. 模糊查找测试：查找姓张的所有成员
-- =====================================================

EXPLAIN ANALYZE
SELECT person_id, name, gender, birth_year, generation, genealogy_id
FROM persons
WHERE name LIKE '%张%'
ORDER BY name, birth_year;


-- =====================================================
-- 10. 同族谱内辈分统计
-- =====================================================

SELECT
    g.name AS 族谱名,
    p.generation AS 辈分,
    COUNT(*) AS 人数,
    ROUND(AVG(p.death_year - p.birth_year), 2) AS 平均寿命,
    MIN(p.birth_year) AS 最早出生,
    MAX(p.birth_year) AS 最晚出生
FROM persons p
JOIN genealogies g ON p.genealogy_id = g.genealogy_id
WHERE p.death_year IS NOT NULL AND p.birth_year IS NOT NULL
GROUP BY g.genealogy_id, p.generation
ORDER BY g.name, p.generation;


-- =====================================================
-- 11. 族谱男女比例统计
-- =====================================================

SELECT
    g.name AS 族谱名,
    g.genealogy_id,
    COUNT(*) AS 总人数,
    SUM(CASE WHEN p.gender = 'M' THEN 1 ELSE 0 END) AS 男性人数,
    SUM(CASE WHEN p.gender = 'F' THEN 1 ELSE 0 END) AS 女性人数,
    ROUND(SUM(CASE WHEN p.gender = 'M' THEN 1 ELSE 0 END) * 100.0 / COUNT(*), 2) AS 男性比例,
    ROUND(SUM(CASE WHEN p.gender = 'F' THEN 1 ELSE 0 END) * 100.0 / COUNT(*), 2) AS 女性比例
FROM persons p
JOIN genealogies g ON p.genealogy_id = g.genealogy_id
GROUP BY g.genealogy_id, g.name;
