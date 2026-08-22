#ifndef ALCHEMY_BATTLE_TYPES_H
#define ALCHEMY_BATTLE_TYPES_H

#include "layout_guard.h"

enum {
    BATTLE_TARGET_CAPACITY = 14,
    BATTLE_PLAN_SIZE = 0x64,
    BATTLE_ACTION_SIZE = 0x10,
    BATTLE_UNIT_SIZE = 0x14c
};

/*
 * Work item assembled by the battle-command resolver and consumed once for
 * each selected target.  The parallel arrays deliberately remain separate:
 * callers populate and consume them with different signedness.  GS2 uses the
 * first byte of this area for a second actor and therefore has thirteen
 * direct target IDs.
 */
struct BattlePlan {
    u8 actor_id;                                  /* 0x00 */
    s8 target_count;                              /* 0x01 */
#ifdef BATTLE_SECOND_ACTOR
    u8 actor_id2;                                 /* 0x02 */
    u8 target_ids[BATTLE_TARGET_CAPACITY - 1];    /* 0x03 */
#else
    u8 target_ids[BATTLE_TARGET_CAPACITY];        /* 0x02 */
#endif
    s8 target_offsets[BATTLE_TARGET_CAPACITY];    /* 0x10 */
    s8 target_adjustments[BATTLE_TARGET_CAPACITY]; /* 0x1e */
    s8 target_modifiers[BATTLE_TARGET_CAPACITY];  /* 0x2c */
    s8 target_results[BATTLE_TARGET_CAPACITY];    /* 0x3a */
#ifdef BATTLE_SECOND_ACTOR
    u8 unknown_48[2];                             /* 0x48 */
    s16 command;                                  /* 0x4a */
#else
    s16 command;                                  /* 0x48 */
    u8 unknown_4a[2];
#endif
    s32 action_id;                  /* 0x4c */
    s32 range_index;                /* 0x50 */
    s32 outcome;                    /* 0x54 */
    u32 presentation_flags;         /* 0x58 */
    s32 failure;                    /* 0x5c */
    u32 pending_amount_60;          /* 0x60 */
};

/* Static action definition returned by Func_08077080. */
struct BattleAction {
    u8 target_mode;                 /* 0x00 */
    u8 target_flags;                /* 0x01 */
    u8 damage_class;                /* 0x02 */
    u8 effect;                      /* 0x03 */
    u8 unknown_04[4];
    u8 range;                       /* 0x08 */
    u8 pp_cost;                     /* 0x09 */
    u16 power;                      /* 0x0a */
    u8 type_0c;                     /* 0x0c */
    u8 unknown_0d[3];
};

/*
 * Live combatant record returned by Func_08077008.  Only fields established
 * by multiple battle owners are named.  The unresolved condition bytes keep
 * offset-based names until their meanings are independently witnessed.
 */
struct BattleUnit {
    u8 unknown_000[0x34];
    s16 max_hp;                     /* 0x034 */
    s16 max_pp;                     /* 0x036 */
    s16 hp;                         /* 0x038 */
    s16 pp;                         /* 0x03a */
    u16 attack;                     /* 0x03c */
    u16 defense;                    /* 0x03e */
    u16 agility;                    /* 0x040 */
    u8 luck;                        /* 0x042 */
    u8 action_entry_count;          /* 0x043 */
    u8 unknown_044[0x94];
    u16 inventory[15];              /* 0x0d8 */
    u8 unknown_0f6[0x32];
    u8 class_id;                    /* 0x128 */
    u8 class_bonus_disabled;        /* 0x129 */
    u8 status_12a;                  /* 0x12a */
    /* Nonzero levels reduce incoming damage to 1/2 or 1/10. */
    u8 guard_level;                 /* 0x12b */
    u8 status_12c;                  /* 0x12c */
    u8 status_12d;                  /* 0x12d */
    u8 status_12e;                  /* 0x12e */
    u8 status_12f;                  /* 0x12f */
    u8 restraint;                   /* 0x130 */
    s8 poison;                  /* 0x131 */
    u8 attack_modifier_turns;       /* 0x132 */
    s8 attack_modifier;             /* 0x133 */
    u8 defense_modifier_turns;      /* 0x134 */
    s8 defense_modifier;            /* 0x135 */
    u8 res_modifier_turns;                  /* 0x136 */
    s8 res_modifier;                  /* 0x137 */
    u8 delusion;                  /* 0x138 */
    s8 confusion;                  /* 0x139 */
    u8 charm;                  /* 0x13a */
    u8 stun;                  /* 0x13b */
    u8 sleep;                  /* 0x13c */
    u8 psy_seal;                  /* 0x13d */
    u8 refrain;                  /* 0x13e */
    s8 reflect;                  /* 0x13f */
    u8 evil_spirit;                  /* 0x140 */
    u8 death_count;                  /* 0x141 */
    u8 unknown_142[2];
    u8 ready_pose;                  /* 0x144 「みがまえた！」で2 */
    u8 cannot_move;                 /* 0x145 */
    u8 agility_modifier_turns;      /* 0x146 */
    s8 agility_modifier;            /* 0x147 */
    s8 battle_end_state;            /* 0x148 */
    u8 unknown_149[3];
};

LAYOUT_SIZE_GUARD(BattlePlan_Size, struct BattlePlan, BATTLE_PLAN_SIZE);
#ifdef BATTLE_SECOND_ACTOR
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetIds,
    struct BattlePlan,
    target_ids,
    0x03);
#else
LAYOUT_OFFSET_GUARD(
    BattlePlan_TargetIds,
    struct BattlePlan,
    target_ids,
    0x02);
#endif
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

LAYOUT_SIZE_GUARD(
    BattleAction_Size,
    struct BattleAction,
    BATTLE_ACTION_SIZE);
LAYOUT_OFFSET_GUARD(
    BattleAction_Effect,
    struct BattleAction,
    effect,
    0x03);
LAYOUT_OFFSET_GUARD(
    BattleAction_Power,
    struct BattleAction,
    power,
    0x0a);
LAYOUT_OFFSET_GUARD(
    BattleAction_Type,
    struct BattleAction,
    type_0c,
    0x0c);

LAYOUT_SIZE_GUARD(BattleUnit_Size, struct BattleUnit, BATTLE_UNIT_SIZE);
LAYOUT_OFFSET_GUARD(BattleUnit_Hp, struct BattleUnit, hp, 0x038);
LAYOUT_OFFSET_GUARD(
    BattleUnit_Inventory,
    struct BattleUnit,
    inventory,
    0x0d8);
LAYOUT_OFFSET_GUARD(
    BattleUnit_GuardLevel,
    struct BattleUnit,
    guard_level,
    0x12b);
LAYOUT_OFFSET_GUARD(
    BattleUnit_Status13a,
    struct BattleUnit,
    charm,
    0x13a);
LAYOUT_OFFSET_GUARD(
    BattleUnit_Status140,
    struct BattleUnit,
    evil_spirit,
    0x140);

#endif
