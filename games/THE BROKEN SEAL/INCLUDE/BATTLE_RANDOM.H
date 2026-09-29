#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

#include "TYPES.H"

u32 BattleRandom16(void);

/* The battle's linear congruential generator state. */
extern s32 gBattleRandomSeed;

#endif
