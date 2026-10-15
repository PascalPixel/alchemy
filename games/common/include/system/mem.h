#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "edition.h"
#include "types.h"

#if GAME_TLA
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void *Runtime_AllocateBlock(s32 slot, s32 size);
#endif
void Runtime_ReleaseHeapBlock(s32 slot);
#if GAME_TLA
void *MemAlloc(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
#endif
void Runtime_BumpFree(void *buffer);
void MemFree(void *buffer);
void Runtime_FillFreeHeapWords(u32 value);
#if GAME_TLA
void Runtime_InitializeHeap(void);
#endif

static __inline__ void *Runtime_CallAlternatePool(void *(*allocate)(s32), s32 size)
{
    /* FAKEMATCH: the inline call prevents size sharing, avoiding 12 extra fade bytes and changed gauge registers. */
    return allocate(size);
}

extern u8 gEwramHeap[];
extern u8 gIwramHeap[];

#endif
