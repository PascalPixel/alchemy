#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/// @file
/// What every part of the game object shares: its includes, the declarations
/// its parts disagreed on and one inventory helper. game.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves.

#include "types.h"
#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "minigame.h"
#include "sound/sound.h"
#include "system/system.h"
#include "edition.h"
#include "version.h"
#include "string.h"
#include "system/heap_release.h"
#include "system/dma.h"
#include "system/iwram_call.h"
#include "system/frame.h"
#include "fixed_math.h"
#include "graphics/resource.h"
#include "system/scene.h"
#include "system/main_routine_work.h"
#include "game/game_flags.h"
#include "owner_state.h"
#include "game/owner_value.h"
#include "game/item.h"
#include "inventory.h"
#include "battle_type.h"
#include "battle/battle_unit.h"
#include "battle/battle_summon.h"
#include "battle/battle_random.h"
#include "game/enemy_definition.h"
#include "djinn_ids.h"
#include "enemy_ids.h"
#include "item_ids.h"
#include "battle/range_falloff.h"
#include "module/game/game.h"

// The game object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
struct AbilityDefinition *Ability_GetDirect(s32 actionId);

/// Keeps occupied slots in order and clears the unused inventory tail, for
/// Inventory_ClearSlot and Inventory_Remove (inventory.c) and
/// InventorySnapshot_Save (snapshot.c). A slot is empty when it is 0.
static inline void Inventory_CompactSlots(s16 *inventory) {
  s32 usedSlots = 0;
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (inventory[slot] != 0)
      inventory[usedSlots++] = inventory[slot];
  }
  for (; usedSlots < INVENTORY_SLOTS; usedSlots++)
    inventory[usedSlots] = 0;
}

#include "battle/battle_unit.h"
#include "owner_state.h"

/// One of the 48 element presets an enemy record names.
struct ElementPreset {
  u32 element;
  u8 digits[4];
  struct BattleElementStats values[4];
};

struct TradeOfferList {
  struct TradeOffer offers[72];
  s32 count;
};

struct TradeOfferTable {
  u8 unk00[8];
  struct TradeOfferList list;
};

enum { ITEM_COUNTER_COUNT = 300 };

struct ClassAbilities {
  u8 unk00[0x10];
  struct ClassAbility abilities[16];
};

enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

struct OwnerGrowthRecord {
  u8 unk00[0x50];
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
  u8 unk92[0x22];
};

struct OwnerStart {
  u8 unk00[150];
  u8 level;
  u8 unk97;
  u16 items[13];
};

struct OwnerName {
  u8 name[14];
  u8 nameFlags;
  u8 unk0f[5];
  u16 hpRatio;
  u16 ppRatio;
};

struct OwnerLevelState {
  u8 unk000[0x0f];
  u8 level;
  u16 baseHp;
  u16 basePp;
  u8 unk014[4];
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 baseTurns;
  u8 base20;
  u8 base21;
  u8 unk022[0x102];
  u32 experience;
  u8 character;
  u8 classId;
};

struct OwnerGrowth {
  u8 unk00[0x50];
  s16 hp[6];
  s16 pp[6];
  u16 atk[6];
  u16 def[6];
  u16 agi[6];
  u8 luck[6];
};

struct LevelUpWork {
  s32 classId;
  s32 level;
  struct OwnerGrowth *growth;
  u8 unused0c[0x20];
};

struct EnemyRecord {
  u8 unk00[15];
  u8 level;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 turns;
  u8 hpRegen;
  u8 ppRegen;
  u16 items[4];
  u8 counts[4];
  u8 unk2a[34];
};

/// The running totals Unit_Calculate builds a unit's stats in.
/// The class's stat multipliers as Unit_Calculate reads them, ClassDefinition
/// with its first eight bytes left opaque.
struct ClassScaleView {
  u8 seriesAndRequired[8]; ///< ClassDefinition.series and required
  u8 hp;
  u8 pp;
  u8 atk;
  u8 def;
  u8 agi;
  u8 luck;
};

struct StatWork {
  s32 hp;          ///< maximum HP
  s32 pp;          ///< maximum PP
  s32 atk;         ///< Attack, 0 to UNIT_STAT_MAX once clamped
  s32 def;         ///< Defense, 0 to UNIT_STAT_MAX once clamped
  s32 agi;         ///< Agility, 0 to UNIT_STAT_MAX once clamped
  s32 unused;      ///< never written; a gap between Agility and Luck
  s32 luck;        ///< Luck, 0 to UNIT_LUCK_MAX once clamped
  s32 actionCount; ///< actions a turn
  s32 hpRegen;     ///< HP regained each turn
  s32 ppRegen;     ///< PP regained each turn
  struct {
    s32 power;
    s32 resistance;
  } element[ELEMENT_COUNT];    ///< each element's power and resistance
  s32 kind;                    ///< the item effect being applied
  s32 unusedAfterKind[2];      ///< never written
  s32 amount;                  ///< the amount of the item effect being applied
  struct ItemDefinition *item; ///< the equipped item being applied
  s32 unusedAfterItem;         ///< never written
};

struct UnitDjinnSets {
  u8 unk00[0xf8];
  u32 djinn[4];
};

extern u8 gItemCounters[ITEM_COUNTER_COUNT];

extern u16 Data_02000458[];

#endif
