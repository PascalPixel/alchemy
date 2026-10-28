#ifndef ALCHEMY_MODULE_WINDOW_GLYPH_ARM_H
#define ALCHEMY_MODULE_WINDOW_GLYPH_ARM_H

#include "types.h"

struct Window;

s32 Glyph_DrawArm(struct Window *window, u32 character, u32 x, u32 y,
                  const u8 *font);
void Tile_BuildCellPictures(u16 *scanlineBounds, u16 *tilemap);

#endif
