#ifndef GENEALOGYMANAGER_H
#define GENEALOGYMANAGER_H

#include <QWidget>
#include <QSqlQueryModel>
#include "User.h"

namespace Ui {
class GenealogyManager;
}

class GenealogyManager : public QWidget
{
    Q_OBJECT

public:
    explicit GenealogyManager(const User& currentUser, QWidget *parent = nullptr);
    ~GenealogyManager();

    void refreshGenealogies();

signals:
    void genealogySelected(int genealogyId);
    void genealogyCreated(int genealogyId);
    void genealogyDeleted(int genealogyId);

public slots:
    void onCreateGenealogy();

private slots:
    void onEditGenealogy();
    void onDeleteGenealogy();
    void onInviteUser();
    void onGenealogyTableClicked(const QModelIndex& index);
    void onGenealogyTableDoubleClicked(const QModelIndex& index);

private:
    void setupGenealogyModel();
    bool isOwner(int creatorId);

    Ui::GenealogyManager *ui;
    User m_currentUser;
    QSqlQueryModel* m_genealogyModel;
    int m_currentGenealogyId;
};

#endif
