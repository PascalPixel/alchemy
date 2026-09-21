#include "DMA.H"

void *Func_080048f4(s32, u32);
#define Runtime_AllocateBlock Func_080048f4
void Func_080f3078(u32, void *, void *, s32);
void Func_080041d8(void (*)(void), s32);
#define Scheduler_AddOrUpdateCallback Func_080041d8
void Func_080f2f10(void);

#define TitlePalette_InitializeBuffers Func_080f377c

void TitlePalette_InitializeBuffers(void)
{
    volatile u32 zero;
    u8 *buffer;
    s32 operation;

    buffer = Runtime_AllocateBlock(32, 0x3004);
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000c01, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, buffer, 0x84000080, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, buffer + 512, 0x84000080, (volatile u32 *)0x040000d4);
    Func_080f3078(0x10000, buffer, buffer + 4096, 0);
    operation = 3200;
    Scheduler_AddOrUpdateCallback(Func_080f2f10, operation);
}
