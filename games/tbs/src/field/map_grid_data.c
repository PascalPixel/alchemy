/// @file
/// Map terrain: the height curve of each cell kind and the world map's terrain
/// behaviour.

#include "unit/field/map_grid_data.h"
#include "unit/field/field.h"
#include "field.h"
#include "types.h"

/// Terrain height functions, by enum TerrainKind, called with (the remaining
/// samples, x within the cell, y within the cell). Each reads only the
/// arguments it needs. Read by Map_GetTerrainHeight and Map_CheckObjectStep
/// (terrain.c).
const TerrainHeightFn gMap_TerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
    [TERRAIN_FLAT] = Curve_GetFirstSampleA,
    [TERRAIN_SLOPE_X] = Curve_LerpTwoSamples,
    [TERRAIN_SLOPE_Y] = Curve_LerpTwoSamplesB,
    [TERRAIN_STEP_SUM_DIAGONAL] = Curve_StepAtSummedPosition,
    [TERRAIN_STEP_DIFFERENCE_DIAGONAL] = Curve_StepAtDifferencePosition,
    [TERRAIN_BEND_SUM_DIAGONAL] = Curve_LerpThreeSamplesByRatio,
    [TERRAIN_BEND_DIFFERENCE_DIAGONAL] = Curve_LerpThreeSamplesAtDifference,
    [TERRAIN_ROUNDED_SQUARE] = Curve_LerpTwoSamplesByTable,
    [TERRAIN_STEP_X] = Curve_StepAtHalf,
    [TERRAIN_STEP_Y] = Curve_StepAtHalfB,
    [TERRAIN_BEND_X] = Curve_LerpThreeSamplesAtHalf,
    [TERRAIN_BEND_Y] = Curve_LerpThreeSamplesAtHalfB,
    [TERRAIN_SAMPLE_TABLE] = Curve_LookupSampleByTable,
    [TERRAIN_SAMPLE_TABLE_MIRRORED] = Curve_LookupSampleByTableReversed,
    [TERRAIN_FLAT_B] = Curve_GetFirstSampleB,
    [TERRAIN_FLAT_C] = Curve_GetFirstSampleC,
};

/// World map terrain behaviour (enum WorldMapTerrainBehavior), indexed by the
/// collision value plus 0x10 when the block word's top bit is set, or plus
/// 0x20 (whatever that bit) for WORLD_MAP_REGION_SNOWFIELD. Read only by
/// GetWorldMapTerrainBehavior.
// clang-format off
const u8 gWorldMap_TerrainBehaviorTable[48] = {
    0, 0, 0, 0, 0, 1, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 4, 7, 1, 1, 6, 1, 1, 1, 1, 0, 0, // 0x10
    0, 0, 0, 0, 0, 5, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0, // 0x20: the snowfield
};
// clang-format on

/// The eight compass directions as (x, y) offsets of radius 40. No code in
/// The Broken Seal names it.
const s8 gWorldMap_DirectionOffsets[8][2] = {
    {0, -40}, {28, -28}, {40, 0},  {28, 28},
    {0, 40},  {-28, 28}, {-40, 0}, {-28, -28},
};

/// Two 1.0s (Q16). No code in The Broken Seal names it; The Lost Age's
/// battle effects read its copy as a unit scale.
const u32 gWorldMap_UnitScale[2] = {
    0x10000,
    0x10000,
};
