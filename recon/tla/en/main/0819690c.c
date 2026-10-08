#include "text_reader.h"
#include "mem.h"

extern const u8 Func_08015570[];

extern u8 UiText_LookupMessageCodeSize[];

void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)UiText_LookupMessageCodeSize;
    } while (0);
    routine = (void (*)(struct TextReader *, s32))MemAlloc(size);
    Dma_Set((const void *)Func_08015570, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine(reader, message);
    MemFree(routine);
}
