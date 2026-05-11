-- =====================================================
-- "寻根溯源"族谱管理系统 - PostgreSQL数据库架构2.0
-- 执行方式：psql -U postgres -f schema.sql
-- =====================================================

-- 1. 创建数据库
DROP DATABASE IF EXISTS family_genealogy;
CREATE DATABASE family_genealogy
    WITH OWNER = postgres
    ENCODING = 'UTF8'
    LC_COLLATE = 'C'
    LC_CTYPE = 'C'
    TEMPLATE = template0;   -- 使用 template0 确保 locale 设置生效

-- 连接到新数据库
\c family_genealogy;

-- 启用模糊查询所需的扩展
CREATE EXTENSION IF NOT EXISTS pg_trgm;

-- =====================================================
-- 2. 用户表 (users)
-- =====================================================
DROP TABLE IF EXISTS users CASCADE;
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,   -- UNIQUE自动创建索引
    password_hash VARCHAR(255) NOT NULL,
    email VARCHAR(100),
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

-- =====================================================
-- 3. 族谱表 (genealogies)
-- =====================================================
DROP TABLE IF EXISTS genealogies CASCADE;
CREATE TABLE genealogies (
    genealogy_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    surname VARCHAR(50) NOT NULL,
    compile_time DATE,
    description TEXT,
    creator_id INT NOT NULL REFERENCES users(user_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_genealogies_creator ON genealogies(creator_id);
CREATE INDEX idx_genealogies_surname ON genealogies(surname);

-- =====================================================
-- 4. 用户-族谱协作表 (user_genealogy)
-- =====================================================
DROP TABLE IF EXISTS user_genealogy CASCADE;
CREATE TABLE user_genealogy (
    user_id INT NOT NULL REFERENCES users(user_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    genealogy_id INT NOT NULL REFERENCES genealogies(genealogy_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    role VARCHAR(20) NOT NULL DEFAULT 'editor',
    joined_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (user_id, genealogy_id)
);

CREATE INDEX idx_user_genealogy_genealogy ON user_genealogy(genealogy_id);

-- =====================================================
-- 5. 家庭表 (families) —— 先创建，persons再引用
-- =====================================================
DROP TABLE IF EXISTS families CASCADE;
CREATE TABLE families (
    family_id SERIAL PRIMARY KEY,
    husband_id INT,
    wife_id INT,
    genealogy_id INT NOT NULL REFERENCES genealogies(genealogy_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    marriage_year INT,
    -- 约束：至少存在一位家长，且夫妻不能为同一人
    CONSTRAINT chk_at_least_one_parent CHECK (husband_id IS NOT NULL OR wife_id IS NOT NULL),
    CONSTRAINT chk_not_same_person CHECK (husband_id IS NULL OR wife_id IS NULL OR husband_id <> wife_id)
);

CREATE INDEX idx_families_husband ON families(husband_id);
CREATE INDEX idx_families_wife ON families(wife_id);
CREATE INDEX idx_families_genealogy ON families(genealogy_id);

-- =====================================================
-- 6. 成员表 (persons) —— 延迟外键解决循环引用
-- =====================================================
DROP TABLE IF EXISTS persons CASCADE;
CREATE TABLE persons (
    person_id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    gender CHAR(1) NOT NULL CHECK (gender IN ('M', 'F')),
    birth_year INT,
    death_year INT,
    biography TEXT,
    generation INT NOT NULL DEFAULT 1,
    genealogy_id INT NOT NULL REFERENCES genealogies(genealogy_id)
        ON DELETE CASCADE ON UPDATE CASCADE,
    birth_family_id INT DEFERRABLE INITIALLY DEFERRED REFERENCES families(family_id)
        ON DELETE SET NULL ON UPDATE CASCADE,
    CONSTRAINT chk_birth_year CHECK (birth_year IS NULL OR birth_year > 0),
    CONSTRAINT chk_death_year CHECK (death_year IS NULL OR death_year >= birth_year)
);

-- 索引
CREATE INDEX idx_persons_name ON persons(name);                  -- 前缀匹配
CREATE INDEX idx_persons_name_trgm ON persons USING GIN (name gin_trgm_ops); -- 全模糊匹配
CREATE INDEX idx_persons_gender ON persons(gender);
CREATE INDEX idx_persons_generation ON persons(generation);
CREATE INDEX idx_persons_genealogy ON persons(genealogy_id);
CREATE INDEX idx_persons_birth_family ON persons(birth_family_id);
CREATE INDEX idx_persons_genealogy_generation ON persons(genealogy_id, generation);

-- =====================================================
-- 7. 为 families 表添加外键（延迟检查，解决循环）
-- =====================================================
ALTER TABLE families ADD CONSTRAINT fk_families_husband
    FOREIGN KEY (husband_id) REFERENCES persons(person_id)
    ON DELETE SET NULL ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED;

ALTER TABLE families ADD CONSTRAINT fk_families_wife
    FOREIGN KEY (wife_id) REFERENCES persons(person_id)
    ON DELETE SET NULL ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED;

-- =====================================================
-- 8. 函数：自动计算辈分
-- =====================================================
CREATE OR REPLACE FUNCTION calculate_generation(p_birth_family_id INT)
RETURNS INT AS $$
DECLARE
    parent_generation INT DEFAULT 0;
    father_id INT;
    mother_id INT;
BEGIN
    IF p_birth_family_id IS NULL THEN
        RETURN 1;
    END IF;

    SELECT husband_id, wife_id INTO father_id, mother_id
    FROM families
    WHERE family_id = p_birth_family_id;

    IF father_id IS NOT NULL THEN
        SELECT generation INTO parent_generation
        FROM persons WHERE person_id = father_id;
        RETURN parent_generation + 1;
    ELSIF mother_id IS NOT NULL THEN
        SELECT generation INTO parent_generation
        FROM persons WHERE person_id = mother_id;
        RETURN parent_generation + 1;
    END IF;

    RETURN 1;
END;
$$ LANGUAGE plpgsql;

-- =====================================================
-- 9. 触发器：自动维护辈分
-- =====================================================
CREATE OR REPLACE FUNCTION trg_person_generation()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.birth_family_id IS NOT NULL THEN
        NEW.generation := calculate_generation(NEW.birth_family_id);
    END IF;
    IF NEW.generation IS NULL OR NEW.generation = 0 THEN
        NEW.generation := 1;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_person_before_insert ON persons;
CREATE TRIGGER trg_person_before_insert
BEFORE INSERT ON persons
FOR EACH ROW EXECUTE FUNCTION trg_person_generation();

DROP TRIGGER IF EXISTS trg_person_before_update ON persons;
CREATE TRIGGER trg_person_before_update
BEFORE UPDATE OF birth_family_id ON persons
FOR EACH ROW EXECUTE FUNCTION trg_person_generation();

-- =====================================================
-- 10. 触发器：验证父母出生年份早于子女
-- =====================================================
CREATE OR REPLACE FUNCTION trg_verify_birth_years()
RETURNS TRIGGER AS $$
DECLARE
    father_birth INT;
    mother_birth INT;
BEGIN
    IF NEW.birth_family_id IS NOT NULL THEN
        SELECT p.birth_year INTO father_birth
        FROM families f
        JOIN persons p ON f.husband_id = p.person_id
        WHERE f.family_id = NEW.birth_family_id;

        SELECT p.birth_year INTO mother_birth
        FROM families f
        JOIN persons p ON f.wife_id = p.person_id
        WHERE f.family_id = NEW.birth_family_id;

        IF father_birth IS NOT NULL AND NEW.birth_year IS NOT NULL
           AND father_birth >= NEW.birth_year THEN
            RAISE EXCEPTION '父亲的出生年份必须早于子女';
        END IF;

        IF mother_birth IS NOT NULL AND NEW.birth_year IS NOT NULL
           AND mother_birth >= NEW.birth_year THEN
            RAISE EXCEPTION '母亲的出生年份必须早于子女';
        END IF;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS trg_verify_birth_years_trigger ON persons;
CREATE TRIGGER trg_verify_birth_years_trigger
BEFORE INSERT OR UPDATE OF birth_family_id, birth_year ON persons
FOR EACH ROW EXECUTE FUNCTION trg_verify_birth_years();

-- =====================================================
-- 11. 视图
-- =====================================================

-- 成员详细信息（含父母姓名、年龄）
CREATE OR REPLACE VIEW v_person_details AS
SELECT
    p.person_id,
    p.name,
    p.gender,
    p.birth_year,
    p.death_year,
    p.biography,
    p.generation,
    p.genealogy_id,
    g.name AS genealogy_name,
    g.surname,
    p.birth_family_id,
    f.husband_id,
    f.wife_id,
    father.name AS father_name,
    father.person_id AS father_id,
    mother.name AS mother_name,
    mother.person_id AS mother_id,
    CASE
        WHEN p.birth_year IS NULL THEN NULL
        WHEN p.death_year IS NULL THEN 
            EXTRACT(YEAR FROM AGE(CURRENT_DATE, MAKE_DATE(p.birth_year, 1, 1)))::INT
        ELSE p.death_year - p.birth_year
    END AS age
FROM persons p
JOIN genealogies g ON p.genealogy_id = g.genealogy_id
LEFT JOIN families f ON p.birth_family_id = f.family_id
LEFT JOIN persons father ON f.husband_id = father.person_id
LEFT JOIN persons mother ON f.wife_id = mother.person_id;

-- 子女视图（根据 birth_family_id 直接查询）
CREATE OR REPLACE VIEW v_children AS
SELECT
    f.family_id,
    f.husband_id,
    f.wife_id,
    child.person_id AS child_id,
    child.name AS child_name,
    child.gender AS child_gender,
    child.birth_year AS child_birth_year,
    child.generation AS child_generation
FROM families f
JOIN persons child ON child.birth_family_id = f.family_id;

-- =====================================================
-- 12. 存储过程：查询配偶及子女
-- =====================================================
CREATE OR REPLACE FUNCTION sp_get_spouse_and_children(p_person_id INT)
RETURNS TABLE (
    relation_type VARCHAR(20),
    related_id INT,
    related_name VARCHAR(100),
    gender CHAR(1),
    birth_year INT,
    generation INT
) AS $$
BEGIN
    RETURN QUERY
    -- 配偶
    SELECT
        '配偶'::VARCHAR(20),
        spouse.person_id,
        spouse.name,
        spouse.gender,
        spouse.birth_year,
        spouse.generation
    FROM persons spouse
    WHERE spouse.person_id IN (
        SELECT wife_id FROM families WHERE husband_id = p_person_id
        UNION
        SELECT husband_id FROM families WHERE wife_id = p_person_id
    )

    UNION ALL

    -- 子女
    SELECT
        '子女'::VARCHAR(20),
        child.person_id,
        child.name,
        child.gender,
        child.birth_year,
        child.generation
    FROM persons child
    WHERE child.birth_family_id IN (
        SELECT family_id FROM families
        WHERE husband_id = p_person_id OR wife_id = p_person_id
    );
END;
$$ LANGUAGE plpgsql;

-- =====================================================
-- 13. 存储过程：向上追溯祖先
-- =====================================================
CREATE OR REPLACE FUNCTION sp_get_ancestors(p_person_id INT)
RETURNS TABLE (
    person_id INT,
    name VARCHAR(100),
    gender CHAR(1),
    birth_year INT,
    death_year INT,
    generation INT,
    level INT,
    path TEXT
) AS $$
BEGIN
    RETURN QUERY
    WITH RECURSIVE ancestry_cte AS (
        SELECT
            p.person_id,
            p.name,
            p.gender,
            p.birth_year,
            p.death_year,
            p.generation,
            p.birth_family_id,
            0 AS ancestor_level,
            p.person_id::TEXT AS path
        FROM persons p
        WHERE p.person_id = p_person_id

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
            ac.path || '->' || parent.person_id::TEXT
        FROM ancestry_cte ac
        JOIN families f ON ac.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE ac.ancestor_level < 50
          AND parent.person_id != ac.person_id
          AND position(parent.person_id::TEXT IN ac.path) = 0
    )
    SELECT
        person_id, name, gender, birth_year, death_year, generation,
        ancestor_level, path
    FROM ancestry_cte
    WHERE ancestor_level > 0
    ORDER BY ancestor_level, generation;
END;
$$ LANGUAGE plpgsql;

-- =====================================================
-- 14. 存储过程：向下追溯后代（使用 birth_family_id）
-- =====================================================
CREATE OR REPLACE FUNCTION sp_get_descendants(p_person_id INT, max_depth INT DEFAULT 30)
RETURNS TABLE (
    person_id INT,
    name VARCHAR(100),
    gender CHAR(1),
    birth_year INT,
    death_year INT,
    generation INT,
    level INT,
    path TEXT
) AS $$
BEGIN
    RETURN QUERY
    WITH RECURSIVE descendants_cte AS (
        SELECT
            p.person_id,
            p.name,
            p.gender,
            p.birth_year,
            p.death_year,
            p.generation,
            0 AS descendant_level,
            p.person_id::TEXT AS path
        FROM persons p
        WHERE p.person_id = p_person_id

        UNION ALL

        SELECT
            child.person_id,
            child.name,
            child.gender,
            child.birth_year,
            child.death_year,
            child.generation,
            dc.descendant_level + 1,
            dc.path || '->' || child.person_id::TEXT
        FROM descendants_cte dc
        JOIN families f ON f.husband_id = dc.person_id OR f.wife_id = dc.person_id
        JOIN persons child ON child.birth_family_id = f.family_id
        WHERE dc.descendant_level < max_depth
          AND position(child.person_id::TEXT IN dc.path) = 0
    )
    SELECT
        person_id, name, gender, birth_year, death_year, generation,
        descendant_level, path
    FROM descendants_cte
    WHERE descendant_level > 0
    ORDER BY descendant_level, birth_year;
END;
$$ LANGUAGE plpgsql;

-- =====================================================
-- 15. 存储过程：查找两人亲缘关系（基于共同祖先）
-- =====================================================
CREATE OR REPLACE FUNCTION sp_find_relationship(person1_id INT, person2_id INT)
RETURNS TABLE (
    common_ancestor_id INT,
    common_ancestor_name VARCHAR(100),
    generation INT,
    path_to_person1 TEXT,
    path_to_person2 TEXT
) AS $$
BEGIN
    RETURN QUERY
    WITH RECURSIVE
    anc1 AS (
        SELECT
            person_id,
            birth_family_id,
            generation,
            person_id::TEXT AS path,
            0 AS level
        FROM persons WHERE person_id = person1_id

        UNION ALL

        SELECT
            parent.person_id,
            parent.birth_family_id,
            parent.generation,
            anc1.path || '->' || parent.person_id::TEXT,
            anc1.level + 1
        FROM anc1
        JOIN families f ON anc1.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE anc1.level < 50
    ),
    anc2 AS (
        SELECT
            person_id,
            birth_family_id,
            generation,
            person_id::TEXT AS path,
            0 AS level
        FROM persons WHERE person_id = person2_id

        UNION ALL

        SELECT
            parent.person_id,
            parent.birth_family_id,
            parent.generation,
            anc2.path || '->' || parent.person_id::TEXT,
            anc2.level + 1
        FROM anc2
        JOIN families f ON anc2.birth_family_id = f.family_id
        JOIN persons parent ON f.husband_id = parent.person_id OR f.wife_id = parent.person_id
        WHERE anc2.level < 50
    )
    SELECT
        a1.person_id,
        p.name,
        p.generation,
        a1.path,
        a2.path
    FROM anc1 a1
    JOIN anc2 a2 ON a1.person_id = a2.person_id
    JOIN persons p ON a1.person_id = p.person_id
    ORDER BY p.generation DESC
    LIMIT 1;
END;
$$ LANGUAGE plpgsql;

-- =====================================================
-- 16. 初始化示例数据（测试用）
-- =====================================================
INSERT INTO users (username, password_hash, email) VALUES
('admin', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 'admin@example.com');

INSERT INTO genealogies (name, surname, compile_time, description, creator_id) VALUES
('示例族谱', '张', '2024-01-01', '这是一个示例族谱', 1);

INSERT INTO user_genealogy (user_id, genealogy_id, role) VALUES
(1, 1, 'creator');

-- 第一代祖先
INSERT INTO persons (name, gender, birth_year, death_year, generation, genealogy_id) VALUES
('张始祖', 'M', 1700, 1780, 1, 1),
('张始祖夫人', 'F', 1705, 1785, 1, 1);

-- 家庭
INSERT INTO families (husband_id, wife_id, genealogy_id, marriage_year) VALUES
(1, 2, 1, 1725);

-- 子女（birth_family_id指向家庭1，辈分由触发器自动计算）
INSERT INTO persons (name, gender, birth_year, death_year, genealogy_id, birth_family_id) VALUES
('张长子', 'M', 1728, 1800, 1, 1),
('张次子', 'M', 1732, 1805, 1, 1),
('张长女', 'F', 1735, 1810, 1, 1);

COMMIT;