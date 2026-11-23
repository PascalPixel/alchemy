#ifndef ALCHEMY_BATTLE_ESCAPE_H
#define ALCHEMY_BATTLE_ESCAPE_H

/// @file
/// Auto battle's escape: the party running from the battle.

#include "edition.h"
#include "battle_action.h"

#include "types.h"

s32 BattleFlee_CheckSuccess(void);
s32 BattleFlee_PlayRun(struct BattleActionRecord *action);

#endif
