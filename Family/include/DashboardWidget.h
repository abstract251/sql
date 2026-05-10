#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class DashboardWidget;
}

class DashboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardWidget(int genealogyId, QWidget *parent = nullptr);
    ~DashboardWidget();

    void refreshData();

signals:
    void genealogySelectionChanged(int genealogyId);

private slots:
    void onGenealogyComboChanged(int index);
    void updateStatistics();

private:
    Ui::DashboardWidget *ui;
    int m_currentGenealogyId;
    QSqlQueryModel* m_statsModel;
};

#endif
