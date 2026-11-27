#ifndef ALCHEMY_EFFECT_SPRITE_H
#define ALCHEMY_EFFECT_SPRITE_H

/* The sprites a field effect draws itself, placed, scaled and turned. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "graphics/render_input.h"

/* A field effect's sprite, drawn by fdEffectSprDraw: an OAM entry linked into
   the bucket of its depth, with the screen position (16.16), scale (16.16) and
   angle it is placed from. fdEffectSprInit fills one; a depth of -1 hides it.
   The pivot is the pixel of the picture that sits on the position. */
typedef struct EFFECT_SPR {
	struct EFFECT_SPR *next;
	OAM_DATA		   oam;
	FIXED			   x;
	FIXED			   y;
	FIXED			   scaleX;
	FIXED			   scaleY;
	Uint16			   ang;
	Sint16			   depth;
	Uint16			   width;
	Uint16			   height;
	Sint16			   pivotX;
	Sint16			   pivotY;
} EFFECT_SPR;

STATIC_ASSERT(sizeof(EFFECT_SPR) == 40);
STATIC_ASSERT(offsetof(EFFECT_SPR, x) == 12);
STATIC_ASSERT(offsetof(EFFECT_SPR, ang) == 28);
STATIC_ASSERT(offsetof(EFFECT_SPR, pivotX) == 36);

/* An EFFECT_SPR depth that leaves the sprite undrawn. */
#define EFFECT_SPRITE_HIDDEN (-1)

#endif
