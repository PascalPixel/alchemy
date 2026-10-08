#include "local.h"

s32 Func_02000784(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;
    s32 dxsq = dx * dx;
    s32 dzsq = dz * dz;

    return Iwram_Sqrt(dxsq + dzsq);
}
