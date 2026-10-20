#ifndef ALCHEMY_BATTLE_RANDOM_H
#define ALCHEMY_BATTLE_RANDOM_H

/// @file
/// gBattleRandomSeed, the battle's random seed.

#include "edition.h"
#include "types.h"

/// BattleRandom's seed, apart from Random16's so the battle's rolls repeat on
/// both consoles of a link battle: it starts at 0 there, and each round the
/// parent (link side 0) copies gRandomSeed into it and sends it to the child.
extern s32 gBattleRandomSeed;

#endif
