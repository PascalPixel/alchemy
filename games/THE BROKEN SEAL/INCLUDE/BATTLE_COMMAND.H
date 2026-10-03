#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

#include "BATTLE_RUNTIME.H"
#include "ITEM.H"
extern u8 gDebugMode[];
extern u8 gKeysHeld[];
#include "BATTLE_WORK.H"

/* Player and AI commands use the session's BattleActionRecord. */

#define BATTLE_AUTO_MODE    (*(volatile u8 *)gDebugMode)
#define BATTLE_OPTIONS      (*(volatile u32 *)gKeysHeld)

/* Per ability: the action's flag word (abilities 0 to 518) and the status
   it applies (abilities 0 to 517, zero for none). */
extern const u32 Battle_ActionFlags[];
extern const u8 Battle_ActionStatus[];

s32 BattleCommand_BuildPlan(
    struct BattleActionRecord *request,
    struct BattlePlan *plan);
s32 Func_080bbb0c(struct BattlePlan *plan, s32 target_slot);

#endif
