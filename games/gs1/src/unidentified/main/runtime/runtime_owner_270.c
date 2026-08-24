#include "curve.h"

s32 Func_08011d34(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 maximum;
    u32 position;

    first = samples[0] << CURVE_VALUE_SHIFT;
    second = samples[1] << CURVE_VALUE_SHIFT;
    maximum = first;
    if (second > first) {
        maximum = second;
    }
    position = start + end;
    if (position == CURVE_FULL_STEPS - 1) {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1) {
        second = first;
    }
    return second;
}
