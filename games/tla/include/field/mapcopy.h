#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

/// @file
/// Copying rectangles of map cells and their cell tiles.

#include "edition.h"
#include "types.h"

void MapGrid_CopyCellTilesIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                      u32 width, u32 height);
void Map_CopyCellsRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                       s32 dstY);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY);

#endif
