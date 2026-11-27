// An unused array of the sprite module that points only at itself. The module's animations come in
// three blocks, and this array sits after the first, between Tret's animation list and Laurel's
// animations.

#include "types.h"

// Unused: four words, each this array's own address; no sprite record or code names it.
const void *const SpriteUnusedSelfPointersAfterFirstBlock[] = {
	SpriteUnusedSelfPointersAfterFirstBlock,
	SpriteUnusedSelfPointersAfterFirstBlock,
	SpriteUnusedSelfPointersAfterFirstBlock,
	SpriteUnusedSelfPointersAfterFirstBlock,
};
