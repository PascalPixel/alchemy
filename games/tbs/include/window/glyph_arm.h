#ifndef ALCHEMY_GLYPH_ARM_H
#define ALCHEMY_GLYPH_ARM_H

// The ARM functions copied to RAM to run: a character drawn into a text window's canvas, and the
// screen transition's window edges read from the windows' borders (window/glyph_arm.c).

#include "types.h"

struct WINDOW;

Sint32
	 mnGlyphDrawArm(struct WINDOW *window, Uint32 character, Uint32 x, Uint32 y, const Uint8 *font);
void mnScanlineWindowBuildEdges(Uint16 *scanlineBounds, Uint16 *tilemap);

#endif
