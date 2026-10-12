#include "curve.h"

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
    return samples[Curve_SampleIndexTable[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
    const u8 *table = Curve_SampleIndexTable;
    u32 index;

    /* FAKEMATCH: ⚓️ loads the index table before scaling the row (one reordered pair, differ 2 without it) */
    asm volatile("");
    index = (row << 4) - position;
    index += CURVE_FULL_STEPS - 1;
    return samples[table[index]] << CURVE_VALUE_SHIFT;
}
