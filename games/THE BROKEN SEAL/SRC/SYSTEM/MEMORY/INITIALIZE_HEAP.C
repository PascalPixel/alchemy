#include "DMA.H"
#include "RUNTIME_MEM.H"

extern u8 gWorkSlot[];
struct HeapState { void *next_ewram; void *next_iwram; u8 entries[248]; };
void Runtime_InitializeHeap(void)
{
    struct HeapState *work = (struct HeapState *)gWorkSlot;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x85000040, (volatile u32 *)0x040000d4);
    work->next_iwram = gIwramHeap;
    work->next_ewram = gEwramHeap;
}
