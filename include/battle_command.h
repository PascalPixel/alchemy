#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

#include "battle_runtime.h"
#include "item.h"

/* Player/AI command consumed by the battle-command resolver. */
struct BattleCommandRequest {
    s16 actor_id;                   /* 0x00 */
    s16 unknown_02;
    s16 unknown_04;
    s16 command;                    /* 0x06: dispatch selector, 0..99 */
    s16 parameter;                  /* 0x08: action, inventory slot, or position */
    s16 unknown_0a;
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

/* Normal and mirrored seven-unit turn orders, each terminated by 255. */
struct BattleTurnOrder {
    u8 unknown_00[0x58];
    s16 normal[7];                  /* 0x58 */
    s16 mirrored[7];                /* 0x66 */
};

LAYOUT_SIZE_GUARD(
    BattleCommandRequest_Size,
    struct BattleCommandRequest,
    0x0c);
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
LAYOUT_SIZE_GUARD(
    BattleTurnOrder_Size,
    struct BattleTurnOrder,
    0x74);
LAYOUT_OFFSET_GUARD(
    BattleTurnOrder_Normal,
    struct BattleTurnOrder,
    normal,
    0x58);
LAYOUT_OFFSET_GUARD(
    BattleTurnOrder_Mirrored,
    struct BattleTurnOrder,
    mirrored,
    0x66);

#define BATTLE_TURN_ORDER \
    (*(struct BattleTurnOrder **)0x03001e74)
#define BATTLE_AUTO_MODE    (*(volatile u8 *)0x03001f54)
#define BATTLE_OPTIONS      (*(volatile u32 *)0x03001ae8)

#define BATTLE_ACTION_FLAGS  ((const u32 *)0x080c2da0)
#define BATTLE_ACTION_STATUS ((const u8 *)0x080c2b98)

s32 Func_080be378(
    struct BattleCommandRequest *request,
    struct BattlePlan *plan);
s32 Func_080bbb0c(struct BattlePlan *plan, s32 target_slot);

#define BattleCommand_BuildPlan       Func_080be378
#define BattleAction_ResolveTarget    Func_080bbb0c

#endif
