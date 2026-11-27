// Two unused arrays of the sprite module that each point only at themselves. They sit after the
// second of the module's three blocks of animations, between the young man in green's animation
// list and the woman in the pink apron's animations.

#include "types.h"

// Unused: five words, each this array's own address; no sprite record or code names it.
const void *const SpriteUnusedSelfPointersAfterSecondBlock[] = {
    SpriteUnusedSelfPointersAfterSecondBlock,
    SpriteUnusedSelfPointersAfterSecondBlock,
    SpriteUnusedSelfPointersAfterSecondBlock,
    SpriteUnusedSelfPointersAfterSecondBlock,
    SpriteUnusedSelfPointersAfterSecondBlock,
};

// Unused: as SpriteUnusedSelfPointersAfterSecondBlock, right before the woman in the pink apron's
// animations.
const void *const SpriteUnusedSelfPointersBeforeThirdBlock[] = {
    SpriteUnusedSelfPointersBeforeThirdBlock,
    SpriteUnusedSelfPointersBeforeThirdBlock,
    SpriteUnusedSelfPointersBeforeThirdBlock,
    SpriteUnusedSelfPointersBeforeThirdBlock,
    SpriteUnusedSelfPointersBeforeThirdBlock,
};
