#ifndef TREEVIEWWIDGET_H
#define TREEVIEWWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlQueryModel>

namespace Ui {
class TreeViewWidget;
}

class TreeViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeViewWidget(int genealogyId, QWidget *parent = nullptr);
    ~TreeViewWidget();

    void setGenealogyId(int genealogyId);
    void refreshTree();

signals:
    void personSelected(int personId);

private slots:
    void onPersonSelected(const QModelIndex& index);
    void onExpandAll();
    void onCollapseAll();
    void onRefreshTree();
    void onItemExpanded(const QModelIndex& index);

private:
    void loadChildren(QStandardItem* parentItem, int parentId);
    bool checkHasChildren(int personId);
    QStandardItem* createPersonItem(int personId, const QString& name, QChar gender, int generation);

    Ui::TreeViewWidget *ui;
    int m_currentGenealogyId;
    int m_selectedRootId;
    QStandardItemModel* m_treeModel;
};

#endif
