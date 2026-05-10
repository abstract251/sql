#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    User() = default;
    User(int id, const QString& username, const QString& email = QString());

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString username() const { return m_username; }
    void setUsername(const QString& username) { m_username = username; }

    QString email() const { return m_email; }
    void setEmail(const QString& email) { m_email = email; }

    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime& createdAt) { m_createdAt = createdAt; }

private:
    int m_id = 0;
    QString m_username;
    QString m_email;
    QDateTime m_createdAt;
};

#endif
