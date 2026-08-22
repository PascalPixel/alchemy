#include "curve.h"

s32 Func_08011f14(const s8 *samples, u32 position, u32 row)
{
    return samples[Data_080133fc[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}
