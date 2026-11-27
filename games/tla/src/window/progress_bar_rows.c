/* The saving screen's progress bar, laid out by step: its own file, between the
   progress bar's tiles and the word-aligned tables after it. */

#include "types.h"

/* The progress bar's tiles (progress.png), from its left end to its right: the
   empty bar in green, the filled part in yellow. */
enum {
	BAR_LEFT_EMPTY = 0,
	BAR_EMPTY = 1,
	BAR_RIGHT_EMPTY = 2,
	BAR_LEFT_FULL = 3,
	BAR_FRONT = 4, /* where the yellow ends */
	BAR_FULL = 5,
	BAR_RIGHT_FULL = 7,
};

/* clang-format off */
/* The saving screen's progress bar by step, a row of its 12 tiles for each of */
/* its seven steps. */
const Uint8 WindowProgressBarRows[7 * 12] = {
	BAR_LEFT_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_RIGHT_EMPTY,
	BAR_LEFT_FULL, BAR_FULL, BAR_FRONT, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_RIGHT_EMPTY,
	BAR_LEFT_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FRONT, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_RIGHT_EMPTY,
	BAR_LEFT_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FRONT, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_EMPTY, BAR_RIGHT_EMPTY,
	BAR_LEFT_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FRONT, BAR_EMPTY, BAR_EMPTY, BAR_RIGHT_EMPTY,
	BAR_LEFT_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FRONT, BAR_RIGHT_EMPTY,
	BAR_LEFT_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_FULL, BAR_RIGHT_FULL,
};
/* clang-format on */
