#include "curve.h"

s32 Func_08011e88(s8 *samples, u32 position)
{
    s32 scaledEndpoint;
    s32 scaledMiddle;
    s32 weightedDelta;

    scaledEndpoint = *samples++ << CURVE_VALUE_SHIFT;
    scaledMiddle = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        weightedDelta = (scaledMiddle - scaledEndpoint) * position;
        if (weightedDelta < 0) {
            weightedDelta += CURVE_HALF_STEPS - 1;
        }
        return scaledEndpoint + (weightedDelta >> 3);
    }

    {
        s32 segmentPosition;

        scaledEndpoint = *samples << CURVE_VALUE_SHIFT;
        segmentPosition = position;
        scaledEndpoint -= scaledMiddle;
        segmentPosition -= CURVE_HALF_STEPS;
        weightedDelta = scaledEndpoint * segmentPosition;
        if (weightedDelta < 0) {
            weightedDelta += CURVE_HALF_STEPS - 1;
        }
        return scaledMiddle + (weightedDelta >> 3);
    }
}
