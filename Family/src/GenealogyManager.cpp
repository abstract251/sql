#include "GenealogyManager.h"
#include "ui_genealogy_manager.h"
#include "DatabaseManager.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QDebug>

GenealogyManager::GenealogyManager(const User& currentUser, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GenealogyManager)
    , m_currentUser(currentUser)
    , m_currentGenealogyId(0)
{
    ui->setupUi(this);

    m_genealogyModel = new QSqlQueryModel(this);
    ui->genealogyTableView->setModel(m_genealogyModel);
    ui->genealogyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->genealogyTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->genealogyTableView, &QTableView::clicked, this, &GenealogyManager::onGenealogyTableClicked);
    connect(ui->genealogyTableView, &QTableView::doubleClicked, this, &GenealogyManager::onGenealogyTableDoubleClicked);

    connect(ui->createPushButton, &QPushButton::clicked, this, &GenealogyManager::onCreateGenealogy);
    connect(ui->editPushButton, &QPushButton::clicked, this, &GenealogyManager::onEditGenealogy);
    connect(ui->deletePushButton, &QPushButton::clicked, this, &GenealogyManager::onDeleteGenealogy);
    connect(ui->invitePushButton, &QPushButton::clicked, this, &GenealogyManager::onInviteUser);
    connect(ui->refreshPushButton, &QPushButton::clicked, this, &GenealogyManager::refreshGenealogies);

    refreshGenealogies();
}

GenealogyManager::~GenealogyManager()
{
    delete ui;
}

void GenealogyManager::refreshGenealogies()
{
    QVariantList genealogies = DatabaseManager::instance().getGenealogiesForUser(m_currentUser.id());

    m_genealogyModel->setQuery("SELECT 1");
    m_genealogyModel->setQuery("SELECT genealogy_id, name, surname, compile_time, description, creator_id FROM genealogies WHERE 1=0");

    if (genealogies.isEmpty()) {
        return;
    }

    QList<QVariantMap> rows;
    for (const QVariant& v : genealogies) {
        QVariantMap map = v.toMap();
        rows.append(map);
    }

    QString queryStr = QString(R"(
        SELECT g.genealogy_id, g.name, g.surname,
               g.compile_time, g.description,
               CASE WHEN g.creator_id = %1 THEN '是' ELSE '否' END AS 是否创建者
        FROM genealogies g
        JOIN user_genealogy ug ON g.genealogy_id = ug.genealogy_id
        WHERE ug.user_id = %1
        ORDER BY g.name
    )").arg(m_currentUser.id());

    m_genealogyModel->setQuery(queryStr);
    m_genealogyModel->setHeaderData(0, Qt::Horizontal, "ID");
    m_genealogyModel->setHeaderData(1, Qt::Horizontal, "谱名");
    m_genealogyModel->setHeaderData(2, Qt::Horizontal, "姓氏");
    m_genealogyModel->setHeaderData(3, Qt::Horizontal, "修谱时间");
    m_genealogyModel->setHeaderData(4, Qt::Horizontal, "简介");
    m_genealogyModel->setHeaderData(5, Qt::Horizontal, "创建者");

    ui->genealogyTableView->resizeColumnsToContents();
}

void GenealogyManager::onCreateGenealogy()
{
    QDialog dialog(this);
    dialog.setWindowTitle("创建族谱");
    dialog.setMinimumWidth(400);

    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setPlaceholderText("请输入谱名");
    form.addRow("谱名:", &nameEdit);

    QLineEdit surnameEdit;
    surnameEdit.setPlaceholderText("请输入姓氏");
    form.addRow("姓氏:", &surnameEdit);

    QDateEdit dateEdit;
    dateEdit.setDate(QDate::currentDate());
    dateEdit.setCalendarPopup(true);
    form.addRow("修谱时间:", &dateEdit);

    QLineEdit descEdit;
    descEdit.setPlaceholderText("请输入简介(可选)");
    form.addRow("简介:", &descEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString name = nameEdit.text().trimmed();
    QString surname = surnameEdit.text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入谱名!");
        return;
    }

    if (surname.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入姓氏!");
        return;
    }

    if (DatabaseManager::instance().createGenealogy(
            m_currentUser.id(), name, surname,
            dateEdit.date().toString("yyyy-MM-dd"),
            descEdit.text())) {
        QMessageBox::information(this, "成功", "族谱创建成功!");
        refreshGenealogies();
    } else {
        QMessageBox::critical(this, "错误", "族谱创建失败!");
    }
}

void GenealogyManager::onEditGenealogy()
{
    if (m_currentGenealogyId <= 0) {
        QMessageBox::information(this, "提示", "请先选择要编辑的族谱!");
        return;
    }

    if (!isOwner(m_currentGenealogyId)) {
        QMessageBox::warning(this, "权限不足", "只有族谱创建者才能编辑!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT name, surname, compile_time, description FROM genealogies WHERE genealogy_id = ?");
    query.addBindValue(m_currentGenealogyId);
    query.exec();
    query.next();

    QDialog dialog(this);
    dialog.setWindowTitle("编辑族谱");
    dialog.setMinimumWidth(400);

    QFormLayout form(&dialog);

    QLineEdit nameEdit(query.value(0).toString());
    form.addRow("谱名:", &nameEdit);

    QLineEdit surnameEdit(query.value(1).toString());
    form.addRow("姓氏:", &surnameEdit);

    QDateEdit dateEdit;
    if (query.value(2).isValid()) {
        dateEdit.setDate(query.value(2).toDate());
    } else {
        dateEdit.setDate(QDate::currentDate());
    }
    form.addRow("修谱时间:", &dateEdit);

    QLineEdit descEdit(query.value(3).toString());
    form.addRow("简介:", &descEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    if (DatabaseManager::instance().updateGenealogy(
            m_currentGenealogyId, nameEdit.text().trimmed(), surnameEdit.text().trimmed(),
            dateEdit.date().toString("yyyy-MM-dd"), descEdit.text())) {
        QMessageBox::information(this, "成功", "族谱更新成功!");
        refreshGenealogies();
    } else {
        QMessageBox::critical(this, "错误", "族谱更新失败!");
    }
}

void GenealogyManager::onDeleteGenealogy()
{
    if (m_currentGenealogyId <= 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的族谱!");
        return;
    }

    if (!isOwner(m_currentGenealogyId)) {
        QMessageBox::warning(this, "权限不足", "只有族谱创建者才能删除!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除该族谱吗?此操作不可恢复!",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteGenealogy(m_currentGenealogyId)) {
            QMessageBox::information(this, "成功", "族谱删除成功!");
            m_currentGenealogyId = 0;
            emit genealogyDeleted(m_currentGenealogyId);
            refreshGenealogies();
        } else {
            QMessageBox::critical(this, "错误", "族谱删除失败!");
        }
    }
}

void GenealogyManager::onInviteUser()
{
    if (m_currentGenealogyId <= 0) {
        QMessageBox::information(this, "提示", "请先选择要邀请用户参与的族谱!");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("邀请用户");
    dialog.setMinimumWidth(300);

    QFormLayout form(&dialog);

    QLineEdit usernameEdit;
    usernameEdit.setPlaceholderText("请输入用户名");
    form.addRow("用户名:", &usernameEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString username = usernameEdit.text().trimmed();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入用户名!");
        return;
    }

    if (username == m_currentUser.username()) {
        QMessageBox::warning(this, "输入错误", "不能邀请自己!");
        return;
    }

    int userId = DatabaseManager::instance().getUserId(username);
    if (userId <= 0) {
        QMessageBox::warning(this, "用户不存在", "该用户不存在!");
        return;
    }

    if (DatabaseManager::instance().isUserInGenealogy(userId, m_currentGenealogyId)) {
        QMessageBox::information(this, "提示", "该用户已经在本族谱中了!");
        return;
    }

    if (DatabaseManager::instance().inviteUser(m_currentGenealogyId, userId, "editor")) {
        QMessageBox::information(this, "成功", QString("已成功邀请 %1 加入族谱!").arg(username));
    } else {
        QMessageBox::critical(this, "错误", "邀请用户失败!");
    }
}

void GenealogyManager::onGenealogyTableClicked(const QModelIndex& index)
{
    if (index.isValid()) {
        m_currentGenealogyId = m_genealogyModel->data(m_genealogyModel->index(index.row(), 0)).toInt();
    }
}

void GenealogyManager::onGenealogyTableDoubleClicked(const QModelIndex& index)
{
    if (index.isValid()) {
        m_currentGenealogyId = m_genealogyModel->data(m_genealogyModel->index(index.row(), 0)).toInt();
        emit genealogySelected(m_currentGenealogyId);
    }
}

bool GenealogyManager::isOwner(int creatorId)
{
    return creatorId == m_currentUser.id();
}
