#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

#include "btlsys.h"
#include "item.h"
extern u8 gDebugMode[];
extern u8 gKeysHeld[];
#include "btlwork.h"

#define BATTLE_AUTO_MODE    (*(volatile u8 *)gDebugMode)
#define BATTLE_OPTIONS      (*(volatile u32 *)gKeysHeld)

extern const u32 Battle_ActionFlags[];
extern const u8 Battle_ActionStatus[];

s32 BattleCommand_BuildPlan(
    struct BattleActionRecord *request,
    struct BtlPlan *plan);

#endif
