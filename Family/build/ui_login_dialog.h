/********************************************************************************
** Form generated from reading UI file 'login_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_DIALOG_H
#define UI_LOGIN_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QVBoxLayout *verticalLayout_2;
    QLabel *titleLabel;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QCheckBox *togglePasswordCheckBox;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_3;
    QPushButton *loginPushButton;
    QPushButton *registerPushButton_2;
    QWidget *registerPage;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *registerUsernameLineEdit;
    QLineEdit *registerPasswordLineEdit;
    QLineEdit *registerConfirmPasswordLineEdit;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_6;
    QPushButton *registerPushButton;
    QPushButton *backToLoginPushButton;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(400, 350);
        LoginDialog->setModal(true);
        stackedWidget = new QStackedWidget(LoginDialog);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(30, 20, 340, 280));
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        verticalLayout_2 = new QVBoxLayout(loginPage);
        verticalLayout_2->setObjectName("verticalLayout_2");
        titleLabel = new QLabel(loginPage);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(titleLabel);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        usernameLineEdit = new QLineEdit(loginPage);
        usernameLineEdit->setObjectName("usernameLineEdit");

        verticalLayout->addWidget(usernameLineEdit);

        passwordLineEdit = new QLineEdit(loginPage);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(passwordLineEdit);

        togglePasswordCheckBox = new QCheckBox(loginPage);
        togglePasswordCheckBox->setObjectName("togglePasswordCheckBox");

        verticalLayout->addWidget(togglePasswordCheckBox);


        verticalLayout_2->addLayout(verticalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        loginPushButton = new QPushButton(loginPage);
        loginPushButton->setObjectName("loginPushButton");

        verticalLayout_3->addWidget(loginPushButton);

        registerPushButton_2 = new QPushButton(loginPage);
        registerPushButton_2->setObjectName("registerPushButton_2");

        verticalLayout_3->addWidget(registerPushButton_2);


        verticalLayout_2->addLayout(verticalLayout_3);

        stackedWidget->addWidget(loginPage);
        registerPage = new QWidget();
        registerPage->setObjectName("registerPage");
        verticalLayout_5 = new QVBoxLayout(registerPage);
        verticalLayout_5->setObjectName("verticalLayout_5");
        label_5 = new QLabel(registerPage);
        label_5->setObjectName("label_5");
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_5);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        registerUsernameLineEdit = new QLineEdit(registerPage);
        registerUsernameLineEdit->setObjectName("registerUsernameLineEdit");

        verticalLayout_4->addWidget(registerUsernameLineEdit);

        registerPasswordLineEdit = new QLineEdit(registerPage);
        registerPasswordLineEdit->setObjectName("registerPasswordLineEdit");
        registerPasswordLineEdit->setEchoMode(QLineEdit::Password);

        verticalLayout_4->addWidget(registerPasswordLineEdit);

        registerConfirmPasswordLineEdit = new QLineEdit(registerPage);
        registerConfirmPasswordLineEdit->setObjectName("registerConfirmPasswordLineEdit");
        registerConfirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

        verticalLayout_4->addWidget(registerConfirmPasswordLineEdit);


        verticalLayout_5->addLayout(verticalLayout_4);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_4);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        registerPushButton = new QPushButton(registerPage);
        registerPushButton->setObjectName("registerPushButton");

        verticalLayout_6->addWidget(registerPushButton);

        backToLoginPushButton = new QPushButton(registerPage);
        backToLoginPushButton->setObjectName("backToLoginPushButton");

        verticalLayout_6->addWidget(backToLoginPushButton);


        verticalLayout_5->addLayout(verticalLayout_6);

        stackedWidget->addWidget(registerPage);

        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\347\231\273\345\275\225 - \345\257\273\346\240\271\346\272\257\346\272\220\346\227\217\350\260\261\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        titleLabel->setText(QCoreApplication::translate("LoginDialog", "\345\257\273\346\240\271\346\272\257\346\272\220\\n\346\227\217\350\260\261\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        usernameLineEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\345\220\215", nullptr));
        passwordLineEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201", nullptr));
        togglePasswordCheckBox->setText(QCoreApplication::translate("LoginDialog", "\346\230\276\347\244\272\345\257\206\347\240\201", nullptr));
        loginPushButton->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        registerPushButton_2->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214\346\226\260\347\224\250\346\210\267", nullptr));
        label_5->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\346\263\250\345\206\214", nullptr));
        registerUsernameLineEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\345\220\215", nullptr));
        registerPasswordLineEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201 (\350\207\263\345\260\2216\344\275\215)", nullptr));
        registerConfirmPasswordLineEdit->setPlaceholderText(QCoreApplication::translate("LoginDialog", "\347\241\256\350\256\244\345\257\206\347\240\201", nullptr));
        registerPushButton->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        backToLoginPushButton->setText(QCoreApplication::translate("LoginDialog", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_DIALOG_H
