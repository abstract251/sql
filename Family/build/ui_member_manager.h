/********************************************************************************
** Form generated from reading UI file 'member_manager.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMBER_MANAGER_H
#define UI_MEMBER_MANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MemberManager
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *searchLineEdit;
    QPushButton *searchPushButton;
    QPushButton *clearSearchPushButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *addPushButton;
    QPushButton *editPushButton;
    QPushButton *deletePushButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *refreshPushButton;
    QTableView *memberTableView;

    void setupUi(QWidget *MemberManager)
    {
        if (MemberManager->objectName().isEmpty())
            MemberManager->setObjectName("MemberManager");
        MemberManager->resize(900, 600);
        verticalLayout = new QVBoxLayout(MemberManager);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        searchLineEdit = new QLineEdit(MemberManager);
        searchLineEdit->setObjectName("searchLineEdit");

        horizontalLayout->addWidget(searchLineEdit);

        searchPushButton = new QPushButton(MemberManager);
        searchPushButton->setObjectName("searchPushButton");

        horizontalLayout->addWidget(searchPushButton);

        clearSearchPushButton = new QPushButton(MemberManager);
        clearSearchPushButton->setObjectName("clearSearchPushButton");

        horizontalLayout->addWidget(clearSearchPushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        addPushButton = new QPushButton(MemberManager);
        addPushButton->setObjectName("addPushButton");

        horizontalLayout_2->addWidget(addPushButton);

        editPushButton = new QPushButton(MemberManager);
        editPushButton->setObjectName("editPushButton");

        horizontalLayout_2->addWidget(editPushButton);

        deletePushButton = new QPushButton(MemberManager);
        deletePushButton->setObjectName("deletePushButton");

        horizontalLayout_2->addWidget(deletePushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        refreshPushButton = new QPushButton(MemberManager);
        refreshPushButton->setObjectName("refreshPushButton");

        horizontalLayout_2->addWidget(refreshPushButton);


        verticalLayout->addLayout(horizontalLayout_2);

        memberTableView = new QTableView(MemberManager);
        memberTableView->setObjectName("memberTableView");
        memberTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        memberTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        memberTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(memberTableView);


        retranslateUi(MemberManager);

        QMetaObject::connectSlotsByName(MemberManager);
    } // setupUi

    void retranslateUi(QWidget *MemberManager)
    {
        MemberManager->setWindowTitle(QCoreApplication::translate("MemberManager", "\346\210\220\345\221\230\347\256\241\347\220\206", nullptr));
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MemberManager", "\350\276\223\345\205\245\345\247\223\345\220\215\350\277\233\350\241\214\346\250\241\347\263\212\346\220\234\347\264\242...", nullptr));
        searchPushButton->setText(QCoreApplication::translate("MemberManager", "\346\220\234\347\264\242", nullptr));
        clearSearchPushButton->setText(QCoreApplication::translate("MemberManager", "\346\270\205\351\231\244", nullptr));
        addPushButton->setText(QCoreApplication::translate("MemberManager", "\346\267\273\345\212\240\346\210\220\345\221\230", nullptr));
        editPushButton->setText(QCoreApplication::translate("MemberManager", "\347\274\226\350\276\221\346\210\220\345\221\230", nullptr));
        deletePushButton->setText(QCoreApplication::translate("MemberManager", "\345\210\240\351\231\244\346\210\220\345\221\230", nullptr));
        refreshPushButton->setText(QCoreApplication::translate("MemberManager", "\345\210\267\346\226\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MemberManager: public Ui_MemberManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEMBER_MANAGER_H
