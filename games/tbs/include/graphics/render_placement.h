#ifndef ALCHEMY_RENDER_PLACEMENT_H
#define ALCHEMY_RENDER_PLACEMENT_H

/* @file */
/* A sprite's screen position and scale. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* A sprite's screen position and its two vertical offsets, in 16.16 pixels. */
typedef struct SPR_PLACEMENT {
#ifdef UBFIX
	union {
		struct {
			FIXED x;	  /* screen x of the sprite's centre */
			FIXED height; /* how far the image is raised above y */
			FIXED y;	  /* screen y at the ground; sets the depth slot */
			FIXED groundHeight; /* how far the shadow is raised above y */
		};
		FIXED values[4];
	};
#else
	FIXED x;			/* screen x of the sprite's centre */
	FIXED height;		/* how far the image is raised above y */
	FIXED y;			/* screen y at the ground; sets the depth slot */
	FIXED groundHeight; /* how far the shadow is raised above y */
#endif
} SPR_PLACEMENT;

/* Returns the placement's values as an array: x, height, y, groundHeight. */
/* UB: the legacy renderer walks across the scalar members as an array. UBFIX */
/* supplies a real array; an always-on union changes producer scheduling. */
static inline const s32 *
sySprPlacementReadValues(const SPR_PLACEMENT *placement) {
#ifdef UBFIX
	return placement->values;
#else
	return &placement->x;
#endif
}

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
			FIXED x;
			FIXED y;
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
