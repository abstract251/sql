#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QItemSelection>
#include "User.h"

class LoginDialog;
class DashboardWidget;
class GenealogyManager;
class MemberManager;
class TreeViewWidget;
class AncestorQueryWidget;
class RelationshipQueryWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setCurrentUser(const User& user);
    User currentUser() const { return m_currentUser; }
    bool isLoggedIn() const { return m_currentUser.id() > 0; }

signals:
    void userLoggedIn(const User& user);
    void userLoggedOut();

private slots:
    void onLoginSuccess(const User& user);
    void onLogout();
    void onGenealogySelected(int genealogyId);
    void showLoginDialog();

private:
    void setupUi();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    void createDockWidgets();
    void connectSignals();
    void updateWindowTitle();

    User m_currentUser;
    LoginDialog* m_loginDialog;

    QTabWidget* m_centralTabWidget;
    DashboardWidget* m_dashboardWidget;
    GenealogyManager* m_genealogyManager;
    MemberManager* m_memberManager;
    TreeViewWidget* m_treeViewWidget;
    AncestorQueryWidget* m_ancestorQueryWidget;
    RelationshipQueryWidget* m_relationshipQueryWidget;
};

#endif
