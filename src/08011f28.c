#include "curve.h"

s32 Func_08011f28(const s8 *samples, u32 position, u32 row)
{
    return samples[
        Data_080133fc[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
        << CURVE_VALUE_SHIFT;
}
