/// @file
/// Map ground: the height curve of each cell kind and the world map's battle
/// environments.

#include "field/map_grid_data.h"
#include "field/field.h"

/// Ground height functions, by enum TerrainKind, called with (the remaining
/// samples, x within the cell, y within the cell).
const TerrainHeightFn gMapGrid_TerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
    [TERRAIN_FLAT] = Terrain_GetFlatHeight,
    [TERRAIN_SLOPE_X] = Terrain_GetSlopeXHeight,
    [TERRAIN_SLOPE_Y] = Terrain_GetSlopeYHeight,
    [TERRAIN_STEP_SUM_DIAGONAL] = Terrain_GetStepSumDiagonalHeight,
    [TERRAIN_STEP_DIFFERENCE_DIAGONAL] =
        Terrain_GetStepDifferenceDiagonalHeight,
    [TERRAIN_BEND_SUM_DIAGONAL] = Terrain_GetBendSumDiagonalHeight,
    [TERRAIN_BEND_DIFFERENCE_DIAGONAL] =
        Terrain_GetBendDifferenceDiagonalHeight,
    [TERRAIN_ROUNDED_SQUARE] = Terrain_GetRoundedSquareHeight,
    [TERRAIN_STEP_X] = Terrain_GetStepXHeight,
    [TERRAIN_STEP_Y] = Terrain_GetStepYHeight,
    [TERRAIN_BEND_X] = Terrain_GetBendXHeight,
    [TERRAIN_BEND_Y] = Terrain_GetBendYHeight,
    [TERRAIN_SAMPLE_TABLE] = Terrain_GetSampleTableHeight,
    [TERRAIN_SAMPLE_TABLE_MIRRORED] = Terrain_GetSampleTableMirroredHeight,
    [TERRAIN_FLAT_B] = Terrain_GetFlatBHeight,
    [TERRAIN_FLAT_C] = Terrain_GetFlatCHeight,
};

#define ENV(name) BATTLE_ENVIRONMENT_##name

/// World map battle environments (enum BattleEnvironment), indexed by the
/// collision value plus 16 when the block word's top bit is set, or plus 32
/// (whatever that bit) for WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD.
// clang-format off
const u8 gWorldMap_BattleEnvironments[48] = {
    // Collision values 0 to 15.
    ENV(NONE), ENV(NONE), ENV(NONE), ENV(NONE),
    ENV(NONE), ENV(GRASSLAND), ENV(SHALLOWS), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(FOREST), ENV(GRASSLAND), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
    // The same, where the block word's top bit is set.
    ENV(NONE), ENV(NONE), ENV(NONE), ENV(NONE),
    ENV(NONE), ENV(BEACH), ENV(SHALLOWS), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(SNOW_FOREST), ENV(GRASSLAND), ENV(GRASSLAND),
    ENV(GRASSLAND), ENV(GRASSLAND), ENV(NONE), ENV(NONE),
    // The same, in the snowfield.
    ENV(NONE), ENV(NONE), ENV(NONE), ENV(NONE),
    ENV(NONE), ENV(SNOWFIELD), ENV(SNOWFIELD), ENV(SNOWFIELD),
    ENV(SNOWFIELD), ENV(SNOW_FOREST), ENV(SNOWFIELD), ENV(SNOWFIELD),
    ENV(SNOWFIELD), ENV(SNOWFIELD), ENV(NONE), ENV(NONE),
};
#undef ENV
// clang-format on

/// The eight compass directions as (x, y) offsets of radius 40. No code in
/// The Broken Seal names it.
const s8 gWorldMap_DirectionOffsets[8][2] = {
    {0, -40}, {28, -28}, {40, 0},  {28, 28},
    {0, 40},  {-28, 28}, {-40, 0}, {-28, -28},
};

/// Two 1.0s (16.16). No code in The Broken Seal names it; The Lost Age's
/// battle effects read its copy as a unit scale.
const u32 gWorldMap_UnitScale[2] = {
    FIXED_ONE,
    FIXED_ONE,
};
