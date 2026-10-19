#ifndef ALCHEMY_UNIT_FIELD_MAPDATA_H
#define ALCHEMY_UNIT_FIELD_MAPDATA_H

#include "types.h"

/// Height samples and pixel coordinates within a 16x16 terrain cell.
typedef s32 (*TerrainHeightFn)(const u8 *heightBytes, s32 pixelX, s32 pixelY);

extern const TerrainHeightFn gMap_TerrainHeightFunctions[16];
extern const s8 gWorldMap_DirectionOffsets[8][2];
extern const u32 gWorldMap_UnitScale[2];

#endif
