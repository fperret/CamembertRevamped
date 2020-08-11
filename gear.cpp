#include "gear.h"

Gear::Gear(ARMOR_TYPE p_armorType, WEAPON_TYPE p_weaponType, RARITY p_rarity) : m_armorType(p_armorType), m_weaponType(p_weaponType), m_rarity(p_rarity)
{

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
