#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

/// @file
/// The battle's own random numbers, which a link battle keeps in step on both
/// consoles.

#include "edition.h"
#include "types.h"

u32 Battle_Random(void);

/// Battle_Random's seed, apart from Random16's so the battle's rolls repeat on
/// both consoles of a link battle: it starts at 0 there, and each turn the
/// parent (link side 0) copies gRandomSeed into it and sends it to the child.
extern s32 gBattleRandomSeed;

/// Sets gBattleRandomSeed, as the battle's writes to it do: a new battle's 0
/// and the seed a link turn exchanges.
#define Battle_SetRandomSeed(seed)                                             \
  do {                                                                         \
    gBattleRandomSeed = (seed);                                                \
  } while (0)

#endif
