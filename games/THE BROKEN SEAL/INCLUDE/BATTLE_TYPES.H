#ifndef ALCHEMY_BATTLE_TYPES_H
#define ALCHEMY_BATTLE_TYPES_H

#include "LAYOUT_GUARD.H"
#include "BATTLE_UNIT.H"

enum {
    BATTLE_TARGET_CAPACITY = 14,
    BATTLE_PLAN_SIZE = 0x64
};

struct BattlePlan {
    u8 actor_id;
    s8 target_count;
    u8 target_ids[BATTLE_TARGET_CAPACITY];
    s8 target_offsets[BATTLE_TARGET_CAPACITY];
    s8 target_adjustments[BATTLE_TARGET_CAPACITY];
    s8 target_modifiers[BATTLE_TARGET_CAPACITY];
    s8 target_results[BATTLE_TARGET_CAPACITY];
    s16 command;
    u8 unknown_4a[2];
    s32 action_id;
    s32 range_index;
    s32 outcome;
    u32 presentation_flags;
    s32 failure;
    u32 spirit_dmg;
};

LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetIds,
    struct BattlePlan,
    target_ids,
    0x02);
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetAdjustments,
    struct BattlePlan,
    target_adjustments,
    0x1e);
LAYOUT_OFFSET_GUARD(
    BattlePlan_ActionId,
    struct BattlePlan,
    action_id,
    0x4c);
LAYOUT_OFFSET_GUARD(
    BattlePlan_SpiritDmg,
    struct BattlePlan,
    spirit_dmg,
    0x60);

#endif
