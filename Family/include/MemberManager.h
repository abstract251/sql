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
    void searchMembers(const QString& namePattern, int birthYear = 0, int deathYear = 0, int generation = 0);

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
    void onSurnameChanged(int index);

private:
    void setupMemberModel();
    void loadFamilyComboBox();
    void loadSurnames();

    Ui::MemberManager *ui;
    int m_currentGenealogyId;
    int m_currentMemberId;
    QSqlQueryModel* m_memberModel;
    QSqlQueryModel* m_familyModel;
};

#endif
