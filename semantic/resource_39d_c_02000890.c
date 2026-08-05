#include "types.h"



extern s32 Func_02003854(s32 value);
s32 Func_02000890(s32 *a, s32 *b)
{
    s32 dz = (a[2] - b[2]) >> 16;
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;

    return Func_02003854(dx * dx + dy * dy + dz * dz);
}
