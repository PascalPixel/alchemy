/// @file
/// Map terrain: the height curve of each cell kind and the world map's terrain
/// behaviour.

#include "unit/field/map_grid_data.h"
#include "unit/field/field.h"
#include "field.h"
#include "types.h"

/// Terrain height functions, by enum TerrainKind, called with (the remaining
/// samples, x within the cell, y within the cell). Each reads only the
/// arguments it needs. Read by MapGrid_GetTerrainHeight and
/// MapGrid_CheckCollision (terrain.c).
const TerrainHeightFn gMapGrid_TerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
    [TERRAIN_FLAT] = Terrain_GetFirstSampleA,
    [TERRAIN_SLOPE_X] = Terrain_LerpTwoSamples,
    [TERRAIN_SLOPE_Y] = Terrain_LerpTwoSamplesB,
    [TERRAIN_STEP_SUM_DIAGONAL] = Terrain_StepAtSummedPosition,
    [TERRAIN_STEP_DIFFERENCE_DIAGONAL] = Terrain_StepAtDifferencePosition,
    [TERRAIN_BEND_SUM_DIAGONAL] = Terrain_LerpThreeSamplesByRatio,
    [TERRAIN_BEND_DIFFERENCE_DIAGONAL] = Terrain_LerpThreeSamplesAtDifference,
    [TERRAIN_ROUNDED_SQUARE] = Terrain_LerpTwoSamplesByTable,
    [TERRAIN_STEP_X] = Terrain_StepAtHalf,
    [TERRAIN_STEP_Y] = Terrain_StepAtHalfB,
    [TERRAIN_BEND_X] = Terrain_LerpThreeSamplesAtHalf,
    [TERRAIN_BEND_Y] = Terrain_LerpThreeSamplesAtHalfB,
    [TERRAIN_SAMPLE_TABLE] = Terrain_LookupSampleByTable,
    [TERRAIN_SAMPLE_TABLE_MIRRORED] = Terrain_LookupSampleByTableReversed,
    [TERRAIN_FLAT_B] = Terrain_GetFirstSampleB,
    [TERRAIN_FLAT_C] = Terrain_GetFirstSampleC,
};

/// World map terrain behaviour (enum BattleEnvironment), indexed by the
/// collision value plus 0x10 when the block word's top bit is set, or plus
/// 0x20 (whatever that bit) for WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD. Read only
/// by WorldMap_GetBattleEnvironment.
// clang-format off
const u8 gWorldMap_BattleEnvironments[48] = {
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
