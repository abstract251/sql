/********************************************************************************
** Form generated from reading UI file 'ancestor_query_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANCESTOR_QUERY_WIDGET_H
#define UI_ANCESTOR_QUERY_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AncestorQueryWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *searchPushButton;
    QPushButton *clearPushButton;
    QTreeView *ancestorTreeView;

    void setupUi(QWidget *AncestorQueryWidget)
    {
        if (AncestorQueryWidget->objectName().isEmpty())
            AncestorQueryWidget->setObjectName("AncestorQueryWidget");
        AncestorQueryWidget->resize(900, 600);
        verticalLayout = new QVBoxLayout(AncestorQueryWidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(AncestorQueryWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        searchPushButton = new QPushButton(AncestorQueryWidget);
        searchPushButton->setObjectName("searchPushButton");

        horizontalLayout->addWidget(searchPushButton);

        clearPushButton = new QPushButton(AncestorQueryWidget);
        clearPushButton->setObjectName("clearPushButton");

        horizontalLayout->addWidget(clearPushButton);


        verticalLayout->addLayout(horizontalLayout);

        ancestorTreeView = new QTreeView(AncestorQueryWidget);
        ancestorTreeView->setObjectName("ancestorTreeView");
        ancestorTreeView->setAnimated(true);
        ancestorTreeView->setHeaderHidden(false);
        ancestorTreeView->setIndentation(20);

        verticalLayout->addWidget(ancestorTreeView);


        retranslateUi(AncestorQueryWidget);

        QMetaObject::connectSlotsByName(AncestorQueryWidget);
    } // setupUi

    void retranslateUi(QWidget *AncestorQueryWidget)
    {
        AncestorQueryWidget->setWindowTitle(QCoreApplication::translate("AncestorQueryWidget", "\347\245\226\345\205\210\346\237\245\350\257\242", nullptr));
        label->setText(QCoreApplication::translate("AncestorQueryWidget", "\350\276\223\345\205\245\346\210\220\345\221\230ID\346\237\245\350\257\242\345\205\266\347\245\226\345\205\210:", nullptr));
        searchPushButton->setText(QCoreApplication::translate("AncestorQueryWidget", "\346\237\245\350\257\242", nullptr));
        clearPushButton->setText(QCoreApplication::translate("AncestorQueryWidget", "\346\270\205\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AncestorQueryWidget: public Ui_AncestorQueryWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANCESTOR_QUERY_WIDGET_H
