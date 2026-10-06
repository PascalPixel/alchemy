#ifndef ALCHEMY_BATTLE_UNIT_H
#define ALCHEMY_BATTLE_UNIT_H

#include "LAYOUT_GUARD.H"

enum {
    BATTLE_ACTION_SIZE = 0x10,
    BATTLE_UNIT_SIZE = 0x14c,
    OWNER_ACTION_ID_MASK = 0x3fff
};

struct OwnerActionSlot {
    u16 encoded_action;
    u16 unknown_02;
};

struct BattleElementStats {
    s16 power;
    s16 resist;
};

struct BattleAction {
    u8 target_mode;
    u8 target_flags;
    u8 damage_class;
    u8 effect;
    u8 unknown_04[4];
    u8 range;
    u8 pp_cost;
    u16 power;
    u8 type_0c;
    u8 unknown_0d[3];
};

enum BattleActionUse {
    ACTION_USABLE_IN_FIELD = 0x40,
    ACTION_USABLE_IN_BATTLE = 0x80
};

struct BattleUnit {
    u8 name[0x0f];
    u8 level;
    s16 base_hp;
    s16 base_pp;
    s16 hp_gauge;
    s16 pp_gauge;
    u16 base_attack;
    u16 base_defense;
    u16 base_agility;
    u8 base_luck;
    u8 base_action_count;
    u8 base_hp_regen;
    u8 base_pp_regen;
    u8 unknown_022[2];
    struct BattleElementStats base_elements[4];
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
    u16 attack;
    u16 defense;
    u16 agility;
    u8 luck;
    u8 action_entry_count;
    u8 hp_regen;
    u8 pp_regen;
    u8 unknown_046[2];
    struct BattleElementStats elements[4];
    struct OwnerActionSlot action_slots[32];
    u16 inventory[15];
    u8 unknown_0f6[2];
    u32 djinn_available[4];
    u32 djinn_active[4];
    u8 djinn_owned_counts[4];
    u8 djinn_active_counts[4];
    u32 rewards;
    u32 experience;
    u8 class_id;
    u8 class_index;
    u8 status_12a;
#if GAME_TLA
    u8 guard_level;
#else
    s8 guard_level;
#endif
    s8 element_modifier[4];
    s8 restraint;
    s8 poison;
    u8 attack_modifier_turns;
    s8 attack_modifier;
    u8 defense_modifier_turns;
    s8 defense_modifier;
    u8 res_modifier_turns;
    s8 res_modifier;
    u8 delusion;
    s8 confusion;
    u8 charm;
    u8 stun;
    u8 sleep;
    u8 psy_seal;
    u8 refrain;
    s8 reflect;
    u8 evil_spirit;
    u8 death_count;
    u8 unknown_142[2];
    u8 ready_pose;
    u8 cannot_move;
    u8 agility_modifier_turns;
    s8 agility_modifier;
    s8 battle_end_state;
    u8 unknown_149;
    u16 enemy_id;
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
