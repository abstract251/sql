/********************************************************************************
** Form generated from reading UI file 'relationship_query_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RELATIONSHIP_QUERY_WIDGET_H
#define UI_RELATIONSHIP_QUERY_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RelationshipQueryWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *searchPushButton;
    QPushButton *clearPushButton;
    QTableView *resultTableView;

    void setupUi(QWidget *RelationshipQueryWidget)
    {
        if (RelationshipQueryWidget->objectName().isEmpty())
            RelationshipQueryWidget->setObjectName("RelationshipQueryWidget");
        RelationshipQueryWidget->resize(900, 600);
        verticalLayout = new QVBoxLayout(RelationshipQueryWidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(RelationshipQueryWidget);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        searchPushButton = new QPushButton(groupBox);
        searchPushButton->setObjectName("searchPushButton");

        horizontalLayout->addWidget(searchPushButton);

        clearPushButton = new QPushButton(groupBox);
        clearPushButton->setObjectName("clearPushButton");

        horizontalLayout->addWidget(clearPushButton);


        verticalLayout->addWidget(groupBox);

        resultTableView = new QTableView(RelationshipQueryWidget);
        resultTableView->setObjectName("resultTableView");
        resultTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        resultTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(resultTableView);


        retranslateUi(RelationshipQueryWidget);

        QMetaObject::connectSlotsByName(RelationshipQueryWidget);
    } // setupUi

    void retranslateUi(QWidget *RelationshipQueryWidget)
    {
        RelationshipQueryWidget->setWindowTitle(QCoreApplication::translate("RelationshipQueryWidget", "\344\272\262\347\274\230\345\205\263\347\263\273\346\237\245\350\257\242", nullptr));
        groupBox->setTitle(QCoreApplication::translate("RelationshipQueryWidget", "\346\237\245\350\257\242\346\235\241\344\273\266", nullptr));
        label->setText(QCoreApplication::translate("RelationshipQueryWidget", "\350\276\223\345\205\245\344\270\244\344\270\252\346\210\220\345\221\230ID\346\237\245\350\257\242\344\272\262\347\274\230\345\205\263\347\263\273:", nullptr));
        searchPushButton->setText(QCoreApplication::translate("RelationshipQueryWidget", "\346\237\245\350\257\242", nullptr));
        clearPushButton->setText(QCoreApplication::translate("RelationshipQueryWidget", "\346\270\205\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RelationshipQueryWidget: public Ui_RelationshipQueryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RELATIONSHIP_QUERY_WIDGET_H
