#ifndef ALCHEMY_RENDER_PLACEMENT_H
#define ALCHEMY_RENDER_PLACEMENT_H

/* @file */
/* Where a sprite is drawn and at what scale. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/* A sprite's screen position and its two vertical offsets, in 16.16 pixels. */
typedef struct SPR_PLACEMENT {
#ifdef UBFIX
	union {
		struct {
			FIXED x;
			FIXED height;
			FIXED y;
			FIXED groundHeight;
		};
		FIXED values[4];
	};
#else
	FIXED x;
	FIXED height;
	FIXED y;
	FIXED groundHeight;
#endif
} SPR_PLACEMENT;

/* Returns the placement's values as a writable array. */
static inline s32 *sySprPlacementValues(SPR_PLACEMENT *placement) {
#ifdef UBFIX
	return placement->values;
#else
	return &placement->x;
#endif
}

/* Horizontal and vertical sprite scale; FIXED_ONE is the original size. */
typedef struct SPR_SCALE {
	union {
		struct {
			FIXED x; /* 16.16 */
			FIXED y; /* 16.16 */
		};
		s32 values[2];
	};
} SPR_SCALE;

STATIC_ASSERT(sizeof(SPR_PLACEMENT) == 16);
STATIC_ASSERT(offsetof(SPR_PLACEMENT, height) == 4);
STATIC_ASSERT(offsetof(SPR_PLACEMENT, y) == 8);
STATIC_ASSERT(offsetof(SPR_PLACEMENT, groundHeight) == 12);
STATIC_ASSERT(sizeof(SPR_SCALE) == 8);
STATIC_ASSERT(offsetof(SPR_SCALE, y) == 4);

#endif
