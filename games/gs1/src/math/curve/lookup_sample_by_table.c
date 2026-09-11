#include "scene.h"
#include "abi/math/curve/lookup_sample_by_table.h"
#include "curve.h"

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
    return samples[gRom[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}
