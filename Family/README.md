# 寻根溯源 —— 族谱管理系统

基于 **C++ / Qt5 / PostgreSQL** 开发的桌面族谱管理系统，用于管理族谱、家族成员及亲属关系，提供用户认证、族谱管理、成员管理、族谱树展示、祖先与后代查询、亲属关系查询以及数据统计等功能。

## 技术栈

* **开发语言：** C++17
* **GUI 框架：** Qt 5
* **数据库：** PostgreSQL
* **数据库访问：** Qt SQL / QPSQL
* **构建工具：** CMake、qmake
* **数据生成：** Python 3
* **版本管理：** Git

## 功能介绍

### 用户认证

* 用户注册与登录
* 用户名唯一性检查
* 密码 SHA-256 哈希处理
* 登录状态管理
* 用户与族谱关联管理
* 族谱协作者及角色管理

### 族谱管理

* 创建族谱
* 修改族谱信息
* 删除族谱
* 查看当前用户参与的族谱
* 管理族谱协作者

### 成员管理

* 添加、修改、删除成员
* 成员信息查询
* 按姓名进行模糊搜索
* 分页查询成员数据
* 管理成员与族谱之间的关系

### 族谱树

* 以树形结构展示家族关系
* 支持节点展开与折叠
* 按需加载族谱节点
* 使用集合进行节点去重，避免重复加载

### 关系查询

系统提供多种家族关系查询功能：

* 祖先查询
* 后代查询
* 配偶查询
* 亲属关系查询
* 共同祖先查询
* 亲属关系路径查询

### 数据统计

Dashboard 提供族谱数据统计，包括：

* 族谱数量
* 成员数量
* 男女成员比例
* 代数统计
* 平均寿命等统计信息

## 数据库设计

项目使用 PostgreSQL 存储用户、族谱及成员关系数据，主要数据表包括：

| 数据表              | 说明            |
| ---------------- | ------------- |
| `users`          | 用户信息          |
| `genealogies`    | 族谱信息          |
| `user_genealogy` | 用户与族谱的关联及角色信息 |
| `persons`        | 族谱成员信息        |
| `families`       | 家庭及婚姻关系       |

数据库通过主键、外键、唯一约束、检查约束等机制保证数据完整性。

### 族谱关系查询

针对多代族谱关系，数据库中使用 PostgreSQL 的 **递归 CTE** 及 **PL/pgSQL 函数**实现：

* 祖先递归查询
* 后代递归查询
* 两人关系查询
* 共同祖先查询
* 关系路径分析

主要关系查询函数包括：

```text
sp_get_ancestors()
sp_get_descendants()
sp_find_relationship()
```

## 项目结构

```text
sql/
├── Family/
│   ├── CMakeLists.txt            # CMake 构建配置
│   ├── Family.pro               # qmake 项目文件
│   ├── main.cpp                 # 程序入口
│   ├── mainwindow.cpp
│   ├── mainwindow.ui
│   │
│   ├── include/                 # 头文件
│   │   ├── DatabaseManager.h
│   │   ├── LoginDialog.h
│   │   ├── MainWindow.h
│   │   ├── GenealogyManager.h
│   │   ├── MemberManager.h
│   │   ├── TreeViewWidget.h
│   │   ├── AncestorQueryWidget.h
│   │   ├── ChildrenQueryWidget.h
│   │   ├── SpouseQueryWidget.h
│   │   ├── RelationshipQueryWidget.h
│   │   └── DashboardWidget.h
│   │
│   ├── src/                     # C++ 源文件
│   │   ├── DatabaseManager.cpp
│   │   ├── LoginDialog.cpp
│   │   ├── MainWindow.cpp
│   │   ├── GenealogyManager.cpp
│   │   ├── MemberManager.cpp
│   │   ├── TreeViewWidget.cpp
│   │   ├── AncestorQueryWidget.cpp
│   │   ├── ChildrenQueryWidget.cpp
│   │   ├── SpouseQueryWidget.cpp
│   │   ├── RelationshipQueryWidget.cpp
│   │   └── DashboardWidget.cpp
│   │
│   ├── ui/                      # Qt Designer UI 文件
│   │   ├── login_dialog.ui
│   │   ├── dashboard_widget.ui
│   │   ├── genealogy_manager.ui
│   │   ├── member_manager.ui
│   │   ├── tree_view_widget.ui
│   │   ├── ancestor_query_widget.ui
│   │   ├── relationship_query_widget.ui
│   │   ├── spouse_query_widget.ui
│   │   └── children_query_widget.ui
│   │
│   ├── resources/               # Qt 资源文件
│   │
│   └── database/                # 数据库脚本及测试数据
│       ├── schema.sql
│       ├── generate_data.py
│       ├── import_data.sql
│       ├── fix_data.sql
│       ├── update_procedures.sql
│       ├── query_for_report.sql
│       └── csv_data/
│
├── README.md                    # 项目说明
└── README.en.md                 # English README
```

## 环境要求

### 开发环境

* Qt 5.15+
* C++17 编译器
* CMake 3.16+
* PostgreSQL
* Python 3.8+（仅用于生成测试数据）

### Qt 数据库驱动

项目通过 Qt SQL 的 **QPSQL** 驱动连接 PostgreSQL。

运行程序前需要确保当前 Qt 环境已经安装对应的 PostgreSQL 驱动。

## 数据库配置

程序通过 `config.ini` 配置 PostgreSQL 连接参数。

首次运行时可以根据本机环境修改：

```ini
[Database]
Host=localhost
Port=5432
Name=family_genealogy
User=postgres
Password=your_password
```

其中：

* `Host`：PostgreSQL 服务地址
* `Port`：PostgreSQL 服务端口，默认 `5432`
* `Name`：数据库名称
* `User`：数据库用户名
* `Password`：数据库密码

请确保 PostgreSQL 服务已经启动，并且对应用户拥有数据库访问权限。

## 创建数据库

首先创建项目使用的数据库：

```sql
CREATE DATABASE family_genealogy;
```

然后进入数据库：

```bash
psql -U postgres -d family_genealogy
```

执行项目中的数据库初始化脚本：

```bash
psql -U postgres -d family_genealogy -f Family/database/schema.sql
```

根据需要，还可以继续执行项目中的其他 SQL 脚本。

## 构建项目

### 使用 CMake

进入 `Family` 目录：

```bash
cd Family
```

创建构建目录：

```bash
mkdir build
cd build
```

执行 CMake 配置：

```bash
cmake ..
```

编译：

```bash
cmake --build . --config Release
```

生成可执行文件后即可运行。

### 使用 Qt Creator

也可以直接使用 Qt Creator 打开：

```text
Family/Family.pro
```

配置 Qt、编译器以及 PostgreSQL 环境后进行构建和运行。

## 运行

启动 PostgreSQL 服务并完成数据库配置后运行程序：

```bash
./FamilyGenealogy
```

Windows 环境下可直接运行生成的：

```text
FamilyGenealogy.exe
```

## 测试数据

项目提供 Python 脚本用于生成大规模族谱测试数据：

```bash
cd Family/database
python generate_data.py
```

脚本会生成：

* **10 个族谱**
* 第 1 个族谱约 **60000 名成员**
* 其他族谱分别生成约 **6000 名成员**
* 总成员数量达到 **10 万级**
* 最大支持约 **35 代**族谱数据

生成的数据包括：

```text
persons.csv
families.csv
genealogies.csv
users.csv
user_genealogy.csv
```

生成的数据保存在：

```text
Family/database/csv_data/
```

可结合数据库导入脚本将测试数据导入 PostgreSQL，用于测试大规模数据下的成员查询、族谱树展示、关系查询及统计功能。

## 核心设计

### 前端架构

使用 Qt5 构建桌面 GUI，基于：

```text
QMainWindow
    │
    └── QTabWidget
          ├── 族谱管理
          ├── 成员管理
          ├── 族谱树
          ├── 关系查询
          └── 数据统计
```

不同功能通过独立 Widget 进行模块化管理。

### 数据访问层

通过 `DatabaseManager` 对数据库访问进行统一封装：

* 单例数据库管理
* PostgreSQL 连接管理
* SQL 查询执行
* 预处理 SQL
* 数据库初始化
* 用户及族谱相关数据访问

### 族谱关系处理

通过 `persons` 和 `families` 等数据表建立成员关系，并使用 PostgreSQL 的递归查询处理多代族谱关系。

查询结果再由 Qt 前端转换为树形结构或关系路径进行展示。

## 项目亮点

* 基于 Qt5 完成模块化桌面应用开发
* 使用 PostgreSQL 管理复杂族谱关系数据
* 使用递归 CTE 处理多级祖先、后代及亲属关系
* 使用延迟加载方式展示族谱树
* 使用集合去重减少重复节点加载
* 提供 10 万级测试数据生成脚本
* 同时支持 CMake 与 qmake 构建

## License

本项目主要用于课程设计、学习及个人项目实践。
