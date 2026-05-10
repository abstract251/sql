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

    QVariantList ancestors = DatabaseManager::instance().getAncestors(personId);

    if (ancestors.isEmpty()) {
        QMessageBox::information(this, "查询结果", "未找到该成员的祖先信息!");
        m_ancestorTreeModel->clear();
        return;
    }

    displayAncestorTree(ancestors);
}

void AncestorQueryWidget::displayAncestorTree(const QVariantList& ancestors)
{
    m_ancestorTreeModel->clear();

    if (ancestors.isEmpty()) {
        return;
    }

    QStandardItem* rootItem = new QStandardItem(QString("祖先链"));
    rootItem->setEditable(false);

    QMap<int, QStandardItem*> levelItems;
    levelItems[0] = rootItem;

    int currentLevel = -1;
    QStandardItem* currentParent = rootItem;

    for (const QVariant& v : ancestors) {
        QVariantMap map = v.toMap();
        int level = map["level"].toInt();
        QString name = map["name"].toString();
        QChar gender = map["gender"].toString().at(0);
        int generation = map["generation"].toInt();
        int personId = map["person_id"].toInt();

        QString text = QString("%1 (%2) [%3代]").arg(name).arg(gender == 'M' ? "男" : "女").arg(generation);

        QStandardItem* personItem = new QStandardItem(text);
        personItem->setData(personId, Qt::UserRole + 1);
        personItem->setEditable(false);

        if (level > currentLevel) {
            if (levelItems.contains(level - 1)) {
                currentParent = levelItems[level - 1];
            }
            currentLevel = level;
        }

        currentParent->appendRow(personItem);
        levelItems[level] = personItem;
        currentParent = personItem;
    }

    m_ancestorTreeModel->appendRow(rootItem);
    ui->ancestorTreeView->expandAll();
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
