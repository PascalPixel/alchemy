#ifndef ALCHEMY_DEBUG_H
#define ALCHEMY_DEBUG_H

/// @file
/// DEBUG

#include "types.h"
#include "edition.h"

s32 Scene_ApplyEntrance(s32);
s32 Field_PlaceAtEntrance(void);

/// The palette editor's swatch tiles, compressed; it decompresses them to
/// tile 208 of character block 0, one tile a colour.
extern const u8 gDebug_PaletteSwatchTiles[];

#endif
