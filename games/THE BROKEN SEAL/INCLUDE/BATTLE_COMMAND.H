#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

#include "BATTLE_RUNTIME.H"
#include "ITEM.H"
extern u8 gDebugMode[];
extern u8 gKeysHeld[];
#include "BATTLE_WORK.H"

/* Player/AI command consumed by the battle-command resolver: one of the
   session's queued action records. */
struct BattleCommandRequest {
    s16 actor_id;                   /* 0x00 */
    s16 unknown_02;
    s16 unknown_04;
    s16 command;                    /* 0x06: dispatch selector, 0..99 */
    s16 parameter;                  /* 0x08: action, inventory slot, or Djinni */
    s16 target;                     /* 0x0a: position in bits 0-3, bit 7 the enemy side */
    s16 range;                      /* 0x0c */
    s16 unknown_0e;
};

struct BattleSummonDefinition {
    u16 action_id;                  /* 0x00 */
    u16 unknown_02;
    u8 element_costs[4];            /* 0x04 */
};

struct BattlePendingAction {
    u8 element;                     /* 0x00 */
    u8 unknown_01;
    u8 object_id;                   /* 0x02 */
    s8 state;                       /* 0x03 */
};

struct BattlePendingActions {
    u8 unknown_000[8];
    struct BattlePendingAction entries[64];
    s32 entry_count;                /* 0x108 */
};


LAYOUT_SIZE_GUARD(
    BattleCommandRequest_Size,
    struct BattleCommandRequest,
    0x10);
LAYOUT_OFFSET_GUARD(
    BattleCommandRequest_Command,
    struct BattleCommandRequest,
    command,
    0x06);
LAYOUT_OFFSET_GUARD(
    BattleCommandRequest_Parameter,
    struct BattleCommandRequest,
    parameter,
    0x08);
LAYOUT_SIZE_GUARD(
    BattleSummonDefinition_Size,
    struct BattleSummonDefinition,
    0x08);
LAYOUT_SIZE_GUARD(
    BattlePendingAction_Size,
    struct BattlePendingAction,
    0x04);
LAYOUT_SIZE_GUARD(
    BattlePendingActions_Size,
    struct BattlePendingActions,
    0x10c);
LAYOUT_OFFSET_GUARD(
    BattlePendingActions_Count,
    struct BattlePendingActions,
    entry_count,
    0x108);

#define BATTLE_AUTO_MODE    (*(volatile u8 *)gDebugMode)
#define BATTLE_OPTIONS      (*(volatile u32 *)gKeysHeld)

/* Per ability: the action's flag word (abilities 0 to 518) and the status
   it applies (abilities 0 to 517, zero for none). */
extern const u32 Battle_ActionFlags[];
extern const u8 Battle_ActionStatus[];

s32 BattleCommand_BuildPlan(
    struct BattleCommandRequest *request,
    struct BattlePlan *plan);
s32 Func_080bbb0c(struct BattlePlan *plan, s32 target_slot);

#endif
