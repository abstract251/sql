#ifndef CHILDRENQUERYWIDGET_H
#define CHILDRENQUERYWIDGET_H

#include <QWidget>

namespace Ui {
class ChildrenQueryWidget;
}

class ChildrenQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChildrenQueryWidget(int genealogyId, QWidget *parent = nullptr);
    ~ChildrenQueryWidget();

    void setGenealogyId(int genealogyId);

private slots:
    void onSearchClicked();

private:
    Ui::ChildrenQueryWidget *ui;
    int m_currentGenealogyId;
};

#endif // CHILDRENQUERYWIDGET_H
