# 寻根溯源 - 族谱管理系统

基于Qt5和MySQL开发的族谱管理系统。

## 项目结构

```
Family/
├── CMakeLists.txt          # CMake构建配置
├── database/
│   ├── schema.sql          # 数据库架构
│   ├── queries.sql         # SQL查询语句
│   └── csv_data/           # CSV数据文件目录
├── scripts/
│   └── generate_data.py    # 模拟数据生成脚本
├── src/
│   ├── main.cpp
│   ├── DatabaseManager.cpp
│   ├── User.cpp
│   ├── Genealogy.cpp
│   ├── Person.cpp
│   ├── Family.cpp
│   ├── MainWindow.cpp
│   ├── LoginDialog.cpp
│   ├── DashboardWidget.cpp
│   ├── GenealogyManager.cpp
│   ├── MemberManager.cpp
│   ├── TreeViewWidget.cpp
│   ├── AncestorQueryWidget.cpp
│   └── RelationshipQueryWidget.cpp
├── include/
│   ├── DatabaseManager.h
│   ├── User.h
│   ├── Genealogy.h
│   ├── Person.h
│   ├── Family.h
│   ├── MainWindow.h
│   ├── LoginDialog.h
│   ├── DashboardWidget.h
│   ├── GenealogyManager.h
│   ├── MemberManager.h
│   ├── TreeViewWidget.h
│   ├── AncestorQueryWidget.h
│   └── RelationshipQueryWidget.h
├── ui/
│   ├── mainwindow.ui
│   ├── login_dialog.ui
│   ├── dashboard_widget.ui
│   ├── genealogy_manager.ui
│   ├── member_manager.ui
│   ├── tree_view_widget.ui
│   ├── ancestor_query_widget.ui
│   └── relationship_query_widget.ui
└── resources/
    └── resources.qrc
```

## 环境要求

- Qt 5.15+
- MySQL 8.0+
- CMake 3.16+
- Python 3.8+ (用于数据生成)

## 构建步骤

### 1. 创建数据库

```bash
mysql -u root -p < database/schema.sql
```

### 2. 生成模拟数据（可选）

```bash
cd scripts
python generate_data.py
```

这将生成12个族谱，约10万+成员数据。

### 3. 构建项目

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 4. 运行

```bash
./FamilyGenealogy
```

## 功能模块

1. **用户认证** - 注册、登录
2. **族谱管理** - 创建、编辑、删除、邀请协作者
3. **成员管理** - 增删改查、模糊搜索
4. **Dashboard** - 家族统计、男女比例、各辈分分析
5. **树形预览** - 以层级树展示家族关系
6. **祖先查询** - 递归查询某人的所有祖先
7. **亲缘关系查询** - 查找两人之间的亲缘路径

## 数据库说明

### 主要表结构

- `users` - 用户表
- `genealogies` - 族谱表
- `user_genealogy` - 用户-族谱协作关系表
- `persons` - 成员表
- `families` - 家庭/婚姻表
- `parent_child` - 亲子关系表

### 关键设计

- `birth_family_id`: 指向成员出生的家庭，便于递归追溯祖先
- `generation`: 辈分字段，加速统计分析

## SQL查询功能

- 递归CTE查询祖先/后代
- 亲缘关系路径查找
- 辈分统计分析
- 模糊搜索
