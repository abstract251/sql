#ifndef SPOUSEQUERYWIDGET_H
#define SPOUSEQUERYWIDGET_H

#include <QWidget>

namespace Ui {
class SpouseQueryWidget;
}

class SpouseQueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpouseQueryWidget(int genealogyId, QWidget *parent = nullptr);
    ~SpouseQueryWidget();

    void setGenealogyId(int genealogyId);

private slots:
    void onSearchClicked();

private:
    Ui::SpouseQueryWidget *ui;
    int m_currentGenealogyId;
};

#endif // SPOUSEQUERYWIDGET_H
