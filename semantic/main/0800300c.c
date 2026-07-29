#include "types.h"

extern u16 Value_0000c3ff;
extern u16 Value_00001001;

void Func_0800300c(void)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    volatile u32 *dma;
    u32 source;
    u32 destination;
    u32 control;
    u16 disabled;

    disabled = 0;
    *interruptMaster = disabled;

    destination = 0x03000000;
    dma = (volatile u32 *)0x040000d4;
    source = 0x08000770;
    control = 0x84000500;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    *(u32 *)0x03007ffc = destination;

    source = 0x08007320;
    destination = 0x030000e0;
    control = 0x8400000e;
    dma = (volatile u32 *)0x040000d4;
    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;

    *(u16 *)0x03000000 = disabled;
    *(u16 *)0x04000130 = (u16)(u32)&Value_0000c3ff;
    *(u16 *)0x04000132 = (u16)(u32)&Value_00001001;
    *interruptMaster = 1;
}
