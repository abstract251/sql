#include "Genealogy.h"

Genealogy::Genealogy(int id, const QString& name, const QString& surname, const QDate& compileTime)
    : m_id(id), m_name(name), m_surname(surname), m_compileTime(compileTime)
{
}
