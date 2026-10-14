#include "unit/field/mapdata.h"
#include "unit/field/field.h"
#include "field.h"
#include "types.h"

//--------------------------------------------------------------------------
// 地形の高さと振る舞い
// Map terrain: the height curve of each cell kind and the world map's terrain behaviour.
//--------------------------------------------------------------------------

// 地形の高さ関数: セルの当たりの最初のバイトの下位 4 ビットで選び、
// (残りの標本, セル内の x, セル内の y) で呼ぶ。それぞれ要る引数だけを読む。
const TerrainHeightFn Map_TerrainHeightFunctions[16] =
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
	Curve_LerpThreeSamplesAtHalf,
	Curve_LerpThreeSamplesAtHalfB,
	Curve_LookupSampleByTable,
	Curve_LookupSampleByTableReversed,
	Curve_GetFirstSampleB,
	Curve_GetFirstSampleC,
};

// ワールドマップの地形の振る舞い: 当たりの値に、ブロックの語の
// 最上位ビットが立っていれば 0x10、地形の種類が 21 なら 0x20 を足して引く
const u8 WorldMap_TerrainBehaviorTable[48] =
{
	0, 0, 0, 0, 0, 1, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 4, 7, 1, 1, 6, 1, 1, 1, 1, 0, 0, // 0x10
	0, 0, 0, 0, 0, 5, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0, // 0x20 種類 21
};

// 半径 40 の 8 方向 (x, y)
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

// 1.0 (Q16) が 2 つ
const u32 WorldMap_UnitScale[2] =
{
	0x10000, 0x10000,
};
