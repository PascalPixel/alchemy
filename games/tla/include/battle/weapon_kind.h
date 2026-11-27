#ifndef ALCHEMY_WEAPON_KIND_H
#define ALCHEMY_WEAPON_KIND_H
// The kinds of weapon a party member's attack shows, and how BattleUnitWeaponKinds packs one with
// its item.

// BattleUnitWeaponKinds: a weapon's item id with its kind above it.
#define WEAPON_ITEM_MASK  0x1ff
#define WEAPON_KIND_SHIFT 9

// The kinds of weapon a party member's attack shows, by the rows of BattleUnitWeaponKinds.
typedef enum BATTLE_WEAPON_KIND
{
    WEAPON_KIND_LONG_SWORD,
    WEAPON_KIND_LIGHT_BLADE,
    WEAPON_KIND_AXE,
    WEAPON_KIND_MACE,
    WEAPON_KIND_STAFF,
    WEAPON_KIND_ANKH,
    WEAPON_KIND_NONE // an item the list lacks
} BATTLE_WEAPON_KIND;

// A row of BattleUnitWeaponKinds: the item and its kind.
#define WEAPON(item, kind) ((item) | (kind) << WEAPON_KIND_SHIFT)

#endif
