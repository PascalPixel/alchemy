#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

#include "layout_guard.h"
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

LAYOUT_SIZE_GUARD(ActionDefinition_Size, struct ActionDefinition, 12);
LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_ActorId2, struct BattlePlan, actorId2, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetIds, struct BattlePlan, targetIds, 0x03);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetMultiplier, struct BattlePlan,
                    targetMultiplier, 0x1f);
LAYOUT_OFFSET_GUARD(BattlePlan_Command, struct BattlePlan, command, 0x4a);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BattlePlan, abilityId, 0x4c);
LAYOUT_OFFSET_GUARD(BattlePlan_HauntDamage, struct BattlePlan, hauntDamage,
                    0x60);

#endif
