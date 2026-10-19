#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

/// @file
/// BattleWork, the battle's own work, and the battle display work.

#include "unit/battle/battle.h"
#include "types.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle_event.h"
#include "battle_session.h"

struct BattlePromptSprite {
  struct BattlePromptSprite *next;
  u16 y : 8;
  u16 flags : 8;
  u16 x : 9;
  u16 size : 7;
  u16 tile : 10;
  u16 palette : 6;
  u16 unk0a;
};

struct UiWindow;

struct BattleDisplayOffset {
  s32 unk00;
  u16 x;
  u16 y;
};

struct BattleDisplayWork {
  struct UiWindow *window;
  struct BattleDisplayOffset *offset;
  s32 marked;
};

STATIC_ASSERT(sizeof(struct BattleDisplayOffset) == 8);
STATIC_ASSERT(sizeof(struct BattleDisplayWork) == 12);
STATIC_ASSERT(offsetof(struct BattleDisplayWork, marked) == 8);
extern struct BattleDisplayWork *gBattleDisplayWork;

struct BattleEventState {
  struct BattleEventQueue queue;
  s32 phase;
  s32 eventIndex;
  s32 timer;
  struct BattlePromptSprite displayEntry;
  s32 displaySource;
  s32 actorId;
  s32 pendingCue;
  s32 flags;
};

/// Six enemies at most, and a turn's queue of up to 20 actions.
enum { BATTLE_ENEMY_CAPACITY = 6, BATTLE_ACTION_CAPACITY = 20 };

struct BattleWork {
  s32 enemyGroup;
  s16 spriteSlots[6];
  u16 enemyClasses[6];
  u32 summonMasks[6];
  s8 summonChannels[6];
  u8 unk03a[2];
  u16 firstDefeated;
  u16 defeatState;
  u8 summonCount;
  u8 partyStatus;
  u8 battleType;
  u8 challengeFlag;
  u8 twoSided;
  u8 encounterMode;
  u8 escapeFailures;
  u8 unk047;
  u8 ownerSlots[8];
  u8 linkSide;
  u8 linkMisses;
  u8 linkPaused;
  u8 unk053;
  s32 promptSlot;
  s16 partyUnits[7];
  s16 enemyIds[BATTLE_ENEMY_CAPACITY + 1]; ///< ends with BATTLE_UNIT_LIST_END
  struct BattleObj slots[14];
  u8 placement[14];
  u8 unk2ea[2];
  struct BattleActionRecord actions[BATTLE_ACTION_CAPACITY];
  u8 unk42c[0x104];
  struct BattleSpoils spoils;
  u16 palette[128];
  s32 brightness;
  u16 background;
  u8 unk64a[4];
  u16 highlightStep;
  u16 highlightMode;
  u8 unk652[2];
  struct BattlePlan plan;
  struct BattleEventState events;
  s32 debugValue;
};

STATIC_ASSERT(sizeof(struct BattleWork) == 0x82c);

STATIC_ASSERT(offsetof(struct BattleWork, partyStatus) == 0x041);
STATIC_ASSERT(offsetof(struct BattleWork, challengeFlag) == 0x043);
STATIC_ASSERT(offsetof(struct BattleWork, partyUnits) == 0x058);
STATIC_ASSERT(offsetof(struct BattleWork, enemyIds) == 0x066);
STATIC_ASSERT(offsetof(struct BattleWork, slots) == 0x074);
STATIC_ASSERT(offsetof(struct BattleWork, placement) == 0x2dc);
STATIC_ASSERT(offsetof(struct BattleWork, actions) == 0x2ec);
STATIC_ASSERT(offsetof(struct BattleWork, spoils) == 0x530);
STATIC_ASSERT(offsetof(struct BattleWork, palette) == 0x544);
STATIC_ASSERT(offsetof(struct BattleWork, brightness) == 0x644);
STATIC_ASSERT(offsetof(struct BattleWork, background) == 0x648);
STATIC_ASSERT(offsetof(struct BattleWork, plan) == 0x654);
STATIC_ASSERT(offsetof(struct BattleWork, events) == 0x6b8);
STATIC_ASSERT(offsetof(struct BattleEventState, phase) == 0x148);
STATIC_ASSERT(offsetof(struct BattleEventState, flags) == 0x16c);

extern struct BattleWork *gBattleWork;

#endif
