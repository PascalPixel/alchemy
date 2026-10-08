#include "curve.h"

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
    return samples[
        Curve_SampleIndexTable[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
        << CURVE_VALUE_SHIFT;
}
