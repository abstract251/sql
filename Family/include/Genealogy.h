#ifndef GENEALOGY_H
#define GENEALOGY_H

#include <QString>
#include <QDate>

class Genealogy
{
public:
    Genealogy() = default;
    Genealogy(int id, const QString& name, const QString& surname,
              const QDate& compileTime = QDate());

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString surname() const { return m_surname; }
    void setSurname(const QString& surname) { m_surname = surname; }

    QDate compileTime() const { return m_compileTime; }
    void setCompileTime(const QDate& compileTime) { m_compileTime = compileTime; }

    QString description() const { return m_description; }
    void setDescription(const QString& description) { m_description = description; }

    int creatorId() const { return m_creatorId; }
    void setCreatorId(int creatorId) { m_creatorId = creatorId; }

private:
    int m_id = 0;
    QString m_name;
    QString m_surname;
    QDate m_compileTime;
    QString m_description;
    int m_creatorId = 0;
};

#endif
