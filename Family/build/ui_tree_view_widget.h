/********************************************************************************
** Form generated from reading UI file 'tree_view_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREE_VIEW_WIDGET_H
#define UI_TREE_VIEW_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TreeViewWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *refreshPushButton;
    QPushButton *expandAllPushButton;
    QPushButton *collapseAllPushButton;
    QSpacerItem *horizontalSpacer;
    QTreeView *treeView;

    void setupUi(QWidget *TreeViewWidget)
    {
        if (TreeViewWidget->objectName().isEmpty())
            TreeViewWidget->setObjectName("TreeViewWidget");
        TreeViewWidget->resize(900, 600);
        verticalLayout = new QVBoxLayout(TreeViewWidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        refreshPushButton = new QPushButton(TreeViewWidget);
        refreshPushButton->setObjectName("refreshPushButton");

        horizontalLayout->addWidget(refreshPushButton);

        expandAllPushButton = new QPushButton(TreeViewWidget);
        expandAllPushButton->setObjectName("expandAllPushButton");

        horizontalLayout->addWidget(expandAllPushButton);

        collapseAllPushButton = new QPushButton(TreeViewWidget);
        collapseAllPushButton->setObjectName("collapseAllPushButton");

        horizontalLayout->addWidget(collapseAllPushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        treeView = new QTreeView(TreeViewWidget);
        treeView->setObjectName("treeView");
        treeView->setAnimated(true);
        treeView->setHeaderHidden(false);
        treeView->setIndentation(20);

        verticalLayout->addWidget(treeView);


        retranslateUi(TreeViewWidget);

        QMetaObject::connectSlotsByName(TreeViewWidget);
    } // setupUi

    void retranslateUi(QWidget *TreeViewWidget)
    {
        TreeViewWidget->setWindowTitle(QCoreApplication::translate("TreeViewWidget", "\346\240\221\345\275\242\351\242\204\350\247\210", nullptr));
        refreshPushButton->setText(QCoreApplication::translate("TreeViewWidget", "\345\210\267\346\226\260\346\240\221", nullptr));
        expandAllPushButton->setText(QCoreApplication::translate("TreeViewWidget", "\345\261\225\345\274\200\345\205\250\351\203\250", nullptr));
        collapseAllPushButton->setText(QCoreApplication::translate("TreeViewWidget", "\346\212\230\345\217\240\345\205\250\351\203\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TreeViewWidget: public Ui_TreeViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREE_VIEW_WIDGET_H
