#ifndef GEAR_H
#define GEAR_H

#include <QWidget>

class Gear : QObject
{

    Q_OBJECT

public:

    enum ARMOR_TYPE {
        NOT_ARMOR = -1,
        LIGHT = 0,
        HEAVY,
        MAGICAL,
        RANGED,
        LAST_ARMOR
    };

    enum WEAPON_TYPE {
        NOT_WEAPON = -1,
        BEAST = 0,
        CULT,
        DEMON,
        GOBLIN,
        GOLEM,
        MILITIA,
        OUTLAW,
        TROLL,
        UNDEAD,
        LAST_WEAPON
    };

    enum RARITY {
        COMMON = 0,
        RARE,
        EPIC,
        LEGENDARY,
        UNIQUE,
        LAST_RARITY

    };

    Gear();
    Gear(ARMOR_TYPE p_armorType, WEAPON_TYPE p_weaponType, RARITY p_rarity);
    Gear(const Gear &p_source);
    Gear &operator=(const Gear &p_other);

    QString getName() const;

    static QString armorTypeToString(ARMOR_TYPE p_armorType);
    static QString weaponTypeToString(WEAPON_TYPE p_weaponType);
    static QString rarityToString(RARITY p_rarity);

private:

    ARMOR_TYPE  m_armorType;
    WEAPON_TYPE m_weaponType;
    RARITY      m_rarity;

    bool m_isArmor;

};

#endif // GEAR_H
