#ifndef ALCHEMY_BLITTER_H
#define ALCHEMY_BLITTER_H

// The battle effect argument and the rectangle-drawing functions battle effects call.

#include "edition.h"
#include "types.h"

struct BATTLE_EFFECT_PARM;

// btEffectBuildBlitter's flags: which edges the built blitter clips at and how it mirrors the
// picture.
enum
{
	// Skips columns outside the canvas; without it a width mask wraps x.
	BLITTER_CLIP_X   = 1,
	BLITTER_CLIP_Y   = 2, // skips rows outside the canvas's height
	COMPOSITE_FLIP_X = 4, // mirrors the picture left to right
	COMPOSITE_FLIP_Y = 8, // mirrors the picture top to bottom
	COMPOSITE_FLIP   = COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y
};

// btEffectBuildBlitter's modes: how a picture's pixels combine with the canvas's (the fragments in
// blitter_fragments.s).
enum
{
	COMPOSITE_COPY,        // writes every pixel
	COMPOSITE_TRANSPARENT, // writes every nonzero pixel
	COMPOSITE_LIGHTEN,     // keeps the brighter of the two
	COMPOSITE_ADDITIVE     // adds the two, capped at 63
};

// A blitter btEffectBuildBlitter writes into a heap slot: it draws a width by height picture from
// source onto the destination canvas with its top left at (x, y), in pixels.
typedef Sint32 (*DRAW_RECTANGLE)(
	void *destination, const void *source, Sint32 x, Sint32 y, Sint32 width, Sint32 height);

#endif
