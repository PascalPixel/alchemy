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

#define BATTLE_AUTO_MODE gDebugMode
#define BATTLE_OPTIONS gKeysHeld

extern const u32 gBattle_ActionFlags[];
extern const u8 gBattle_ActionStatus[];

#endif
