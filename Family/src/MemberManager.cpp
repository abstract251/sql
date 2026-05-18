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
#include <QDate>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSqlQuery>
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
    connect(ui->surnameComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MemberManager::onSurnameChanged);

    loadSurnames();
    refreshMembers();
}

MemberManager::~MemberManager()
{
    delete ui;
}

void MemberManager::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
    loadSurnames();
    refreshMembers();
}

void MemberManager::loadSurnames()
{
    ui->surnameComboBox->clear();
    ui->surnameComboBox->addItem("全部", "");

    QSqlQuery query(DatabaseManager::instance().database());
    QString queryStr;
    
    if (m_currentGenealogyId > 0) {
        queryStr = QString(R"(
            SELECT DISTINCT SUBSTRING(name FROM 1 FOR 1) AS surname, COUNT(*) as cnt
            FROM persons
            WHERE genealogy_id = %1
            GROUP BY SUBSTRING(name FROM 1 FOR 1)
            ORDER BY cnt DESC
        )").arg(m_currentGenealogyId);
    } else {
        queryStr = R"(
            SELECT DISTINCT SUBSTRING(name FROM 1 FOR 1) AS surname, COUNT(*) as cnt
            FROM persons
            GROUP BY SUBSTRING(name FROM 1 FOR 1)
            ORDER BY cnt DESC
        )";
    }

    if (query.exec(queryStr)) {
        while (query.next()) {
            QString surname = query.value(0).toString();
            int count = query.value(1).toInt();
            ui->surnameComboBox->addItem(QString("%1 (%2人)").arg(surname).arg(count), surname);
        }
    }
}

void MemberManager::refreshMembers()
{
    QString queryStr;
    QString selectedSurname = ui->surnameComboBox->currentData().toString();
    
    if (m_currentGenealogyId <= 0) {
        if (selectedSurname.isEmpty()) {
            queryStr = QString(R"(
                SELECT
                    person_id AS ID,
                    name AS 姓名,
                    CASE gender WHEN 'M' THEN '男' ELSE '女' END AS 性别,
                    birth_year AS 出生年,
                    death_year AS 卒年,
                    generation AS 辈分
                FROM persons
                ORDER BY generation, birth_year
                LIMIT 500
            )");
        } else {
            queryStr = QString(R"(
                SELECT
                    person_id AS ID,
                    name AS 姓名,
                    CASE gender WHEN 'M' THEN '男' ELSE '女' END AS 性别,
                    birth_year AS 出生年,
                    death_year AS 卒年,
                    generation AS 辈分
                FROM persons
                WHERE name LIKE '%1%%'
                ORDER BY generation, birth_year
                LIMIT 500
            )").arg(selectedSurname);
        }
    } else {
        if (selectedSurname.isEmpty()) {
            queryStr = QString(R"(
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
        } else {
            queryStr = QString(R"(
                SELECT
                    person_id AS ID,
                    name AS 姓名,
                    CASE gender WHEN 'M' THEN '男' ELSE '女' END AS 性别,
                    birth_year AS 出生年,
                    death_year AS 卒年,
                    generation AS 辈分
                FROM persons
                WHERE genealogy_id = %1 AND name LIKE '%2%%'
                ORDER BY generation, birth_year
                LIMIT 500
            )").arg(m_currentGenealogyId).arg(selectedSurname);
        }
    }

    QSqlQuery query(DatabaseManager::instance().database());
    query.exec(queryStr);
    m_memberModel->setQuery(query);
    ui->memberTableView->resizeColumnsToContents();
}

void MemberManager::searchMembers(const QString& namePattern, int birthYear, int deathYear, int generation, int memberId)
{
    Q_UNUSED(birthYear);
    Q_UNUSED(deathYear);
    
    QString selectedSurname = ui->surnameComboBox->currentData().toString();
    
    int currentYear = QDate::currentDate().year();
    int ageMin = ui->ageMinSpinBox->value();
    int ageMax = ui->ageMaxSpinBox->value();
    int spouseStatus = ui->spouseComboBox->currentIndex();
    int genderIndex = ui->genderComboBox->currentIndex();
    
    int birthYearMin = ui->birthYearMinSpinBox->value();
    int birthYearMax = ui->birthYearMaxSpinBox->value();
    int deathYearMin = ui->deathYearMinSpinBox->value();
    int deathYearMax = ui->deathYearMaxSpinBox->value();
    
    QStringList conditions;
    
    if (m_currentGenealogyId > 0) {
        conditions.append(QString("genealogy_id = %1").arg(m_currentGenealogyId));
    }
    
    if (memberId > 0) {
        conditions.append(QString("person_id = %1").arg(memberId));
    }
    
    if (!selectedSurname.isEmpty()) {
        conditions.append(QString("name LIKE '%1%%'").arg(selectedSurname));
    }
    
    if (!namePattern.isEmpty()) {
        conditions.append(QString("name LIKE '%%1%'").arg(namePattern));
    }
    
    if (generation > 0) {
        conditions.append(QString("generation = %1").arg(generation));
    }
    
    if (genderIndex == 1) {
        conditions.append("gender = 'M'");
    } else if (genderIndex == 2) {
        conditions.append("gender = 'F'");
    }
    
    if (birthYearMin > 0 && birthYearMax > 0) {
        conditions.append(QString("birth_year BETWEEN %1 AND %2").arg(birthYearMin).arg(birthYearMax));
    } else if (birthYearMin > 0) {
        conditions.append(QString("birth_year >= %1").arg(birthYearMin));
    } else if (birthYearMax > 0) {
        conditions.append(QString("birth_year <= %1").arg(birthYearMax));
    }
    
    if (deathYearMin > 0 && deathYearMax > 0) {
        conditions.append(QString("death_year BETWEEN %1 AND %2 AND death_year > 0").arg(deathYearMin).arg(deathYearMax));
    } else if (deathYearMin > 0) {
        conditions.append(QString("death_year >= %1 AND death_year > 0").arg(deathYearMin));
    } else if (deathYearMax > 0) {
        conditions.append(QString("death_year <= %1 AND death_year > 0").arg(deathYearMax));
    }
    
    if (ageMin > 0) {
        int birthYearMax = currentYear - ageMin;
        conditions.append(QString("birth_year <= %1").arg(birthYearMax));
    }
    
    if (ageMax > 0) {
        conditions.append(QString(
            "(CASE WHEN death_year > 0 THEN death_year - birth_year ELSE %1 - birth_year END) <= %2"
        ).arg(currentYear).arg(ageMax));
    }
    
    if (spouseStatus == 1) {
        conditions.append(QString(
            "person_id IN (SELECT husband_id FROM families WHERE husband_id IS NOT NULL) OR "
            "person_id IN (SELECT wife_id FROM families WHERE wife_id IS NOT NULL)"
        ));
    } else if (spouseStatus == 2) {
        conditions.append(QString(
            "person_id NOT IN (SELECT husband_id FROM families WHERE husband_id IS NOT NULL) AND "
            "person_id NOT IN (SELECT wife_id FROM families WHERE wife_id IS NOT NULL)"
        ));
    }
    
    QString whereClause;
    if (!conditions.isEmpty()) {
        whereClause = "WHERE " + conditions.join(" AND ");
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
        %1
        ORDER BY generation, birth_year
        LIMIT 500
    )").arg(whereClause);

    QSqlQuery query(DatabaseManager::instance().database());
    query.exec(queryStr);
    m_memberModel->setQuery(query);
    ui->memberTableView->resizeColumnsToContents();
}

void MemberManager::onAddMember()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加成员");
    dialog.setMinimumWidth(400);

    QFormLayout form(&dialog);

    QComboBox genealogyCombo;
    genealogyCombo.addItem("请选择族谱", 0);
    QVariantList genealogies = DatabaseManager::instance().getAllGenealogies();
    for (const QVariant& g : genealogies) {
        QVariantMap map = g.toMap();
        genealogyCombo.addItem(map["name"].toString(), map["genealogy_id"]);
    }
    form.addRow("族谱:", &genealogyCombo);

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
    familyCombo.addItem("请先选择族谱", 0);
    form.addRow("出生家庭:", &familyCombo);

    auto loadFamilies = [&familyCombo](int selectedGenealogyId) {
        familyCombo.clear();
        familyCombo.addItem("无/未知", 0);
        if (selectedGenealogyId > 0) {
            QSqlQuery familyQuery(DatabaseManager::instance().database());
            familyQuery.prepare(R"(
                SELECT f.family_id, CONCAT(COALESCE(hp.name, ''), ' & ', COALESCE(wp.name, '')) AS family_name
                FROM families f
                LEFT JOIN persons hp ON f.husband_id = hp.person_id
                LEFT JOIN persons wp ON f.wife_id = wp.person_id
                WHERE f.genealogy_id = ?
                ORDER BY family_id
            )");
            familyQuery.addBindValue(selectedGenealogyId);
            if (familyQuery.exec()) {
                while (familyQuery.next()) {
                    int fid = familyQuery.value(0).toInt();
                    QString fname = familyQuery.value(1).toString();
                    if (fname.trimmed() == " & ") {
                        fname = QString("家庭 %1").arg(fid);
                    }
                    familyCombo.addItem(fname, fid);
                }
            }
        }
    };

    QObject::connect(&genealogyCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), [&genealogyCombo, loadFamilies]() {
        int selectedId = genealogyCombo.currentData().toInt();
        loadFamilies(selectedId);
    });

    if (m_currentGenealogyId > 0) {
        int idx = genealogyCombo.findData(m_currentGenealogyId);
        if (idx >= 0) {
            genealogyCombo.setCurrentIndex(idx);
            loadFamilies(m_currentGenealogyId);
        }
    }

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    int genealogyId = genealogyCombo.currentData().toInt();
    if (genealogyId <= 0) {
        QMessageBox::warning(this, "输入错误", "请选择族谱!");
        return;
    }

    QString name = nameEdit.text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入姓名!");
        return;
    }

    QString genderStr = genderCombo.currentData().toString();
    QChar gender = genderStr.isEmpty() ? 'M' : genderStr.at(0);
    int birthYear = birthYearSpin.value();
    int deathYear = deathYearSpin.value() > 0 ? deathYearSpin.value() : 0;
    int generation = generationSpin.value();
    int familyId = familyCombo.currentData().toInt();

    int personId = DatabaseManager::instance().addMember(
        name, gender, birthYear, deathYear, QString(), generation, genealogyId, familyId);

    if (personId > 0) {
        QMessageBox::information(this, "成功", "成员添加成功!");
        loadSurnames();
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
    deathYearSpin.setValue(data["death_year"].toInt() > 0 ? data["death_year"].toInt() : 0);
    deathYearSpin.setSpecialValueText("在世");
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
    QString genderStr = genderCombo.currentData().toString();
    QChar gender = genderStr.isEmpty() ? 'M' : genderStr.at(0);
    int birthYear = birthYearSpin.value();
    int deathYear = deathYearSpin.value() > 0 ? deathYearSpin.value() : 0;
    int generation = generationSpin.value();

    if (DatabaseManager::instance().updateMember(m_currentMemberId, name, gender, birthYear, deathYear, QString(), generation)) {
        QMessageBox::information(this, "成功", "成员更新成功!");
        loadSurnames();
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
            loadSurnames();
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
    int generation = ui->generationLineEdit->text().trimmed().toInt();
    int memberId = ui->idLineEdit->text().trimmed().toInt();
    
    searchMembers(namePattern, 0, 0, generation, memberId);
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
    ui->idLineEdit->clear();
    ui->searchLineEdit->clear();
    ui->generationLineEdit->clear();
    ui->genderComboBox->setCurrentIndex(0);
    ui->birthYearMinSpinBox->setValue(0);
    ui->birthYearMaxSpinBox->setValue(0);
    ui->deathYearMinSpinBox->setValue(0);
    ui->deathYearMaxSpinBox->setValue(0);
    ui->ageMinSpinBox->setValue(0);
    ui->ageMaxSpinBox->setValue(0);
    ui->spouseComboBox->setCurrentIndex(0);
    refreshMembers();
}

void MemberManager::onSurnameChanged(int index)
{
    Q_UNUSED(index);
    refreshMembers();
}

void MemberManager::loadFamilyComboBox()
{
}
