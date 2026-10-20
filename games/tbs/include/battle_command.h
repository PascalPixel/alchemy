#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

/// @file
/// The battle command names: auto battle and the battle options (aliases of
/// debug and key globals), and the actions' flag and status tables.

#include "battle_system.h"
#include "item.h"
#include "keystate.h"
#include "frame.h"
#include "battle_work.h"

/// gDebugMode under the battle's name. While it and flag 0x16d are set,
/// holding R as an action is planned defeats every enemy, or with Select the
/// party (BattleCommand_BuildPlan).
#define BATTLE_AUTO_MODE gDebugMode
/// gKeysHeld under the battle's name: the keys held this frame.
#define BATTLE_OPTIONS gKeysHeld

/// Each ability's staging bits (BattlePlan.presentationFlags), by ability id
/// 0 to 518 (battle/actflags.tsv).
extern const u32 gBattle_ActionFlags[];
/// Each ability's outcome staging (BattlePlan.outcome), 0 for the default, by
/// ability id 0 to 517 (battle/actstat.tsv).
extern const u8 gBattle_ActionStatus[];

#endif
