#ifndef ALCHEMY_MODULE_GAME_GAME_H
#define ALCHEMY_MODULE_GAME_GAME_H

/// @file
/// What games/tla/src/game/ declares for the files that use it, written once.

#include "types.h"
#include "battle/battle_unit.h"
#include "owner_state.h"

struct ItemDefinition;
struct LevelUpResult;

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

void Unit_Calculate(s32 owner);

s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 itemId);

extern const s32 Data_080c6604[4][4];

extern u8 gItemCounters[ITEM_COUNTER_COUNT];

s32 Unit_KnowsPsynergy(s32 owner, s32 value);
struct LevelUpResult *Unit_TryLevelUp(s32 unitId, struct LevelUpResult *gains);

extern u16 Data_02000458[];

void Func_080addf0(void);

extern const u8 Data_080b1f2c[4][5];

void Party_InitializeCharacters(void);

s32 Party_AddMember(s32 owner);

s32 Unit_LearnPsynergy(s32 owner, s32 ability);

s32 Party_RemoveMember(s32 owner);

void Djinn_ActivateOfferedForParty(void);

extern const u16 Data_080b1284[6];

void Func_080ae358(void);
s16 Unit_AddPp(s32 owner, s32 delta);

s32 PartyInventory_Remove(s32 item_id);
s32 PartyInventory_CountItem(s32 itemId);
s32 PartyInventory_CountFreeSlots(void);

u32 Party_GetAverageLevel(void);

s32 PartyInventory_FindRoom(s32 itemId);
s32 Inventory_GetQuantity(s32 owner, s32 slot);

s32 Party_AdjustCoins(s32 amount);

void Party_SetLeaderJenna(void);
void Party_SetLeaderFelix(void);

#endif
