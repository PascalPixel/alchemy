#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

#include "LAYOUT_GUARD.H"

/* The battle records both games share. Each game's BATTLE_TYPES.H adds its
   own BattlePlan, whose target layout differs between them. */

enum {
    BATTLE_ACTION_SIZE = 0x10,
    BATTLE_UNIT_SIZE = 0x14c
};

struct OwnerActionSlot {
    u16 encoded_action;
    u16 unknown_02;
};

struct BattleElementStats {
    s16 power;
    s16 resist;
};

/* Static action definition returned by the battle action table lookup. */
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
 * Live combatant record returned by Runtime_GetObject.  Only fields established
 * by multiple battle owners are named.  The unresolved condition bytes keep
 * offset-based names until their meanings are independently witnessed.
 */
struct BattleUnit {
    u8 name[0x0f];                  /* 0x000 */
    u8 level;                       /* 0x00f */
    s16 base_hp;
    s16 base_pp;
    s16 hp_gauge;                   /* 0x014, hp / max_hp in 1/0x4000 */
    s16 pp_gauge;                   /* 0x016 */
    u16 base_attack;
    u16 base_defense;
    u16 base_agility;
    u8 base_luck;
    u8 base_action_count;
    u8 base_hp_regen;
    u8 base_pp_regen;
    u8 unknown_022[2];
    struct BattleElementStats base_elements[4];
    s16 max_hp;                     /* 0x034 */
    s16 max_pp;                     /* 0x036 */
    s16 hp;                         /* 0x038 */
    s16 pp;                         /* 0x03a */
    u16 attack;                     /* 0x03c */
    u16 defense;                    /* 0x03e */
    u16 agility;                    /* 0x040 */
    u8 luck;                        /* 0x042 */
    u8 action_entry_count;          /* 0x043 */
    u8 hp_regen;                    /* 0x044 */
    u8 pp_regen;                    /* 0x045 */
    u8 unknown_046[2];
    /* Elemental power and resistance by element. */
    struct BattleElementStats elements[4];
    struct OwnerActionSlot action_slots[32];
    u16 inventory[15];              /* 0x0d8 */
    u8 unknown_0f6[2];
    u32 djinn_available[4];
    u32 djinn_active[4];
    u8 djinn_owned_counts[4];       /* 0x118, by element */
    u8 djinn_active_counts[4];      /* 0x11c, set Djinn by element */
    u32 rewards;
    u32 experience;                 /* 0x124 */
    u8 class_id;                    /* 0x128 */
    /* Offset of the class name among the class-name messages. */
    u8 class_index;                 /* 0x129 */
    u8 status_12a;                  /* 0x12a */
    /* Nonzero levels reduce incoming damage to 1/2 or 1/10. */
    s8 guard_level;                 /* 0x12b */
    s8 element_modifier[4];
    s8 restraint;                   /* 0x130 */
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
    u8 unknown_149;
    u16 enemy_id;                   /* 0x14a */
};

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

LAYOUT_SIZE_GUARD(OwnerActionSlot_Size, struct OwnerActionSlot, 4);
LAYOUT_SIZE_GUARD(BattleElementStats_Size, struct BattleElementStats, 4);
LAYOUT_SIZE_GUARD(BattleUnit_Size, struct BattleUnit, BATTLE_UNIT_SIZE);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseHp, struct BattleUnit, base_hp, 0x10);
LAYOUT_OFFSET_GUARD(BattleUnit_BaseAttack, struct BattleUnit, base_attack, 0x18);
LAYOUT_OFFSET_GUARD(BattleUnit_Actions, struct BattleUnit, action_slots, 0x58);
LAYOUT_OFFSET_GUARD(BattleUnit_AvailableDjinn, struct BattleUnit, djinn_available, 0xf8);
LAYOUT_OFFSET_GUARD(BattleUnit_ActiveDjinn, struct BattleUnit, djinn_active, 0x108);
LAYOUT_OFFSET_GUARD(BattleUnit_Rewards, struct BattleUnit, rewards, 0x120);
LAYOUT_OFFSET_GUARD(BattleUnit_Hp, struct BattleUnit, hp, 0x038);
LAYOUT_OFFSET_GUARD(
    BattleUnit_BaseElements,
    struct BattleUnit,
    base_elements,
    0x024);
LAYOUT_OFFSET_GUARD(BattleUnit_Level, struct BattleUnit, level, 0x00f);
LAYOUT_OFFSET_GUARD(BattleUnit_Elements, struct BattleUnit, elements, 0x048);
LAYOUT_OFFSET_GUARD(
    BattleUnit_ReadyPose,
    struct BattleUnit,
    ready_pose,
    0x144);
LAYOUT_OFFSET_GUARD(
    BattleUnit_Inventory,
    struct BattleUnit,
    inventory,
    0x0d8);
LAYOUT_OFFSET_GUARD(
    BattleUnit_DjinnOwnedCounts,
    struct BattleUnit,
    djinn_owned_counts,
    0x118);
LAYOUT_OFFSET_GUARD(
    BattleUnit_DjinnActiveCounts,
    struct BattleUnit,
    djinn_active_counts,
    0x11c);
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

LAYOUT_OFFSET_GUARD(BattleUnit_EnemyId, struct BattleUnit, enemy_id, 0x14a);

#endif
