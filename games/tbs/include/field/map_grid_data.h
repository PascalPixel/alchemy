#ifndef ALCHEMY_MAP_GRID_DATA_H
#define ALCHEMY_MAP_GRID_DATA_H

// The ground height function of each terrain kind, and the World Map's small tables beside it
// (map_grid_data.c).

#include "types.h"
#include "field/field.h"

// A terrain kind's ground height: from a metatile's height samples and a pixel (pixelX, pixelY)
// within the 16 by 16 metatile, the height in 16.16 pixels.
typedef Sint32 (*TERRAIN_HEIGHT_FN)(const Uint8 *heightBytes, Sint32 pixelX, Sint32 pixelY);

extern const TERRAIN_HEIGHT_FN MapGridTerrainHeightFunctions[TERRAIN_KIND_COUNT];
extern const Sint8             WorldMapDirectionOffsets[8][2];
extern const Uint32            WorldMapUnitScale[2];

#endif
