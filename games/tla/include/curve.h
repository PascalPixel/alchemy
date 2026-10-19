#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

#include "types.h"

extern u8 gCurve_LerpWeightTable[];
extern u8 gCurve_SampleIndexTable[];

enum {
  /// Turns a height sample, in 8-pixel units, into 16.16 fixed-point pixels.
  CURVE_VALUE_SHIFT = 19,
  CURVE_HALF_STEPS = 8,  ///< pixels to the middle of a 16-pixel cell
  CURVE_FULL_STEPS = 16, ///< pixels across a cell
};

s32 Curve_GetFirstSampleA(const s8 *samples);
s32 Curve_LerpTwoSamples(const s8 *samples, s32 position);
s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 position);
s32 Curve_StepAtSummedPosition(const s8 *samples, s32 start, s32 end);
s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row);

#endif
