#ifndef ALCHEMY_MAP_GRID_DATA_H
#define ALCHEMY_MAP_GRID_DATA_H

/// @file
/// The ground height function of each terrain kind, and the World Map's
/// small tables beside it (map_grid_data.c).

#include "types.h"
#include "field/field.h"

/// A terrain kind's ground height: from a cell's height samples and a pixel
/// (pixelX, pixelY) within the 16 by 16 cell, the height in 16.16 pixels.
typedef s32 (*TerrainHeightFn)(const u8 *heightBytes, s32 pixelX, s32 pixelY);

extern const TerrainHeightFn
    gMapGrid_TerrainHeightFunctions[TERRAIN_KIND_COUNT];
extern const s8 gWorldMap_DirectionOffsets[8][2];
extern const u32 gWorldMap_UnitScale[2];

#endif
