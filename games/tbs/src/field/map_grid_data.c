// Map ground: the height curve of each metatile kind and the world map's battle environments.

#include "field/map_grid_data.h"

// Ground height functions, by TERRAIN_KIND, called with (the remaining samples, x within the
// metatile, y within the metatile).
const TERRAIN_HEIGHT_FN MapGridTerrainHeightFunctions[TERRAIN_KIND_COUNT] = {
    [TERRAIN_FLAT]                     = fdTerrainGetFlatHeight,
    [TERRAIN_SLOPE_X]                  = fdTerrainGetSlopeXHeight,
    [TERRAIN_SLOPE_Y]                  = fdTerrainGetSlopeYHeight,
    [TERRAIN_STEP_SUM_DIAGONAL]        = fdTerrainGetStepSumDiagonalHeight,
    [TERRAIN_STEP_DIFFERENCE_DIAGONAL] = fdTerrainGetStepDifferenceDiagonalHeight,
    [TERRAIN_BEND_SUM_DIAGONAL]        = fdTerrainGetBendSumDiagonalHeight,
    [TERRAIN_BEND_DIFFERENCE_DIAGONAL] = fdTerrainGetBendDifferenceDiagonalHeight,
    [TERRAIN_ROUNDED_SQUARE]           = fdTerrainGetRoundedSquareHeight,
    [TERRAIN_STEP_X]                   = fdTerrainGetStepXHeight,
    [TERRAIN_STEP_Y]                   = fdTerrainGetStepYHeight,
    [TERRAIN_BEND_X]                   = fdTerrainGetBendXHeight,
    [TERRAIN_BEND_Y]                   = fdTerrainGetBendYHeight,
    [TERRAIN_SAMPLE_TABLE]             = fdTerrainGetSampleTableHeight,
    [TERRAIN_SAMPLE_TABLE_MIRRORED]    = fdTerrainGetSampleTableMirroredHeight,
    [TERRAIN_FLAT_B]                   = fdTerrainGetFlatBHeight,
    [TERRAIN_FLAT_C]                   = fdTerrainGetFlatCHeight,
};

#define ENV(name) BATTLE_ENVIRONMENT_##name

// World map battle environments (BATTLE_ENVIRONMENT), indexed by the collision value plus 16 when
// the block word's top bit is set, or plus 32 (whatever that bit) for
// WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD.
// clang-format off
const Uint8 WorldMapBattleEnvironments[48] = {
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

// The eight compass directions as (x, y) offsets of radius 40. No code in The Broken Seal names it.
const Sint8 WorldMapDirectionOffsets[8][2] = {
    {0,   -40},
    {28,  -28},
    {40,  0  },
    {28,  28 },
    {0,   40 },
    {-28, 28 },
    {-40, 0  },
    {-28, -28},
};

// Two 1.0s (16.16). No code in The Broken Seal names it; The Lost Age's battle effects read its
// copy as a unit scale.
const Uint32 WorldMapUnitScale[2] = {
    toFIXED(1),
    toFIXED(1),
};
