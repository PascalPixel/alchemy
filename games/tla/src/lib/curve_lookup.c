#include "curve.h"

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
    return samples[Curve_SampleIndexTable[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}
