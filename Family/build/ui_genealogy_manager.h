/********************************************************************************
** Form generated from reading UI file 'genealogy_manager.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENEALOGY_MANAGER_H
#define UI_GENEALOGY_MANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GenealogyManager
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *createPushButton;
    QPushButton *editPushButton;
    QPushButton *deletePushButton;
    QPushButton *invitePushButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *refreshPushButton;
    QTableView *genealogyTableView;

    void setupUi(QWidget *GenealogyManager)
    {
        if (GenealogyManager->objectName().isEmpty())
            GenealogyManager->setObjectName("GenealogyManager");
        GenealogyManager->resize(900, 600);
        verticalLayout = new QVBoxLayout(GenealogyManager);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        createPushButton = new QPushButton(GenealogyManager);
        createPushButton->setObjectName("createPushButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        createPushButton->setIcon(icon);

        horizontalLayout->addWidget(createPushButton);

        editPushButton = new QPushButton(GenealogyManager);
        editPushButton->setObjectName("editPushButton");

        horizontalLayout->addWidget(editPushButton);

        deletePushButton = new QPushButton(GenealogyManager);
        deletePushButton->setObjectName("deletePushButton");

        horizontalLayout->addWidget(deletePushButton);

        invitePushButton = new QPushButton(GenealogyManager);
        invitePushButton->setObjectName("invitePushButton");

        horizontalLayout->addWidget(invitePushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        refreshPushButton = new QPushButton(GenealogyManager);
        refreshPushButton->setObjectName("refreshPushButton");

        horizontalLayout->addWidget(refreshPushButton);


        verticalLayout->addLayout(horizontalLayout);

        genealogyTableView = new QTableView(GenealogyManager);
        genealogyTableView->setObjectName("genealogyTableView");
        genealogyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        genealogyTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        genealogyTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(genealogyTableView);


        retranslateUi(GenealogyManager);

        QMetaObject::connectSlotsByName(GenealogyManager);
    } // setupUi

    void retranslateUi(QWidget *GenealogyManager)
    {
        GenealogyManager->setWindowTitle(QCoreApplication::translate("GenealogyManager", "\346\227\217\350\260\261\347\256\241\347\220\206", nullptr));
        createPushButton->setText(QCoreApplication::translate("GenealogyManager", "\345\210\233\345\273\272\346\227\217\350\260\261", nullptr));
        editPushButton->setText(QCoreApplication::translate("GenealogyManager", "\347\274\226\350\276\221\346\227\217\350\260\261", nullptr));
        deletePushButton->setText(QCoreApplication::translate("GenealogyManager", "\345\210\240\351\231\244\346\227\217\350\260\261", nullptr));
        invitePushButton->setText(QCoreApplication::translate("GenealogyManager", "\351\202\200\350\257\267\347\224\250\346\210\267", nullptr));
        refreshPushButton->setText(QCoreApplication::translate("GenealogyManager", "\345\210\267\346\226\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GenealogyManager: public Ui_GenealogyManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENEALOGY_MANAGER_H
