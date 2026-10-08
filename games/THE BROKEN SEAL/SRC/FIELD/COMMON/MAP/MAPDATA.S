	.section .rodata
@ 地形の高さ関数
	.global Map_TerrainHeightFunctions
Map_TerrainHeightFunctions:
	.4byte Curve_GetFirstSampleA
	.4byte Curve_LerpTwoSamples
	.4byte Curve_LerpTwoSamplesB
	.4byte Curve_StepAtSummedPosition
	.4byte Curve_StepAtDifferencePosition
	.4byte Curve_LerpThreeSamplesByRatio
	.4byte Curve_LerpThreeSamplesAtDifference
	.4byte Curve_LerpTwoSamplesByTable
	.4byte Curve_StepAtHalf
	.4byte Curve_StepAtHalfB
	.4byte Curve_LerpThreeSamplesAtHalf
	.4byte Curve_LerpThreeSamplesAtHalfB
	.4byte Curve_LookupSampleByTable
	.4byte Curve_LookupSampleByTableReversed
	.4byte Curve_GetFirstSampleB
	.4byte Curve_GetFirstSampleC

@ 地形の振る舞い
	.global WorldMap_TerrainBehaviorTable
WorldMap_TerrainBehaviorTable:
	.byte 0, 0, 0, 0, 0, 1, 7, 1, 1, 2, 1, 1, 1, 1, 0, 0
	.byte 0, 0, 0, 0, 0, 4, 7, 1, 1, 6, 1, 1, 1, 1, 0, 0
	.byte 0, 0, 0, 0, 0, 5, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0

@ 半径 40 の 8 方向
	.global WorldMap_DirectionOffsets
WorldMap_DirectionOffsets:
	.byte 0, -40
	.byte 28, -28
	.byte 40, 0
	.byte 28, 28
	.byte 0, 40
	.byte -28, 28
	.byte -40, 0
	.byte -28, -28

	.global WorldMap_UnitScale
WorldMap_UnitScale:
	.4byte 0x10000, 0x10000
