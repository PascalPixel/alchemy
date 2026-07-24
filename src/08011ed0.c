typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08011ed0(u8 *samples, u32 unused, u32 position)
{
    s32 scaledEndpoint;
    s32 scaledMiddle;
    s32 segmentPosition;
    s32 weightedDelta;
    s32 tailDelta;

    scaledEndpoint = *samples++ << 19;
    scaledMiddle = *samples++ << 19;
    if (position <= 7U) {
        weightedDelta = (scaledMiddle - scaledEndpoint) * position;
        if (weightedDelta < 0) {
            weightedDelta += 7;
        }
        return scaledEndpoint + (weightedDelta >> 3);
    }

    scaledEndpoint = *samples << 19;
    segmentPosition = position;
    scaledEndpoint -= scaledMiddle;
    segmentPosition -= 8;
    tailDelta = scaledEndpoint * segmentPosition;
    if (tailDelta < 0) {
        tailDelta += 7;
    }
    return scaledMiddle + (tailDelta >> 3);
}
