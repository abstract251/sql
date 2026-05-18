-- =====================================================
-- "寻根溯源"族谱管理系统 - 实验结果展示查询脚本
-- 请按顺序执行以下查询，用于生成实验报告中的结果截图
-- =====================================================

SET client_encoding TO 'UTF8';

-- =====================================================
-- 查询1: 显示数据库基本统计信息
-- =====================================================
SELECT 
    '用户数' AS "指标", COUNT(*) AS "数值" FROM users
UNION ALL
SELECT '族谱数', COUNT(*) FROM genealogies
UNION ALL
SELECT '成员数', COUNT(*) FROM persons
UNION ALL
SELECT '家庭数', COUNT(*) FROM families
UNION ALL
SELECT '用户-族谱关系', COUNT(*) FROM user_genealogy;

-- =====================================================
-- 查询2: 显示前5个族谱及其基本信息
-- =====================================================
SELECT 
    genealogy_id AS "族谱ID",
    name AS "谱名",
    surname AS "姓氏",
    compile_time AS "修谱时间",
    creator_id AS "创建者ID"
FROM genealogies
ORDER BY genealogy_id
LIMIT 5;

-- =====================================================
-- 查询3: 显示各族谱的成员数统计
-- =====================================================
SELECT 
    g.genealogy_id AS "族谱ID",
    g.name AS "谱名",
    COUNT(p.person_id) AS "成员数",
    MIN(p.generation) AS "起始代",
    MAX(p.generation) AS "最大代",
    MAX(p.generation) - MIN(p.generation) + 1 AS "代数"
FROM genealogies g
LEFT JOIN persons p ON g.genealogy_id = p.genealogy_id
GROUP BY g.genealogy_id, g.name
ORDER BY g.genealogy_id;

-- =====================================================
-- 查询4: 展示一个族谱（比如ID=1）中前10个成员
-- =====================================================
SELECT 
    person_id AS "成员ID",
    name AS "姓名",
    CASE gender WHEN 'M' THEN '男' ELSE '女' END AS "性别",
    birth_year AS "出生年份",
    death_year AS "卒年",
    generation AS "辈分",
    birth_family_id AS "出生家庭ID"
FROM persons
WHERE genealogy_id = 1
ORDER BY generation, birth_year
LIMIT 10;

-- =====================================================
-- 查询5: 展示成员的性别比例统计
-- =====================================================
SELECT 
    '男性' AS "性别", COUNT(*) AS "人数", 
    ROUND(COUNT(*) * 100.0 / (SELECT COUNT(*) FROM persons), 2) AS "占比(%)"
FROM persons WHERE gender = 'M'
UNION ALL
SELECT 
    '女性', COUNT(*), 
    ROUND(COUNT(*) * 100.0 / (SELECT COUNT(*) FROM persons), 2)
FROM persons WHERE gender = 'F';

-- =====================================================
-- 查询6: 展示各代的平均寿命统计（以族谱1为例）
-- =====================================================
SELECT 
    generation AS "辈分",
    COUNT(*) AS "人数",
    ROUND(AVG(CASE WHEN death_year IS NOT NULL AND death_year > 0 THEN death_year - birth_year ELSE NULL END), 1) AS "平均寿命"
FROM persons
WHERE genealogy_id = 1 
  AND birth_year IS NOT NULL 
  AND death_year IS NOT NULL
  AND death_year > birth_year
GROUP BY generation
ORDER BY generation
LIMIT 15;

-- =====================================================
-- 查询7: 测试祖先查询存储过程 - 查找某人的祖先
-- =====================================================
-- 先选一个成员（比如第一个有birth_family_id的成员）
WITH sample_person AS (
    SELECT person_id, name, birth_family_id 
    FROM persons 
    WHERE birth_family_id IS NOT NULL
    ORDER BY person_id
    LIMIT 1
)
SELECT person_id AS "示例成员ID", name AS "示例成员姓名" 
FROM sample_person;

-- 使用这个ID调用存储过程（假设上面返回的ID是某个值，实际执行时请替换）
-- 例如：SELECT * FROM sp_get_ancestors(100);

-- =====================================================
-- 查询8: 测试亲缘关系查询 - 查找两个人的关系
-- =====================================================
-- 选择两个有关系的成员（示例）
WITH family_members AS (
    SELECT 
        f.husband_id, f.wife_id,
        p1.name AS husband_name,
        p2.name AS wife_name,
        (SELECT person_id FROM persons WHERE birth_family_id = f.family_id LIMIT 1) AS child_id
    FROM families f
    JOIN persons p1 ON f.husband_id = p1.person_id
    JOIN persons p2 ON f.wife_id = p2.person_id
    WHERE f.genealogy_id = 1
    LIMIT 1
)
SELECT 
    husband_id AS "丈夫ID", husband_name AS "丈夫姓名",
    wife_id AS "妻子ID", wife_name AS "妻子姓名",
    child_id AS "子女ID"
FROM family_members;

-- 用这两个ID测试亲缘关系查询（例如 husband_id 和 child_id）
-- 例如：SELECT * FROM sp_find_relationship(丈夫ID, 子女ID);

-- =====================================================
-- 查询9: 展示所有创建的索引
-- =====================================================
SELECT 
    tablename AS "表名",
    indexname AS "索引名",
    indexdef AS "索引定义"
FROM pg_indexes
WHERE schemaname = 'public'
ORDER BY tablename, indexname;

-- =====================================================
-- 查询10: 展示存储过程定义（概要）
-- =====================================================
SELECT 
    proname AS "存储过程名",
    prorettype::regtype AS "返回类型",
    pg_get_function_arguments(oid) AS "参数"
FROM pg_proc
WHERE proname LIKE 'sp_%'
ORDER BY proname;
