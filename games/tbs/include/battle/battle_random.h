#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

/* BattleRandomSeed, the battle's random seed. */

#include "edition.h"
#include "types.h"

/* gmBattleRandom's seed, apart from syRandom's so the battle's rolls repeat on
   both consoles of a link battle: it starts at 0 there, and each turn the
   parent (link side 0) copies RandomSeed into it and sends it to the child. */
extern Sint32 BattleRandomSeed;

/* Sets BattleRandomSeed, as every write to it does: a new battle's 0, the seed
   a link turn exchanges and each roll's next seed. */
#define SET_RANDOM_SEED(seed) \
	do { \
		BattleRandomSeed = (seed); \
	} while(0)

#endif
