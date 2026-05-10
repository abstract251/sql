#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "MainWindow.h"
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("寻根溯源 - 族谱管理系统");
    app.setApplicationVersion("1.0");
    app.setStyle("fusion");

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("family_genealogy");
    db.setUserName("postgres");
    db.setPassword("123456");
    db.setPort(5432);

    DatabaseManager::instance().setDatabase(db);

    if (!DatabaseManager::instance().checkAndCreateDatabase()) {
        QString errorMsg = DatabaseManager::instance().lastError();
        QMessageBox::critical(nullptr, "数据库创建失败",
            QString("无法创建数据库，请检查以下事项：\n\n"
                    "1. PostgreSQL服务是否正在运行\n"
                    "2. 数据库连接参数是否正确\n"
                    "3. 用户是否有权限创建数据库\n\n"
                    "错误详情：%1").arg(errorMsg));
        return 1;
    }

    if (!db.open()) {
        QString err = db.lastError().text();
        QMessageBox::critical(nullptr, "数据库连接错误",
            QString("无法连接到PostgreSQL数据库:\n%1\n\n请确保PostgreSQL服务正在运行。").arg(err));
        return 1;
    }

    if (!DatabaseManager::instance().initializeDatabase()) {
        QMessageBox::critical(nullptr, "数据库初始化失败",
            QString("无法初始化数据库表结构，请检查SQL文件是否存在。"));
        return 1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}
