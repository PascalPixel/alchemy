#ifndef ALCHEMY_WEAPON_KIND_H
#define ALCHEMY_WEAPON_KIND_H
/// @file
/// The kinds of weapon a party member's attack shows, and how
/// gBattleUnit_WeaponKinds packs one with its item.

/// gBattleUnit_WeaponKinds: a weapon's item id with its kind above it.
#define WEAPON_ITEM_MASK 0x1ff
#define WEAPON_KIND_SHIFT 9

/// The kinds of weapon a party member's attack shows, by the rows of
/// gBattleUnit_WeaponKinds.
enum BattleWeaponKind {
  WEAPON_KIND_LONG_SWORD,
  WEAPON_KIND_LIGHT_BLADE,
  WEAPON_KIND_AXE,
  WEAPON_KIND_MACE,
  WEAPON_KIND_STAFF,
  WEAPON_KIND_ANKH,
  WEAPON_KIND_NONE ///< an item the list lacks
};

/// A row of gBattleUnit_WeaponKinds: the item and its kind.
#define WEAPON(item, kind) ((item) | (kind) << WEAPON_KIND_SHIFT)

#endif
