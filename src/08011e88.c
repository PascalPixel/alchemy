typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08011e88(s8 *samples, u32 position)
{
    s32 scaledEndpoint;
    s32 scaledMiddle;
    s32 weightedDelta;

    scaledEndpoint = *samples++ << 19;
    scaledMiddle = *samples++ << 19;
    if (position <= 7U) {
        weightedDelta = (scaledMiddle - scaledEndpoint) * position;
        if (weightedDelta < 0) {
            weightedDelta += 7;
        }
        return scaledEndpoint + (weightedDelta >> 3);
    }

    {
        s32 segmentPosition;

        scaledEndpoint = *samples << 19;
        segmentPosition = position;
        scaledEndpoint -= scaledMiddle;
        segmentPosition -= 8;
        weightedDelta = scaledEndpoint * segmentPosition;
        if (weightedDelta < 0) {
            weightedDelta += 7;
        }
        return scaledMiddle + (weightedDelta >> 3);
    }
}
