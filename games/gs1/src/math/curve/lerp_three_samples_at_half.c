#include "curve.h"

s32 Curve_LerpThreeSamplesAtHalf(s8 *samples, u32 position)
{
    s32 a;
    s32 b;
    s32 delta;

    a = *samples++ << CURVE_VALUE_SHIFT;
    b = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        delta = (b - a) * position;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return a + (delta >> 3);
    }

    {
        s32 pos;

        a = *samples << CURVE_VALUE_SHIFT;
        pos = position;
        a -= b;
        pos -= CURVE_HALF_STEPS;
        delta = a * pos;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return b + (delta >> 3);
    }
}
