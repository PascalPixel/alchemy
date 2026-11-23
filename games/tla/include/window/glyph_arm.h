#ifndef ALCHEMY_WINDOW_GLYPH_ARM_H
#define ALCHEMY_WINDOW_GLYPH_ARM_H

/// @file
/// The text canvas renderer, run from RAM as ARM code (glyph_arm.c).

#include "types.h"

struct Window;

/// Draws one character into a window's text canvas at pixel (x, y) of its
/// interior, from `font` or the main font when it is NULL, and returns how far
/// the text advances.
s32 Glyph_DrawArm(struct Window *window, u32 character, u32 x, u32 y,
                  const u8 *font);

#endif
