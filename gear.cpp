#include "gear.h"

Gear::Gear() : m_armorType(NOT_ARMOR), m_weaponType(NOT_WEAPON), m_rarity(COMMON), m_bodyPart(NOT_BODY)
{

}

Gear::Gear(ARMOR_TYPE p_armorType, WEAPON_TYPE p_weaponType, RARITY p_rarity, BODY_PART p_bodyPart) : m_armorType(p_armorType), m_weaponType(p_weaponType), m_rarity(p_rarity), m_bodyPart(p_bodyPart)
{
    m_isArmor = p_armorType == NOT_ARMOR ? false : true;
}

Gear::Gear(const Gear &p_source)
{
    m_armorType = p_source.m_armorType;
    m_weaponType = p_source.m_weaponType;
    m_rarity = p_source.m_rarity;
    m_isArmor = p_source.m_isArmor;
    m_bodyPart = p_source.m_bodyPart;
}

Gear &Gear::operator=(const Gear &p_other)
{
    m_armorType = p_other.m_armorType;
    m_weaponType = p_other.m_weaponType;
    m_rarity = p_other.m_rarity;
    m_isArmor = p_other.m_isArmor;
    m_bodyPart = p_other.m_bodyPart;
    return *this;
}

QString Gear::armorTypeToString(ARMOR_TYPE p_armorType)
{
    switch (p_armorType) {
        case LIGHT:
            return QString("Light");
            break;

        case HEAVY:
            return QString("Heavy");
            break;

        case MAGICAL:
            return QString("Magical");
            break;

        case RANGED:
            return QString("Ranged");
            break;

        default:
            return QString("");
            break;
    }
}

QString Gear::weaponTypeToString(WEAPON_TYPE p_weaponType)
{
    switch (p_weaponType) {
        case BEAST:
            return QString("Vs Beast");
            break;

        case CULT:
            return QString("Vs Cult");
            break;

        case DEMON:
            return QString("Vs Demon");
            break;

        case GOBLIN:
            return QString("Vs Goblin");
            break;

        case GOLEM:
            return QString("Vs Golem");
            break;

        case MILITIA:
            return QString("Vs Militia");
            break;

        case OUTLAW:
            return QString("Vs Outlaw");
            break;

        case TROLL:
            return QString("Vs Troll");
            break;

        case UNDEAD:
            return QString("Vs Undead");
            break;

        default:
            return QString("");
            break;
    }
}

QString Gear::rarityToString(RARITY p_rarity)
{
    switch (p_rarity) {
        case COMMON:
            return QString("Common");
            break;

        case RARE:
            return QString("Rare");
            break;

        case EPIC:
            return QString("Epic");
            break;

        case LEGENDARY:
            return QString("Legendary");
            break;

        case UNIQUE:
            return QString("Unique");
            break;

        default:
            return QString("");
            break;
    }
}

QString Gear::bodyPartToString(BODY_PART p_bodyPart)
{
    switch (p_bodyPart) {
        case CHEST:
            return QString("Chest");
            break;

        case HEAD:
            return QString("Head");
            break;

        case ARMS:
            return QString("Arms");
            break;

        case LEGS:
            return QString("Legs");
            break;

        case SHOULDERS:
            return QString("Shoulders");
            break;


        default:
            return QString("");
            break;
    }
}

QString Gear::getName() const
{
    return rarityToString(m_rarity) + " " + (m_isArmor ? (armorTypeToString(m_armorType) + " " + bodyPartToString(m_bodyPart)) : weaponTypeToString(m_weaponType));
}
