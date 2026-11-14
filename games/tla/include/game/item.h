#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

/// @file
/// Items: their definitions, types, flags and effects.

#include "static_assert.h"
#include "edition.h"

struct BattleUnit;

/// An inventory entry's parts, and a unit's slot count.
/// bits: 15..11 quantity | 10 broken | 9 equipped | 8..0 item id
enum InventoryEncoding {
  INVENTORY_ITEM_ID_MASK = 0x1ff,
  INVENTORY_EQUIPPED = 0x200,
  INVENTORY_BROKEN = 0x400,
  /// The item id with its broken and equipped bits, all but the quantity.
  INVENTORY_ENTRY_MASK = 0x7ff,
  /// The quantity field holds the count less one; a stack (ITEM_STACKABLE)
  /// grows to 30 (Inventory_AddItem).
  INVENTORY_QUANTITY_SHIFT = 11,
  INVENTORY_QUANTITY_STEP = 0x800, ///< one more in the stack
  INVENTORY_QUANTITY_MASK = 0xf800,
  INVENTORY_SLOTS = 15 ///< BattleUnit.inventory's entries
};

/// The equipMask bit of character `no` (ISAAC to PIERS):
/// Item_CanUnitEquipDirect reads the bit of the unit's character.
#define EQUIP_CHARACTER(no) (1 << (no))
/// Every one of the eight characters' bits.
#define EQUIP_ALL_CHARACTERS 0xff

/// The bits of ItemDefinition.flags.
enum ItemFlags {
  ITEM_CURSED = 1, ///< equipping it curses the wearer (BattleUnit.curse)
  /// Once equipped it cannot be taken off, or dropped while worn.
  ITEM_CANNOT_UNEQUIP = 2,
  /// A rare item: the rare weapons and armour, keys and the other
  /// one-of-a-kind items carry it.
  ITEM_RARE = 4,
  /// An important item: never dropped, and worth nothing to a shop.
  ITEM_INDISPENSABLE = 8,
  ITEM_STACKABLE = 0x10, ///< kept in one inventory entry with a quantity
  /// Left behind when The Broken Seal's data is transferred
  /// (DataTransfer_Apply): the Elemental Stars and their bags, and that
  /// game's errands, from the Healing Fungus to the Lil' Turtle.
  ITEM_NOT_TRANSFERRED = 0x20
};

/// What using an item costs (struct ItemDefinition's useType).
#define ITEM_USE_FREE 0      ///< nothing: it is kept, as equipment is
#define ITEM_USE_CONSUMED 1  ///< one is used up
#define ITEM_USE_MAY_BREAK 2 ///< may break when used
/// Not used up: equipped, it teaches its actionId as Psynergy.
#define ITEM_USE_TEACHES 3
#define ITEM_USE_EMPTIES 4 ///< Hermes' Water leaves an Empty Bottle

/// What Item_GetEquipmentGroup sorts an item into.
enum EquipmentGroup {
  EQUIPMENT_GROUP_NONE = 0, ///< anything not a weapon or armor, rings included
  EQUIPMENT_GROUP_WEAPON = 1,
  EQUIPMENT_GROUP_ARMOR = 2
};

/// Why Inventory_CheckDiscard or Inventory_CheckUnequip refuses an item, as
/// bits; 0 when it may go.
enum InventoryRefusal {
  /// Equipped and ITEM_CANNOT_UNEQUIP: "You can't remove it!"
  INVENTORY_REFUSE_CANNOT_UNEQUIP = 1,
  /// ITEM_INDISPENSABLE: "You can't drop it!"
  INVENTORY_REFUSE_INDISPENSABLE = 2,
  INVENTORY_REFUSE_EMPTY = 8 ///< the slot holds no item
};

/// An item's type (struct ItemDefinition's type, item_table.c's type field).
/// A unit equips one item of each type, except Psynergy items.
enum ItemType {
  /// Not equipment: Herb, the keys, unused rows.
  ITEM_TYPE_NONE = 0,
  ITEM_TYPE_WEAPON = 1, ///< swords, axes, maces, rods and staffs
  ITEM_TYPE_ARMOR = 2,  ///< armor, clothes and robes
  ITEM_TYPE_SHIELD = 3, ///< shields, gloves and armlets
  ITEM_TYPE_HELM = 4,   ///< helms, caps, crowns and circlets
  ITEM_TYPE_BOOTS = 5,  ///< Hyper Boots to Turtle Boots and the greaves
  /// The items that teach Psynergy while held: Orb of Force to Teleport
  /// Lapis.
  ITEM_TYPE_PSYNERGY = 6,
  /// The Trident alone: "A weapon with a unique purpose". The Broken Seal
  /// gives this type to no item.
  ITEM_TYPE_TRIDENT = 7,
  ITEM_TYPE_RING = 8,  ///< Adept Ring to War Ring
  ITEM_TYPE_SHIRT = 9, ///< Mythril Shirt to Divine Camisole
  /// Mysterious Card, Trainer's Whip and Tomegathericon: "Equip to change
  /// your class".
  ITEM_TYPE_CLASS_CHANGE = 10,
  /// The four Elemental Stars and the Mythril Bags that hold them; the item
  /// menu treats them as it does items that are not equipment.
  ITEM_TYPE_ELEMENTAL_STAR = 11
};

/// How many effects an item has while equipped (ItemDefinition.effects).
#define EQUIP_EFFECT_SLOTS 4
/// A multiplier of 10 tenths, which keeps a stat as it is: the classes'
/// stats (ClassDefinition) and the EQUIP_EFFECT_SCALE_ effects are in tenths.
#define STAT_SCALE_ONE 10

enum EquipEffectKind {
  EQUIP_EFFECT_NONE,
  EQUIP_EFFECT_ADD_HP,
  EQUIP_EFFECT_ADD_HP_REGEN,
  EQUIP_EFFECT_ADD_PP,
  EQUIP_EFFECT_ADD_PP_REGEN,
  EQUIP_EFFECT_ADD_AGILITY,
  EQUIP_EFFECT_ADD_LUCK,
  EQUIP_EFFECT_SCALE_HP,
  EQUIP_EFFECT_SCALE_HP_REGEN,
  EQUIP_EFFECT_SCALE_PP,
  EQUIP_EFFECT_SCALE_PP_REGEN,
  EQUIP_EFFECT_SCALE_ATTACK,
  EQUIP_EFFECT_SCALE_DEFENSE,
  EQUIP_EFFECT_SCALE_AGILITY,
  EQUIP_EFFECT_SCALE_LUCK,
  /// Element power and resistance, in the element order Venus, Mercury, Mars,
  /// Jupiter.
  EQUIP_EFFECT_ADD_VENUS_POWER,
  EQUIP_EFFECT_ADD_MERCURY_POWER,
  EQUIP_EFFECT_ADD_MARS_POWER,
  EQUIP_EFFECT_ADD_JUPITER_POWER,
  EQUIP_EFFECT_ADD_VENUS_RESIST,
  EQUIP_EFFECT_ADD_MERCURY_RESIST,
  EQUIP_EFFECT_ADD_MARS_RESIST,
  EQUIP_EFFECT_ADD_JUPITER_RESIST,
  EQUIP_EFFECT_ADD_UNLEASH_CHANCE, ///< percent added to the weapon's unleash
  /// Summed by Unit_Calculate into equipBonus[1]; nothing reads the total.
  EQUIP_EFFECT_ADD_COUNTERSTRIKE_RATE,
  /// "The curse can be removed": sets CURSE_REMOVABLE, which lifts
  /// CURSE_BOUND.
  EQUIP_EFFECT_CURSE_REMOVABLE,
  EQUIP_EFFECT_ADD_ACTIONS, ///< more actions each turn
  EQUIP_EFFECT_LURE ///< sets FLAG_LURE_CAP while worn: encounters come faster
};

/// One effect of an item while equipped: its kind (enum EquipEffectKind) and
/// amount.
struct EquipEffect {
  u8 kind;          ///< enum EquipEffectKind; 0 for none
  s8 amount;        ///< what the kind adds or scales by
  u8 reserved02[2]; ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct EquipEffect) == 0x04);
STATIC_ASSERT(offsetof(struct EquipEffect, kind) == 0x00);
STATIC_ASSERT(offsetof(struct EquipEffect, amount) == 0x01);

/// An item's definition: its price, type, who can equip it, its bonuses and
/// unleash, its effects and the action it is used for.
struct ItemDefinition {
  u16 price;          ///< coins
  u8 type;            ///< enum ItemType; 0 for an item that is not equipment
  u8 flags;           ///< enum ItemFlags
  u16 equipMask;      ///< one bit per character that can equip it, bit 0 Isaac
  u16 icon;           ///< indexes gItemIcons
  s16 primaryBonus;   ///< Attack added while equipped
  s8 secondaryBonus;  ///< Defense added while equipped
  u8 unleashChance;   ///< in steps of 5 percent
  u8 useType;         ///< what using it costs (an ITEM_USE_ value)
  u8 reserved0d;      ///< 0 in every row
  u16 unleashAbility; ///< the action a weapon unleashes, or 0

  u8 reserved10[4]; ///< 0 in every row
  s32 element;      ///< a weapon's attack element; ELEMENT_NONE for none
  struct EquipEffect
      effects[EQUIP_EFFECT_SLOTS]; ///< what it does while equipped
  u16 abilityId;                   ///< the ability used, or taught as Psynergy
  u8 reserved2a[2];                ///< 0 in every row
};

STATIC_ASSERT(sizeof(struct ItemDefinition) == 0x2c);
STATIC_ASSERT(offsetof(struct ItemDefinition, price) == 0x00);
STATIC_ASSERT(offsetof(struct ItemDefinition, type) == 0x02);
STATIC_ASSERT(offsetof(struct ItemDefinition, flags) == 0x03);
STATIC_ASSERT(offsetof(struct ItemDefinition, equipMask) == 0x04);
STATIC_ASSERT(offsetof(struct ItemDefinition, icon) == 0x06);
STATIC_ASSERT(offsetof(struct ItemDefinition, primaryBonus) == 0x08);
STATIC_ASSERT(offsetof(struct ItemDefinition, secondaryBonus) == 0x0a);
STATIC_ASSERT(offsetof(struct ItemDefinition, unleashChance) == 0x0b);
STATIC_ASSERT(offsetof(struct ItemDefinition, useType) == 0x0c);
STATIC_ASSERT(offsetof(struct ItemDefinition, unleashAbility) == 0x0e);
STATIC_ASSERT(offsetof(struct ItemDefinition, element) == 0x14);
STATIC_ASSERT(offsetof(struct ItemDefinition, effects) == 0x18);
STATIC_ASSERT(offsetof(struct ItemDefinition, abilityId) == 0x28);

struct ItemDefinition *Item_GetDirectFar(s32 item);
struct ItemDefinition *Item_GetDirect(s32 item);
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner);
s32 RollWeaponUnleash(struct BattleUnit *owner);

s32 RollWeaponUnleashFar(struct BattleUnit *owner);
s32 Item_CanUnitEquip(s32 unit, s32 item);
s32 Item_CanUnitEquipDirect(s32 unit, s32 item);
s32 Item_IsCompatibleWithUnit(s32 owner, s32 item);
s32 Item_GetEquipmentGroup(s32 item);
u8 Item_GetTargetMode(s32 item);
s32 RareItemStock_Adjust(s32 counter, s32 delta);
s32 Item_AdjustCounter(s32 item, s32 delta);

#endif
