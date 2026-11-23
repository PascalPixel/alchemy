/// @file
/// Two unused arrays of the sprite bank that each point only at themselves.
/// They sit after the second of the bank's three blocks of animations, between
/// the young man in green's animation list and the woman in the pink apron's
/// animations.

#include "types.h"

/// Unused: five words, each this array's own address; no sprite record or code
/// names it.
const void *const gSprite_UnusedSelfPointersAfterSecondBlock[] = {
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
};

/// Unused: as gSprite_UnusedSelfPointersAfterSecondBlock, right before the
/// woman in the pink apron's animations.
const void *const gSprite_UnusedSelfPointersBeforeThirdBlock[] = {
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
};
