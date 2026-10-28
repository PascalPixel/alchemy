#ifndef ALCHEMY_BATTLE_SPRITE_ID_H
#define ALCHEMY_BATTLE_SPRITE_ID_H

/// @file
/// The parts of a battle sprite id.

#include "types.h"
#include "edition.h"
#include "far_call.h"

/// A battle sprite id's low twelve bits; the two-part sprites' second part
/// is their id + SPRITE_SECOND_PART.
#define SPRITE_ID_MASK 0xfff
#define SPRITE_SECOND_PART 0x2001

#endif
