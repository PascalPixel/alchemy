#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/// @file
/// What every part of the game object shares: its includes, the declarations
/// its parts disagreed on and one inventory helper. game.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves.

#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "version.h"
#include "string.h"
#include "heap_release.h"
#include "dma.h"
#include "iwram_call.h"
#include "frame.h"
#include "fixed_math.h"
#include "resource.h"
#include "scene.h"
#include "main_routine_work.h"
#include "game_flags.h"
#include "owner_state.h"
#include "owner_value.h"
#include "item.h"
#include "inventory.h"
#include "battle_type.h"
#include "battle_unit.h"
#include "battle_summon.h"
#include "battle_random.h"
#include "enemy_definition.h"
#include "range_falloff.h"
#include "module/game/game.h"

// The game object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
/* VIEW(game): routine GameFlag_Set as s32 GameFlag_Set(s32 flag);
 * flag_call.h has void GameFlag_Set(s32 flag). */
extern s32 GameFlag_SetBit__game(s32 flag) __asm__("GameFlag_Set");
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

struct ClassRecord {
  u8 unk00[8];
  u8 hp;
  u8 pp;
  u8 atk;
  u8 def;
  u8 agi;
  u8 luck;
};

struct StatWork {
  s32 hp;
  s32 pp;
  s32 atk;
  s32 def;
  s32 agi;
  s32 unused14;
  s32 luck;
  s32 turns;
  s32 stat20;
  s32 stat24;
  s32 element[4][2];
  s32 kind;
  s32 unused4c[2];
  s32 amount;
  struct ItemDefinition *item;
  s32 unused5c;
};

struct UnitDjinnSets {
  u8 unk00[0xf8];
  u32 djinn[4];
};

extern u8 gItemCounters[ITEM_COUNTER_COUNT];

extern u16 Data_02000458[];

#endif
