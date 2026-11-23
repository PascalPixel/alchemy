#ifndef ALCHEMY_BATTLE_STATUS_ICON_H
#define ALCHEMY_BATTLE_STATUS_ICON_H

/// @file
/// The status icons shown over a unit in battle.

#include "edition.h"
#include "battle_action.h"

void BattleUnit_BuildStatusFlags(s32 id, struct BattleObject *slot);
void BattleStatusIcon_Cycle(struct BattleObject *slot);

#endif
