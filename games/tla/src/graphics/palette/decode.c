#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

extern const u8 Resource_DecodeHalfwordLzCode[];
extern u8       Resource_DecompressHalfwordsCodeSize[];

void Resource_DecompressHalfwords(const void *source, void *destination)
{
    u8 routine[(u32)Resource_DecompressHalfwordsCodeSize];

    Dma_Set((const void *)Resource_DecodeHalfwordLzCode, routine,
        ((u32)Resource_DecompressHalfwordsCodeSize >> 2) | 0x84000000, REG_DMA3);
    ((void (*)(const void *, void *))routine)(source, destination);
}

s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do
    {
        size = (u32)Resource_Decode01CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))MemAlloc(size);
    Dma_Set((const void *)Resource_Decode01Code, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do
    {
        result = routine(source, destination);
    } while (0);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do
    {
        size = (u32)Resource_Decode2CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))MemAlloc(size);
    Dma_Set((const void *)Resource_Decode2Code, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do
    {
        result = routine(source, destination);
    } while (0);
    MemFree(routine);
    return result;
}

s32 Resource_DecodeByteLzInRam(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do
    {
        size = (u32)Resource_DecodeByteLzCodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))MemAlloc(size);
    Dma_Set((const void *)Resource_DecodeByteLzCode, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do
    {
        result = routine(source, destination);
    } while (0);
    MemFree(routine);
    return result;
}
