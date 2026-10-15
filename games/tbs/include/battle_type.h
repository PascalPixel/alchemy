#ifndef ALCHEMY_BATTLE_TYPE_H
#define ALCHEMY_BATTLE_TYPE_H

#include "layout_guard.h"
#include "battle_unit.h"

enum
{
    BATTLE_TARGET_CAPACITY = 14,
    BATTLE_PLAN_SIZE = 0x64
};

struct BattlePlan
{
    u8  aid;
    s8  targetCount;
    u8  targetIds[BATTLE_TARGET_CAPACITY];
    s8  targetDist[BATTLE_TARGET_CAPACITY];
    s8  targetAdj[BATTLE_TARGET_CAPACITY];
    s8  targetCrit[BATTLE_TARGET_CAPACITY];
    s8  targetHit[BATTLE_TARGET_CAPACITY];
    s16 cmd;
    u8  unk4a[2];
    s32 abilityId;
    s32 element;
    s32 outcome;
    u32 presentationFlags;
    s32 failure;
    u32 hauntDmg;
};

LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetIds, struct BattlePlan, targetIds, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetAdj, struct BattlePlan, targetAdj, 0x1e);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BattlePlan, abilityId, 0x4c);
LAYOUT_OFFSET_GUARD(BattlePlan_SpiritDmg, struct BattlePlan, hauntDmg, 0x60);

#endif
