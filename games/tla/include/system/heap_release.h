#ifndef ALCHEMY_HEAP_RELEASE_H
#define ALCHEMY_HEAP_RELEASE_H

/// @file
/// The heap's allocators and releases (system/system_heap.c).

#include "edition.h"
#include "types.h"

void *HeapSlot_Alloc(s32 slot, s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
/// Releases heap slot `slot`'s block, and with it every later block of the same
/// pool, by setting the pool's next free address back to the block's start.
void HeapSlot_Release(s32 slot);
void *Heap_Alloc(s32 size);
void *Heap_AllocEwram(s32 size);
/// Releases `buffer`, a block taken with no heap slot, and every later block of
/// its pool, as HeapSlot_Release does.
void Heap_Release(void *buffer);
void Heap_FillFreeWords(u32 value);
void Heap_Initialize(void);

/// A block of size bytes from the allocator given (Heap_Alloc or
/// Heap_AllocEwram), with no heap slot.
static __inline__ void *Heap_CallAlternatePool(void *(*allocate)(s32),
                                               s32 size) {
  return allocate(size);
}

/// The EWRAM heap's space, 32 KB from 0x02030000.
extern u8 gEwramHeap[];
/// The IWRAM heap's space, from 0x03001300 up to Ram_IwramHeapEnd.
extern u8 gIwramHeap[];

#endif
