#ifndef ALCHEMY_BATTLE_ESCAPE_H
#define ALCHEMY_BATTLE_ESCAPE_H

#include "types.h"

s32 Func_080b90f8(void);
s32 Func_080b8824(s16 *action);

#define BattleEscape_CheckSuccess Func_080b90f8
#define BattleEscape_PlayRun Func_080b8824

#endif
