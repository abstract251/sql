-- =====================================================
-- 数据导入脚本
-- 使用 PostgreSQL COPY 命令批量导入 CSV 数据
-- =====================================================

SET client_encoding TO 'UTF8';

-- =====================================================
-- 步骤1: 设置事务和约束
-- =====================================================
BEGIN;

-- 推迟所有外键约束检查到事务结束
SET CONSTRAINTS ALL DEFERRED;

-- =====================================================
-- 步骤2: 清空现有数据（禁用外键检查）
-- =====================================================
ALTER TABLE persons DISABLE TRIGGER ALL;
ALTER TABLE families DISABLE TRIGGER ALL;
ALTER TABLE genealogies DISABLE TRIGGER ALL;
ALTER TABLE user_genealogy DISABLE TRIGGER ALL;
ALTER TABLE users DISABLE TRIGGER ALL;

-- 清空数据（使用 DELETE 而不是 TRUNCATE 避免级联问题）
DELETE FROM user_genealogy;
DELETE FROM families;
DELETE FROM persons;
DELETE FROM genealogies;
DELETE FROM users;

-- 重置序列
ALTER SEQUENCE persons_person_id_seq RESTART WITH 1;
ALTER SEQUENCE families_family_id_seq RESTART WITH 1;
ALTER SEQUENCE genealogies_genealogy_id_seq RESTART WITH 1;
ALTER SEQUENCE users_user_id_seq RESTART WITH 1;

-- =====================================================
-- 步骤3: 导入数据
-- =====================================================

-- 1. 导入 users（用户表）
COPY users(user_id, username, password_hash, email, created_at)
FROM 'e:/DevelopProjects/DatabaseLab/code/Family/database/csv_data/users.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- 2. 导入 genealogies（族谱表）
COPY genealogies(genealogy_id, name, surname, compile_time, description, creator_id)
FROM 'e:/DevelopProjects/DatabaseLab/code/Family/database/csv_data/genealogies.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- 3. 导入 persons（成员表）
COPY persons(person_id, name, gender, birth_year, death_year, biography, generation, genealogy_id, birth_family_id)
FROM 'e:/DevelopProjects/DatabaseLab/code/Family/database/csv_data/persons.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- 4. 导入 families（家庭表）
COPY families(family_id, husband_id, wife_id, genealogy_id, marriage_year)
FROM 'e:/DevelopProjects/DatabaseLab/code/Family/database/csv_data/families.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- 5. 导入 user_genealogy（用户-族谱关联表）
COPY user_genealogy(user_id, genealogy_id, role)
FROM 'e:/DevelopProjects/DatabaseLab/code/Family/database/csv_data/user_genealogy.csv'
WITH (FORMAT csv, HEADER true, NULL '');

-- =====================================================
-- 步骤4: 重新启用触发器和约束
-- =====================================================
ALTER TABLE users ENABLE TRIGGER ALL;
ALTER TABLE genealogies ENABLE TRIGGER ALL;
ALTER TABLE persons ENABLE TRIGGER ALL;
ALTER TABLE families ENABLE TRIGGER ALL;
ALTER TABLE user_genealogy ENABLE TRIGGER ALL;

-- =====================================================
-- 步骤5: 提交事务（此时才检查外键约束）
-- =====================================================
COMMIT;

-- =====================================================
-- 步骤6: 验证导入结果
-- =====================================================
SELECT '成员总数' AS 表名, COUNT(*) AS 记录数 FROM persons
UNION ALL
SELECT '家庭总数', COUNT(*) FROM families
UNION ALL
SELECT '族谱总数', COUNT(*) FROM genealogies
UNION ALL
SELECT '用户总数', COUNT(*) FROM users
UNION ALL
SELECT '用户-族谱关联数', COUNT(*) FROM user_genealogy;

-- 检查代数分布（只显示前20代）
SELECT generation AS 辈分, COUNT(*) AS 人数
FROM persons
GROUP BY generation
ORDER BY generation
LIMIT 20;

-- 检查最大代数和统计信息
SELECT '最大辈分' AS 统计项, MAX(generation) AS 值 FROM persons
UNION ALL
SELECT '最小辈分', MIN(generation) FROM persons
UNION ALL
SELECT '总族谱数' AS 统计项, (SELECT COUNT(*) FROM genealogies) AS 值
UNION ALL
SELECT '总成员数' AS 统计项, (SELECT COUNT(*) FROM persons) AS 值;

-- =====================================================
-- 导入完成提示
-- =====================================================
SELECT '数据导入完成！' AS 提示信息;
