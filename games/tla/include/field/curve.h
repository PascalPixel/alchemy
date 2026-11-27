#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

// The ground height of a map metatile from its shape's height samples, one function for each kind
// of slope, and the tables they read.

#include "fixed_math.h"
#include "types.h"

extern Uint8 TerrainRoundedSquareMask[];
// The sample fdTerrainGetSampleTableHeight reads at each position: 0, 1 or 2, split along two
// diagonals.
extern Uint8 TerrainSampleIndexTable[];

enum
{
    // Turns a height sample, in 8-pixel units, into 16.16 fixed-point pixels.
    CURVE_VALUE_SHIFT = 19,
    CURVE_HALF_STEPS  = 8,  // pixels to the middle of a 16-pixel metatile
    CURVE_FULL_STEPS  = 16, // pixels across a metatile
};

FIXED fdTerrainGetFlatHeight(const Sint8 *samples);
FIXED fdTerrainGetSlopeXHeight(const Sint8 *samples, Sint32 pos);
FIXED fdTerrainGetSlopeYHeight(const Sint8 *samples, Sint32 unused, Sint32 pos);
FIXED fdTerrainGetStepSumDiagonalHeight(const Sint8 *samples, Sint32 start, Sint32 end);
FIXED fdTerrainGetSampleTableHeight(const Sint8 *samples, Uint32 pos, Uint32 row);

#endif
