/* Where the element glow's sparks and rings sit on their sprite sheet. */

#include "types.h"

/* Byte offsets into STAR_FIELD_STATE.sheet, a byte a pixel, by ticks of life
   left. */
const Sint32 ElementGlowSparkPictures[] = {0, 1, 5, 14, 30, 46, 62};

/* The spark's width and height in pixels by ticks of life left. */
const Uint8 ElementGlowSparkSizes[] = {1, 2, 3, 4, 4, 4, 4};

/* The ring's four 32 by 32 ticks, after the sparks' 78 bytes. */
const Sint32 ElementGlowRingPictures[] = {
	78,
	78 + 1 * 32 * 32,
	78 + 2 * 32 * 32,
	78 + 3 * 32 * 32,
};
