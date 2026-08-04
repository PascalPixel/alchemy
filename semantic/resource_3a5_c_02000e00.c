#include "types.h"

extern s32 Func_030001d8();

/* Return the distance between two fixed-point X/Z positions. */
s32 Func_02000e00(const s32 *left, const s32 *right)
{
    s32 dx = (right[0] - left[0]) >> 16;
    s32 dz = (right[1] - left[1]) >> 16;
    return Func_030001d8(dx * dx + dz * dz);
}
