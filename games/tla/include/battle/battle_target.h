#ifndef ALCHEMY_BATTLE_TARGET_H
#define ALCHEMY_BATTLE_TARGET_H

/// @file
/// A fallen target replaced by a living unit of its side.

#include "edition.h"
#include "battle_action.h"

#include "types.h"

s32 BattleAbility_ReplaceDefeated(const struct BattleActionRecord *action);

#endif
