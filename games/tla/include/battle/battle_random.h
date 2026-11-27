#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

/* The battle's own random numbers, which a link battle keeps in step on both
   consoles. */

#include "edition.h"
#include "types.h"
#include "system.h"

/* Sets BattleRandomSeed, as the battle's writes to it do: a new battle's 0 and
   the seed a link turn exchanges. */
#define SET_RANDOM_SEED(seed) \
	do { \
		BattleRandomSeed = (seed); \
	} while(0)

#endif
