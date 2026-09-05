#include "curve.h"

#define Curve_StepAtHalfB Func_08011e6c

s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 position)
{
    s32 value;
    if (position < CURVE_HALF_STEPS)
        value = samples[0];
    else
        value = samples[1];
    return value << CURVE_VALUE_SHIFT;
}
