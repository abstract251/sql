#include "Person.h"

Person::Person(int id, const QString& name, QChar gender, int birthYear, int deathYear, int generation)
    : m_id(id), m_name(name), m_gender(gender), m_birthYear(birthYear), m_deathYear(deathYear), m_generation(generation)
{
}
