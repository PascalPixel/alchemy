#include "CURVE.H"

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
    return samples[Data_0802eec4[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}
