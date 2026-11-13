#ifndef ALCHEMY_HEAP_RELEASE_H
#define ALCHEMY_HEAP_RELEASE_H

/// @file
/// The heaps: allocating and freeing blocks.

#include "edition.h"
#include "types.h"

/// Releases heap slot `slot`'s block, and with it every later block of the same
/// pool, by setting the pool's next free address back to the block's start.
void HeapSlot_Release(s32 slot);
/// Releases `buffer`, a block taken with no heap slot, and every later block of
/// its pool, as HeapSlot_Release does.
void Heap_Release(void *buffer);

static __inline__ void *Heap_CallAlternatePool(void *(*allocate)(s32),
                                               s32 size) {
  return allocate(size);
}

/// The EWRAM heap's space, 32 KB from 0x02030000.
extern u8 gEwramHeap[];
/// The IWRAM heap's space, from 0x03002000 up to Ram_IwramHeapEnd.
extern u8 gIwramHeap[];

#endif
