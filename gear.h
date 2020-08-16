#ifndef GEAR_H
#define GEAR_H

#include <QWidget>

class Gear : QObject
{

    Q_OBJECT

public:

    enum BODY_PART {
        NOT_BODY = -1,
        CHEST = 0,
        HEAD,
        ARMS,
        LEGS,
        SHOULDERS,
        LAST_BODY
    };

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
    Gear(ARMOR_TYPE p_armorType, WEAPON_TYPE p_weaponType, RARITY p_rarity, BODY_PART p_bodyPart);
    Gear(const Gear &p_source);
    Gear &operator=(const Gear &p_other);

    QString getName() const;

    static QString armorTypeToString(ARMOR_TYPE p_armorType);
    static QString weaponTypeToString(WEAPON_TYPE p_weaponType);
    static QString rarityToString(RARITY p_rarity);
    static QString bodyPartToString(BODY_PART p_bodyPart);

private:

    ARMOR_TYPE  m_armorType;
    WEAPON_TYPE m_weaponType;
    RARITY      m_rarity;
    BODY_PART   m_bodyPart;

    bool m_isArmor;

};

#endif // GEAR_H
