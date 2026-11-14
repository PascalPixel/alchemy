#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

/// @file
/// gBattleRandomSeed, the battle's random seed.

#include "edition.h"
#include "types.h"

/// Battle_Random's seed, apart from Random16's so the battle's rolls repeat on
/// both consoles of a link battle: it starts at 0 there, and each turn the
/// parent (link side 0) copies gRandomSeed into it and sends it to the child.
extern s32 gBattleRandomSeed;

/// Sets gBattleRandomSeed, as every write to it does: a new battle's 0, the
/// seed a link turn exchanges and each roll's next seed. The name is ours.
#define Battle_SetRandomSeed(seed)                                             \
  do {                                                                         \
    gBattleRandomSeed = (seed);                                                \
  } while (0)

#endif
