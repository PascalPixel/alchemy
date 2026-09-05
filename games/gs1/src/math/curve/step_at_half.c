#include "curve.h"

#define Curve_StepAtHalf Func_08011e50

s32 Curve_StepAtHalf(const s8 *samples, u32 position)
{
    s32 value;
    if (position < CURVE_HALF_STEPS)
        value = samples[0];
    else
        value = samples[1];
    return value << CURVE_VALUE_SHIFT;
}
