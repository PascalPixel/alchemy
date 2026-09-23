#include "DMA.H"
#include "RUNTIME_MEM.H"

/* Object_UpdateAll is ARM code that runs from a heap copy of itself; the
   copy length is a link-time symbol holding the routine's 1,256-byte size. */
extern u8 Data_0800a494[];
extern u8 Data_000004e8[];

void Object_RunUpdateAllFromHeap(void)
{
    void (*routine)(void);

    routine = (void (*)(void))Runtime_BumpAllocate((s32)Data_000004e8);
    Dma_Set(Data_0800a494, routine, 0x84000000 | ((u32)Data_000004e8 >> 2),
        (volatile u32 *)0x040000d4);
    routine();
    Sys_Free(routine);
}
