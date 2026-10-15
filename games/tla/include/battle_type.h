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
    s8 target_count;
    u8 actor_id2;
    u8 target_ids[BATTLE_TARGET_CAPACITY];
    s8 target_offset0;
    s8 target_dist[BATTLE_TARGET_CAPACITY];
    s8 target_adjustment0;
    s8 target_adj[BATTLE_TARGET_CAPACITY];
    s8 target_modifier0;
    s8 target_crit[BATTLE_TARGET_CAPACITY];
    s8 target_result0;
    s8 target_hit[BATTLE_TARGET_CAPACITY];
    u8 unknown_48[2];
    s16 cmd;
    s32 ability_id;
    s32 element;
    s32 outcome;
    u32 presentation_flags;
    s32 failure;
    u32 haunt_dmg;
};

struct ActionDefinition {
    u8 unknown_00[12];
};

LAYOUT_SIZE_GUARD(ActionDefinition_Size, struct ActionDefinition, 12);
LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
LAYOUT_OFFSET_GUARD(BattlePlan_ActorId2, struct BattlePlan, actor_id2, 0x02);
LAYOUT_OFFSET_GUARD(BattlePlan_TargetIds, struct BattlePlan, target_ids, 0x03);
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetAdj,
    struct BattlePlan,
    target_adj,
    0x1f);
LAYOUT_OFFSET_GUARD(BattlePlan_Command, struct BattlePlan, cmd, 0x4a);
LAYOUT_OFFSET_GUARD(BattlePlan_ActionId, struct BattlePlan, ability_id, 0x4c);
LAYOUT_OFFSET_GUARD(
    BattlePlan_SpiritDmg,
    struct BattlePlan,
    haunt_dmg,
    0x60);

#endif
