#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

#include "layout_guard.h"
#include "../../common/include/battle/battle_unit.h"

enum {
    BATTLE_TARGET_CAPACITY = 13,
    BATTLE_PLAN_SIZE = 0x64
};

struct BattlePlan {
    u8 aid;
    s8 targetCount;
    u8 actorId2;
    u8 targetIds[BATTLE_TARGET_CAPACITY];
    s8 targetOffset0;
    s8 targetDist[BATTLE_TARGET_CAPACITY];
    s8 targetAdjustment0;
    s8 targetAdj[BATTLE_TARGET_CAPACITY];
    s8 targetModifier0;
    s8 targetCrit[BATTLE_TARGET_CAPACITY];
    s8 targetResult0;
    s8 targetHit[BATTLE_TARGET_CAPACITY];
    u8 unk48[2];
    s16 cmd;
    s32 abilityId;
    s32 element;
    s32 outcome;
    u32 presentationFlags;
    s32 failure;
    u32 hauntDmg;
};

struct ActionDefinition {
    u8 unk00[12];
};

LAYOUT_SIZE_GUARD(ActionDefinition_Size, struct ActionDefinition, 12);
LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_ActorId2, struct BattlePlan, actorId2, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetIds, struct BattlePlan, targetIds, 0x03);
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetAdj,
    struct BattlePlan,
    targetAdj,
    0x1f);
LAYOUT_OFFSET_GUARD(BattlePlan_Command, struct BattlePlan, cmd, 0x4a);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BattlePlan, abilityId, 0x4c);
LAYOUT_OFFSET_GUARD(
    BattlePlan_SpiritDmg,
    struct BattlePlan,
    hauntDmg,
    0x60);

#endif
