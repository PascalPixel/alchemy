#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

/* @file */
/* Copying rectangles of map cells and their cell tiles. */

#include "edition.h"
#include "types.h"

void fdMapGridCopyCellTilesIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
									   u32 width, u32 height);

#endif
