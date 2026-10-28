#ifndef ALCHEMY_HEAP_RELEASE_H
#define ALCHEMY_HEAP_RELEASE_H

#include "edition.h"
#include "types.h"

void *HeapSlot_Alloc(s32 slot, s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void *Heap_Alloc(s32 size);
void *Heap_AllocEwram(s32 size);
void Heap_ReleaseEwram(void *buffer);
void Heap_Release(void *buffer);
void Heap_FillFreeWords(u32 value);
void Heap_Initialize(void);

static __inline__ void *Heap_CallAlternatePool(void *(*allocate)(s32),
                                               s32 size) {
  /* FAKEMATCH: the inline call prevents size sharing, avoiding 12 extra fade
   * bytes and changed gauge registers. */
  return allocate(size);
}

extern u8 gEwramHeap[];
extern u8 gIwramHeap[];

#endif
