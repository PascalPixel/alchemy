#include "DMA.H"
#include "RUNTIME_MEM.H"

u32 Resource_GetTableEntry(u32 index);
s32 Resource_DecodeType01(const void *source, void *destination);

/* The bl-pair fixer (PATCH_THUMB_BRANCH.S) is ARM code that runs from a heap
   copy of itself; the copy length is a link-time symbol. */
extern u8 Resource_PatchThumbBranchCodeSize[];

/* Decodes a code resource into place, then adjusts its Thumb calls for the
   address it now runs from. */
void Resource_LoadCode(u32 index, void *destination)
{
    void (*routine)(void *, s32);
    s32 end;
    u32 size;

    end = Resource_DecodeType01((const void *)Resource_GetTableEntry(index), destination);
    /* FAKEMATCH: the wrapper keeps the size load after the decode. */
    do {
        size = (u32)Resource_PatchThumbBranchCodeSize;
    } while (0);
    routine = (void (*)(void *, s32))Runtime_BumpAllocate(size);
    Dma_Set((const void *)0x8002d5c, routine, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    routine(destination, end);
    Sys_Free(routine);
}
