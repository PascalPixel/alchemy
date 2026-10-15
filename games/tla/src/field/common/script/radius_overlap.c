#include "types.h"

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 radius = radius_a + radius_b;

    if (!(dx > 0x400000) && !(dz > 0x400000) && dx * dx + dy * dy + dz * dz < radius * radius)
        return 0;
    return -1;
}
