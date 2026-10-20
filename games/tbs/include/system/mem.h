#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

/// @file
/// The heaps: allocating and freeing blocks.

#include "edition.h"
#include "types.h"

void Runtime_ReleaseHeapBlock(s32 slot);
void Runtime_BumpFree(void *buffer);
void MemFree(void *buffer);
void Runtime_FillFreeHeapWords(u32 value);

static __inline__ void *Runtime_CallAlternatePool(void *(*allocate)(s32),
                                                  s32 size) {
  /* FAKEMATCH(wrapper): the size passes through the inline's parameter and is
   * built again at the call, not shared with its other uses; as a direct call,
   * 13 of 34 instructions differ in tbs-en (Curtain_Start). */
  return allocate(size);
}

extern u8 gEwramHeap[];
extern u8 gIwramHeap[];

#endif
