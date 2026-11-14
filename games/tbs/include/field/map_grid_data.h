#ifndef ALCHEMY_MAP_GRID_DATA_H
#define ALCHEMY_MAP_GRID_DATA_H

#include "types.h"
#include "field/field.h"

/// Height samples and pixel coordinates within a 16x16 map cell.
typedef s32 (*TerrainHeightFn)(const u8 *heightBytes, s32 pixelX, s32 pixelY);

extern const TerrainHeightFn
    gMapGrid_TerrainHeightFunctions[TERRAIN_KIND_COUNT];
extern const s8 gWorldMap_DirectionOffsets[8][2];
extern const u32 gWorldMap_UnitScale[2];

#endif
