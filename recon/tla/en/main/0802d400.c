/*
 * Draft: Curve_LookupSampleByTableReversed does not yet match; 4 halfwords differ from ☀️'s C, first at +0x0 (ldr r3, [pc, #12]).
 * Links as recon/tla/raw/0802d400.s.
 */
#include "CURVE.H"

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
    return samples[
        Curve_SampleIndexTable[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
        << CURVE_VALUE_SHIFT;
}
