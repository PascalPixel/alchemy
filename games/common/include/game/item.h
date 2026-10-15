#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

#include "layout_guard.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct BattleUnit;

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
  ITEM_INDISPENSABLE = 8,
  ITEM_STACKABLE = 0x10
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
  // Element power and resistance, in the element order Venus, Mercury, Mars,
  // Jupiter
  ITEM_EFFECT_ADD_VENUS_POWER,
  ITEM_EFFECT_ADD_MERCURY_POWER,
  ITEM_EFFECT_ADD_MARS_POWER,
  ITEM_EFFECT_ADD_JUPITER_POWER,
  ITEM_EFFECT_ADD_VENUS_RESIST,
  ITEM_EFFECT_ADD_MERCURY_RESIST,
  ITEM_EFFECT_ADD_MARS_RESIST,
  ITEM_EFFECT_ADD_JUPITER_RESIST,
  ITEM_EFFECT_ADD_UNLEASH_CHANCE, // percent added to the weapon's unleash
  ITEM_EFFECT_24,                 // summed by UnitCalc; TBS never reads it
  ITEM_EFFECT_25,                 // sets restraint bit 3, which lifts bit 0
  ITEM_EFFECT_ADD_TURNS,          // more actions each round
  ITEM_EFFECT_27                  // sets flag 0x167 while worn
};

struct ItemEffect {
  u8 kind;
  s8 amount;
  u8 reserved02[2];
};

LAYOUT_SIZE_GUARD(ItemEffect_Size, struct ItemEffect, 0x04);
LAYOUT_OFFSET_GUARD(ItemEffect_Kind, struct ItemEffect, kind, 0x00);
LAYOUT_OFFSET_GUARD(ItemEffect_Amount, struct ItemEffect, amount, 0x01);

struct ItemDefinition {
  s16 price;
  u8 type;
  u8 flags;
  u16 equipMask;
  u16 icon;
  s16 primaryBonus;
  s8 secondaryBonus;
  u8 unleashChance; // in steps of 5 percent
  u8 useType;
  u8 reserved0d;
  u16 unleashAction; // the action a weapon unleashes, or 0

  u8 reserved10[4];
  s32 element;
  struct ItemEffect effects[4];
  u16 actionId;
  u8 reserved2a[2];
};

LAYOUT_SIZE_GUARD(ItemDefinition_Size, struct ItemDefinition, 0x2c);
LAYOUT_OFFSET_GUARD(ItemDefinition_Price, struct ItemDefinition, price, 0x00);
LAYOUT_OFFSET_GUARD(ItemDefinition_Type, struct ItemDefinition, type, 0x02);
LAYOUT_OFFSET_GUARD(ItemDefinition_Flags, struct ItemDefinition, flags, 0x03);
LAYOUT_OFFSET_GUARD(ItemDefinition_EquipMask, struct ItemDefinition, equipMask,
                    0x04);
LAYOUT_OFFSET_GUARD(ItemDefinition_Icon, struct ItemDefinition, icon, 0x06);
LAYOUT_OFFSET_GUARD(ItemDefinition_PrimaryBonus, struct ItemDefinition,
                    primaryBonus, 0x08);
LAYOUT_OFFSET_GUARD(ItemDefinition_SecondaryBonus, struct ItemDefinition,
                    secondaryBonus, 0x0a);
LAYOUT_OFFSET_GUARD(ItemDefinition_UnleashChance, struct ItemDefinition,
                    unleashChance, 0x0b);
LAYOUT_OFFSET_GUARD(ItemDefinition_UseType, struct ItemDefinition, useType,
                    0x0c);
LAYOUT_OFFSET_GUARD(ItemDefinition_UnleashAction, struct ItemDefinition,
                    unleashAction, 0x0e);
LAYOUT_OFFSET_GUARD(ItemDefinition_Element, struct ItemDefinition, element,
                    0x14);
LAYOUT_OFFSET_GUARD(ItemDefinition_Effects, struct ItemDefinition, effects,
                    0x18);
LAYOUT_OFFSET_GUARD(ItemDefinition_ActionId, struct ItemDefinition, actionId,
                    0x28);

struct ItemDefinition *Item_Get(s32 item);
#if GAME_TLA
struct ItemDefinition *Item_GetDirect(s32 item);
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner);
s32 RollWeaponUnleash(struct BattleUnit *owner);
#endif

#if GAME_TLA
s32 RollWeaponUnleashFar(struct BattleUnit *owner);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
#endif
#if GAME_TLA
s32 Item_CanOwnerEquipDirect(s32 owner, s32 item);
s32 Item_IsCompatibleWithOwner(s32 owner, s32 item);
s32 Item_GetEquipmentGroup(s32 item);
u8 Item_GetTargetMode(s32 item);
s32 ItemCounter_Adjust(s32 counter, s32 delta);
s32 Item_AdjustCounter(s32 item, s32 delta);
#endif

#endif
