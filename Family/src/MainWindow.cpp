#include "MainWindow.h"
#include "LoginDialog.h"
#include "DashboardWidget.h"
#include "GenealogyManager.h"
#include "MemberManager.h"
#include "TreeViewWidget.h"
#include "AncestorQueryWidget.h"
#include "RelationshipQueryWidget.h"
#include "SpouseQueryWidget.h"
#include "ChildrenQueryWidget.h"
#include "DatabaseManager.h"
#include "HelpDialog.h"
#include "ui_mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QDockWidget>
#include <QLabel>
#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_loginDialog(nullptr)
    , m_centralTabWidget(nullptr)
    , m_dashboardWidget(nullptr)
    , m_genealogyManager(nullptr)
    , m_memberManager(nullptr)
    , m_treeViewWidget(nullptr)
    , m_ancestorQueryWidget(nullptr)
    , m_relationshipQueryWidget(nullptr)
    , m_spouseQueryWidget(nullptr)
    , m_childrenQueryWidget(nullptr)
{
    setupUi();
    showLoginDialog();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    resize(1200, 800);
    createMenuBar();
    createStatusBar();

    m_centralTabWidget = new QTabWidget(this);
    setCentralWidget(m_centralTabWidget);
}

void MainWindow::createMenuBar()
{
    QMenuBar* menuBar = this->menuBar();

    QMenu* fileMenu = menuBar->addMenu("文件(&F)");

    QAction* logoutAction = new QAction("切换用户(&L)", this);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::showLoginDialog);
    fileMenu->addAction(logoutAction);

    fileMenu->addSeparator();

    QAction* exitAction = new QAction("退出(&X)", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    fileMenu->addAction(exitAction);

    QMenu* genealogyMenu = menuBar->addMenu("族谱(&G)");

    QAction* createGenealogyAction = new QAction("创建族谱(&N)", this);
    createGenealogyAction->setShortcut(QKeySequence::New);
    connect(createGenealogyAction, &QAction::triggered, this, [this]() {
        if (m_genealogyManager) {
            m_centralTabWidget->setCurrentWidget(m_genealogyManager);
            m_genealogyManager->onCreateGenealogy();
        }
    });
    genealogyMenu->addAction(createGenealogyAction);

    genealogyMenu->addSeparator();

    QAction* memberMgrAction = new QAction("成员管理(&M)", this);
    connect(memberMgrAction, &QAction::triggered, this, [this]() {
        if (m_memberManager) m_centralTabWidget->setCurrentWidget(m_memberManager);
    });
    genealogyMenu->addAction(memberMgrAction);

    QAction* treeViewAction = new QAction("树形预览(&T)", this);
    connect(treeViewAction, &QAction::triggered, this, [this]() {
        if (m_treeViewWidget) m_centralTabWidget->setCurrentWidget(m_treeViewWidget);
    });
    genealogyMenu->addAction(treeViewAction);

    genealogyMenu->addSeparator();

    QAction* ancestorAction = new QAction("祖先查询(&A)", this);
    connect(ancestorAction, &QAction::triggered, this, [this]() {
        if (m_ancestorQueryWidget) m_centralTabWidget->setCurrentWidget(m_ancestorQueryWidget);
    });
    genealogyMenu->addAction(ancestorAction);

    QAction* relationAction = new QAction("亲缘关系查询(&R)", this);
    connect(relationAction, &QAction::triggered, this, [this]() {
        if (m_relationshipQueryWidget) m_centralTabWidget->setCurrentWidget(m_relationshipQueryWidget);
    });
    genealogyMenu->addAction(relationAction);

    QMenu* helpMenu = menuBar->addMenu("帮助(&H)");

    QAction* helpAction = new QAction("帮助(&H)", this);
    helpAction->setShortcut(QKeySequence::HelpContents);
    connect(helpAction, &QAction::triggered, this, [this]() {
        HelpDialog helpDialog(this);
        helpDialog.exec();
    });
    helpMenu->addAction(helpAction);

    helpMenu->addSeparator();

    QAction* aboutAction = new QAction("关于(&A)", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "关于",
            "寻根溯源-族谱管理系统\n\n"
            "Version beta1.0\n");
    });
    helpMenu->addAction(aboutAction);
}



void MainWindow::createStatusBar()
{
    QStatusBar* statusBar = this->statusBar();
    QLabel* userLabel = new QLabel("未登录", this);
    statusBar->addPermanentWidget(userLabel);

    connect(this, &MainWindow::userLoggedIn, this, [userLabel](const User& user) {
        userLabel->setText(QString("当前用户: %1").arg(user.username()));
    });

    connect(this, &MainWindow::userLoggedOut, this, [userLabel]() {
        userLabel->setText("未登录");
    });
}

void MainWindow::setCurrentUser(const User& user)
{
    m_currentUser = user;
    updateWindowTitle();
    emit userLoggedIn(user);
}

void MainWindow::onLoginSuccess(const User& user)
{
    setCurrentUser(user);

    m_genealogyManager = new GenealogyManager(user, this);
    m_centralTabWidget->addTab(m_genealogyManager, "族谱管理");

    QVariantList genealogies = DatabaseManager::instance().getGenealogiesForUser(user.id());
    int firstGenealogyId = 0;
    
    // 只有非 admin 用户才设置 firstGenealogyId 为第一个族谱 ID
    if (user.username() != "admin" && !genealogies.isEmpty()) {
        firstGenealogyId = genealogies.first().toMap()["genealogy_id"].toInt();
    }
    // admin 用户 firstGenealogyId 保持为 0，表示查看所有族谱的数据

    m_dashboardWidget = new DashboardWidget(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_dashboardWidget, "仪表盘");

    m_memberManager = new MemberManager(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_memberManager, "成员管理");

    m_treeViewWidget = new TreeViewWidget(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_treeViewWidget, "树形预览");

    m_ancestorQueryWidget = new AncestorQueryWidget(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_ancestorQueryWidget, "祖先查询");

    m_relationshipQueryWidget = new RelationshipQueryWidget(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_relationshipQueryWidget, "亲缘关系");

    m_spouseQueryWidget = new SpouseQueryWidget(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_spouseQueryWidget, "配偶查询");

    m_childrenQueryWidget = new ChildrenQueryWidget(firstGenealogyId, this);
    m_centralTabWidget->addTab(m_childrenQueryWidget, "子女查询");

    statusBar()->showMessage(QString("欢迎, %1!").arg(user.username()), 3000);
    
    show();
}

void MainWindow::onLogout()
{
    m_currentUser = User();
    emit userLoggedOut();
    showLoginDialog();
}

void MainWindow::onGenealogySelected(int genealogyId)
{
    if (m_dashboardWidget) m_dashboardWidget->refreshData();
    if (m_memberManager) m_memberManager->setGenealogyId(genealogyId);
    if (m_treeViewWidget) m_treeViewWidget->setGenealogyId(genealogyId);
    if (m_ancestorQueryWidget) m_ancestorQueryWidget->setGenealogyId(genealogyId);
    if (m_relationshipQueryWidget) m_relationshipQueryWidget->setGenealogyId(genealogyId);
}

void MainWindow::showLoginDialog()
{
    hide();

    if (!m_loginDialog) {
        m_loginDialog = new LoginDialog(this);
    }

    m_loginDialog->setModal(true);

    QObject::disconnect(m_loginDialog, &LoginDialog::loginSuccess, this, nullptr);
    connect(m_loginDialog, &LoginDialog::loginSuccess, this, &MainWindow::onLoginSuccess);

    while (m_centralTabWidget && m_centralTabWidget->count() > 0) {
        QWidget* tab = m_centralTabWidget->widget(0);
        m_centralTabWidget->removeTab(0);
        delete tab;
    }
    m_dashboardWidget = nullptr;
    m_genealogyManager = nullptr;
    m_memberManager = nullptr;
    m_treeViewWidget = nullptr;
    m_ancestorQueryWidget = nullptr;
    m_relationshipQueryWidget = nullptr;

    m_loginDialog->show();
}

void MainWindow::updateWindowTitle()
{
    if (m_currentUser.id() > 0) {
        setWindowTitle(QString("寻根溯源 - 族谱管理系统 [%1]").arg(m_currentUser.username()));
    } else {
        setWindowTitle("寻根溯源 - 族谱管理系统");
    }
}
