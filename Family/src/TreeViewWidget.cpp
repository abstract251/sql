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

    QString name;
    if (m_selectedRootId > 0) {
        QVariantList member = DatabaseManager::instance().getMember(m_selectedRootId);
        if (!member.isEmpty()) {
            name = member.first().toMap()["name"].toString();
        }
    }

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
            name = query.value(1).toString();
        }
    }

    if (m_selectedRootId > 0) {
        QVariantList member = DatabaseManager::instance().getMember(m_selectedRootId);
        if (!member.isEmpty()) {
            QVariantMap map = member.first().toMap();
            QChar gender = map["gender"].toString().at(0);
            QStandardItem* rootItem = createPersonItem(m_selectedRootId, map["name"].toString(), gender, map["generation"].toInt());

            QVariantList descendants = DatabaseManager::instance().getDescendants(m_selectedRootId, 10);

            QMap<int, QStandardItem*> personItems;
            personItems[m_selectedRootId] = rootItem;

            for (const QVariant& v : descendants) {
                QVariantMap descMap = v.toMap();
                int personId = descMap["person_id"].toInt();
                QChar gender = descMap["gender"].toString().at(0);

                QStandardItem* personItem = createPersonItem(personId, descMap["name"].toString(), gender, descMap["generation"].toInt());
                personItems[personId] = personItem;
            }

            for (const QVariant& v : descendants) {
                QVariantMap descMap = v.toMap();
                int personId = descMap["person_id"].toInt();
                QString path = descMap["path"].toString();

                QStringList pathParts = path.split("->");
                if (pathParts.size() >= 2) {
                    int parentId = pathParts[pathParts.size() - 2].toInt();
                    if (personItems.contains(parentId)) {
                        personItems[parentId]->appendRow(personItems[personId]);
                    }
                }
            }

            m_treeModel->appendRow(rootItem);
            ui->treeView->expandAll();
        }
    }
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
