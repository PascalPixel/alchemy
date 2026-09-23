#include "DMA.H"
#include "RUNTIME_MEM.H"

/* The message lookup is ARM code (LOOKUP_SYMBOL.S) that runs from a heap copy
   of itself; the copy length is a link-time symbol. */
extern u8 UiText_LookupMessageCodeSize[];

/* Positions a text reader at the start of a message. */
void UiText_LookupMessage(s32 first, s32 second)
{
    void (*routine)(s32, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)UiText_LookupMessageCodeSize;
    } while (0);
    routine = (void (*)(s32, s32))Runtime_BumpAllocate(size);
    Dma_Set((const void *)0x8015570, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine(first, second);
    Sys_Free(routine);
}
