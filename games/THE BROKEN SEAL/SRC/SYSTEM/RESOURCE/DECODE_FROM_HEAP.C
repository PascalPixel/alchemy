#include "DMA.H"
#include "RUNTIME_MEM.H"

/* Each stream decoder is ARM code that runs from a heap copy of itself; the
   copy lengths are link-time symbols. */
extern u8 Resource_DecodeType01CodeSize[];
extern u8 Resource_DecodeType2CodeSize[];
extern u8 Resource_DecodeByteLzCodeSize[];

/* Decodes stream types 0 and 1 (DECODE_01.S). */
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
    Dma_Set((const void *)0x8002544, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

/* Decodes stream type 2 (DECODE_2.S). */
s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeType2CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)0x8002808, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

/* Decodes byte LZ streams (DECODE_BYTE_LZ.S). */
s32 Resource_DecodeByteLz(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeByteLzCodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)0x8001b70, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}
