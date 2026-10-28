#ifndef ALCHEMY_RESOURCE_H
#define ALCHEMY_RESOURCE_H

/// @file
/// The parts of a battle sprite id.

#include "types.h"
#include "edition.h"
#include "farcall.h"

/// A battle sprite id's low twelve bits; the two-part sprites' second part
/// is their id + SPRITE_SECOND_PART.
#define SPRITE_ID_MASK 0xfff
#define SPRITE_SECOND_PART 0x2001

#endif
