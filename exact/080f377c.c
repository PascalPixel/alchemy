#include "types.h"

void *Func_080048f4(s32, u32);
void Func_080f3078(u32, void *, void *, s32);
void Func_080041d8(void (*)(void), s32);
void Func_080f2f10(void);

void Func_080f377c(void)
{
    u32 zero;
    u32 *source;
    u8 *buffer;
    volatile u32 *dma;

    buffer = Func_080048f4(32, 0x3004);
    source = &zero;
    *source = 0;
    dma = (volatile u32 *)0x040000d4;
    {
        u32 from = (u32)source;
        u32 to = (u32)buffer;
        u32 control = 0x85000c01;

        dma[0] = from;
        dma[1] = to;
        dma[2] = control;
    }
    {
        u32 from = 0x05000000;
        u32 to = (u32)buffer;
        u32 control = 0x84000080;

        dma[0] = from;
        dma[1] = to;
        dma[2] = control;
    }
    {
        u32 from = 0x05000200;
        u32 to = (u32)(buffer + 512);
        u32 control = 0x84000080;

        dma[0] = from;
        dma[1] = to;
        dma[2] = control;
    }
    Func_080f3078(0x10000, buffer, buffer + 4096, 0);
    Func_080041d8(Func_080f2f10, 3200);
}
