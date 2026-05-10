/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLogout;
    QAction *actionExit;
    QAction *actionCreateGenealogy;
    QAction *actionMemberManager;
    QAction *actionTreeView;
    QAction *actionAncestorQuery;
    QAction *actionRelationQuery;
    QAction *actionAbout;
    QAction *actionDashboard;
    QAction *actionGenealogyMgr;
    QAction *actionMemberMgr;
    QAction *actionTreePreview;
    QAction *actionAncestor;
    QAction *actionRelation;
    QAction *actionExportCSV;
    QAction *actionExportSQL;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuExport;
    QMenu *menuGenealogy;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QTabWidget *centralTabWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        actionLogout = new QAction(MainWindow);
        actionLogout->setObjectName("actionLogout");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionCreateGenealogy = new QAction(MainWindow);
        actionCreateGenealogy->setObjectName("actionCreateGenealogy");
        actionMemberManager = new QAction(MainWindow);
        actionMemberManager->setObjectName("actionMemberManager");
        actionTreeView = new QAction(MainWindow);
        actionTreeView->setObjectName("actionTreeView");
        actionAncestorQuery = new QAction(MainWindow);
        actionAncestorQuery->setObjectName("actionAncestorQuery");
        actionRelationQuery = new QAction(MainWindow);
        actionRelationQuery->setObjectName("actionRelationQuery");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionDashboard = new QAction(MainWindow);
        actionDashboard->setObjectName("actionDashboard");
        actionGenealogyMgr = new QAction(MainWindow);
        actionGenealogyMgr->setObjectName("actionGenealogyMgr");
        actionMemberMgr = new QAction(MainWindow);
        actionMemberMgr->setObjectName("actionMemberMgr");
        actionTreePreview = new QAction(MainWindow);
        actionTreePreview->setObjectName("actionTreePreview");
        actionAncestor = new QAction(MainWindow);
        actionAncestor->setObjectName("actionAncestor");
        actionRelation = new QAction(MainWindow);
        actionRelation->setObjectName("actionRelation");
        actionExportCSV = new QAction(MainWindow);
        actionExportCSV->setObjectName("actionExportCSV");
        actionExportSQL = new QAction(MainWindow);
        actionExportSQL->setObjectName("actionExportSQL");
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1200, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        menuExport = new QMenu(menuFile);
        menuExport->setObjectName("menuExport");
        menuGenealogy = new QMenu(menuBar);
        menuGenealogy->setObjectName("menuGenealogy");
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        mainToolBar->setMovable(false);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        centralTabWidget = new QTabWidget(MainWindow);
        centralTabWidget->setObjectName("centralTabWidget");
        centralTabWidget->setDocumentMode(false);
        MainWindow->setCentralWidget(centralTabWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        statusBar->setSizeGripEnabled(true);
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuGenealogy->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLogout);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuFile->addAction(menuExport->menuAction());
        menuExport->addAction(actionExportCSV);
        menuExport->addAction(actionExportSQL);
        menuGenealogy->addAction(actionCreateGenealogy);
        menuGenealogy->addSeparator();
        menuGenealogy->addAction(actionMemberManager);
        menuGenealogy->addAction(actionTreeView);
        menuGenealogy->addSeparator();
        menuGenealogy->addAction(actionAncestorQuery);
        menuGenealogy->addAction(actionRelationQuery);
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(actionDashboard);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionGenealogyMgr);
        mainToolBar->addAction(actionMemberMgr);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionTreePreview);
        mainToolBar->addAction(actionAncestor);
        mainToolBar->addAction(actionRelation);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\257\273\346\240\271\346\272\257\346\272\220 - \346\227\217\350\260\261\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        actionLogout->setText(QCoreApplication::translate("MainWindow", "\345\210\207\346\215\242\347\224\250\346\210\267(&L)", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272(&X)", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCreateGenealogy->setText(QCoreApplication::translate("MainWindow", "\345\210\233\345\273\272\346\227\217\350\260\261(&N)", nullptr));
#if QT_CONFIG(shortcut)
        actionCreateGenealogy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionMemberManager->setText(QCoreApplication::translate("MainWindow", "\346\210\220\345\221\230\347\256\241\347\220\206(&M)", nullptr));
        actionTreeView->setText(QCoreApplication::translate("MainWindow", "\346\240\221\345\275\242\351\242\204\350\247\210(&T)", nullptr));
        actionAncestorQuery->setText(QCoreApplication::translate("MainWindow", "\347\245\226\345\205\210\346\237\245\350\257\242(&A)", nullptr));
        actionRelationQuery->setText(QCoreApplication::translate("MainWindow", "\344\272\262\347\274\230\345\205\263\347\263\273\346\237\245\350\257\242(&R)", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216(&A)", nullptr));
        actionDashboard->setText(QCoreApplication::translate("MainWindow", "\344\273\252\350\241\250\347\233\230", nullptr));
        actionGenealogyMgr->setText(QCoreApplication::translate("MainWindow", "\346\227\217\350\260\261\347\256\241\347\220\206", nullptr));
        actionMemberMgr->setText(QCoreApplication::translate("MainWindow", "\346\210\220\345\221\230\347\256\241\347\220\206", nullptr));
        actionTreePreview->setText(QCoreApplication::translate("MainWindow", "\346\240\221\345\275\242\351\242\204\350\247\210", nullptr));
        actionAncestor->setText(QCoreApplication::translate("MainWindow", "\347\245\226\345\205\210\346\237\245\350\257\242", nullptr));
        actionRelation->setText(QCoreApplication::translate("MainWindow", "\344\272\262\347\274\230\346\237\245\350\257\242", nullptr));
        actionExportCSV->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272CSV", nullptr));
        actionExportSQL->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272SQL", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
        menuExport->setTitle(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272", nullptr));
        menuGenealogy->setTitle(QCoreApplication::translate("MainWindow", "\346\227\217\350\260\261(&G)", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "\344\270\273\345\267\245\345\205\267\346\240\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
