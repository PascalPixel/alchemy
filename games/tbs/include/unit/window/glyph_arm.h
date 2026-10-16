#ifndef ALCHEMY_UNIT_WINDOW_GLYPH_ARM_H
#define ALCHEMY_UNIT_WINDOW_GLYPH_ARM_H

#include "types.h"

struct UiWindow;

s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character, u32 x, u32 y,
                        const u8 *font);
void Tile_BuildMetatiles(u16 *scanlineBounds, u16 *frameTiles);

#endif
