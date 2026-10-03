#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "TYPES.H"

/* These allocators return a cached or new block, or NULL if both heaps are
   full. TBS uses slot numbers; TLA passes byte offsets into its slot bank. */
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void *Runtime_BumpAllocate(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *buffer);
void Sys_Free(void *buffer);
void Runtime_FillFreeHeapWords(u32 value);
void Runtime_InitializeHeap(void);

static __inline__ void *Runtime_CallAlternatePool(
    void *(*allocate)(s32), s32 size)
{
    /* FAKEMATCH: direct allocation shares the size across later calls,
       growing the fade by 12 bytes and changing the gauge's registers. */
    return allocate(size);
}

/* Where Runtime_InitializeHeap starts the two heaps: EWRAM up to its end,
   IWRAM up to the stacks. */
extern u8 gEwramHeap[];
extern u8 gIwramHeap[];

#endif
