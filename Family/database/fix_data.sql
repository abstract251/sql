-- =====================================================
-- 族谱管理系统 - 数据修复脚本
-- 用于修复 generation 字段和重新导入数据
-- 适用于 PostgreSQL
-- =====================================================

-- 步骤 1: 禁用触发器（防止触发器覆盖 CSV 数据中的 generation 值）
ALTER TABLE persons DISABLE TRIGGER ALL;

-- 步骤 2: 清空现有数据（谨慎操作）
TRUNCATE TABLE persons RESTART IDENTITY CASCADE;
TRUNCATE TABLE families RESTART IDENTITY CASCADE;

-- 步骤 3: 重新导入 persons 数据
-- 注意: CSV 文件路径需要根据实际情况调整
COPY persons(person_id, name, gender, birth_year, death_year, biography, generation, genealogy_id, birth_family_id)
FROM 'e:\Desktop\sql_work\Family\database\csv_data\persons.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- 步骤 4: 重新导入 families 数据
COPY families(family_id, husband_id, wife_id, genealogy_id, marriage_year)
FROM 'e:\Desktop\sql_work\Family\database\csv_data\families.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- 步骤 5: 重新启用触发器
ALTER TABLE persons ENABLE TRIGGER ALL;

-- 步骤 6: 验证数据导入结果
SELECT person_id, name, gender, generation, birth_family_id
FROM persons
ORDER BY generation, person_id
LIMIT 30;

-- 步骤 7: 如果 generation 仍然不正确，手动更新
-- 从 CSV 重新导入 generation 值
CREATE TEMP TABLE temp_persons_import (
    person_id INT,
    name VARCHAR(100),
    gender CHAR(1),
    birth_year INT,
    death_year INT,
    biography TEXT,
    generation INT,
    genealogy_id INT,
    birth_family_id INT
);

COPY temp_persons_import FROM 'e:\Desktop\sql_work\Family\database\csv_data\persons.csv' WITH (FORMAT csv, HEADER true);

UPDATE persons p SET generation = t.generation
FROM temp_persons_import t
WHERE p.person_id = t.person_id;

DROP TABLE temp_persons_import;

-- 步骤 8: 最终验证
SELECT 
    generation AS "辈分",
    COUNT(*) AS "人数",
    MIN(name) AS "示例姓名"
FROM persons
GROUP BY generation
ORDER BY generation;

-- 步骤 9: 测试存储过程
-- 测试祖先查询（选择 person_id=8）
SELECT '祖先查询测试:' AS test;
SELECT person_id, name, gender, generation, level, path
FROM sp_get_ancestors(8)
ORDER BY level;

-- 测试后代查询（选择 person_id=3）
SELECT '后代查询测试:' AS test;
SELECT person_id, name, gender, generation, level, path
FROM sp_get_descendants(3, 10)
ORDER BY level;
