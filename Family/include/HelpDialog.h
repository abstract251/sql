#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget* parent = nullptr);
    ~HelpDialog() = default;

private:
    void setupUi();
    QFrame* createFaqItem(const QString& question, const QString& answer);

    QScrollArea* m_scrollArea;
};

#endif
