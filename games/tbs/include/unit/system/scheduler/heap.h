#ifndef ALCHEMY_UNIT_SYSTEM_SCHEDULER_HEAP_H
#define ALCHEMY_UNIT_SYSTEM_SCHEDULER_HEAP_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

void Runtime_InitializeHeap(void);
s32 Runtime_GetRemainingIwram(void);
s32 Runtime_GetRemainingEwram(void);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void *Runtime_AllocateBlock(s32 kind, s32 size);
void *MemAlloc(s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void RuntimeMemory_ReservedNoOp(void);

#endif
