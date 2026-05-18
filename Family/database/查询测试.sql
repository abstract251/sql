-- 1. 给定成员ID，查询其配偶及所有子女 (假设ID=1)
SELECT '1. 成员ID=1 的配偶及子女' AS query;
SELECT * FROM sp_get_spouse_and_children(1);

-- 2. 递归向上追溯祖先 (假设ID=5)
SELECT '2. 成员ID=5 的祖先' AS query;
SELECT * FROM sp_get_ancestors(5);

-- 3. 统计平均寿命最长的一代人 (族谱ID=1)
SELECT '3. 族谱1 平均寿命最长的一代' AS query;
SELECT generation, COUNT(*) AS count, AVG(death_year - birth_year)::NUMERIC(10,2) AS avg_lifespan
FROM persons
WHERE genealogy_id = 1 AND death_year IS NOT NULL AND birth_year IS NOT NULL
GROUP BY generation
ORDER BY avg_lifespan DESC
LIMIT 1;

-- 4. 年龄超过50岁、且没有配偶的男性成员 (前10行)
SELECT '4. 年龄>50且无配偶的男性(前10)' AS query;
SELECT p.person_id, p.name, p.birth_year, 
       EXTRACT(YEAR FROM AGE(MAKE_DATE(2026,1,1), MAKE_DATE(p.birth_year,1,1))) AS age
FROM persons p
WHERE p.gender = 'M' AND p.birth_year < 1976 AND p.death_year IS NULL
  AND NOT EXISTS (SELECT 1 FROM families f WHERE f.husband_id = p.person_id)
LIMIT 10;

-- 5. 出生年份早于该辈分平均出生年份的成员 (族谱ID=1, 前10行)
SELECT '5. 出生早于同辈平均的成员(族谱1,前10)' AS query;
WITH avg_gen AS (
    SELECT generation, AVG(birth_year) AS avg_birth
    FROM persons WHERE genealogy_id = 1 AND birth_year IS NOT NULL
    GROUP BY generation
)
SELECT p.person_id, p.name, p.gender, p.birth_year, p.generation, ag.avg_birth
FROM persons p JOIN avg_gen ag ON p.generation = ag.generation
WHERE p.genealogy_id = 1 AND p.birth_year < ag.avg_birth
ORDER BY p.generation, p.birth_year
LIMIT 10;

-- 6. 男女比例 (所有族谱)
SELECT '7. 各族谱男女比例' AS query;
SELECT g.name AS genealogy_name,
       COUNT(*) AS total,
       COUNT(*) FILTER (WHERE p.gender = 'M') AS male,
       COUNT(*) FILTER (WHERE p.gender = 'F') AS female
FROM persons p JOIN genealogies g ON p.genealogy_id = g.genealogy_id
GROUP BY g.name
ORDER BY total DESC;

-- 7. 曾孙查询 (性能测试用，从始祖ID=1查四代曾孙，可能为0如果代数不够，可换ID)
SELECT '8. 始祖ID=1的曾孙(深度4)' AS query;
WITH RECURSIVE desc_tree AS (
    SELECT person_id, name, birth_year, 0 AS depth
    FROM persons WHERE person_id = 1
    UNION ALL
    SELECT c.person_id, c.name, c.birth_year, d.depth + 1
    FROM desc_tree d
    JOIN families f ON f.husband_id = d.person_id OR f.wife_id = d.person_id
    JOIN persons c ON c.birth_family_id = f.family_id
    WHERE d.depth < 4
)
SELECT * FROM desc_tree WHERE depth = 4 LIMIT 20;