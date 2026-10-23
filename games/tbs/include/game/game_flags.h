#ifndef ALCHEMY_GAME_FLAGS_H
#define ALCHEMY_GAME_FLAGS_H

/// @file
/// The byte of gGameFlagBytes that holds a flag.

#include "edition.h"
#include "types.h"

/// The byte of gGameFlagBytes that holds `flag`: bits 3 to 11 of its number,
/// isolated by shifting up and back down, which is how the game computes it.
#define GAME_FLAG_BYTE(flag) (((u32)(flag) << 20) >> 23)

#endif
