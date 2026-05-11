#include "RelationshipQueryWidget.h"
#include "ui_relationship_query_widget.h"
#include "DatabaseManager.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QDebug>

RelationshipQueryWidget::RelationshipQueryWidget(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RelationshipQueryWidget)
    , m_currentGenealogyId(genealogyId)
{
    ui->setupUi(this);

    m_relationshipModel = new QStandardItemModel(this);
    m_relationshipModel->setColumnCount(4);
    m_relationshipModel->setHeaderData(0, Qt::Horizontal, "成员1");
    m_relationshipModel->setHeaderData(1, Qt::Horizontal, "成员2");
    m_relationshipModel->setHeaderData(2, Qt::Horizontal, "公共祖先");
    m_relationshipModel->setHeaderData(3, Qt::Horizontal, "关系类型");

    ui->resultTableView->setModel(m_relationshipModel);
    ui->resultTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultTableView->resizeColumnsToContents();

    connect(ui->searchPushButton, &QPushButton::clicked, this, &RelationshipQueryWidget::onSearchRelationship);
    connect(ui->clearPushButton, &QPushButton::clicked, this, &RelationshipQueryWidget::onClearResults);
}

RelationshipQueryWidget::~RelationshipQueryWidget()
{
    delete ui;
}

void RelationshipQueryWidget::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
}

void RelationshipQueryWidget::onSearchRelationship()
{
    bool ok;
    int person1Id = QInputDialog::getInt(this, "亲缘关系查询",
        "请输入第一个成员ID:", 0, 1, 99999999, 1, &ok);

    if (!ok) {
        return;
    }

    int person2Id = QInputDialog::getInt(this, "亲缘关系查询",
        "请输入第二个成员ID:", 0, 1, 99999999, 1, &ok);

    if (!ok) {
        return;
    }

    QVariantList person1Data = DatabaseManager::instance().getMember(person1Id);
    QVariantList person2Data = DatabaseManager::instance().getMember(person2Id);

    if (person1Data.isEmpty()) {
        QMessageBox::warning(this, "查询失败", QString("未找到ID为%1的成员!").arg(person1Id));
        return;
    }

    if (person2Data.isEmpty()) {
        QMessageBox::warning(this, "查询失败", QString("未找到ID为%1的成员!").arg(person2Id));
        return;
    }

    QString person1Name = person1Data.first().toMap()["name"].toString();
    QString person2Name = person2Data.first().toMap()["name"].toString();

    QVariantList relationship = DatabaseManager::instance().findRelationship(person1Id, person2Id);

    m_relationshipModel->setRowCount(0);

    if (relationship.isEmpty()) {
        QMessageBox::information(this, "查询结果",
            QString("成员 [%1] 和 [%2] 之间没有找到亲缘关系!").arg(person1Name).arg(person2Name));
        return;
    }

    QVariantMap relMap = relationship.first().toMap();
    QString commonAncestorName = relMap["common_ancestor_name"].toString();

    QString path1 = relMap["path_to_person1"].toString();
    QString path2 = relMap["path_to_person2"].toString();

    QStringList path1Parts = path1.split("->");
    QStringList path2Parts = path2.split("->");

    int level1 = path1Parts.size() - 1;
    int level2 = path2Parts.size() - 1;

    QString relationshipType = determineRelationshipType(level1, level2);

    int row = 0;
    m_relationshipModel->insertRow(row);

    m_relationshipModel->setData(m_relationshipModel->index(row, 0), person1Name);
    m_relationshipModel->setData(m_relationshipModel->index(row, 1), person2Name);
    m_relationshipModel->setData(m_relationshipModel->index(row, 2), commonAncestorName);
    m_relationshipModel->setData(m_relationshipModel->index(row, 3), relationshipType);

    ui->resultTableView->resizeColumnsToContents();
}

QString RelationshipQueryWidget::determineRelationshipType(int level1, int level2)
{
    if (level1 == 1 && level2 == 1) {
        return "同胞/兄弟姐妹";
    } else if (level1 == 1) {
        return QString("叔侄/姑侄 (相隔%1代)").arg(level2 - 1);
    } else if (level2 == 1) {
        return QString("叔侄/姑侄 (相隔%1代)").arg(level1 - 1);
    } else if (level1 == 2 && level2 == 2) {
        return "堂兄弟/表兄弟";
    } else {
        return QString("远亲 (共同祖先在%1代)").arg(30 - qMin(level1, level2));
    }
}

void RelationshipQueryWidget::onClearResults()
{
    m_relationshipModel->setRowCount(0);
}
