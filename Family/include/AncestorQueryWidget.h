#ifndef ANCESTORQUERYWIDGET_H
#define ANCESTORQUERYWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSet>

namespace Ui {
class AncestorQueryWidget;
}

class AncestorQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AncestorQueryWidget(int genealogyId, QWidget *parent = nullptr);
    ~AncestorQueryWidget();

    void setGenealogyId(int genealogyId);

signals:
    void personSelected(int personId);

private slots:
    void onSearchAncestors();
    void onClearResults();
    void onPersonSelected(const QModelIndex& index);
    void onItemExpanded(const QModelIndex& index);

private:
    void displayAncestorTree(const QVariantMap& startPerson);
    void loadParents(QStandardItem* parentItem, int personId);
    void addPersonItem(QStandardItem* parentItem, int personId);
    bool checkHasParents(int personId);

    Ui::AncestorQueryWidget *ui;
    QStandardItemModel* m_ancestorTreeModel;
    int m_currentGenealogyId;
    int m_startPersonId;
    QSet<int> m_loadedIds;
};

#endif // ANCESTORQUERYWIDGET_H
