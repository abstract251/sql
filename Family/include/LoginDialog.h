#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include "User.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    User getUser() const { return m_user; }

signals:
    void loginSuccess(const User& user);

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onTogglePasswordEcho(bool checked);

private:
    bool validateLogin();
    bool registerUser();

    Ui::LoginDialog *ui;
    User m_user;
};

#endif
