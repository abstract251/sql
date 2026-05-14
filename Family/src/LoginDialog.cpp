#include "LoginDialog.h"
#include "ui_login_dialog.h"
#include "DatabaseManager.h"
#include "User.h"

#include <QCryptographicHash>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("用户登录 - 寻根溯源族谱管理系统");
    setFixedSize(400, 300);
    setModal(true);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    ui->registerPasswordLineEdit->setEchoMode(QLineEdit::Password);
    ui->registerConfirmPasswordLineEdit->setEchoMode(QLineEdit::Password);

    connect(ui->loginPushButton, &QPushButton::clicked, this, &LoginDialog::onLoginButtonClicked);
    connect(ui->registerPushButton_2, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->registerPushButton, &QPushButton::clicked, this, &LoginDialog::onRegisterButtonClicked);
    connect(ui->backToLoginPushButton, &QPushButton::clicked, this, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->togglePasswordCheckBox, &QCheckBox::toggled, this, &LoginDialog::onTogglePasswordEcho);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::onLoginButtonClicked()
{
    if (!validateLogin()) {
        return;
    }

    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();
    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    qDebug() << "Login attempt with username:" << username;
    qDebug() << "Password hash:" << passwordHash;

    if (DatabaseManager::instance().validateUser(username, passwordHash)) {
        int userId = DatabaseManager::instance().getUserId(username);
        m_user = User(userId, username);
        
        QDialog welcomeDialog(this);
        welcomeDialog.setWindowTitle("登录成功");
        welcomeDialog.setFixedSize(300, 120);
        welcomeDialog.setModal(true);
        
        QVBoxLayout layout(&welcomeDialog);
        QLabel welcomeLabel(QString("欢迎, %1!").arg(username), &welcomeDialog);
        welcomeLabel.setAlignment(Qt::AlignCenter);
        welcomeLabel.setStyleSheet("font-size: 16px; font-weight: bold;");
        layout.addWidget(&welcomeLabel);
        
        QLabel countdownLabel("对话框将在 3 秒后自动关闭...", &welcomeDialog);
        countdownLabel.setAlignment(Qt::AlignCenter);
        layout.addWidget(&countdownLabel);
        
        QPushButton okButton("确定", &welcomeDialog);
        okButton.setFixedWidth(100);
        connect(&okButton, &QPushButton::clicked, &welcomeDialog, &QDialog::accept);
        layout.addWidget(&okButton);
        layout.setAlignment(&okButton, Qt::AlignCenter);
        
        QTimer timer;
        int remainingSeconds = 3;
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            remainingSeconds--;
            if (remainingSeconds > 0) {
                countdownLabel.setText(QString("对话框将在 %1 秒后自动关闭...").arg(remainingSeconds));
            } else {
                timer.stop();
                welcomeDialog.accept();
            }
        });
        timer.start(1000);
        countdownLabel.setText("对话框将在 3 秒后自动关闭...");
        
        welcomeDialog.exec();
        
        emit loginSuccess(m_user);
        accept();
    } else {
        qDebug() << "Login failed - invalid username or password";
        QMessageBox::warning(this, "登录失败", "用户名或密码错误!");
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->setFocus();
    }
}

void LoginDialog::onRegisterButtonClicked()
{
    QString username = ui->registerUsernameLineEdit->text().trimmed();
    QString password = ui->registerPasswordLineEdit->text();
    QString confirmPassword = ui->registerConfirmPasswordLineEdit->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名不能为空!");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "密码不能为空!");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致!");
        return;
    }

    if (password.length() < 6) {
        QMessageBox::warning(this, "注册失败", "密码长度至少为6个字符!");
        return;
    }

    if (DatabaseManager::instance().userExists(username)) {
        QMessageBox::warning(this, "注册失败", "用户名已存在!");
        return;
    }

    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    if (DatabaseManager::instance().createUser(username, passwordHash)) {
        QMessageBox::information(this, "注册成功", "用户注册成功!请登录。");
        ui->stackedWidget->setCurrentIndex(0);
        ui->usernameLineEdit->setText(username);
        ui->passwordLineEdit->clear();
        ui->passwordLineEdit->setFocus();
    } else {
        QMessageBox::critical(this, "注册失败", "创建用户失败,请重试!");
    }
}

void LoginDialog::onTogglePasswordEcho(bool checked)
{
    ui->passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

bool LoginDialog::validateLogin()
{
    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入用户名!");
        ui->usernameLineEdit->setFocus();
        return false;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入密码!");
        ui->passwordLineEdit->setFocus();
        return false;
    }

    return true;
}

bool LoginDialog::registerUser()
{
    return true;
}
