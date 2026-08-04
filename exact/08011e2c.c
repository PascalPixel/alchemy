#include "curve.h"

s32 Func_08011e2c(const s8 *samples, s32 position, s32 row)
{
    s32 start;

    start = samples[0] << CURVE_VALUE_SHIFT;
    return start
        + (((samples[1] << CURVE_VALUE_SHIFT) - start)
           * Data_080132fc[position + (row * CURVE_FULL_STEPS)]);
}
