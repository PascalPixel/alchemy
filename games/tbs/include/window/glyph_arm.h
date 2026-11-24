#ifndef ALCHEMY_GLYPH_ARM_H
#define ALCHEMY_GLYPH_ARM_H

/// @file
/// The ARM routines copied to RAM to run: a character drawn into a text
/// window's canvas, and the screen transition's window edges read from the
/// windows' borders (window/glyph_arm.c).

#include "types.h"

struct Window;

s32 Glyph_DrawArm(struct Window *window, u32 character, u32 x, u32 y,
                  const u8 *font);
void Tile_BuildCellTiles(u16 *scanlineBounds, u16 *tilemap);

#endif
