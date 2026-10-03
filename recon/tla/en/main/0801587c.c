#include "RESOURCE.H"
#include "RUNTIME_MEM.H"

extern const u8 Resource_DecodeHalfwordLz[];

/* Linker sizes determine the stack allocation for the copied ARM entries. */
extern u8 Resource_DecompressHalfwordsCodeSize[];

s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeType01CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Func_08002544, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}
