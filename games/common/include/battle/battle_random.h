#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

#include "edition.h"
#include "types.h"

#if GAME_TLA
u32 BtlRnd(void);
#endif

extern s32 gBattleRandomSeed;

#endif
