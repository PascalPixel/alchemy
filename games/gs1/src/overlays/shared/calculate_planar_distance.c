#include "types.h"

typedef s32 (*IwramSqrt)(s32);

s32 CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz * dz;
    s32 dx_squared = dx * dx;

    return ((IwramSqrt)0x030001d8)(dx_squared + dz_squared);
}
