#ifndef PERSON_H
#define PERSON_H

#include <QString>

class Person
{
public:
    Person() = default;
    Person(int id, const QString& name, QChar gender, int birthYear = 0,
           int deathYear = 0, int generation = 1);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QChar gender() const { return m_gender; }
    void setGender(QChar gender) { m_gender = gender; }

    int birthYear() const { return m_birthYear; }
    void setBirthYear(int birthYear) { m_birthYear = birthYear; }

    int deathYear() const { return m_deathYear; }
    void setDeathYear(int deathYear) { m_deathYear = deathYear; }

    int age() const {
        if (m_birthYear == 0) return 0;
        int endYear = (m_deathYear > 0) ? m_deathYear : 2026;
        return endYear - m_birthYear;
    }

    int generation() const { return m_generation; }
    void setGeneration(int generation) { m_generation = generation; }

    int genealogyId() const { return m_genealogyId; }
    void setGenealogyId(int genealogyId) { m_genealogyId = genealogyId; }

    int birthFamilyId() const { return m_birthFamilyId; }
    void setBirthFamilyId(int birthFamilyId) { m_birthFamilyId = birthFamilyId; }

    QString biography() const { return m_biography; }
    void setBiography(const QString& biography) { m_biography = biography; }

    QString fatherName() const { return m_fatherName; }
    void setFatherName(const QString& name) { m_fatherName = name; }

    QString motherName() const { return m_motherName; }
    void setMotherName(const QString& name) { m_motherName = name; }

private:
    int m_id = 0;
    QString m_name;
    QChar m_gender;
    int m_birthYear = 0;
    int m_deathYear = 0;
    int m_generation = 1;
    int m_genealogyId = 0;
    int m_birthFamilyId = 0;
    QString m_biography;
    QString m_fatherName;
    QString m_motherName;
};

#endif
