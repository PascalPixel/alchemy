#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

#include "types.h"

extern u8 gTerrain_LerpWeightTable[];
extern u8 gTerrain_SampleIndexTable[];

enum {
  /// Turns a height sample, in 8-pixel units, into 16.16 fixed-point pixels.
  CURVE_VALUE_SHIFT = 19,
  CURVE_HALF_STEPS = 8,  ///< pixels to the middle of a 16-pixel cell
  CURVE_FULL_STEPS = 16, ///< pixels across a cell
};

s32 Terrain_GetFlatHeight(const s8 *samples);
s32 Terrain_GetSlopeXHeight(const s8 *samples, s32 position);
s32 Terrain_GetSlopeYHeight(const s8 *samples, s32 unused, s32 position);
s32 Terrain_GetStepSumDiagonalHeight(const s8 *samples, s32 start, s32 end);
s32 Terrain_GetSampleTableHeight(const s8 *samples, u32 position, u32 row);

#endif
