#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "TYPES.H"

void Runtime_FillFreeHeapWords(u32 value);
u32 Runtime_BumpAllocate(s32 size);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);

static __inline__ s16 *Runtime_CallAlternatePool(
    s16 *(*allocate)(s32), s32 size)
{
    /* FAKEMATCH: direct allocation shares the size across later calls,
       growing the fade by 12 bytes and changing the gauge's registers. */
    return allocate(size);
}

/* Legacy register-call veneer declaration; new code must call the typed target. */
void _call_via_r3(void *dst, void *src, s32 size, void *desc);

/* Where Runtime_InitializeHeap starts the two heaps: EWRAM up to its end,
   IWRAM up to the stacks. */
extern u8 gEwramHeap[];
extern u8 gIwramHeap[];

#endif
