#ifndef ALCHEMY_GAME_FLAGS_H
#define ALCHEMY_GAME_FLAGS_H

/// @file
/// The byte of gGameFlagBytes that holds a flag.

#include "edition.h"
#include "types.h"

/// The byte of gGameFlagBytes that holds `flag`: bits 3 to 11 of its number,
/// isolated by shifting up and back down, which is how the game computes it.
/// A number past 4,095 wraps onto a lower flag rather than reaching the memory
/// after gGameFlagBytes.
#define GAME_FLAG_BYTE(flag) (((u32)(flag) << 20) >> 23)

/// The bit of its byte that holds `flag`: the low three bits of its number.
#define GAME_FLAG_BIT(flag) (1 << ((flag) & 7))

#endif
