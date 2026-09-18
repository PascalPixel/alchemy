#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

#include "TYPES.H"

enum {
    CURVE_VALUE_SHIFT = 19,
    CURVE_HALF_STEPS = 8,
    CURVE_FULL_STEPS = 16,
};

s32 Curve_GetFirstSampleA(const s8 *samples);
s32 Curve_LerpTwoSamples(const s8 *samples, s32 position);
s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 position);
s32 Curve_StepAtSummedPosition(const s8 *samples, s32 start, s32 end);

#endif
