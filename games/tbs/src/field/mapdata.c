#include "unit/field/mapdata.h"
#include "unit/field/field.h"
#include "field.h"
#include "types.h"

// Map terrain: the height curve of each cell kind and the world map's terrain
// behaviour.

/// Terrain height functions, chosen by the low four bits of a cell's first
/// collision byte and called with (the remaining samples, x within the cell,
/// y within the cell). Each reads only the arguments it needs.
const TerrainHeightFn gMap_TerrainHeightFunctions[16] = {
    Curve_GetFirstSampleA,
    Curve_LerpTwoSamples,
    Curve_LerpTwoSamplesB,
    Curve_StepAtSummedPosition,
    Curve_StepAtDifferencePosition,
    Curve_LerpThreeSamplesByRatio,
    Curve_LerpThreeSamplesAtDifference,
    Curve_LerpTwoSamplesByTable,
    Curve_StepAtHalf,
    Curve_StepAtHalfB,
    Curve_LerpThreeSamplesAtHalf,
    Curve_LerpThreeSamplesAtHalfB,
    Curve_LookupSampleByTable,
    Curve_LookupSampleByTableReversed,
    Curve_GetFirstSampleB,
    Curve_GetFirstSampleC,
};

/// World map terrain behaviour, indexed by the collision value plus 0x10 when
/// the block word's top bit is set, or plus 0x20 (whatever that bit) for
/// terrain kind 21.
// clang-format off
const u8 gWorldMap_TerrainBehaviorTable[48] = {
    0, 0, 0, 0, 0, 1, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 4, 7, 1, 1, 6, 1, 1, 1, 1, 0, 0, // 0x10
    0, 0, 0, 0, 0, 5, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0, // 0x20: terrain kind 21
};
// clang-format on

/// The eight compass directions as (x, y) offsets of radius 40.
const s8 gWorldMap_DirectionOffsets[8][2] = {
    {0, -40}, {28, -28}, {40, 0},  {28, 28},
    {0, 40},  {-28, 28}, {-40, 0}, {-28, -28},
};

/// Two 1.0s (Q16); battle effects read them as a unit scale.
const u32 gWorldMap_UnitScale[2] = {
    0x10000,
    0x10000,
};
