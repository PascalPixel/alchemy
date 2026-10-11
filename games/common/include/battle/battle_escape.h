#ifndef ALCHEMY_BATTLE_ESCAPE_H
#define ALCHEMY_BATTLE_ESCAPE_H

#include "edition.h"
#include "battle_session.h"

#include "types.h"

#if GAME_TLA
s32 BattleEscape_CheckSuccess(void);
s32 BattleEscape_PlayRun(struct BattleActionRecord *action);
#endif

#endif
