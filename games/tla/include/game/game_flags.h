#ifndef ALCHEMY_GAME_FLAGS_H
#define ALCHEMY_GAME_FLAGS_H

/* The byte and bit of GameFlagBytes that hold a flag. */

#include "edition.h"
#include "types.h"

/* The byte of GameFlagBytes that holds `flag`: bits 3 to 11 of its number,
   isolated by shifting up and back down, which is how the game computes it. */
#define GAME_FLAG_BYTE(flag) (((Uint32)(flag) << 20) >> 23)

/* The bit of its byte that holds `flag`: the low three bits of its number. */
#define GAME_FLAG_BIT(flag) (1 << ((flag) & 7))

Uint32 syGameFlagSetByte(Uint32 flag, Uint8 value);

#endif
