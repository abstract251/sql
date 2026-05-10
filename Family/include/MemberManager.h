#ifndef MEMBERMANAGER_H
#define MEMBERMANAGER_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class MemberManager;
}

class MemberManager : public QWidget
{
    Q_OBJECT

public:
    explicit MemberManager(int genealogyId, QWidget *parent = nullptr);
    ~MemberManager();

    void setGenealogyId(int genealogyId);
    void refreshMembers();
    void searchMembers(const QString& namePattern);

signals:
    void memberSelected(int memberId);
    void memberAdded(int memberId);
    void memberUpdated(int memberId);
    void memberDeleted(int memberId);

private slots:
    void onAddMember();
    void onEditMember();
    void onDeleteMember();
    void onSearchButtonClicked();
    void onMemberTableClicked(const QModelIndex& index);
    void onMemberTableDoubleClicked(const QModelIndex& index);
    void onClearSearch();

private:
    void setupMemberModel();
    void loadFamilyComboBox();

    Ui::MemberManager *ui;
    int m_currentGenealogyId;
    int m_currentMemberId;
    QSqlQueryModel* m_memberModel;
    QSqlQueryModel* m_familyModel;
};

#endif
