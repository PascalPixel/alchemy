#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
void *Func_080048b0(s32, s32);
#define Runtime_AllocateHeapBlock Func_080048b0
void Func_08095884(void);
#define BattleFx_UpdateAllSlots Func_08095884
void BattleFx_InitializeSlots(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateHeapBlock(56, 0x720);
    zero = 0;
    Dma_Set(&zero, work, 0x850001c8, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback(Func_08095884, 3200);
}
