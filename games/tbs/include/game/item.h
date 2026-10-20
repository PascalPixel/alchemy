#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

/// @file
/// Items: their definitions, types, flags and effects.

#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

struct BattleUnit;

/// An inventory entry's parts, and a unit's slot count.
/// bits: 15..11 quantity | 10 broken | 9 equipped | 8..0 item id
enum InventoryEncoding {
  ITEM_ID_MASK = 0x1ff,
  INVENTORY_EQUIPPED = 0x200,
  INVENTORY_BROKEN = 0x400,
  INVENTORY_ENTRY_MASK = 0x7ff,
  INVENTORY_QUANTITY_SHIFT = 11,
  INVENTORY_QUANTITY_STEP = 0x800,
  INVENTORY_QUANTITY_MASK = 0xf800,
  INVENTORY_SLOTS = 15
};

enum ItemFlags {
  ITEM_CURSED = 1,
  ITEM_CANNOT_UNEQUIP = 2,
  ITEM_RARE = 4, ///< selects the shop's rare item offers
  ITEM_INDISPENSABLE = 8,
  ITEM_STACKABLE = 0x10
};

/// What using an item costs (struct ItemDefinition's useType).
#define ITEM_USE_CONSUMED 1  ///< one is used up
#define ITEM_USE_MAY_BREAK 2 ///< breaks one time in eight
/// Not used up: equipped, it teaches its actionId as Psynergy.
#define ITEM_USE_TEACHES 3
#define ITEM_USE_EMPTIES 4 ///< Hermes' Water leaves an Empty Bottle

/// An item's type (struct ItemDefinition's type, items.tsv's type column).
/// A unit equips one item of each type, except Psynergy items.
enum ItemType {
  ITEM_TYPE_NONE =
      0, ///< not equipment: Herb, Game Ticket, the keys, unused rows
  ITEM_TYPE_WEAPON = 1, ///< swords, axes, maces, rods and staffs
  ITEM_TYPE_ARMOR = 2,  ///< armor, clothes and robes
  ITEM_TYPE_SHIELD = 3, ///< shields, gloves and armlets
  ITEM_TYPE_HELM = 4,   ///< helms, caps, crowns and circlets
  ITEM_TYPE_BOOTS = 5,  ///< Hyper Boots to Turtle Boots
  /// The items that teach Psynergy while held: Orb of Force to Catch Beads.
  ITEM_TYPE_PSYNERGY = 6,
  ITEM_TYPE_RING = 8, ///< Adept Ring to Cleric's Ring
  ITEM_TYPE_SHIRT = 9 ///< Mythril Shirt, Silk Shirt, Running Shirt
};

enum ItemEffectKind {
  ITEM_EFFECT_NONE,
  ITEM_EFFECT_ADD_HP,
  ITEM_EFFECT_ADD_HP_REGEN,
  ITEM_EFFECT_ADD_PP,
  ITEM_EFFECT_ADD_PP_REGEN,
  ITEM_EFFECT_ADD_AGILITY,
  ITEM_EFFECT_ADD_LUCK,
  ITEM_EFFECT_SCALE_HP,
  ITEM_EFFECT_SCALE_HP_REGEN,
  ITEM_EFFECT_SCALE_PP,
  ITEM_EFFECT_SCALE_PP_REGEN,
  ITEM_EFFECT_SCALE_ATTACK,
  ITEM_EFFECT_SCALE_DEFENSE,
  ITEM_EFFECT_SCALE_AGILITY,
  ITEM_EFFECT_SCALE_LUCK,
  /// Element power and resistance, in the element order Venus, Mercury, Mars,
  /// Jupiter
  ITEM_EFFECT_ADD_VENUS_POWER,
  ITEM_EFFECT_ADD_MERCURY_POWER,
  ITEM_EFFECT_ADD_MARS_POWER,
  ITEM_EFFECT_ADD_JUPITER_POWER,
  ITEM_EFFECT_ADD_VENUS_RESIST,
  ITEM_EFFECT_ADD_MERCURY_RESIST,
  ITEM_EFFECT_ADD_MARS_RESIST,
  ITEM_EFFECT_ADD_JUPITER_RESIST,
  ITEM_EFFECT_ADD_UNLEASH_CHANCE, ///< percent added to the weapon's unleash
  ITEM_EFFECT_24,                 ///< summed by UnitCalc; TBS never reads it
  ITEM_EFFECT_25,                 ///< sets restraint bit 3, which lifts bit 0
  ITEM_EFFECT_ADD_TURNS,          ///< more actions each round
  ITEM_EFFECT_LURE ///< sets FLAG_LURE_CAP while worn: encounters come faster
};

/// One effect of an item while equipped: its kind (enum ItemEffectKind) and
/// amount.
struct ItemEffect {
  u8 kind;          ///< enum ItemEffectKind; 0 for none
  s8 amount;        ///< what the kind adds or scales by
  u8 reserved02[2]; ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct ItemEffect) == 0x04);
STATIC_ASSERT(offsetof(struct ItemEffect, kind) == 0x00);
STATIC_ASSERT(offsetof(struct ItemEffect, amount) == 0x01);

/// An item's definition: its price, type, who can equip it, its bonuses and
/// unleash, its effects and the action it is used for.
struct ItemDefinition {
  s16 price;         ///< coins
  u8 type;           ///< enum ItemType; 0 for an item that is not equipment
  u8 flags;          ///< enum ItemFlags
  u16 equipMask;     ///< one bit per character that can equip it, bit 0 Isaac
  u16 icon;          ///< indexes gUiIcon_ItemIconPointers
  s16 primaryBonus;  ///< Attack added while equipped
  s8 secondaryBonus; ///< Defense added while equipped
  u8 unleashChance;  ///< in steps of 5 percent
  u8 useType;        ///< what using it costs (ITEM_USE_), or 0 for nothing
  u8 reserved0d;     ///< not named by TBS code
  u16 unleashAction; ///< the action a weapon unleashes, or 0

  u8 reserved10[4];             ///< not named by TBS code
  s32 element;                  ///< a weapon's attack element; 4 for none
  struct ItemEffect effects[4]; ///< what it does while equipped
  u16 actionId;                 ///< the ability used, or taught as Psynergy
  u8 reserved2a[2];             ///< not named by TBS code
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
STATIC_ASSERT(offsetof(struct ItemDefinition, unleashAction) == 0x0e);
STATIC_ASSERT(offsetof(struct ItemDefinition, element) == 0x14);
STATIC_ASSERT(offsetof(struct ItemDefinition, effects) == 0x18);
STATIC_ASSERT(offsetof(struct ItemDefinition, actionId) == 0x28);

struct ItemDefinition *Item_Get(s32 item);

#endif
