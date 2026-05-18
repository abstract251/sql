cmake_minimum_required(VERSION 3.16)
project(FamilyGenealogy VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Network)
find_package(MySQL)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/DatabaseManager.cpp
    src/User.cpp
    src/Genealogy.cpp
    src/Person.cpp
    src/Family.cpp
    src/MainWindow.cpp
    src/LoginDialog.cpp
    src/DashboardWidget.cpp
    src/GenealogyManager.cpp
    src/MemberManager.cpp
    src/TreeViewWidget.cpp
    src/AncestorQueryWidget.cpp
    src/RelationshipQueryWidget.cpp
    src/SpouseQueryWidget.cpp
    src/ChildrenQueryWidget.cpp
    src/HelpDialog.cpp
)

set(HEADERS
    include/DatabaseManager.h
    include/User.h
    include/Genealogy.h
    include/Person.h
    include/Family.h
    include/MainWindow.h
    include/LoginDialog.h
    include/DashboardWidget.h
    include/GenealogyManager.h
    include/MemberManager.h
    include/TreeViewWidget.h
    include/AncestorQueryWidget.h
    include/RelationshipQueryWidget.h
    include/SpouseQueryWidget.h
    include/ChildrenQueryWidget.h
    include/HelpDialog.h
)

set(FORMS
    ui/login_dialog.ui
    ui/dashboard_widget.ui
    ui/genealogy_manager.ui
    ui/member_manager.ui
    ui/tree_view_widget.ui
    ui/ancestor_query_widget.ui
    ui/relationship_query_widget.ui
    ui/spouse_query_widget.ui
    ui/children_query_widget.ui
)

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${FORMS}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Sql
    Qt5::Network
)

if(MySQL_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${MySQL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${MySQL_LIBRARY})
endif()

qt5_use_modules(${PROJECT_NAME} Widgets Sql Network)
