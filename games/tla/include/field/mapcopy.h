#ifndef ALCHEMY_MAPCOPY_H
#define ALCHEMY_MAPCOPY_H

/* Copying rectangles of map metatiles and their metatile tiles. */

#include "edition.h"
#include "types.h"

void fdMapGridCopyMetatileTilesIndicesRect(Sint32 srcX, Sint32 srcY,
	Uint32 dstX, Uint32 dstY, Uint32 width, Uint32 height);

#endif
