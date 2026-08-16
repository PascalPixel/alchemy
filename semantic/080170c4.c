#include "types.h"

u16 *Func_080170c4(u16 *destination, u32 value, s32 count)
{
    u16 fill;
    u16 *source;
    u32 control;

    if (count > 0) {
        source = &fill;
        fill = value;
        control = 0x81000000 | count;
        ((u32 *)0x040000D4)[0] = (u32)source;
        ((u32 *)0x040000D4)[1] = (u32)destination;
        ((u32 *)0x040000D4)[2] = control;
        destination += count;
    }
    return destination;
}
