#include "TreeViewWidget.h"
#include "ui_tree_view_widget.h"
#include "DatabaseManager.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QDebug>

TreeViewWidget::TreeViewWidget(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TreeViewWidget)
    , m_currentGenealogyId(genealogyId)
    , m_selectedRootId(0)
{
    ui->setupUi(this);

    m_treeModel = new QStandardItemModel(this);
    ui->treeView->setModel(m_treeModel);
    ui->treeView->setHeaderHidden(false);
    ui->treeView->setAnimated(true);
    ui->treeView->setIndentation(20);

    connect(ui->refreshPushButton, &QPushButton::clicked, this, &TreeViewWidget::onRefreshTree);
    connect(ui->expandAllPushButton, &QPushButton::clicked, this, &TreeViewWidget::onExpandAll);
    connect(ui->collapseAllPushButton, &QPushButton::clicked, this, &TreeViewWidget::onCollapseAll);
    connect(ui->treeView, &QTreeView::clicked, this, &TreeViewWidget::onPersonSelected);
    connect(ui->treeView, &QTreeView::expanded, this, &TreeViewWidget::onItemExpanded);
}

TreeViewWidget::~TreeViewWidget()
{
    delete ui;
}

void TreeViewWidget::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
    refreshTree();
}

void TreeViewWidget::refreshTree()
{
    bool ok;
    int rootId = QInputDialog::getInt(this, "选择根节点",
        "请输入起始成员ID(留空则显示第一代):", 0, 0, 99999999, 1, &ok);

    if (!ok) {
        return;
    }

    m_selectedRootId = rootId;
    m_treeModel->clear();

    if (m_selectedRootId == 0) {
        QSqlQuery query;
        if (m_currentGenealogyId > 0) {
            query.prepare("SELECT person_id, name FROM persons WHERE genealogy_id = ? AND generation = 1 LIMIT 1");
            query.addBindValue(m_currentGenealogyId);
        } else {
            query.prepare("SELECT person_id, name FROM persons WHERE generation = 1 LIMIT 1");
        }
        query.exec();
        if (query.next()) {
            m_selectedRootId = query.value(0).toInt();
        }
    }

    if (m_selectedRootId > 0) {
        QVariantList member = DatabaseManager::instance().getMember(m_selectedRootId);
        if (!member.isEmpty()) {
            QVariantMap map = member.first().toMap();
            QChar gender = map["gender"].toString().at(0);
            QStandardItem* rootItem = createPersonItem(m_selectedRootId, map["name"].toString(), gender, map["generation"].toInt());
            rootItem->setData(true, Qt::UserRole + 2);  // 标记已加载

            loadChildren(rootItem, m_selectedRootId);

            m_treeModel->appendRow(rootItem);
            ui->treeView->expandToDepth(1);
        }
    }
}

void TreeViewWidget::loadChildren(QStandardItem* parentItem, int parentId)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.generation
        FROM persons p
        JOIN families f ON p.birth_family_id = f.family_id
        WHERE (f.husband_id = ? OR f.wife_id = ?)
    )");
    query.addBindValue(parentId);
    query.addBindValue(parentId);
    
    QSet<int> addedIds;
    
    if (query.exec()) {
        while (query.next()) {
            int childId = query.value(0).toInt();
            
            if (addedIds.contains(childId)) {
                continue;
            }
            addedIds.insert(childId);

            QString name = query.value(1).toString();
            QChar gender = query.value(2).toString().at(0);
            int generation = query.value(4).toInt();

            QStandardItem* childItem = createPersonItem(childId, name, gender, generation);
            childItem->setData(false, Qt::UserRole + 2);

            bool hasChildren = checkHasChildren(childId);
            if (hasChildren) {
                QStandardItem* placeholder = new QStandardItem("");
                childItem->appendRow(placeholder);
            }

            parentItem->appendRow(childItem);
        }
    }
}

bool TreeViewWidget::checkHasChildren(int personId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM families WHERE husband_id = ? OR wife_id = ?");
    query.addBindValue(personId);
    query.addBindValue(personId);
    query.exec();
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

void TreeViewWidget::onItemExpanded(const QModelIndex& index)
{
    QStandardItem* item = m_treeModel->itemFromIndex(index);
    if (!item) return;

    bool loaded = item->data(Qt::UserRole + 2).toBool();
    if (loaded) return;

    int personId = item->data(Qt::UserRole + 1).toInt();
    
    item->removeRows(0, item->rowCount());
    loadChildren(item, personId);
    item->setData(true, Qt::UserRole + 2);
}

QStandardItem* TreeViewWidget::createPersonItem(int personId, const QString& name, QChar gender, int generation)
{
    QString text = QString("%1 (%2) [%3代]").arg(name).arg(gender == 'M' ? "男" : "女").arg(generation);
    QStandardItem* item = new QStandardItem(text);
    item->setData(personId, Qt::UserRole + 1);
    return item;
}

void TreeViewWidget::onPersonSelected(const QModelIndex& index)
{
    QStandardItem* item = m_treeModel->itemFromIndex(index);
    if (item) {
        int personId = item->data(Qt::UserRole + 1).toInt();
        emit personSelected(personId);
    }
}

void TreeViewWidget::onExpandAll()
{
    ui->treeView->expandAll();
}

void TreeViewWidget::onCollapseAll()
{
    ui->treeView->collapseAll();
}

void TreeViewWidget::onRefreshTree()
{
    refreshTree();
}