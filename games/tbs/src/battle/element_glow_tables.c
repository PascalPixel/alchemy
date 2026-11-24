/// @file
/// Where the element glow's sparks and rings sit on their sprite sheet.

#include "battle/battle.h"

/// Byte offsets into SparkState.sheet, a byte a pixel, by frames of life left.
s32 gElementGlow_StarPictures[] = {0, 1, 5, 14, 30, 46, 62};

/// The spark's width and height in pixels by frames of life left.
u8 gElementGlow_StarSizes[] = {1, 2, 3, 4, 4, 4, 4};

/// The ring's four 32 by 32 frames, after the sparks' 78 bytes.
s32 gElementGlow_RingPictures[] = {
    78,
    78 + 1 * 32 * 32,
    78 + 2 * 32 * 32,
    78 + 3 * 32 * 32,
};

/// Unused: four words after the ring's frames that no code names.
s32 gElementGlow_UnusedWords[] = {0, 10, 20, 0};
