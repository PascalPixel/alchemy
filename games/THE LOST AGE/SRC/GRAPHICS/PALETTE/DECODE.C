#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "IO_REG.H"


s32 Resource_DecodeType01(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_Decode01CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Resource_Decode01Code, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

s32 Resource_DecodeType2(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_Decode2CodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Resource_Decode2Code, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}

s32 Resource_DecodeByteLzInRam(const void *source, void *destination)
{
    s32 (*routine)(const void *, void *);
    s32 result;
    u32 size;

    /* FAKEMATCH: the wrapper keeps the size load after the parameter copies. */
    do {
        size = (u32)Resource_DecodeByteLzCodeSize;
    } while (0);
    routine = (s32 (*)(const void *, void *))Runtime_BumpAllocate(size);
    Dma_Set((const void *)Resource_DecodeByteLzCode, routine, 0x84000000 | (size >> 2), REG_DMA3);
    /* FAKEMATCH: so is the call, which keeps the argument loads in order. */
    do {
        result = routine(source, destination);
    } while (0);
    Sys_Free(routine);
    return result;
}
