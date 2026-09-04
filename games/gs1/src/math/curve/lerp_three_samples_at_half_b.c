#include "curve.h"

#define Curve_LerpThreeSamplesAtHalfB Func_08011ed0

s32 Curve_LerpThreeSamplesAtHalfB(u8 *samples, u32 unused, u32 position)
{
    s32 a;
    s32 b;
    s32 pos;
    s32 delta;
    s32 tail;

    a = *samples++ << CURVE_VALUE_SHIFT;
    b = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        delta = (b - a) * position;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return a + (delta >> 3);
    }

    a = *samples << CURVE_VALUE_SHIFT;
    pos = position;
    a -= b;
    pos -= CURVE_HALF_STEPS;
    tail = a * pos;
    if (tail < 0) {
        tail += CURVE_HALF_STEPS - 1;
    }
    return b + (tail >> 3);
}
