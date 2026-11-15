/// @file
/// Two arrays of the sprite bank, after the second block of animations, that
/// each point only at themselves.

#include "types.h"

/// Unused: five words, each this array's own address; no sprite record or code
/// names it.
const void *const gSprite_UnusedSelfPointers1[] = {
    gSprite_UnusedSelfPointers1, gSprite_UnusedSelfPointers1,
    gSprite_UnusedSelfPointers1, gSprite_UnusedSelfPointers1,
    gSprite_UnusedSelfPointers1,
};

/// Unused: as gSprite_UnusedSelfPointers1.
const void *const gSprite_UnusedSelfPointers2[] = {
    gSprite_UnusedSelfPointers2, gSprite_UnusedSelfPointers2,
    gSprite_UnusedSelfPointers2, gSprite_UnusedSelfPointers2,
    gSprite_UnusedSelfPointers2,
};
