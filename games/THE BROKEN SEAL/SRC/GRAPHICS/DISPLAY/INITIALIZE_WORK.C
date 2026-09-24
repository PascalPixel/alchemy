#include "DMA.H"
void *Runtime_AllocateBlock(s32, s32);
s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void AffineEffect_UpdateFrame(void);
void *AffineEffect_InitializeWork(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(58, 152);
    zero = 0;
    Dma_Set(&zero, work, 0x85000026, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
    return work;
}
