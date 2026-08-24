#include "types.h"

void Func_0200ad3e(void);
void Func_0200ad9c(u32 flags);
void Func_0200aeb6(s32 x, s32 y, s32 z, u32 enabled);
void Func_0200aed0(void);

#define ConfigureSceneMotionFlags Func_020048ac

void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags)
{
    u32 selected;

    Func_0200aeb6(x, y, z, ~flags & 1);
    selected = flags & 0x1111;
    if ((flags & 0x10000000) != 0)
        Func_0200aed0();
    if ((flags & 0x01000000) != 0)
        Func_0200ad3e();
    Func_0200ad9c(selected);
}
