#include "curve.h"

s32 Func_08011ed0(u8 *samples, u32 unused, u32 position)
{
    s32 scaledEndpoint;
    s32 scaledMiddle;
    s32 segmentPosition;
    s32 weightedDelta;
    s32 tailDelta;

    scaledEndpoint = *samples++ << CURVE_VALUE_SHIFT;
    scaledMiddle = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        weightedDelta = (scaledMiddle - scaledEndpoint) * position;
        if (weightedDelta < 0) {
            weightedDelta += CURVE_HALF_STEPS - 1;
        }
        return scaledEndpoint + (weightedDelta >> 3);
    }

    scaledEndpoint = *samples << CURVE_VALUE_SHIFT;
    segmentPosition = position;
    scaledEndpoint -= scaledMiddle;
    segmentPosition -= CURVE_HALF_STEPS;
    tailDelta = scaledEndpoint * segmentPosition;
    if (tailDelta < 0) {
        tailDelta += CURVE_HALF_STEPS - 1;
    }
    return scaledMiddle + (tailDelta >> 3);
}
