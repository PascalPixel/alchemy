#include "ov664.h"

s32 Func_02000cec(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Iwram_Sqrt(dx * dx + dz * dz);
}

s32 Func_02000d18(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a - *b + 0x60000) >> 16;

    return Iwram_Sqrt(dx * dx + dy * dy);
}
