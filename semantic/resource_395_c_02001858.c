#include "types.h"

void Func_02001858(void)
{
    u32 *from;
    u32 *dma;
    u32 source;
    u32 destination;
    u32 control;

    from = (u32 *)0x03001ed0;
    dma = (u32 *)0x040000d4;
    source = *from;
    destination = 0x0200a4e0;
    control = 0x840000e0;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
}
