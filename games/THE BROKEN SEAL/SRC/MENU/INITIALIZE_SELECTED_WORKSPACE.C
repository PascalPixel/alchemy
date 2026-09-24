#include "DMA.H"
void *Runtime_AllocateBlock(s32, s32);
s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void Menu_RunSelectedWorkspaceEntry(void);
void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback(Menu_RunSelectedWorkspaceEntry, 3200);
}
