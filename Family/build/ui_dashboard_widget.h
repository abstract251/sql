/********************************************************************************
** Form generated from reading UI file 'dashboard_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DASHBOARD_WIDGET_H
#define UI_DASHBOARD_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DashboardWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_10;
    QComboBox *genealogyComboBox;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *totalMembersLabel;
    QLabel *label_2;
    QLabel *maleCountLabel;
    QLabel *maleRatioLabel;
    QLabel *label_3;
    QLabel *femaleCountLabel;
    QLabel *label_4;
    QLabel *femaleRatioLabel;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *statsTableWidget;

    void setupUi(QWidget *DashboardWidget)
    {
        if (DashboardWidget->objectName().isEmpty())
            DashboardWidget->setObjectName("DashboardWidget");
        DashboardWidget->resize(900, 600);
        verticalLayout = new QVBoxLayout(DashboardWidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_10 = new QLabel(DashboardWidget);
        label_10->setObjectName("label_10");

        horizontalLayout->addWidget(label_10);

        genealogyComboBox = new QComboBox(DashboardWidget);
        genealogyComboBox->setObjectName("genealogyComboBox");
        genealogyComboBox->setMinimumSize(QSize(200, 0));

        horizontalLayout->addWidget(genealogyComboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        groupBox = new QGroupBox(DashboardWidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        totalMembersLabel = new QLabel(groupBox);
        totalMembersLabel->setObjectName("totalMembersLabel");
        totalMembersLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(totalMembersLabel, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        maleCountLabel = new QLabel(groupBox);
        maleCountLabel->setObjectName("maleCountLabel");
        maleCountLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(maleCountLabel, 0, 3, 1, 1);

        maleRatioLabel = new QLabel(groupBox);
        maleRatioLabel->setObjectName("maleRatioLabel");
        maleRatioLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(maleRatioLabel, 0, 4, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        femaleCountLabel = new QLabel(groupBox);
        femaleCountLabel->setObjectName("femaleCountLabel");
        femaleCountLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(femaleCountLabel, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 1, 2, 1, 1);

        femaleRatioLabel = new QLabel(groupBox);
        femaleRatioLabel->setObjectName("femaleRatioLabel");
        femaleRatioLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(femaleRatioLabel, 1, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 4, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(DashboardWidget);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        statsTableWidget = new QTableWidget(groupBox_2);
        if (statsTableWidget->columnCount() < 5)
            statsTableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        statsTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        statsTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        statsTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        statsTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        statsTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        statsTableWidget->setObjectName("statsTableWidget");
        statsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        statsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_2->addWidget(statsTableWidget);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(DashboardWidget);

        QMetaObject::connectSlotsByName(DashboardWidget);
    } // setupUi

    void retranslateUi(QWidget *DashboardWidget)
    {
        DashboardWidget->setWindowTitle(QCoreApplication::translate("DashboardWidget", "\344\273\252\350\241\250\347\233\230", nullptr));
        label_10->setText(QCoreApplication::translate("DashboardWidget", "\351\200\211\346\213\251\346\227\217\350\260\261:", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DashboardWidget", "\345\256\266\346\227\217\347\273\237\350\256\241\346\246\202\350\247\210", nullptr));
        label->setText(QCoreApplication::translate("DashboardWidget", "\346\200\273\344\272\272\346\225\260:", nullptr));
        totalMembersLabel->setText(QCoreApplication::translate("DashboardWidget", "0", nullptr));
        label_2->setText(QCoreApplication::translate("DashboardWidget", "\347\224\267\346\200\247\344\272\272\346\225\260:", nullptr));
        maleCountLabel->setText(QCoreApplication::translate("DashboardWidget", "0", nullptr));
        maleRatioLabel->setText(QCoreApplication::translate("DashboardWidget", "0%", nullptr));
        label_3->setText(QCoreApplication::translate("DashboardWidget", "\345\245\263\346\200\247\344\272\272\346\225\260:", nullptr));
        femaleCountLabel->setText(QCoreApplication::translate("DashboardWidget", "0", nullptr));
        label_4->setText(QCoreApplication::translate("DashboardWidget", "\345\245\263\346\200\247\346\257\224\344\276\213:", nullptr));
        femaleRatioLabel->setText(QCoreApplication::translate("DashboardWidget", "0%", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("DashboardWidget", "\345\220\204\350\276\210\345\210\206\347\273\237\350\256\241", nullptr));
        QTableWidgetItem *___qtablewidgetitem = statsTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("DashboardWidget", "\350\276\210\345\210\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = statsTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("DashboardWidget", "\344\272\272\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = statsTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("DashboardWidget", "\345\271\263\345\235\207\345\257\277\345\221\275", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = statsTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("DashboardWidget", "\346\234\200\346\227\251\345\207\272\347\224\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = statsTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("DashboardWidget", "\346\234\200\346\231\232\345\207\272\347\224\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DashboardWidget: public Ui_DashboardWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DASHBOARD_WIDGET_H
