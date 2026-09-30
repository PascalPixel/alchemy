/*
 * Draft: Curve_LerpThreeSamplesByRatio does not yet match; 4 halfwords differ from ☀️'s C, first at +0xe (adds r1, r1, r2).
 * Links as recon/tla/raw/0802d2e0.s.
 */
#include "CURVE.H"

s32 Curve_LerpThreeSamplesByRatio(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 middle;
    s32 last;
    s32 result;

    first = *samples++ << CURVE_VALUE_SHIFT;
    middle = *samples << CURVE_VALUE_SHIFT;
    last = samples[1] << CURVE_VALUE_SHIFT;
    start = start + end;
    if ((u32)start == CURVE_FULL_STEPS - 1) {
        result = middle;
    } else if ((u32)start < CURVE_FULL_STEPS - 1) {
        result = first + Math_Div(
            (middle - first) * start,
            CURVE_FULL_STEPS - 1);
    } else {
        start = start - (CURVE_FULL_STEPS - 1);
        result = middle + Math_Div(
            (last - middle) * start,
            CURVE_FULL_STEPS - 1);
    }
    return result;
}
