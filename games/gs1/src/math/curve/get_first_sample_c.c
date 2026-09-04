#include "curve.h"

#define Curve_GetFirstSampleC Func_08011f48

s32 Curve_GetFirstSampleC(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}
