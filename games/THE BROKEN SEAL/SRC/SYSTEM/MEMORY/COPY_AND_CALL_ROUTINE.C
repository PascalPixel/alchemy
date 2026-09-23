#include "TYPES.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"

typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size of the routine copied into the heap. */
extern u8 LoadedRuntime_Size[];

void Runtime_CopyAndCallRoutine(void *argument)
{
    u32 size;
    LoadedRoutine routine;

    /*
     * FAKEMATCH: a loop that runs once around the size load. It is a
     * scheduling barrier: without it the size literal is loaded before the
     * argument is copied to r8.
     */
    do {
        size = (u32)LoadedRuntime_Size;
    } while (0);
    routine = (LoadedRoutine)Runtime_BumpAllocate(size);
    Dma_Set((const void *)0x08001dc8, (void *)routine,
            (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    routine(argument);
    Sys_Free((void *)routine);
}
