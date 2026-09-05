#include "curve.h"

s32 Curve_GetFirstSampleA(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}
