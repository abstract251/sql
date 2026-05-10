#include "MemberManager.h"
#include "ui_member_manager.h"
#include "DatabaseManager.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QDebug>

MemberManager::MemberManager(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MemberManager)
    , m_currentGenealogyId(genealogyId)
    , m_currentMemberId(0)
{
    ui->setupUi(this);

    m_memberModel = new QSqlQueryModel(this);
    ui->memberTableView->setModel(m_memberModel);
    ui->memberTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->memberTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->memberTableView, &QTableView::clicked, this, &MemberManager::onMemberTableClicked);
    connect(ui->memberTableView, &QTableView::doubleClicked, this, &MemberManager::onMemberTableDoubleClicked);

    connect(ui->addPushButton, &QPushButton::clicked, this, &MemberManager::onAddMember);
    connect(ui->editPushButton, &QPushButton::clicked, this, &MemberManager::onEditMember);
    connect(ui->deletePushButton, &QPushButton::clicked, this, &MemberManager::onDeleteMember);
    connect(ui->searchPushButton, &QPushButton::clicked, this, &MemberManager::onSearchButtonClicked);
    connect(ui->refreshPushButton, &QPushButton::clicked, this, &MemberManager::refreshMembers);
    connect(ui->clearSearchPushButton, &QPushButton::clicked, this, &MemberManager::onClearSearch);

    refreshMembers();
}

MemberManager::~MemberManager()
{
    delete ui;
}

void MemberManager::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
    refreshMembers();
}

void MemberManager::refreshMembers()
{
    if (m_currentGenealogyId <= 0) {
        m_memberModel->setQuery("SELECT 1");
        m_memberModel->setQuery("SELECT person_id, name, gender, birth_year, death_year, generation FROM persons WHERE 1=0");
        return;
    }

    QString queryStr = QString(R"(
        SELECT
            person_id AS ID,
            name AS 姓名,
            CASE gender WHEN 'M' THEN '男' ELSE '女' END AS 性别,
            birth_year AS 出生年,
            death_year AS 卒年,
            generation AS 辈分
        FROM persons
        WHERE genealogy_id = %1
        ORDER BY generation, birth_year
        LIMIT 500
    )").arg(m_currentGenealogyId);

    m_memberModel->setQuery(queryStr);
    ui->memberTableView->resizeColumnsToContents();
}

void MemberManager::searchMembers(const QString& namePattern)
{
    if (m_currentGenealogyId <= 0) {
        return;
    }

    QVariantList results = DatabaseManager::instance().searchMembersByName(namePattern, m_currentGenealogyId);

    m_memberModel->setQuery("SELECT 1");
    m_memberModel->setQuery("SELECT person_id, name, gender, birth_year, death_year, generation FROM persons WHERE 1=0");

    if (results.isEmpty()) {
        return;
    }

    QString queryStr = QString(R"(
        SELECT
            person_id AS ID,
            name AS 姓名,
            CASE gender WHEN 'M' THEN '男' ELSE '女' END AS 性别,
            birth_year AS 出生年,
            death_year AS 卒年,
            generation AS 辈分
        FROM persons
        WHERE genealogy_id = %1 AND name LIKE '%%2%'
        ORDER BY name, birth_year
        LIMIT 100
    )").arg(m_currentGenealogyId).arg(namePattern);

    m_memberModel->setQuery(queryStr);
    ui->memberTableView->resizeColumnsToContents();
}

void MemberManager::onAddMember()
{
    if (m_currentGenealogyId <= 0) {
        QMessageBox::information(this, "提示", "请先选择族谱!");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("添加成员");
    dialog.setMinimumWidth(400);

    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setPlaceholderText("请输入姓名");
    form.addRow("姓名:", &nameEdit);

    QComboBox genderCombo;
    genderCombo.addItem("男", "M");
    genderCombo.addItem("女", "F");
    form.addRow("性别:", &genderCombo);

    QSpinBox birthYearSpin;
    birthYearSpin.setRange(1000, 2100);
    birthYearSpin.setValue(2000);
    form.addRow("出生年:", &birthYearSpin);

    QSpinBox deathYearSpin;
    deathYearSpin.setRange(0, 2100);
    deathYearSpin.setValue(0);
    deathYearSpin.setSpecialValueText("在世");
    form.addRow("卒年:", &deathYearSpin);

    QSpinBox generationSpin;
    generationSpin.setRange(1, 100);
    generationSpin.setValue(1);
    form.addRow("辈分:", &generationSpin);

    QComboBox familyCombo;
    familyCombo.addItem("无/未知", 0);
    form.addRow("出生家庭:", &familyCombo);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString name = nameEdit.text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入姓名!");
        return;
    }

    QChar gender = genderCombo.currentData().toChar();
    int birthYear = birthYearSpin.value();
    int deathYear = deathYearSpin.value() > 0 ? deathYearSpin.value() : 0;
    int generation = generationSpin.value();
    int familyId = familyCombo.currentData().toInt();

    int personId = DatabaseManager::instance().addMember(
        name, gender, birthYear, deathYear, QString(), generation, m_currentGenealogyId, familyId);

    if (personId > 0) {
        QMessageBox::information(this, "成功", "成员添加成功!");
        refreshMembers();
        emit memberAdded(personId);
    } else {
        QMessageBox::critical(this, "错误", "成员添加失败!");
    }
}

void MemberManager::onEditMember()
{
    if (m_currentMemberId <= 0) {
        QMessageBox::information(this, "提示", "请先选择要编辑的成员!");
        return;
    }

    QVariantList memberData = DatabaseManager::instance().getMember(m_currentMemberId);
    if (memberData.isEmpty()) {
        QMessageBox::warning(this, "错误", "未找到该成员信息!");
        return;
    }

    QVariantMap data = memberData.first().toMap();

    QDialog dialog(this);
    dialog.setWindowTitle("编辑成员");
    dialog.setMinimumWidth(400);

    QFormLayout form(&dialog);

    QLineEdit nameEdit(data["name"].toString());
    form.addRow("姓名:", &nameEdit);

    QComboBox genderCombo;
    genderCombo.addItem("男", "M");
    genderCombo.addItem("女", "F");
    genderCombo.setCurrentIndex(data["gender"].toString() == "M" ? 0 : 1);
    form.addRow("性别:", &genderCombo);

    QSpinBox birthYearSpin;
    birthYearSpin.setRange(1000, 2100);
    birthYearSpin.setValue(data["birth_year"].toInt());
    form.addRow("出生年:", &birthYearSpin);

    QSpinBox deathYearSpin;
    deathYearSpin.setRange(0, 2100);
    if (data["death_year"].toInt() > 0) {
        deathYearSpin.setValue(data["death_year"].toInt());
    }
    form.addRow("卒年:", &deathYearSpin);

    QSpinBox generationSpin;
    generationSpin.setRange(1, 100);
    generationSpin.setValue(data["generation"].toInt());
    form.addRow("辈分:", &generationSpin);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString name = nameEdit.text().trimmed();
    QChar gender = genderCombo.currentData().toChar();
    int birthYear = birthYearSpin.value();
    int deathYear = deathYearSpin.value() > 0 ? deathYearSpin.value() : 0;
    int generation = generationSpin.value();

    if (DatabaseManager::instance().updateMember(m_currentMemberId, name, gender, birthYear, deathYear, QString(), generation)) {
        QMessageBox::information(this, "成功", "成员更新成功!");
        refreshMembers();
        emit memberUpdated(m_currentMemberId);
    } else {
        QMessageBox::critical(this, "错误", "成员更新失败!");
    }
}

void MemberManager::onDeleteMember()
{
    if (m_currentMemberId <= 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的成员!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除", "确定要删除该成员吗?此操作不可恢复!",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteMember(m_currentMemberId)) {
            QMessageBox::information(this, "成功", "成员删除成功!");
            m_currentMemberId = 0;
            emit memberDeleted(m_currentMemberId);
            refreshMembers();
        } else {
            QMessageBox::critical(this, "错误", "成员删除失败!");
        }
    }
}

void MemberManager::onSearchButtonClicked()
{
    QString namePattern = ui->searchLineEdit->text().trimmed();
    if (namePattern.isEmpty()) {
        QMessageBox::information(this, "提示", "请输入要搜索的姓名!");
        return;
    }
    searchMembers(namePattern);
}

void MemberManager::onMemberTableClicked(const QModelIndex& index)
{
    if (index.isValid()) {
        m_currentMemberId = m_memberModel->data(m_memberModel->index(index.row(), 0)).toInt();
    }
}

void MemberManager::onMemberTableDoubleClicked(const QModelIndex& index)
{
    if (index.isValid()) {
        m_currentMemberId = m_memberModel->data(m_memberModel->index(index.row(), 0)).toInt();
        emit memberSelected(m_currentMemberId);
    }
}

void MemberManager::onClearSearch()
{
    ui->searchLineEdit->clear();
    refreshMembers();
}

void MemberManager::loadFamilyComboBox()
{
}
