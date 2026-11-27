#ifndef ALCHEMY_WINDOW_GLYPH_ARM_H
#define ALCHEMY_WINDOW_GLYPH_ARM_H

/* The text canvas renderer, run from RAM as ARM code (glyph_arm.c). */

#include "types.h"

struct WINDOW;

/* Draws one character into a window's text canvas at pixel (x, y) of its
   interior, from `font` or the main font when it is NULL, and returns how far
   the text advances. */
Sint32 mnGlyphDrawArm(struct WINDOW *window, Uint32 character, Uint32 x,
	Uint32 y, const Uint8 *font);

#endif
