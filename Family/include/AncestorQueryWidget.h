#ifndef ANCESTORQUERYWIDGET_H
#define ANCESTORQUERYWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlQueryModel>

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
    void onPersonSelected(const QModelIndex& index);
    void onClearResults();

private:
    void displayAncestorTree(const QVariantList& ancestors);

    Ui::AncestorQueryWidget *ui;
    int m_currentGenealogyId;
    QStandardItemModel* m_ancestorTreeModel;
};

#endif
