#include "CURVE.H"
#include "FIXED_MATH.H"

s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row)
{
    s32 start;

    start = samples[0] << CURVE_VALUE_SHIFT;
    return start
        + (((samples[1] << CURVE_VALUE_SHIFT) - start)
           * Curve_LerpWeightTable[position + (row *CURVE_FULL_STEPS)]);
}
