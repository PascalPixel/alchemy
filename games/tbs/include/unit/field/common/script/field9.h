#ifndef ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD9_H
#define ALCHEMY_UNIT_FIELD_COMMON_SCRIPT_FIELD9_H

#include "types.h"

s32 Curve_GetFirstSampleA(const s8 *samples);
s32 Curve_LerpTwoSamples(const s8 *samples, s32 position);
s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 position);
s32 Curve_StepAtSummedPosition(const s8 *samples, s32 start, s32 end);
s32 Curve_StepAtDifferencePosition(const s8 *samples, s32 start, s32 end);
s32 Curve_LerpThreeSamplesByRatio(const s8 *samples, s32 start, s32 end);
s32 Curve_LerpThreeSamplesAtDifference(const s8 *samples, s32 start, s32 end);
s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row);
s32 Curve_StepAtHalf(const s8 *samples, u32 position);
s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 position);
s32 Curve_LerpThreeSamplesAtHalf(s8 *samples, u32 position);
s32 Curve_LerpThreeSamplesAtHalfB(u8 *samples, u32 unused, u32 position);
s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row);
s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row);
s32 Curve_GetFirstSampleB(const s8 *samples);
s32 Curve_GetFirstSampleC(const s8 *samples);

#endif
