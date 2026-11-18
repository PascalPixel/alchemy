/// @file
/// Two arrays of the sprite bank, between its second and third blocks of
/// animations, that each point only at themselves.

#include "types.h"

/// Unused: five words right after the second block, each this array's own
/// address; no sprite record or code names it.
const void *const gSprite_UnusedSelfPointersAfterSecondBlock[] = {
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
    gSprite_UnusedSelfPointersAfterSecondBlock,
};

/// Unused: as gSprite_UnusedSelfPointersAfterSecondBlock, right before the
/// third block.
const void *const gSprite_UnusedSelfPointersBeforeThirdBlock[] = {
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
    gSprite_UnusedSelfPointersBeforeThirdBlock,
};
