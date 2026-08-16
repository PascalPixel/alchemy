#include "types.h"

extern char Value_03001f00;

void Func_080c0130(void)
{
    u32 *dma;
    u32 source;
    u32 destination;
    u32 control;
    u32 *entry;
    u32 source2;

    if (((u32 **)&Value_03001f00)[0][2] == 2)
    {
        entry = *(u32 **)(&Value_03001f00 - 136);
        source = (u32)entry + entry[0] * 320;
        destination = 0x0400000c;
        *(u16 *)destination = *(u16 *)(source + 32);
        dma = (u32 *)0x040000b0;
        control = 0xa2600001;
        source += 34;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;

        destination = 0x04000020;
        control = 0x84000004;
        dma = (u32 *)0x040000d4;
        source2 = (u32)entry + 16;

        dma[0] = source2;
        dma[1] = destination;
        dma[2] = control;
    }
}
