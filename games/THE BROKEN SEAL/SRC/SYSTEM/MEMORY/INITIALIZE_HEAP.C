#include "DMA.H"
struct HeapState { void *next_ewram; void *next_iwram; u8 entries[248]; };
void Runtime_InitializeHeap(void)
{
    struct HeapState *work = (struct HeapState *)0x03001e50;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x85000040, (volatile u32 *)0x040000d4);
    work->next_iwram = (void *)0x03002000;
    work->next_ewram = (void *)0x02030000;
}
