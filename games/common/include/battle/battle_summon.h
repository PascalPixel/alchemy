#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

#include "static_assert.h"

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct BattleSpriteDefinition {
  u16 animation;
  u8 spriteBit0 : 1;
  u8 spriteBits1 : 4;
  u8 spriteBits5 : 3;
  u8 layoutBit0 : 1;
  u8 layoutBits1 : 4;
  u8 layoutKind : 3;
  u8 unk04[4];
};

STATIC_ASSERT(sizeof(struct BattleSpriteDefinition) == 8);
extern struct BattleSpriteDefinition gSummon_EntryTable[];

struct SummonDefinition {
  u16 nameMessageId;
  u8 unk02[2];
  u8 djinnRequired[4];
};

struct BattleSummonState {
  u32 availableMask;
};

#if GAME_TLA
const struct SummonDefinition *SummonDefinition_Get(s32 summon_id);
#endif
#if GAME_TLA
s32 EnemySlotGet(void);
s32 EnemyCheck(s32 classId);

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32 BattleGroupRandom(s32 recordId);
s32 EnemyCountDecrement(s32 classId, s32 n);
s32 EnemyCountRelease(s32 actorId);
s32 EnemyCountClear(s32 classId);
s32 EnemyUpdate(void);
#endif

#endif
