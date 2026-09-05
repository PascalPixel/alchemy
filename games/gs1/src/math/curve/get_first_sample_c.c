#include "curve.h"

s32 Curve_GetFirstSampleC(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}
