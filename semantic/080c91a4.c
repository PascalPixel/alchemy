#include "types.h"

struct Dma_080c91a4 {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_080c91a4(void)
{
    u32 *dma = (u32 *)0x040000B0;
    volatile u16 *controlHigh = (volatile u16 *)0x040000BA;
    u32 source = 0x02010000;
    u32 destination = 0x04000040;
    u32 control = 0xA2600001;

    *controlHigh &= 0xC5FF;
    *controlHigh &= 0x7FFF;
    *dma++ = source;
    *dma++ = destination;
    *dma++ = control;
}
