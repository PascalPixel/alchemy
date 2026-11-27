/* @file */
/* Where the element glow's sparks and rings sit on their sprite sheet. */

#include "types.h"

/* Byte offsets into STAR_FIELD_STATE.sheet, a byte a pixel, by frames of life
 */
/* left. */
const s32 ElementGlow_SparkPictures[] = {0, 1, 5, 14, 30, 46, 62};

/* The spark's width and height in pixels by frames of life left. */
const u8 ElementGlow_SparkSizes[] = {1, 2, 3, 4, 4, 4, 4};

/* The ring's four 32 by 32 frames, after the sparks' 78 bytes. */
const s32 ElementGlow_RingPictures[] = {
	78,
	78 + 1 * 32 * 32,
	78 + 2 * 32 * 32,
	78 + 3 * 32 * 32,
};
