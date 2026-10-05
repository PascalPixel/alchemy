#include "TEXT_READER.H"
#include "TYPES.H"
#include "DMA.H"
#include "RUNTIME_MEM.H"
#include "IO_REG.H"

extern const u8 Text_LookupSymbolCode[];
extern u8 Text_LookupSymbolCodeSize[];

/* Positions a text reader at the start of a message, running the ARM
   lookup from a heap copy. */
void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
    void (*routine)(struct TextReader *, s32);
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Text_LookupSymbolCodeSize;
    } while (0);
    routine = (void (*)(struct TextReader *, s32))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Text_LookupSymbolCode, routine, 0x84000000 | (size >> 2), REG_DMA3);
    routine(reader, message);
    Sys_Free(routine);
}
