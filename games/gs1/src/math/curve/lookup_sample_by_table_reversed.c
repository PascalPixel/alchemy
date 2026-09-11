#include "scene.h"
#include "abi/math/curve/lookup_sample_by_table_reversed.h"
#include "curve.h"

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
    return samples[
        gRom[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
        << CURVE_VALUE_SHIFT;
}
