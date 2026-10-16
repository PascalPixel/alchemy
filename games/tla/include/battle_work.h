#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "types.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle_event.h"
#include "ram_buffer.h"
#include "../../common/include/battle/battle_session.h"

struct BattleEventState {
  struct BattleEventQueue queue;
  s32 phase;
  s32 eventIndex;
  s32 timer;
  u8 unk154[0x14];
  s16 pendingCue;
  s16 unk16a;
  s32 flags;
};

STATIC_ASSERT(sizeof(struct BattleEventState) == 0x170);
STATIC_ASSERT(offsetof(struct BattleEventState, pendingCue) == 0x168);

struct BattleWork {
  s32 enemyGroup;
  u8 unk004[0x3d];
  u8 partyStatus;
  u8 unk042;
  u8 challengeFlag;
  u8 twoSided;
  u8 encounterMode;
  u8 unk046[2];
  u8 ownerSlots[8];
  u8 linkSide;
  u8 unk051;
  u8 linkPaused;
  u8 unk053[5];
  s16 partyUnits[7];
  s16 enemyIds[7];
  u8 placement[16];
  struct BattleObj slots[14];
  struct BattleActionRecord actions[20];
  u8 unk42c[0x138];
  u16 queuedSummons[2];
  u8 queuedCharges[2];
  s8 queueHead;
  s8 queueCount;
  u16 palette[128];
  s32 brightness;
  u8 unk670[6];
  u16 highlightStep;
  u16 highlightMode;
  u8 unk67a[0x6a];
  struct BattleEventState events;
  u8 unk854[4];
  s32 unk858;
  s32 unk85c;
  u8 unk860[8];
  u8 unk868;
};

STATIC_ASSERT(offsetof(struct BattleWork, partyStatus) == 0x041);
STATIC_ASSERT(offsetof(struct BattleWork, challengeFlag) == 0x043);
STATIC_ASSERT(offsetof(struct BattleWork, twoSided) == 0x044);
STATIC_ASSERT(offsetof(struct BattleWork, encounterMode) == 0x045);
STATIC_ASSERT(offsetof(struct BattleWork, partyUnits) == 0x058);
STATIC_ASSERT(offsetof(struct BattleWork, enemyIds) == 0x066);
STATIC_ASSERT(offsetof(struct BattleWork, placement) == 0x074);
STATIC_ASSERT(offsetof(struct BattleWork, slots) == 0x084);
STATIC_ASSERT(offsetof(struct BattleWork, actions) == 0x2ec);
STATIC_ASSERT(offsetof(struct BattleWork, queuedSummons) == 0x564);
STATIC_ASSERT(offsetof(struct BattleWork, queueCount) == 0x56b);
STATIC_ASSERT(offsetof(struct BattleWork, highlightStep) == 0x676);
STATIC_ASSERT(offsetof(struct BattleWork, highlightMode) == 0x678);
STATIC_ASSERT(offsetof(struct BattleWork, events) == 0x6e4);
STATIC_ASSERT(offsetof(struct BattleWork, unk868) == 0x868);

#endif
