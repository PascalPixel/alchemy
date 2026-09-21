#include "DMA.H"
void *Func_080048f4(s32, s32);
#define Runtime_AllocateBlock Func_080048f4
s32 Func_080041d8(void (*)(void), s32);
#define Scheduler_AddOrUpdateCallback Func_080041d8
void Func_0801d94c(void);
void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback(Func_0801d94c, 3200);
}
