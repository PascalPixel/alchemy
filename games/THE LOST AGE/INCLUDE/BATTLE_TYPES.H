#ifndef ALCHEMY_BATTLE_TYPES_H
#define ALCHEMY_BATTLE_TYPES_H

#include "LAYOUT_GUARD.H"
#include "../../COMMON/INCLUDE/BATTLE/BATTLE_UNIT.H"

enum {
    BATTLE_TARGET_CAPACITY = 13,
    BATTLE_PLAN_SIZE = 0x64
};

struct BattlePlan {
    u8 actor_id;
    s8 target_count;
    u8 actor_id2;
    u8 target_ids[BATTLE_TARGET_CAPACITY];
    s8 target_offset0;
    s8 target_offsets[BATTLE_TARGET_CAPACITY];
    s8 target_adjustment0;
    s8 target_adjustments[BATTLE_TARGET_CAPACITY];
    s8 target_modifier0;
    s8 target_modifiers[BATTLE_TARGET_CAPACITY];
    s8 target_result0;
    s8 target_results[BATTLE_TARGET_CAPACITY];
    u8 unknown_48[2];
    s16 command;
    s32 action_id;
    s32 range_index;
    s32 outcome;
    u32 presentation_flags;
    s32 failure;
    u32 spirit_dmg;
};

struct ActionDefinition {
    u8 unknown_00[12];
};

LAYOUT_SIZE_GUARD(ActionDefinition_Size, struct ActionDefinition, 12);
LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_ActorId2, struct BattlePlan, actor_id2, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetIds, struct BattlePlan, target_ids, 0x03);
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetAdjustments,
    struct BattlePlan,
    target_adjustments,
    0x1f);
LAYOUT_OFFSET_GUARD(BattlePlan_Command, struct BattlePlan, command, 0x4a);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BattlePlan, action_id, 0x4c);
LAYOUT_OFFSET_GUARD(
    BattlePlan_SpiritDmg,
    struct BattlePlan,
    spirit_dmg,
    0x60);

#endif
