#include "curve.h"

#define Curve_GetFirstSampleA Func_08011ce0

s32 Curve_GetFirstSampleA(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}
