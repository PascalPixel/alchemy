#include "DMA.H"
#include "RUNTIME_MEM.H"

/* Object_UpdateAll is ARM code that runs from a heap copy of itself. */
extern u8 Object_UpdateAll[];
extern u8 Object_UpdateAllCodeSize[];

void Object_RunUpdateAllFromHeap(void)
{
    void (*routine)(void);

    routine = (void (*)(void))Runtime_BumpAllocate((s32)Object_UpdateAllCodeSize);
    Dma_Set(Object_UpdateAll, routine, 0x84000000 | ((u32)Object_UpdateAllCodeSize >> 2),
        (volatile u32 *)0x040000d4);
    routine();
    Sys_Free(routine);
}
