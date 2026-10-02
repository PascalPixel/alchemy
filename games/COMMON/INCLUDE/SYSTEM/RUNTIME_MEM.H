#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "TYPES.H"

void Runtime_FillFreeHeapWords(u32 value);
u32 Runtime_BumpAllocate(s32 size);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);

/* Legacy register-call veneer declaration; new code must call the typed target. */
void _call_via_r3(void *dst, void *src, s32 size, void *desc);

/* Where Runtime_InitializeHeap starts the two heaps: EWRAM up to its end,
   IWRAM up to the stacks. */
extern u8 gEwramHeap[];
extern u8 gIwramHeap[];

#endif
