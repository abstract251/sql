# 数据生成与导入 - 快速操作指南

## 重要提醒

⚠️ **重新生成数据前，请先备份现有数据！**

---

## 选项 A：使用 Python 脚本（推荐）

### 步骤 1：运行 Python 脚本生成数据

```bash
cd e:\Desktop\sql_work\Family\database
python generate_data.py
```

这将生成新的 CSV 文件到 `csv_data/` 目录。

### 步骤 2：导入数据到数据库

```bash
psql -U your_username -d your_database -f import_data.sql
```

或者在 pgAdmin 中打开 `import_data.sql` 并执行。

---

## 选项 B：使用 SQL 存储过程

### 步骤 1：执行 SQL 脚本生成数据

```bash
psql -U your_username -d your_database -f generate_large_data.sql
```

这将直接在数据库中生成数据。

### 步骤 2：验证数据

```sql
-- 检查成员总数
SELECT COUNT(*) AS 总成员数 FROM persons;

-- 检查代数分布
SELECT 
    generation AS 辈分, 
    COUNT(*) AS 人数 
FROM persons 
GROUP BY generation 
ORDER BY generation;

-- 检查最大代数
SELECT MAX(generation) AS 最大代数 FROM persons;

-- 检查族谱统计
SELECT 
    g.name AS 族谱名,
    COUNT(p.person_id) AS 成员数,
    MAX(p.generation) AS 最大代数
FROM genealogies g
LEFT JOIN persons p ON g.genealogy_id = p.genealogy_id
GROUP BY g.genealogy_id, g.name
ORDER BY COUNT(p.person_id) DESC;
```

---

## 选项 C：只修复代数问题

如果只是想增加现有数据的代数，可以修改 CSV 文件：

### 步骤 1：编辑 generate_data.py

找到以下行，修改目标代数：
```python
target_generations = 35  # 从12改为35
```

### 步骤 2：重新生成数据

```bash
python generate_data.py
```

### 步骤 3：清空并重新导入

```sql
TRUNCATE TABLE persons RESTART IDENTITY;
TRUNCATE TABLE families RESTART IDENTITY;
```

然后执行 `import_data.sql`。

---

## 数据导出

生成数据后，可以导出备份：

```bash
psql -U your_username -d your_database -f export_data.sql
```

导出文件将保存在 `database/export/` 目录。

---

## 常见问题

### Q1: Python 脚本执行失败？

确保已安装 Python 3：
```bash
python --version
```

### Q2: 导入时出现外键错误？

检查导入顺序，确保：
1. genealogies 先导入
2. users 先导入
3. families 导入（注意：CSV 中的 husband_id/wife_id 可能引用了不存在的 ID）

### Q3: 触发器干扰数据导入？

在 `import_data.sql` 中已禁用触发器。如果仍有问题，手动禁用：

```sql
ALTER TABLE persons DISABLE TRIGGER ALL;
-- 执行导入
ALTER TABLE persons ENABLE TRIGGER ALL;
```

### Q4: 数据量太大，导入很慢？

可以使用 PostgreSQL 的并行导入功能，或分批导入：

```sql
-- 导入前设置
SET synchronous_commit = off;
SET maintenance_work_mem = '1GB';

-- 导入后恢复
SET synchronous_commit = on;
```

---

## 验证树形预览

数据生成后，运行应用程序并测试：

1. 打开"树形预览"
2. 输入第一代祖先的 ID（通常是 1 或 2）
3. 确认能看到 30+ 代的传承关系

---

## 联系信息

如果遇到问题，请检查：
1. PostgreSQL 服务是否运行
2. 数据库连接配置是否正确
3. CSV 文件路径是否正确
4. 是否有足够的磁盘空间

---

**更新时间**：2026-05-16
