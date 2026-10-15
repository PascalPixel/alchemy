#include "types.h"

//--------------------------------------------------------------------------
// Map terrain: the height curve of each cell kind, the world map's
// terrain behaviour, and the small tables after them.
//--------------------------------------------------------------------------

// Each height function reads only the arguments it needs, so the type
// leaves them unspecified.
typedef s32 (*TerrainHeightFn)();

s32 Curve_GetFirstSampleA();
s32 Curve_LerpTwoSamples();
s32 Curve_LerpTwoSamplesB();
s32 Curve_StepAtSummedPosition();
s32 Curve_StepAtDifferencePosition();
s32 Curve_LerpThreeSamplesByRatio();
s32 Curve_LerpThreeSamplesAtDifference();
s32 Curve_LerpTwoSamplesByTable();
s32 Curve_StepAtHalf();
s32 Curve_StepAtHalfB();
s32 Func_0802d3c4();
s32 Func_0802d3d8();
s32 Curve_LookupSampleByTable();
s32 Curve_LookupSampleByTableReversed();
s32 Func_0802d414();
s32 Func_0802d438();

// The height functions: the low four bits of a cell's first collision
// byte choose one, called with (the remaining samples, x and z in the cell).
const TerrainHeightFn Map_TerrainHeightHandlers[16] =
{
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
    Func_0802d3c4,
    Func_0802d3d8,
    Curve_LookupSampleByTable,
    Curve_LookupSampleByTableReversed,
    Func_0802d414,
    Func_0802d438,
};

// The world map's terrain behaviour, read at a selector plus a row: 0x10
// when the block word's top bit is set, 0x20 for terrain kinds above 40,
// 0x30 for kinds 12 and 15 and 0x40 for kind 36.
const u8 WorldMap_TerrainBehaviorTable[80] =
{
    0, 8, 8, 8, 10, 1, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0,
    0, 8, 8, 8, 10, 4, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0, // 0x10
    0, 9, 9, 9, 9, 5, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0, // 0x20
    0, 8, 8, 8, 10, 3, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0, // 0x30
    0, 9, 9, 9, 9, 11, 11, 11, 11, 12, 11, 11, 11, 11, 0, 0, // 0x40
};

// Eight colours (BGR555).
const u16 Data_0802f054[8] =
{
    0x0000, 0x7c00, 0x03e0, 0x001f, 0x1ce7, 0x3def, 0x7fff, 0x000f,
};

// Stops for a number from 0 to 715 that steps by one or skips: a skip
// forward goes to the next stop above it, back to the last stop below.
// 0xffff ends the list.
const u16 Data_0802f064[10] =
{
    0, 16, 243, 247, 250, 490, 526, 642, 671, 0xffff,
};

// Eight directions at a radius of 40 (x, y).
const s8 WorldMap_DirectionOffsets[8][2] =
{
    {0, -40},
    {28, -28},
    {40, 0},
    {28, 28},
    {0, 40},
    {-28, 28},
    {-40, 0},
    {-28, -28},
};

// Two 1.0s (Q16).
const u32 WorldMap_UnitScale[2] =
{
    0x10000, 0x10000,
};
