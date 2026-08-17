#include "types.h"

extern void Func_020020aa(s32 arg0, s32 arg1);

void Func_02000f74(void)
{
    u32 destination = *(volatile u32 *)0x03001ed0;
    u32 source = 0x05000000;
    u32 control = 0x84000070;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;

    dma3[0] = source;
    dma3[1] = destination;
    dma3[2] = control;

    source = 0x05000200;
    destination += 0x1c0;
    dma3[0] = source;
    dma3[1] = destination;
    dma3[2] = control;

    Func_020020aa(0x10000, 0);
}
