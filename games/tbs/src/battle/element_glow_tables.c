/* Where the element glow's sparks and rings sit on their sprite sheet. */

#include "battle/battle.h"

/* Byte offsets into SPARK_STATE.sheet, a byte a pixel, by ticks of life
   left. */
Sint32 ElementGlowSparkPictures[] = {0, 1, 5, 14, 30, 46, 62};

/* The spark's width and height in pixels by ticks of life left; the 1-pixel
   entry, for none left, is never drawn. */
Uint8 ElementGlowSparkSizes[] = {1, 2, 3, 4, 4, 4, 4};

/* The ring's four 32 by 32 ticks, after the sparks' 78 bytes. */
Sint32 ElementGlowRingPictures[] = {
	78,
	78 + 1 * 32 * 32,
	78 + 2 * 32 * 32,
	78 + 3 * 32 * 32,
};

/* Unused: four words after the ring's ticks that no code names. */
Sint32 ElementGlowUnusedWords[] = {0, 10, 20, 0};
