#include "DMA.H"

void *Runtime_AllocateBlock(s32, u32);
void Unnamed_080f3078(u32, void *, void *, s32);
void Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void TitlePalette_UpdateFade(void);


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
    Unnamed_080f3078(0x10000, buffer, buffer + 4096, 0);
    operation = 3200;
    Scheduler_AddOrUpdateCallback(TitlePalette_UpdateFade, operation);
}
