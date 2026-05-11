#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantList>
#include <QDebug>
#include <QMutex>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    void setDatabase(const QSqlDatabase& db);
    QSqlDatabase database() const;
    
    bool initializeDatabase();
    bool checkAndCreateDatabase();
    bool executeSqlFromFile(const QString& filePath);
    
    QString lastError() const;
    void setLastError(const QString& error);

    bool executeQuery(const QString& queryStr, QSqlQuery& query);
    QSqlQuery executeQuery(const QString& queryStr);
    bool executePreparedQuery(QSqlQuery& query);

    bool userExists(const QString& username);
    int getUserId(const QString& username);
    bool validateUser(const QString& username, const QString& passwordHash);
    bool createUser(const QString& username, const QString& passwordHash);

    QVariantList getGenealogiesForUser(int userId);
    QVariantList getAllGenealogies();

    bool createGenealogy(int creatorId, const QString& name, const QString& surname,
                         const QString& compileTime, const QString& description);
    bool deleteGenealogy(int genealogyId);
    bool updateGenealogy(int genealogyId, const QString& name, const QString& surname,
                         const QString& compileTime, const QString& description);

    QVariantList getMembers(int genealogyId, int limit = 1000, int offset = 0);
    QVariantList searchMembersByName(const QString& namePattern, int genealogyId = -1);
    QVariantList getMember(int personId);
    int addMember(const QString& name, QChar gender, int birthYear, int deathYear,
                  const QString& biography, int generation, int genealogyId, int birthFamilyId = -1);
    bool updateMember(int personId, const QString& name, QChar gender, int birthYear,
                      int deathYear, const QString& biography, int generation);
    bool deleteMember(int personId);

    QVariantList getFamilyMembers(int personId);
    QVariantList getSpouseAndChildren(int personId);

    QVariantList getAncestors(int personId);
    QVariantList getDescendants(int personId, int maxDepth = 10);

    QVariantList findRelationship(int person1Id, int person2Id);

    QVariantList getGenerationStats(int genealogyId);
    QVariantList getGenderStats(int genealogyId);

    bool inviteUser(int genealogyId, int userId, const QString& role = "editor");

    QVariantList getFamilyById(int familyId);
    int createFamily(int husbandId, int wifeId, int genealogyId, int marriageYear);

    bool isUserInGenealogy(int userId, int genealogyId);

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    QMutex m_mutex;
    QString m_lastError;
};

#endif
