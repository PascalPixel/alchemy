#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

#include "static_assert.h"
#include "../../common/include/battle/battle_unit.h"

enum { BATTLE_TARGET_CAPACITY = 13, BATTLE_PLAN_SIZE = 0x64 };

struct BattlePlan {
  u8 actorId;
  s8 targetCount;
  u8 actorId2;
  u8 targetIds[BATTLE_TARGET_CAPACITY];
  s8 targetOffset0;
  s8 targetOffset[BATTLE_TARGET_CAPACITY];
  s8 targetAdjustment0;
  s8 targetMultiplier[BATTLE_TARGET_CAPACITY];
  s8 targetModifier0;
  s8 targetCritical[BATTLE_TARGET_CAPACITY];
  s8 targetResult0;
  s8 targetHit[BATTLE_TARGET_CAPACITY];
  u8 unk48[2];
  s16 command;
  s32 abilityId;
  s32 element;
  s32 outcome;
  u32 presentationFlags;
  s32 failure;
  u32 hauntDamage;
};

struct ActionDefinition {
  u8 unk00[12];
};

STATIC_ASSERT(sizeof(struct ActionDefinition) == 12);
STATIC_ASSERT(sizeof(struct BattlePlan) == BATTLE_PLAN_SIZE);
STATIC_ASSERT(offsetof(struct BattlePlan, actorId2) == 0x02);
STATIC_ASSERT(offsetof(struct BattlePlan, targetIds) == 0x03);
STATIC_ASSERT(offsetof(struct BattlePlan, targetMultiplier) == 0x1f);
STATIC_ASSERT(offsetof(struct BattlePlan, command) == 0x4a);
STATIC_ASSERT(offsetof(struct BattlePlan, abilityId) == 0x4c);
STATIC_ASSERT(offsetof(struct BattlePlan, hauntDamage) == 0x60);

#endif
