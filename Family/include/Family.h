#ifndef FAMILY_H
#define FAMILY_H

#include <QString>

class Family
{
public:
    Family() = default;
    Family(int id, int husbandId, int wifeId, int genealogyId);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int husbandId() const { return m_husbandId; }
    void setHusbandId(int husbandId) { m_husbandId = husbandId; }

    int wifeId() const { return m_wifeId; }
    void setWifeId(int wifeId) { m_wifeId = wifeId; }

    int genealogyId() const { return m_genealogyId; }
    void setGenealogyId(int genealogyId) { m_genealogyId = genealogyId; }

    int marriageYear() const { return m_marriageYear; }
    void setMarriageYear(int marriageYear) { m_marriageYear = marriageYear; }

private:
    int m_id = 0;
    int m_husbandId = 0;
    int m_wifeId = 0;
    int m_genealogyId = 0;
    int m_marriageYear = 0;
};

#endif
