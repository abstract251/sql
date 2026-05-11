# 寻根溯源 - 族谱管理系统 Code Wiki

## 1. 项目概述

### 1.1 项目简介

**寻根溯源**是一套基于Qt5框架和PostgreSQL数据库开发的族谱管理桌面应用程序。该系统提供了完整的族谱管理功能，包括用户认证、族谱创建与管理、家庭成员管理、祖先查询、关系查询等核心功能。

### 1.2 技术栈

| 类别 | 技术选型 |
|------|----------|
| 编程语言 | C++17 |
| 开发框架 | Qt5 (Widgets, Sql, Network) |
| 数据库 | PostgreSQL |
| 构建工具 | CMake / qmake |
| 目标平台 | Windows |

---

## 2. 项目架构

### 2.1 整体架构图

```
┌─────────────────────────────────────────────────────────────────┐
│                        表现层 (UI Layer)                          │
├─────────────────────────────────────────────────────────────────┤
│  MainWindow  │  LoginDialog  │  DashboardWidget  │  其他Widget   │
├─────────────────────────────────────────────────────────────────┤
│                        业务逻辑层 (Business Logic)                │
├─────────────────────────────────────────────────────────────────┤
│  GenealogyManager  │  MemberManager  │  QueryWidgets  │  TreeView│
├─────────────────────────────────────────────────────────────────┤
│                        数据访问层 (Data Access)                   │
├─────────────────────────────────────────────────────────────────┤
│                       DatabaseManager (单例模式)                  │
├─────────────────────────────────────────────────────────────────┤
│                        数据层 (Database)                          │
├─────────────────────────────────────────────────────────────────┤
│         PostgreSQL: users, genealogies, persons, families        │
└─────────────────────────────────────────────────────────────────┘
```

### 2.2 目录结构

```
Family/
├── CMakeLists.txt              # CMake构建配置
├── Family.pro                   # Qt项目配置文件
├── main.cpp                     # 应用程序入口
├── include/                     # 头文件目录
│   ├── DatabaseManager.h        # 数据库管理器（单例）
│   ├── User.h                  # 用户数据模型
│   ├── Person.h                # 成员数据模型
│   ├── Genealogy.h             # 族谱数据模型
│   ├── Family.h                # 家庭数据模型
│   ├── MainWindow.h            # 主窗口
│   ├── LoginDialog.h           # 登录对话框
│   ├── DashboardWidget.h        # 仪表盘组件
│   ├── GenealogyManager.h      # 族谱管理器
│   ├── MemberManager.h         # 成员管理器
│   ├── TreeViewWidget.h        # 族谱树视图
│   ├── AncestorQueryWidget.h   # 祖先查询组件
│   └── RelationshipQueryWidget.h  # 关系查询组件
├── src/                        # 源文件目录
│   ├── main.cpp                # 入口函数
│   ├── DatabaseManager.cpp      # 数据库管理器实现
│   ├── User.cpp                # 用户模型实现
│   ├── Person.cpp              # 成员模型实现
│   ├── Genealogy.cpp           # 族谱模型实现
│   ├── Family.cpp              # 家庭模型实现
│   ├── MainWindow.cpp          # 主窗口实现
│   ├── LoginDialog.cpp         # 登录对话框实现
│   ├── DashboardWidget.cpp     # 仪表盘实现
│   ├── GenealogyManager.cpp    # 族谱管理器实现
│   ├── MemberManager.cpp       # 成员管理器实现
│   ├── TreeViewWidget.cpp      # 树视图实现
│   ├── AncestorQueryWidget.cpp # 祖先查询实现
│   └── RelationshipQueryWidget.cpp  # 关系查询实现
├── ui/                         # Qt UI文件目录
│   ├── mainwindow.ui           # 主窗口UI
│   ├── login_dialog.ui         # 登录对话框UI
│   ├── dashboard_widget.ui     # 仪表盘UI
│   ├── genealogy_manager.ui    # 族谱管理器UI
│   ├── member_manager.ui       # 成员管理器UI
│   ├── tree_view_widget.ui     # 树视图UI
│   ├── ancestor_query_widget.ui  # 祖先查询UI
│   └── relationship_query_widget.ui  # 关系查询UI
├── database/                   # 数据库相关文件
│   ├── schema.sql              # 数据库架构定义
│   └── queries.sql             # SQL查询脚本
└── resources/                  # 资源文件
    └── resources.qrc           # Qt资源文件配置
```

---

## 3. 核心模块说明

### 3.1 数据模型层

#### 3.1.1 User 类
**文件位置**: [User.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/User.h)

用户数据模型，用于存储用户基本信息。

| 属性 | 类型 | 说明 |
|------|------|------|
| m_id | int | 用户唯一标识 |
| m_username | QString | 用户名 |
| m_email | QString | 邮箱 |
| m_createdAt | QDateTime | 创建时间 |

#### 3.1.2 Person 类
**文件位置**: [Person.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/Person.h)

族谱成员数据模型，表示一个家族成员。

| 属性 | 类型 | 说明 |
|------|------|------|
| m_id | int | 成员唯一标识 |
| m_name | QString | 姓名 |
| m_gender | QChar | 性别 ('M'男/'F'女) |
| m_birthYear | int | 出生年份 |
| m_deathYear | int | 去世年份 |
| m_generation | int | 辈分代数 |
| m_genealogyId | int | 所属族谱ID |
| m_birthFamilyId | int | 出生家庭ID |
| m_biography | QString | 生平简介 |
| m_fatherName | QString | 父亲姓名 |
| m_motherName | QString | 母亲姓名 |

**核心方法**:
- `age()`: 计算年龄， жив未去世则使用当前年份(2026)计算

#### 3.1.3 Genealogy 类
**文件位置**: [Genealogy.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/Genealogy.h)

族谱数据模型，表示一个完整的族谱。

| 属性 | 类型 | 说明 |
|------|------|------|
| m_id | int | 族谱唯一标识 |
| m_name | QString | 族谱名称 |
| m_surname | QString | 姓氏 |
| m_compileTime | QDate | 编纂时间 |
| m_description | QString | 族谱描述 |
| m_creatorId | int | 创建者用户ID |

#### 3.1.4 Family 类
**文件位置**: [Family.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/Family.h)

家庭数据模型，表示一对夫妻及其子女组成的基本家庭单元。

| 属性 | 类型 | 说明 |
|------|------|------|
| m_id | int | 家庭唯一标识 |
| m_husbandId | int | 丈夫ID |
| m_wifeId | int | 妻子ID |
| m_genealogyId | int | 所属族谱ID |
| m_marriageYear | int | 结婚年份 |

---

### 3.2 数据访问层

#### 3.2.1 DatabaseManager 类
**文件位置**: [DatabaseManager.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/DatabaseManager.h), [DatabaseManager.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/DatabaseManager.cpp)

数据库管理器，采用**单例模式**，提供所有数据库操作接口。

**核心方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `instance()` | DatabaseManager& | 获取单例实例 |
| `initializeDatabase()` | bool | 初始化数据库表结构 |
| `checkAndCreateDatabase()` | bool | 检查并创建数据库 |
| `executeSqlFromFile()` | bool | 从文件执行SQL |
| `setDatabase()` | void | 设置数据库连接 |
| `database()` | QSqlDatabase | 获取数据库连接 |

**用户管理方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `userExists()` | bool | 检查用户是否存在 |
| `getUserId()` | int | 获取用户ID |
| `validateUser()` | bool | 验证用户登录 |
| `createUser()` | bool | 创建新用户 |

**族谱管理方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `getGenealogiesForUser()` | QVariantList | 获取用户关联的族谱列表 |
| `getAllGenealogies()` | QVariantList | 获取所有族谱 |
| `createGenealogy()` | bool | 创建族谱 |
| `deleteGenealogy()` | bool | 删除族谱 |
| `updateGenealogy()` | bool | 更新族谱信息 |

**成员管理方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `getMembers()` | QVariantList | 获取成员列表(支持分页) |
| `searchMembersByName()` | QVariantList | 按姓名搜索成员 |
| `getMember()` | QVariantList | 获取单个成员详情 |
| `addMember()` | int | 添加成员 |
| `updateMember()` | bool | 更新成员信息 |
| `deleteMember()` | bool | 删除成员 |

**家庭关系方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `getFamilyMembers()` | QVariantList | 获取家庭成员 |
| `getSpouseAndChildren()` | QVariantList | 获取配偶和子女 |
| `getFamilyById()` | QVariantList | 根据ID获取家庭信息 |
| `createFamily()` | int | 创建新家庭 |

**查询方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `getAncestors()` | QVariantList | 追溯祖先(调用存储过程) |
| `getDescendants()` | QVariantList | 追溯后代(调用存储过程) |
| `findRelationship()` | QVariantList | 查找两人关系(调用存储过程) |
| `getGenerationStats()` | QVariantList | 获取辈分统计 |
| `getGenderStats()` | QVariantList | 获取性别统计 |

**其他方法**:

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `inviteUser()` | bool | 邀请用户参与族谱 |
| `isUserInGenealogy()` | bool | 检查用户是否参与族谱 |
| `lastError()` | QString | 获取最后错误信息 |

---

### 3.3 表现层 (UI组件)

#### 3.3.1 MainWindow
**文件位置**: [MainWindow.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/MainWindow.h), [MainWindow.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/MainWindow.cpp)

应用程序主窗口，采用DockWidget和TabWidget的混合布局。

**主要组件**:
- `m_loginDialog`: 登录对话框
- `m_centralTabWidget`: 中央标签页组件
- `m_dashboardWidget`: 仪表盘组件
- `m_genealogyManager`: 族谱管理器组件
- `m_memberManager`: 成员管理器组件
- `m_treeViewWidget`: 族谱树视图组件
- `m_ancestorQueryWidget`: 祖先查询组件
- `m_relationshipQueryWidget`: 关系查询组件

**核心方法**:
- `setCurrentUser()`: 设置当前登录用户
- `onLoginSuccess()`: 登录成功处理
- `onLogout()`: 登出处理
- `onGenealogySelected()`: 族谱选择处理

#### 3.3.2 LoginDialog
**文件位置**: [LoginDialog.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/LoginDialog.h), [LoginDialog.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/LoginDialog.cpp)

用户登录/注册对话框。

**核心方法**:
- `validateLogin()`: 验证登录信息
- `registerUser()`: 注册新用户
- `onLoginButtonClicked()`: 登录按钮点击处理
- `onRegisterButtonClicked()`: 注册按钮点击处理
- `onTogglePasswordEcho()`: 切换密码显示/隐藏

#### 3.3.3 DashboardWidget
**文件位置**: [DashboardWidget.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/DashboardWidget.h), [DashboardWidget.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/DashboardWidget.cpp)

仪表盘组件，显示族谱统计信息。

**功能**:
- 成员总数统计
- 性别比例统计
- 辈分分布统计
- 各代人数柱状图

#### 3.3.4 GenealogyManager
**文件位置**: [GenealogyManager.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/GenealogyManager.h), [GenealogyManager.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/GenealogyManager.cpp)

族谱管理组件，提供族谱的增删改查功能。

**核心方法**:
- `refreshGenealogies()`: 刷新族谱列表
- `onCreateGenealogy()`: 创建族谱
- `onEditGenealogy()`: 编辑族谱
- `onDeleteGenealogy()`: 删除族谱
- `onInviteUser()`: 邀请用户协作

#### 3.3.5 MemberManager
**文件位置**: [MemberManager.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/MemberManager.h), [MemberManager.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/MemberManager.cpp)

族谱成员管理组件，提供成员的增删改查和家庭关系管理。

**核心方法**:
- `refreshMembers()`: 刷新成员列表
- `searchMembers()`: 搜索成员
- `onAddMember()`: 添加成员
- `onEditMember()`: 编辑成员
- `onDeleteMember()`: 删除成员
- `loadFamilyComboBox()`: 加载家庭下拉框

#### 3.3.6 TreeViewWidget
**文件位置**: [TreeViewWidget.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/TreeViewWidget.h), [TreeViewWidget.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/TreeViewWidget.cpp)

族谱树形视图组件，以树状结构展示家族关系。

**核心方法**:
- `setGenealogyId()`: 设置当前族谱ID
- `refreshTree()`: 刷新树视图
- `buildTreeFromRoot()`: 从根节点递归构建树
- `createPersonItem()`: 创建成员节点项
- `onExpandAll()`: 展开全部
- `onCollapseAll()`: 折叠全部

#### 3.3.7 AncestorQueryWidget
**文件位置**: [AncestorQueryWidget.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/AncestorQueryWidget.h), [AncestorQueryWidget.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/AncestorQueryWidget.cpp)

祖先查询组件，用于追溯某人的祖先血脉。

**核心方法**:
- `setGenealogyId()`: 设置当前族谱ID
- `onSearchAncestors()`: 执行祖先查询
- `displayAncestorTree()`: 显示祖先树

#### 3.3.8 RelationshipQueryWidget
**文件位置**: [RelationshipQueryWidget.h](file:///e:/DevelopProjects/DatabaseLab/code/Family/include/RelationshipQueryWidget.h), [RelationshipQueryWidget.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/RelationshipQueryWidget.cpp)

亲缘关系查询组件，查找两个人之间的关系。

**核心方法**:
- `setGenealogyId()`: 设置当前族谱ID
- `onSearchRelationship()`: 执行关系查询
- `determineRelationshipType()`: 判断关系类型

---

## 4. 数据库架构

### 4.1 实体关系图

```
┌─────────────┐       ┌──────────────────┐       ┌─────────────┐
│    users    │──1:N──│  user_genealogy  │──N:1──│ genealogies │
└─────────────┘       └──────────────────┘       └──────┬──────┘
                                                        │
                                                        │ 1:N
                                                        ▼
                                                 ┌───────────┐
                                                 │  families │
                                                 └─────┬─────┘
                                                       │
                                    ┌──────────────────┼──────────────────┐
                                    │                  │                  │
                                    ▼                  ▼                  ▼
                              ┌───────────┐      ┌───────────┐      ┌───────────┐
                              │ persons   │──1:N──│parent_child│──N:1──┘           │
                              └───────────┘      └───────────┘
```

### 4.2 数据表说明

#### 4.2.1 users 用户表
| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| user_id | SERIAL | PRIMARY KEY | 用户ID |
| username | VARCHAR(50) | NOT NULL, UNIQUE | 用户名 |
| password_hash | VARCHAR(255) | NOT NULL | 密码哈希(SHA-256) |
| email | VARCHAR(100) | | 邮箱 |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP | 创建时间 |

#### 4.2.2 genealogies 族谱表
| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| genealogy_id | SERIAL | PRIMARY KEY | 族谱ID |
| name | VARCHAR(100) | NOT NULL | 族谱名称 |
| surname | VARCHAR(50) | NOT NULL | 姓氏 |
| compile_time | DATE | | 编纂时间 |
| description | TEXT | | 族谱描述 |
| creator_id | INT | FOREIGN KEY | 创建者ID |
| created_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP | 创建时间 |

#### 4.2.3 user_genealogy 用户-族谱协作表
| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| user_id | INT | PRIMARY KEY, FOREIGN KEY | 用户ID |
| genealogy_id | INT | PRIMARY KEY, FOREIGN KEY | 族谱ID |
| role | VARCHAR(20) | DEFAULT 'editor' | 角色(creator/editor/viewer) |
| joined_at | TIMESTAMP | DEFAULT CURRENT_TIMESTAMP | 加入时间 |

#### 4.2.4 families 家庭表
| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| family_id | SERIAL | PRIMARY KEY | 家庭ID |
| husband_id | INT | FOREIGN KEY | 丈夫ID(可为NULL) |
| wife_id | INT | FOREIGN KEY | 妻子ID(可为NULL) |
| genealogy_id | INT | FOREIGN KEY | 所属族谱ID |
| marriage_year | INT | | 结婚年份 |

#### 4.2.5 persons 成员表
| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| person_id | SERIAL | PRIMARY KEY | 成员ID |
| name | VARCHAR(100) | NOT NULL | 姓名 |
| gender | CHAR(1) | CHECK IN ('M','F') | 性别 |
| birth_year | INT | CHECK > 0 | 出生年份 |
| death_year | INT | CHECK >= birth_year | 去世年份 |
| biography | TEXT | | 生平简介 |
| generation | INT | DEFAULT 1 | 辈分代数 |
| genealogy_id | INT | FOREIGN KEY | 所属族谱ID |
| birth_family_id | INT | FOREIGN KEY | 出生家庭ID |

#### 4.2.6 parent_child 亲子关系表
| 字段名 | 类型 | 约束 | 说明 |
|--------|------|------|------|
| family_id | INT | PRIMARY KEY, FOREIGN KEY | 家庭ID |
| child_id | INT | PRIMARY KEY, FOREIGN KEY | 子女ID |

---

## 5. 存储过程与函数

### 5.1 calculate_generation 自动计算辈分
**功能**: 根据父母的辈分自动计算子女的辈分

```sql
CREATE OR REPLACE FUNCTION calculate_generation(p_birth_family_id INT)
RETURNS INT AS $$
-- 逻辑: 返回父/母辈分 + 1
-- 如果无父母信息，返回1
$$ LANGUAGE plpgsql;
```

### 5.2 sp_get_ancestors 祖先追溯
**功能**: 递归追溯某人的所有祖先

**参数**: `p_person_id INT` - 成员ID

**返回**:
- person_id, name, gender, birth_year, death_year, generation
- level(祖先层级), path(追溯路径)

### 5.3 sp_get_descendants 后代追溯
**功能**: 递归追溯某人的所有后代

**参数**:
- `p_person_id INT` - 成员ID
- `max_depth INT DEFAULT 10` - 最大追溯深度

**返回**:
- person_id, name, gender, birth_year, death_year, generation
- level(后代层级), path(追溯路径)

### 5.4 sp_find_relationship 关系查找
**功能**: 查找两个人之间的亲缘关系

**参数**:
- `person1_id INT` - 第一人ID
- `person2_id INT` - 第二人ID

**返回**:
- common_ancestor_id, common_ancestor_name - 共同祖先
- generation - 辈分差
- path_to_person1, path_to_person2 - 到两人的路径

---

## 6. 触发器

### 6.1 trg_person_generation 辈分维护触发器
**时机**: INSERT或UPDATE birth_family_id时
**功能**: 自动调用calculate_generation更新辈分

### 6.2 trg_verify_birth_years 出生年份验证
**时机**: INSERT或UPDATE出生信息时
**功能**: 验证父母出生年份早于子女

---

## 7. 模块依赖关系

```
┌─────────────────────────────────────────────────────────────┐
│                        main.cpp                              │
│                   (应用程序入口点)                             │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                       MainWindow                             │
│              (依赖: LoginDialog, User)                       │
└──────────┬──────────┬──────────┬──────────┬────────────────┘
           │          │          │          │
           ▼          ▼          ▼          ▼
┌─────────────────┐ ┌────────┐ ┌────────┐ ┌────────────────┐
│DashboardWidget  │ │Genealog│ │Member  │ │ QueryWidgets   │
│                 │ │yManager│ │Manager │ │(TreeView,      │
│(依赖: Database  │ │        │ │        │ │ Ancestor,      │
│ Manager)        │ │        │ │        │ │ Relationship)  │
└─────────────────┘ └────────┘ └────────┘ └────────────────┘
                          │
                          ▼
┌─────────────────────────────────────────────────────────────┐
│                     DatabaseManager                          │
│               (单例模式，全局数据库访问)                        │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────────┐
│                  PostgreSQL Database                         │
│      (users, genealogies, persons, families表)               │
└─────────────────────────────────────────────────────────────┘
```

---

## 8. 构建与运行

### 8.1 环境要求

| 组件 | 版本要求 |
|------|----------|
| C++编译器 | 支持C++17 (MSVC/GCC/Clang) |
| CMake | >= 3.16 |
| Qt5 | >= 5.15 |
| PostgreSQL | >= 12 |
| PostgreSQL驱动 | QPSQL |

### 8.2 依赖安装

**Qt5安装**:
```bash
# Windows (使用vcpkg)
vcpkg install qt5

# 或从 Qt 官网下载安装包
```

**PostgreSQL安装**:
- 从 PostgreSQL官网 下载安装
- 确保 PostgreSQL 服务正在运行
- 记录用户名(默认postgres)和密码

### 8.3 CMake 构建方式

```bash
# 进入项目目录
cd Family

# 创建构建目录
mkdir build
cd build

# 配置项目
cmake .. -G "NMake Makefiles" ^ 
  -DQt5_DIR="C:/Qt/5.15.2/msvc2019/lib/cmake/Qt5" ^
  -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build . --config Release

# 运行
./FamilyGenealogy.exe
```

### 8.4 qmake 构建方式

```bash
# 进入项目目录
cd Family

# 使用qmake生成Makefile
qmake Family.pro

# 编译
make release

# 运行
./bin/FamilyGenealogy
```

### 8.5 数据库配置

应用程序默认连接配置（在 [main.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/main.cpp) 中定义）:

```cpp
db.setHostName("localhost");
db.setDatabaseName("family_genealogy");
db.setUserName("postgres");
db.setPassword("123456");  // 请根据实际修改
db.setPort(5432);
```

**修改数据库配置**:
编辑 [src/main.cpp](file:///e:/DevelopProjects/DatabaseLab/code/Family/src/main.cpp) 中的连接参数。

### 8.6 初始化数据库

首次运行程序时，`DatabaseManager::initializeDatabase()`会自动:
1. 检查数据库是否存在
2. 如不存在，创建 `family_genealogy` 数据库
3. 创建所有必要的表结构
4. 插入默认管理员账户

**默认账户**:
- 用户名: admin
- 密码: 123456 (SHA-256哈希)

---

## 9. 关键设计模式

### 9.1 单例模式 - DatabaseManager
```cpp
class DatabaseManager {
private:
    DatabaseManager() = default;
    
public:
    static DatabaseManager& instance() {
        static DatabaseManager instance;
        return instance;
    }
    
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;
};
```

### 9.2 MVC模式
- **Model**: User, Person, Genealogy, Family 数据类
- **View**: .ui 文件定义的Qt界面
- **Controller**: 各Widget类的业务逻辑方法

---

## 10. 常见问题排查

### 10.1 数据库连接失败
```
可能原因:
1. PostgreSQL服务未启动
2. 端口5432被占用或防火墙阻止
3. 用户名/密码错误
4. 权限不足

解决方案:
- 检查服务状态: services.msc → PostgreSQL
- 验证连接参数
- 检查pg_hba.conf配置
```

### 10.2 编译找不到Qt模块
```
解决方案:
- 确保Qt5已正确安装
- 设置Qt5_DIR环境变量
- 检查Qt5安装路径
```

### 10.3 数据库表创建失败
```
可能原因:
1. 数据库连接已存在但未打开
2. 已有同名数据库
3. 权限不足

解决方案:
- 手动删除旧数据库后重试
- 检查数据库用户权限
```

---

## 11. 扩展建议

### 11.1 功能扩展
1. **数据导出**: 导出族谱为PDF/Word格式
2. **图片管理**: 添加成员照片功能
3. **批量导入**: 支持从Excel批量导入成员
4. **数据备份**: 数据库定期备份功能

### 11.2 性能优化
1. 为高频查询添加索引
2. 使用数据库连接池
3. 实现数据缓存机制

### 11.3 安全增强
1. 密码使用bcrypt加密
2. 添加操作日志审计
3. 实现基于角色的访问控制(RBAC)

---

## 12. 相关文档

- [ER图](file:///e:/DevelopProjects/DatabaseLab/code/Family/ER图.md)
- [表定义](file:///e:/DevelopProjects/DatabaseLab/code/Family/表定义.md)
- [实现步骤大纲](file:///e:/DevelopProjects/DatabaseLab/code/Family/实现步骤大纲.md)
- [数据库架构脚本](file:///e:/DevelopProjects/DatabaseLab/code/Family/database/schema.sql)

---

*文档生成时间: 2026-05-11*
*项目版本: 1.0*
