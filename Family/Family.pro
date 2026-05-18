#-------------------------------------------------
#
# Project created by QtCreator 2024-01-01
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FamilyGenealogy
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# Source files
SOURCES += \
    src/main.cpp \
    src/DatabaseManager.cpp \
    src/User.cpp \
    src/Genealogy.cpp \
    src/Person.cpp \
    src/Family.cpp \
    src/MainWindow.cpp \
    src/LoginDialog.cpp \
    src/DashboardWidget.cpp \
    src/GenealogyManager.cpp \
    src/MemberManager.cpp \
    src/TreeViewWidget.cpp \
    src/AncestorQueryWidget.cpp \
    src/RelationshipQueryWidget.cpp \
    src/SpouseQueryWidget.cpp \
    src/ChildrenQueryWidget.cpp \
    src/HelpDialog.cpp

# Header files
HEADERS += \
    include/DatabaseManager.h \
    include/User.h \
    include/Genealogy.h \
    include/Person.h \
    include/Family.h \
    include/MainWindow.h \
    include/LoginDialog.h \
    include/DashboardWidget.h \
    include/GenealogyManager.h \
    include/MemberManager.h \
    include/TreeViewWidget.h \
    include/AncestorQueryWidget.h \
    include/RelationshipQueryWidget.h \
    include/SpouseQueryWidget.h \
    include/ChildrenQueryWidget.h \
    include/HelpDialog.h

# UI files
FORMS += \
    ui/login_dialog.ui \
    ui/dashboard_widget.ui \
    ui/genealogy_manager.ui \
    ui/member_manager.ui \
    ui/tree_view_widget.ui \
    ui/ancestor_query_widget.ui \
    ui/relationship_query_widget.ui \
    ui/spouse_query_widget.ui \
    ui/children_query_widget.ui \
    ui/mainwindow.ui

# Resources
RESOURCES += \
    resources/resources.qrc

# Include paths
INCLUDEPATH += $$PWD/include

# Build directory
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build
MOC_DIR = $$PWD/build
RCC_DIR = $$PWD/build
UI_DIR = $$PWD/build
