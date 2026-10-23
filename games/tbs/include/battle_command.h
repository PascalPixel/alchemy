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

/// gDebugMode under the battle's name. While it and FLAG_DEBUG_BATTLE_KEYS are
/// set, holding R as an action is planned defeats every enemy, or with Select
/// the party (BattleTurn_Build).
#define BATTLE_AUTO_MODE gDebugMode
/// gKeysHeld under the battle's name: the keys held this frame.
#define BATTLE_OPTIONS gKeysHeld

/// Each ability's staging bits (BattleTurn.animation), by ability id
/// 0 to 518 (battle/ability_anims.tsv).
extern const u32 gAbilityAnimations[];
/// Each ability's outcome staging (BattleTurn.staging), 0 for the default, by
/// ability id 0 to 517 (battle/ability_staging.tsv).
extern const u8 gAbilityStaging[];

#endif
