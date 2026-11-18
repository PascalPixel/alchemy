/// @file
/// An array of the sprite bank, after the first block of animations, that
/// points only at itself.

#include "types.h"

/// Unused: four words, each this array's own address; no sprite record or code
/// names it.
const void *const gSprite_UnusedSelfPointersAfterFirstBlock[] = {
    gSprite_UnusedSelfPointersAfterFirstBlock,
    gSprite_UnusedSelfPointersAfterFirstBlock,
    gSprite_UnusedSelfPointersAfterFirstBlock,
    gSprite_UnusedSelfPointersAfterFirstBlock,
};
