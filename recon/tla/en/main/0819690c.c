#include "TEXT_READER.H"
#include "RUNTIME_MEM.H"

extern const u8 Func_08015570[];

/* The message lookup is ARM code (LOOKUP_SYMBOL.S) that runs from a heap copy
   of itself; the copy length is a link-time symbol. */
extern u8 UiText_LookupMessageCodeSize[];

/* Positions a text reader at the start of a message. */

void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)UiText_LookupMessageCodeSize;
    } while (0);
    routine = (void (*)(struct TextReader *, s32))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Func_08015570, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine(reader, message);
    Sys_Free(routine);
}
