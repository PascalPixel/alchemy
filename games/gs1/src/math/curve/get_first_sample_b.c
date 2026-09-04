#include "curve.h"

#define Curve_GetFirstSampleB Func_08011f3c

s32 Curve_GetFirstSampleB(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}
