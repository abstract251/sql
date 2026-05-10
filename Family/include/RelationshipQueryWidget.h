#ifndef RELATIONSHIPQUERYWIDGET_H
#define RELATIONSHIPQUERYWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class RelationshipQueryWidget;
}

class RelationshipQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RelationshipQueryWidget(int genealogyId, QWidget *parent = nullptr);
    ~RelationshipQueryWidget();

    void setGenealogyId(int genealogyId);

signals:
    void personSelected(int personId);

private slots:
    void onSearchRelationship();
    void onClearResults();

private:
    QString determineRelationshipType(int level1, int level2);

    Ui::RelationshipQueryWidget *ui;
    int m_currentGenealogyId;
    QStandardItemModel* m_relationshipModel;
};

#endif
