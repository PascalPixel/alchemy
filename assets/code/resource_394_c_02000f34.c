#include "types.h"

extern u32 Data_020092d0;

void Func_02000f34(void)
{
    u32 *sp2 = (u32 *)0x03001ed0;
    u32 *dma = (u32 *)0x040000D4;
    u32 a = *sp2;
    u32 b = (u32)&Data_020092d0;
    u32 c = 0x840000e0;

    dma[0] = a;
    dma[1] = b;
    dma[2] = c;
}
