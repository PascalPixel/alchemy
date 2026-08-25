#include "types.h"

void *Func_080048f4(s32, u32);
void Func_080f3078(u32, void *, void *, s32);
void Func_080041d8(void (*)(void), s32);
void Func_080f2f10(void);

#define TitlePalette_InitializeBuffers Func_080f377c

void TitlePalette_InitializeBuffers(void)
{
    u32 zero;
    u8 *buffer;
    volatile u32 *dma;
    s32 operation;

    buffer = Func_080048f4(32, 0x3004);
    zero = 0;
    dma = (volatile u32 *)0x040000d4;
    dma[0] = (u32)&zero;
    dma[1] = (u32)buffer;
    dma[2] = 0x85000c01;
    dma[0] = 0x05000000;
    dma[1] = (u32)buffer;
    dma[2] = 0x84000080;
    dma[0] = 0x05000200;
    dma[1] = (u32)(buffer + 512);
    dma[2] = 0x84000080;
    Func_080f3078(0x10000, buffer, buffer + 4096, 0);
    operation = 3200;
    Func_080041d8(Func_080f2f10, operation);
}
