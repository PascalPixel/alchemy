#include "curve.h"

s32 Func_0802d414(const s8 *samples, u32 column, u32 row)
{
    s32 value;

    if (row < CURVE_HALF_STEPS)
    {
        if (column < CURVE_HALF_STEPS)
            value = samples[0];
        else
            value = samples[1];
    }
    else
    {
        value = samples[2];
    }
    return value << CURVE_VALUE_SHIFT;
}

s32 Func_0802d438(const s8 *samples, u32 column, u32 row)
{
    s32 value;

    if (row < CURVE_HALF_STEPS)
        value = samples[0];
    else if (column < CURVE_HALF_STEPS)
        value = samples[1];
    else
        value = samples[2];
    return value << CURVE_VALUE_SHIFT;
}
