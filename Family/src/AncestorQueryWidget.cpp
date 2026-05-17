#include "AncestorQueryWidget.h"
#include "ui_ancestor_query_widget.h"
#include "DatabaseManager.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QDebug>

AncestorQueryWidget::AncestorQueryWidget(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AncestorQueryWidget)
    , m_currentGenealogyId(genealogyId)
    , m_startPersonId(0)
{
    ui->setupUi(this);

    m_ancestorTreeModel = new QStandardItemModel(this);
    ui->ancestorTreeView->setModel(m_ancestorTreeModel);
    ui->ancestorTreeView->setHeaderHidden(false);
    ui->ancestorTreeView->setAnimated(true);
    ui->ancestorTreeView->setIndentation(20);

    connect(ui->searchPushButton, &QPushButton::clicked, this, &AncestorQueryWidget::onSearchAncestors);
    connect(ui->clearPushButton, &QPushButton::clicked, this, &AncestorQueryWidget::onClearResults);
    connect(ui->ancestorTreeView, &QTreeView::clicked, this, &AncestorQueryWidget::onPersonSelected);
    connect(ui->ancestorTreeView, &QTreeView::expanded, this, &AncestorQueryWidget::onItemExpanded);
}

AncestorQueryWidget::~AncestorQueryWidget()
{
    delete ui;
}

void AncestorQueryWidget::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
}

void AncestorQueryWidget::onSearchAncestors()
{
    bool ok;
    int personId = QInputDialog::getInt(this, "祖先查询",
        "请输入成员ID:", 0, 1, 99999999, 1, &ok);

    if (!ok) {
        return;
    }

    m_startPersonId = personId;

    QVariantList member = DatabaseManager::instance().getMember(personId);
    if (member.isEmpty()) {
        QMessageBox::information(this, "查询结果", "未找到该成员!");
        return;
    }

    displayAncestorTree(member.first().toMap());
}

void AncestorQueryWidget::displayAncestorTree(const QVariantMap& startPerson)
{
    m_ancestorTreeModel->clear();
    m_loadedIds.clear();

    QString name = startPerson["name"].toString();
    QString genderStr = startPerson["gender"].toString();
    QChar gender = genderStr.isEmpty() ? 'M' : genderStr.at(0);
    int generation = startPerson["generation"].toInt();
    int personId = startPerson["person_id"].toInt();

    QString text = QString("%1 (%2) [%3代]").arg(name).arg(gender == 'M' ? "男" : "女").arg(generation);
    QStandardItem* rootItem = new QStandardItem(text);
    rootItem->setData(personId, Qt::UserRole + 1);
    rootItem->setData(true, Qt::UserRole + 2);  // 已加载
    rootItem->setEditable(false);

    m_loadedIds.insert(personId);
    
    loadParents(rootItem, personId);
    rootItem->setData(true, Qt::UserRole + 2);

    m_ancestorTreeModel->appendRow(rootItem);
    ui->ancestorTreeView->expandAll();
}

bool AncestorQueryWidget::checkHasParents(int personId)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT f.family_id
        FROM persons p
        JOIN families f ON p.birth_family_id = f.family_id
        WHERE p.person_id = ?
    )");
    query.addBindValue(personId);
    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

void AncestorQueryWidget::onItemExpanded(const QModelIndex& index)
{
    QStandardItem* item = m_ancestorTreeModel->itemFromIndex(index);
    if (!item) return;

    bool loaded = item->data(Qt::UserRole + 2).toBool();
    if (loaded) return;

    int personId = item->data(Qt::UserRole + 1).toInt();
    
    item->removeRows(0, item->rowCount());
    
    loadParents(item, personId);
    item->setData(true, Qt::UserRole + 2);
}

void AncestorQueryWidget::loadParents(QStandardItem* parentItem, int personId)
{
    qDebug() << "Loading parents for person:" << personId;
    
    QSqlQuery query;
    query.prepare(R"(
        SELECT f.husband_id, f.wife_id
        FROM persons p
        JOIN families f ON p.birth_family_id = f.family_id
        WHERE p.person_id = ?
    )");
    query.addBindValue(personId);
    
    if (query.exec() && query.next()) {
        int fatherId = query.value(0).toInt();
        int motherId = query.value(1).toInt();
        
        qDebug() << "Found parents - father:" << fatherId << "mother:" << motherId;

        if (fatherId > 0 && !m_loadedIds.contains(fatherId)) {
            addPersonItem(parentItem, fatherId);
        }
        if (motherId > 0 && !m_loadedIds.contains(motherId)) {
            addPersonItem(parentItem, motherId);
        }
    } else {
        qDebug() << "No parents found or query failed";
    }
}

void AncestorQueryWidget::addPersonItem(QStandardItem* parentItem, int personId)
{
    QVariantList member = DatabaseManager::instance().getMember(personId);
    if (member.isEmpty()) {
        return;
    }

    QVariantMap map = member.first().toMap();
    QString name = map["name"].toString();
    QString genderStr = map["gender"].toString();
    QChar gender = genderStr.isEmpty() ? 'M' : genderStr.at(0);
    int generation = map["generation"].toInt();

    QString text = QString("%1 (%2) [%3代]").arg(name).arg(gender == 'M' ? "男" : "女").arg(generation);
    QStandardItem* personItem = new QStandardItem(text);
    personItem->setData(personId, Qt::UserRole + 1);
    personItem->setData(false, Qt::UserRole + 2);
    personItem->setEditable(false);

    bool hasParents = checkHasParents(personId);
    if (hasParents) {
        QStandardItem* placeholder = new QStandardItem("");
        personItem->appendRow(placeholder);
    }

    m_loadedIds.insert(personId);
    parentItem->appendRow(personItem);
}

void AncestorQueryWidget::onPersonSelected(const QModelIndex& index)
{
    QStandardItem* item = m_ancestorTreeModel->itemFromIndex(index);
    if (item) {
        int personId = item->data(Qt::UserRole + 1).toInt();
        if (personId > 0) {
            emit personSelected(personId);
        }
    }
}

void AncestorQueryWidget::onClearResults()
{
    m_ancestorTreeModel->clear();
}
