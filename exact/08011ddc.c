#include "curve.h"

s32 Func_08011ddc(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 third;
    s32 difference;
    s32 delta;

    first = *samples++ << CURVE_VALUE_SHIFT;
    second = samples[0] << CURVE_VALUE_SHIFT;
    third = samples[1];
    difference = end - start;
    start = difference + CURVE_FULL_STEPS - 1;
    third <<= CURVE_VALUE_SHIFT;

    if (start == CURVE_FULL_STEPS - 1)
        return second;

    if ((u32)start < CURVE_FULL_STEPS - 1) {
        delta = second - first;
        difference = delta * start;
        if (difference < 0)
            difference += CURVE_FULL_STEPS - 1;
        difference >>= 4;
        return first + difference;
    } else {
        difference *= third - second;
        if (difference < 0)
            difference += CURVE_FULL_STEPS - 1;
        difference >>= 4;
        return second + difference;
    }
}
