#include "DatabaseManager.h"
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::checkAndCreateDatabase()
{
    QMutexLocker locker(&m_mutex);

    QString dbName = m_db.databaseName();
    QString userName = m_db.userName();
    QString password = m_db.password();
    QString hostName = m_db.hostName();
    int port = m_db.port();

    QSqlDatabase tempDb = QSqlDatabase::addDatabase("QPSQL", "temp_connection");
    tempDb.setHostName(hostName);
    tempDb.setPort(port);
    tempDb.setUserName(userName);
    tempDb.setPassword(password);
    tempDb.setDatabaseName("postgres");

    if (!tempDb.open()) {
        QString errorMsg = tempDb.lastError().text();
        setLastError(errorMsg);
        return false;
    }

    QSqlQuery query(tempDb);
    QString checkDbQuery = QString("SELECT 1 FROM pg_database WHERE datname = '%1'").arg(dbName);
    if (!query.exec(checkDbQuery)) {
        QString errorMsg = query.lastError().text();
        setLastError(errorMsg);
        tempDb.close();
        QSqlDatabase::removeDatabase("temp_connection");
        return false;
    }

    if (!query.next()) {
        QString createDbQuery = QString("CREATE DATABASE \"%1\" WITH ENCODING = 'UTF8'").arg(dbName);
        if (!query.exec(createDbQuery)) {
            QString errorMsg = query.lastError().text();
            setLastError(errorMsg);
            tempDb.close();
            QSqlDatabase::removeDatabase("temp_connection");
            return false;
        }
    }

    tempDb.close();
    QSqlDatabase::removeDatabase("temp_connection");
    setLastError("");
    return true;
}

bool DatabaseManager::executeSqlFromFile(const QString& filePath)
{
    QMutexLocker locker(&m_mutex);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "executeSqlFromFile: Cannot open file:" << filePath;
        return false;
    }

    QTextStream in(&file);
    QString sqlContent = in.readAll();
    file.close();

    QStringList sqlStatements = sqlContent.split(';', Qt::SkipEmptyParts);
    for (int i = sqlStatements.size() - 1; i >= 0; --i) {
        sqlStatements[i] = sqlStatements[i].trimmed();
        if (sqlStatements[i].isEmpty() || sqlStatements[i].startsWith("--")) {
            sqlStatements.removeAt(i);
        }
    }

    qDebug() << "executeSqlFromFile: Total statements to execute:" << sqlStatements.size();

    int stmtCount = 0;
    foreach (QString statement, sqlStatements) {
        stmtCount++;
        
        if (statement.startsWith("\\c ")) {
            qDebug() << "executeSqlFromFile: Skipping \\c command";
            continue;
        }

        if (statement.startsWith("DROP DATABASE") || statement.startsWith("CREATE DATABASE")) {
            qDebug() << "executeSqlFromFile: Skipping DDL statement";
            continue;
        }

        qDebug() << "executeSqlFromFile: Executing statement" << stmtCount << "- first 100 chars:" << statement.left(100);
        
        QSqlQuery query(m_db);
        if (!query.exec(statement)) {
            qDebug() << "executeSqlFromFile: SQL execution failed:" << query.lastError().text();
            qDebug() << "executeSqlFromFile: Failed statement:" << statement;
            return false;
        }
        
        qDebug() << "executeSqlFromFile: Statement" << stmtCount << "completed successfully";
    }

    qDebug() << "executeSqlFromFile: All statements executed successfully";
    return true;
}

bool DatabaseManager::initializeDatabase()
{
    QMutexLocker locker(&m_mutex);

    if (!m_db.isOpen()) {
        if (!m_db.open()) {
            return false;
        }
    }

    QSqlQuery query(m_db);

    // 检查 users 表是否存在
    if (!query.exec("SELECT COUNT(*) FROM information_schema.tables WHERE table_name = 'users'")) {
        return false;
    }

    bool needsInit = false;
    if (query.next() && query.value(0).toInt() == 0) {
        // users 表不存在，需要初始化
        needsInit = true;
    } else {
        // users 表存在，检查是否有数据
        query.exec("SELECT COUNT(*) FROM users");
        if (query.next() && query.value(0).toInt() == 0) {
            // users 表为空，也需要初始化数据
            needsInit = true;
        }
    }

    if (needsInit) {
        QString schemaPath = QCoreApplication::applicationDirPath() + "/../database/schema.sql";
        if (!QFile::exists(schemaPath)) {
            schemaPath = QCoreApplication::applicationDirPath() + "/database/schema.sql";
        }
        if (!QFile::exists(schemaPath)) {
            schemaPath = QDir::currentPath() + "/database/schema.sql";
        }

        if (QFile::exists(schemaPath)) {
            locker.unlock();
            bool result = executeSqlFromFile(schemaPath);
            // 确保 admin 账号存在
            ensureAdminAccount();
            return result;
        } else {
            QStringList createTableQueries = {
                "CREATE TABLE IF NOT EXISTS users ("
                "user_id SERIAL PRIMARY KEY, "
                "username VARCHAR(50) NOT NULL UNIQUE, "
                "password_hash VARCHAR(255) NOT NULL, "
                "email VARCHAR(100), "
                "created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)",

                "CREATE TABLE IF NOT EXISTS genealogies ("
                "genealogy_id SERIAL PRIMARY KEY, "
                "name VARCHAR(100) NOT NULL, "
                "surname VARCHAR(50) NOT NULL, "
                "compile_time DATE, "
                "description TEXT, "
                "creator_id INT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE ON UPDATE CASCADE, "
                "created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP)",

                "CREATE TABLE IF NOT EXISTS user_genealogy ("
                "user_id INT NOT NULL REFERENCES users(user_id) ON DELETE CASCADE ON UPDATE CASCADE, "
                "genealogy_id INT NOT NULL REFERENCES genealogies(genealogy_id) ON DELETE CASCADE ON UPDATE CASCADE, "
                "role VARCHAR(20) NOT NULL DEFAULT 'editor', "
                "joined_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                "PRIMARY KEY (user_id, genealogy_id))",

                "CREATE TABLE IF NOT EXISTS families ("
                "family_id SERIAL PRIMARY KEY, "
                "husband_id INT REFERENCES persons(person_id) ON DELETE SET NULL ON UPDATE CASCADE, "
                "wife_id INT REFERENCES persons(person_id) ON DELETE SET NULL ON UPDATE CASCADE, "
                "genealogy_id INT NOT NULL REFERENCES genealogies(genealogy_id) ON DELETE CASCADE ON UPDATE CASCADE, "
                "marriage_year INT)",

                "CREATE TABLE IF NOT EXISTS persons ("
                "person_id SERIAL PRIMARY KEY, "
                "name VARCHAR(100) NOT NULL, "
                "gender CHAR(1) NOT NULL CHECK (gender IN ('M', 'F')), "
                "birth_year INT, "
                "death_year INT, "
                "biography TEXT, "
                "generation INT NOT NULL DEFAULT 1, "
                "genealogy_id INT NOT NULL REFERENCES genealogies(genealogy_id) ON DELETE CASCADE ON UPDATE CASCADE, "
                "birth_family_id INT REFERENCES families(family_id) ON DELETE SET NULL ON UPDATE CASCADE)",

                "INSERT INTO users (username, password_hash, email) VALUES "
                "('admin', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 'admin@example.com') "
                "ON CONFLICT (username) DO NOTHING",

                "INSERT INTO genealogies (name, surname, compile_time, description, creator_id) VALUES "
                "('示例族谱', '张', '2024-01-01', '这是一个示例族谱', 1) "
                "ON CONFLICT DO NOTHING",

                "INSERT INTO user_genealogy (user_id, genealogy_id, role) VALUES "
                "(1, 1, 'creator') ON CONFLICT DO NOTHING"
            };

            foreach (QString sql, createTableQueries) {
                if (!query.exec(sql)) {
                    return false;
                }
            }
        }
    } else {
        // users 表已存在且有数据，确保 admin 账号存在
        ensureAdminAccount();
    }

    return true;
}

void DatabaseManager::ensureAdminAccount()
{
    if (!m_db.isOpen()) {
        qDebug() << "ensureAdminAccount: Database is not open!";
        return;
    }
    
    // 先检查 admin 用户是否已存在
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    checkQuery.addBindValue("admin");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "ensureAdminAccount: Admin account already exists";
        return;
    }
    
    // 重置 users 表的自增序列（避免主键冲突）
    QSqlQuery resetSeqQuery(m_db);
    resetSeqQuery.exec("SELECT setval('users_user_id_seq', (SELECT MAX(user_id) FROM users))");
    
    // 插入 admin 账号，密码为 "123456"
    // SHA-256("123456") = 8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO users (username, password_hash, email) VALUES 
        (?, '8d969eef6ecad3c29a3a629280e686cf0c3f5d5a86aff3ca12020c923adc6c92', 'admin@example.com')
    )");
    query.addBindValue("admin");
    
    bool result = query.exec();
    
    if (!result) {
        qDebug() << "ensureAdminAccount: Failed to insert admin account:" << query.lastError().text();
    } else {
        qDebug() << "ensureAdminAccount: Admin account created successfully with password '123456'";
    }
}

QString DatabaseManager::lastError() const
{
    return m_lastError;
}

void DatabaseManager::setLastError(const QString& error)
{
    m_lastError = error;
}

void DatabaseManager::setDatabase(const QSqlDatabase& db)
{
    m_db = db;
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}

bool DatabaseManager::executeQuery(const QString& queryStr, QSqlQuery& query)
{
    QMutexLocker locker(&m_mutex);
    if (!query.exec(queryStr)) {
        setLastError(query.lastError().text());
        return false;
    }
    setLastError("");
    return true;
}

QSqlQuery DatabaseManager::executeQuery(const QString& queryStr)
{
    QMutexLocker locker(&m_mutex);
    QSqlQuery query(m_db);
    if (!query.exec(queryStr)) {
        setLastError(query.lastError().text());
    } else {
        setLastError("");
    }
    return query;
}

bool DatabaseManager::executePreparedQuery(QSqlQuery& query)
{
    QMutexLocker locker(&m_mutex);
    bool result = query.exec();
    if (!result) {
        setLastError(query.lastError().text());
    } else {
        setLastError("");
    }
    return result;
}

bool DatabaseManager::userExists(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
    query.addBindValue(username);
    if (executePreparedQuery(query) && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

int DatabaseManager::getUserId(const QString& username)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT user_id FROM users WHERE username = ?");
    query.addBindValue(username);
    if (executePreparedQuery(query) && query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool DatabaseManager::validateUser(const QString& username, const QString& passwordHash)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT password_hash FROM users WHERE username = ?");
    query.addBindValue(username);
    if (executePreparedQuery(query) && query.next()) {
        return query.value(0).toString() == passwordHash;
    }
    return false;
}

bool DatabaseManager::createUser(const QString& username, const QString& passwordHash)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO users (username, password_hash) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(passwordHash);
    return executePreparedQuery(query);
}

QVariantList DatabaseManager::getGenealogiesForUser(int userId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT g.genealogy_id, g.name, g.surname, g.compile_time,
               g.description, g.creator_id, ug.role
        FROM genealogies g
        JOIN user_genealogy ug ON g.genealogy_id = ug.genealogy_id
        WHERE ug.user_id = ?
        ORDER BY g.name
    )");
    query.addBindValue(userId);
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["genealogy_id"] = query.value(0);
            map["name"] = query.value(1);
            map["surname"] = query.value(2);
            map["compile_time"] = query.value(3);
            map["description"] = query.value(4);
            map["creator_id"] = query.value(5);
            map["role"] = query.value(6);
            result.append(map);
        }
    }
    return result;
}

QVariantList DatabaseManager::getAllGenealogies()
{
    QVariantList result;
    QSqlQuery query = executeQuery("SELECT genealogy_id, name, surname, compile_time, description, creator_id FROM genealogies ORDER BY name");
    while (query.next()) {
        QVariantMap map;
        map["genealogy_id"] = query.value(0);
        map["name"] = query.value(1);
        map["surname"] = query.value(2);
        map["compile_time"] = query.value(3);
        map["description"] = query.value(4);
        map["creator_id"] = query.value(5);
        result.append(map);
    }
    return result;
}

bool DatabaseManager::createGenealogy(int creatorId, const QString& name, const QString& surname,
                                      const QString& compileTime, const QString& description)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO genealogies (name, surname, compile_time, description, creator_id) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(surname);
    query.addBindValue(compileTime);
    query.addBindValue(description);
    query.addBindValue(creatorId);

    if (executePreparedQuery(query)) {
        int genealogyId = query.lastInsertId().toInt();
        query.prepare("INSERT INTO user_genealogy (user_id, genealogy_id, role) VALUES (?, ?, 'creator')");
        query.addBindValue(creatorId);
        query.addBindValue(genealogyId);
        return executePreparedQuery(query);
    }
    return false;
}

bool DatabaseManager::deleteGenealogy(int genealogyId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM genealogies WHERE genealogy_id = ?");
    query.addBindValue(genealogyId);
    return executePreparedQuery(query);
}

bool DatabaseManager::updateGenealogy(int genealogyId, const QString& name, const QString& surname,
                                       const QString& compileTime, const QString& description)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE genealogies SET name = ?, surname = ?, compile_time = ?, description = ? WHERE genealogy_id = ?");
    query.addBindValue(name);
    query.addBindValue(surname);
    query.addBindValue(compileTime);
    query.addBindValue(description);
    query.addBindValue(genealogyId);
    return executePreparedQuery(query);
}

QVariantList DatabaseManager::getMembers(int genealogyId, int limit, int offset)
{
    QVariantList result;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT person_id, name, gender, birth_year, death_year, biography,
               generation, genealogy_id, birth_family_id
        FROM persons
        WHERE genealogy_id = ?
        ORDER BY generation, birth_year
        LIMIT ? OFFSET ?
    )");
    query.addBindValue(genealogyId);
    query.addBindValue(limit);
    query.addBindValue(offset);
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["person_id"] = query.value(0);
            map["name"] = query.value(1);
            map["gender"] = query.value(2);
            map["birth_year"] = query.value(3);
            map["death_year"] = query.value(4);
            map["biography"] = query.value(5);
            map["generation"] = query.value(6);
            map["genealogy_id"] = query.value(7);
            map["birth_family_id"] = query.value(8);
            result.append(map);
        }
    }
    return result;
}

QVariantList DatabaseManager::searchMembersByName(const QString& namePattern, int genealogyId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    if (genealogyId > 0) {
        query.prepare(R"(
            SELECT person_id, name, gender, birth_year, death_year, generation, genealogy_id
            FROM persons
            WHERE name LIKE ? AND genealogy_id = ?
            ORDER BY name, birth_year
            LIMIT 100
        )");
        query.addBindValue(QString("%") + namePattern + "%");
        query.addBindValue(genealogyId);
    } else {
        query.prepare(R"(
            SELECT person_id, name, gender, birth_year, death_year, generation, genealogy_id
            FROM persons
            WHERE name LIKE ?
            ORDER BY name, birth_year
            LIMIT 100
        )");
        query.addBindValue(QString("%") + namePattern + "%");
    }
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["person_id"] = query.value(0);
            map["name"] = query.value(1);
            map["gender"] = query.value(2);
            map["birth_year"] = query.value(3);
            map["death_year"] = query.value(4);
            map["generation"] = query.value(5);
            map["genealogy_id"] = query.value(6);
            result.append(map);
        }
    }
    return result;
}

QVariantList DatabaseManager::getMember(int personId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.death_year,
               p.biography, p.generation, p.genealogy_id, p.birth_family_id,
               hp.name AS father_name, wp.name AS mother_name
        FROM persons p
        LEFT JOIN families f ON p.birth_family_id = f.family_id
        LEFT JOIN persons hp ON f.husband_id = hp.person_id
        LEFT JOIN persons wp ON f.wife_id = wp.person_id
        WHERE p.person_id = ?
    )");
    query.addBindValue(personId);
    if (executePreparedQuery(query) && query.next()) {
        QVariantMap map;
        map["person_id"] = query.value(0);
        map["name"] = query.value(1);
        map["gender"] = query.value(2);
        map["birth_year"] = query.value(3);
        map["death_year"] = query.value(4);
        map["biography"] = query.value(5);
        map["generation"] = query.value(6);
        map["genealogy_id"] = query.value(7);
        map["birth_family_id"] = query.value(8);
        map["father_name"] = query.value(9);
        map["mother_name"] = query.value(10);
        result.append(map);
    }
    return result;
}

int DatabaseManager::addMember(const QString& name, QChar gender, int birthYear, int deathYear,
                                 const QString& biography, int generation, int genealogyId, int birthFamilyId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO persons (name, gender, birth_year, death_year, biography, generation, genealogy_id, birth_family_id)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?)
    )");
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(birthYear > 0 ? birthYear : QVariant());
    query.addBindValue(deathYear > 0 ? deathYear : QVariant());
    query.addBindValue(biography);
    query.addBindValue(generation);
    query.addBindValue(genealogyId);
    query.addBindValue(birthFamilyId > 0 ? birthFamilyId : QVariant());
    if (executePreparedQuery(query)) {
        return query.lastInsertId().toInt();
    }
    return -1;
}

bool DatabaseManager::updateMember(int personId, const QString& name, QChar gender, int birthYear,
                                   int deathYear, const QString& biography, int generation)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE persons SET name = ?, gender = ?, birth_year = ?, death_year = ?, biography = ?, generation = ?
        WHERE person_id = ?
    )");
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(birthYear > 0 ? birthYear : QVariant());
    query.addBindValue(deathYear > 0 ? deathYear : QVariant());
    query.addBindValue(biography);
    query.addBindValue(generation);
    query.addBindValue(personId);
    return executePreparedQuery(query);
}

bool DatabaseManager::deleteMember(int personId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM persons WHERE person_id = ?");
    query.addBindValue(personId);
    return executePreparedQuery(query);
}

QVariantList DatabaseManager::getFamilyMembers(int personId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.death_year,
               p.generation, f.family_id
        FROM persons p
        LEFT JOIN families f ON p.birth_family_id = f.family_id
        WHERE p.person_id = ?
    )");
    query.addBindValue(personId);
    if (executePreparedQuery(query) && query.next()) {
        QVariantMap map;
        map["person_id"] = query.value(0);
        map["name"] = query.value(1);
        map["gender"] = query.value(2);
        map["birth_year"] = query.value(3);
        map["death_year"] = query.value(4);
        map["generation"] = query.value(5);
        map["family_id"] = query.value(6);
        result.append(map);
    }
    return result;
}

QVariantList DatabaseManager::getSpouseAndChildren(int personId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    
    // 查询配偶
    query.prepare(R"(
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.generation, '配偶' as relation_type
        FROM persons p
        JOIN families f ON (f.husband_id = p.person_id OR f.wife_id = p.person_id)
        WHERE (f.husband_id = ? OR f.wife_id = ?)
          AND p.person_id != ?
    )");
    query.addBindValue(personId);
    query.addBindValue(personId);
    query.addBindValue(personId);
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["person_id"] = query.value(0);
            map["name"] = query.value(1);
            map["gender"] = query.value(2);
            map["birth_year"] = query.value(3);
            map["generation"] = query.value(4);
            map["relation_type"] = query.value(5);
            result.append(map);
        }
    }
    
    // 查询子女
    query.prepare(R"(
        SELECT p.person_id, p.name, p.gender, p.birth_year, p.generation, '子女' as relation_type
        FROM persons p
        JOIN families f ON p.birth_family_id = f.family_id
        WHERE (f.husband_id = ? OR f.wife_id = ?)
    )");
    query.addBindValue(personId);
    query.addBindValue(personId);
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["person_id"] = query.value(0);
            map["name"] = query.value(1);
            map["gender"] = query.value(2);
            map["birth_year"] = query.value(3);
            map["generation"] = query.value(4);
            map["relation_type"] = query.value(5);
            result.append(map);
        }
    }
    
    return result;
}

QVariantList DatabaseManager::getAncestors(int personId)
{
    QVariantList result;
    QSqlQuery query = executeQuery(QString("SELECT * FROM sp_get_ancestors(%1)").arg(personId));
    while (query.next()) {
        QVariantMap map;
        map["person_id"] = query.value(0);
        map["name"] = query.value(1);
        map["gender"] = query.value(2);
        map["birth_year"] = query.value(3);
        map["death_year"] = query.value(4);
        map["generation"] = query.value(5);
        map["level"] = query.value(6);
        map["path"] = query.value(7);
        result.append(map);
    }
    return result;
}

QVariantList DatabaseManager::getDescendants(int personId, int maxDepth)
{
    QVariantList result;
    QSqlQuery query = executeQuery(QString("SELECT * FROM sp_get_descendants(%1, %2)").arg(personId).arg(maxDepth));
    while (query.next()) {
        QVariantMap map;
        map["person_id"] = query.value(0);
        map["name"] = query.value(1);
        map["gender"] = query.value(2);
        map["birth_year"] = query.value(3);
        map["death_year"] = query.value(4);
        map["generation"] = query.value(5);
        map["level"] = query.value(6);
        map["path"] = query.value(7);
        result.append(map);
    }
    return result;
}

QVariantList DatabaseManager::findRelationship(int person1Id, int person2Id)
{
    QVariantList result;
    QSqlQuery query = executeQuery(QString("SELECT * FROM sp_find_relationship(%1, %2)").arg(person1Id).arg(person2Id));
    while (query.next()) {
        QVariantMap map;
        map["common_ancestor_id"] = query.value(0);
        map["common_ancestor_name"] = query.value(1);
        map["generation"] = query.value(2);
        map["path_to_person1"] = query.value(3);
        map["path_to_person2"] = query.value(4);
        map["relationship_type"] = query.value(5);
        result.append(map);
    }
    return result;
}

QVariantList DatabaseManager::getGenderStats(int genealogyId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    if (genealogyId > 0) {
        query.prepare(R"(
            SELECT gender, COUNT(*) as count
            FROM persons
            WHERE genealogy_id = ?
            GROUP BY gender
        )");
        query.addBindValue(genealogyId);
    } else {
        query.prepare("SELECT gender, COUNT(*) as count FROM persons GROUP BY gender");
    }
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["gender"] = query.value(0);
            map["count"] = query.value(1);
            result.append(map);
        }
    }
    return result;
}

QVariantList DatabaseManager::getGenerationStats(int genealogyId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    if (genealogyId > 0) {
        query.prepare(R"(
            SELECT generation, COUNT(*) as count
            FROM persons
            WHERE genealogy_id = ?
            GROUP BY generation
            ORDER BY generation
        )");
        query.addBindValue(genealogyId);
    } else {
        query.prepare("SELECT generation, COUNT(*) as count FROM persons GROUP BY generation ORDER BY generation");
    }
    if (executePreparedQuery(query)) {
        while (query.next()) {
            QVariantMap map;
            map["generation"] = query.value(0);
            map["count"] = query.value(1);
            result.append(map);
        }
    }
    return result;
}

bool DatabaseManager::inviteUser(int genealogyId, int userId, const QString& role)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO user_genealogy (user_id, genealogy_id, role) VALUES (?, ?, ?)");
    query.addBindValue(userId);
    query.addBindValue(genealogyId);
    query.addBindValue(role);
    return executePreparedQuery(query);
}

QVariantList DatabaseManager::getFamilyById(int familyId)
{
    QVariantList result;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT f.family_id, f.husband_id, f.wife_id, f.genealogy_id, f.marriage_year,
               hp.name as husband_name, wp.name as wife_name
        FROM families f
        LEFT JOIN persons hp ON f.husband_id = hp.person_id
        LEFT JOIN persons wp ON f.wife_id = wp.person_id
        WHERE f.family_id = ?
    )");
    query.addBindValue(familyId);
    if (executePreparedQuery(query) && query.next()) {
        QVariantMap map;
        map["family_id"] = query.value(0);
        map["husband_id"] = query.value(1);
        map["wife_id"] = query.value(2);
        map["genealogy_id"] = query.value(3);
        map["marriage_year"] = query.value(4);
        map["husband_name"] = query.value(5);
        map["wife_name"] = query.value(6);
        result.append(map);
    }
    return result;
}

int DatabaseManager::createFamily(int husbandId, int wifeId, int genealogyId, int marriageYear)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO families (husband_id, wife_id, genealogy_id, marriage_year)
        VALUES (?, ?, ?, ?)
    )");
    query.addBindValue(husbandId > 0 ? husbandId : QVariant());
    query.addBindValue(wifeId > 0 ? wifeId : QVariant());
    query.addBindValue(genealogyId);
    query.addBindValue(marriageYear > 0 ? marriageYear : QVariant());
    if (executePreparedQuery(query)) {
        return query.lastInsertId().toInt();
    }
    return -1;
}

bool DatabaseManager::isUserInGenealogy(int userId, int genealogyId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM user_genealogy WHERE user_id = ? AND genealogy_id = ?");
    query.addBindValue(userId);
    query.addBindValue(genealogyId);
    if (executePreparedQuery(query) && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
