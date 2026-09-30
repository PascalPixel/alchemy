#ifndef ALCHEMY_BATTLE_TYPES_H
#define ALCHEMY_BATTLE_TYPES_H

#include "LAYOUT_GUARD.H"
#include "../../COMMON/INCLUDE/BATTLE/BATTLE_UNIT.H"

enum {
    BATTLE_TARGET_CAPACITY = 14,
    BATTLE_PLAN_SIZE = 0x64
};

/*
 * Work item assembled by the battle-command resolver and consumed once for
 * each selected target.  The parallel arrays deliberately remain separate:
 * callers populate and consume them with different signedness.
 */
struct BattlePlan {
    u8 actor_id;                                  /* 0x00 */
    s8 target_count;                              /* 0x01 */
    u8 target_ids[BATTLE_TARGET_CAPACITY];        /* 0x02 */
    s8 target_offsets[BATTLE_TARGET_CAPACITY];    /* 0x10 */
    s8 target_adjustments[BATTLE_TARGET_CAPACITY]; /* 0x1e */
    s8 target_modifiers[BATTLE_TARGET_CAPACITY];  /* 0x2c */
    s8 target_results[BATTLE_TARGET_CAPACITY];    /* 0x3a */
    s16 command;                                  /* 0x48 */
    u8 unknown_4a[2];
    s32 action_id;                  /* 0x4c */
    s32 range_index;                /* 0x50 */
    s32 outcome;                    /* 0x54 */
    u32 presentation_flags;         /* 0x58 */
    s32 failure;                    /* 0x5c */
    u32 pending_amount_60;          /* 0x60 */
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
    BattlePlan_PendingAmount60,
    struct BattlePlan,
    pending_amount_60,
    0x60);

#endif
