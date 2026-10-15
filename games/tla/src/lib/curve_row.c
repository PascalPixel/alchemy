#include "curve.h"

s32 Func_0802d3c4(const s8 *samples, u32 row, u32 position)
{
    return samples[gCurve_SampleIndexTable[position + (row << 4)]] << CURVE_VALUE_SHIFT;
}

s32 Func_0802d3d8(const s8 *samples, u32 row, u32 position)
{
    const u8 *table = gCurve_SampleIndexTable;
    u32 index;

    /* FAKEMATCH: ⚓️ loads the index table before scaling the row */
    asm volatile("");
    index = (row << 4) - position;
    index += CURVE_FULL_STEPS - 1;
    return samples[table[index]] << CURVE_VALUE_SHIFT;
}
