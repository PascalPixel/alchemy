#ifndef ALCHEMY_BATTLE_SYSTEM_H
#define ALCHEMY_BATTLE_SYSTEM_H

/// @file
/// BattleUnit_Recalculate, which recalculates a unit's stats.

#include "playback_queue.h"
#include "battle_action_run.h"
#include "edition.h"
#include "far_call.h"

void BattleUnit_Recalculate(s32 unitId);

#endif
