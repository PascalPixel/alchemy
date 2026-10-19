/// @file
/// The heap: its two bump allocators and named slots.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

// The heap: two bump allocators, one in IWRAM up to Ram_IwramHeapEnd and one
// in EWRAM up to its end (0x02040000), and 62 named slots for long-lived
// blocks (enum HeapSlot). Nothing is freed one block at a time.

/// Empties the heap and both pools.
void Runtime_InitializeHeap(void) {
  union HeapState *work = (union HeapState *)gWorkSlot;

  Dma_Fill32(0, work, 0x100);
  work->heap.nextIwram = gIwramHeap;
  work->heap.nextEwram = gEwramHeap;
}

/// Returns the IWRAM bytes left to allocate.
s32 Runtime_GetRemainingIwram(void) {
  union HeapState *work = (union HeapState *)gWorkSlot;

  return (s32)(*(u8 (*)[]) & gCartridgeResetMarker) - (s32)work->heap.nextIwram;
}

/// Returns the EWRAM bytes left to allocate.
s32 Runtime_GetRemainingEwram(void) {
  union HeapState *work = (union HeapState *)gWorkSlot;

  return 0x02040000 - (s32)work->heap.nextEwram;
}

/// Returns heap slot `kind`'s block, first allocating `size` bytes for it,
/// from IWRAM when they fit and EWRAM otherwise. Returns NULL when neither has
/// room.
void *Runtime_AllocateHeapBlock(s32 kind, s32 size) {
  union HeapState *work;
  s32 alignedSize;
  u32 address;
  u32 nextAddress;
  u32 next;
  u32 cachedAddress;

  work = (union HeapState *)gWorkSlot;
  cachedAddress = (u32)work->slots[kind];
  if (cachedAddress == 0) {
    cachedAddress = (u32)work->heap.nextIwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = cachedAddress + alignedSize;
    if (next >= (u32)Ram_IwramHeapEnd) {
      address = (u32)work->heap.nextEwram;
      nextAddress = address + alignedSize;
      if (nextAddress >= 0x02040000U) {
        return NULL;
      }
      work->heap.nextEwram = (void *)nextAddress;
      work->slots[kind] = (void *)address;
      return (void *)address;
    }
    work->heap.nextIwram = (void *)next;
    work->slots[kind] = (void *)cachedAddress;
    return (void *)cachedAddress;
  }
  return (void *)cachedAddress;
}

/// As Runtime_AllocateHeapBlock, but tries EWRAM first.
void *Runtime_AllocateBlock(s32 kind, s32 size) {
  union HeapState *work;
  u32 alignedSize;
  u32 next;
  u32 address;
  u32 nextAddress;
  u32 cachedAddress;

  work = (union HeapState *)gWorkSlot;
  cachedAddress = (u32)work->slots[kind];
  if (cachedAddress == 0) {
    address = (u32)work->heap.nextEwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = address + alignedSize;
    if (next >= 0x02040000U) {
      address = (u32)work->heap.nextIwram;
      nextAddress = address + alignedSize;
      if (nextAddress >= (u32)Ram_IwramHeapEnd) {
        return NULL;
      }
      work->heap.nextIwram = (void *)nextAddress;
      work->slots[kind] = (void *)address;
      return (void *)address;
    }
    work->heap.nextEwram = (void *)next;
    work->slots[kind] = (void *)address;
    return (void *)address;
  }
  return (void *)cachedAddress;
}

/// Allocates `size` bytes (rounded up to words), from IWRAM when they fit and
/// EWRAM otherwise. Returns NULL when neither has room.
void *MemAlloc(s32 size) {
  union HeapState *work = (union HeapState *)gWorkSlot;
  u32 nextAddress;
  u32 next;
  u32 allocationAddress;
  u32 alignedWords = ((u32)size + 3) >> 2;

  allocationAddress = (u32)work->heap.nextIwram;
  size = alignedWords << 2;
  next = allocationAddress + size;
  if (next >= (u32)Ram_IwramHeapEnd) {
    allocationAddress = (u32)work->heap.nextEwram;
    nextAddress = allocationAddress + size;
    if (nextAddress >= 0x02040000U) {
      return NULL;
    }
    work->heap.nextEwram = (void *)nextAddress;
  } else {
    work->heap.nextIwram = (void *)next;
  }
  return (void *)allocationAddress;
}

/// As MemAlloc, but tries EWRAM first.
void *Runtime_BumpAllocateAlternatePool(s32 size) {
  union HeapState *work = (union HeapState *)gWorkSlot;
  u32 alternateNextAddress;
  u32 primaryNextAddress;
  u32 allocationAddress;
  u32 alignedWords = ((u32)size + 3) >> 2;

  allocationAddress = (u32)work->heap.nextEwram;
  size = alignedWords << 2;
  primaryNextAddress = allocationAddress + size;
  if (primaryNextAddress >= 0x02040000U) {
    allocationAddress = (u32)work->heap.nextIwram;
    alternateNextAddress = allocationAddress + size;
    if (alternateNextAddress >= (u32)Ram_IwramHeapEnd) {
      return NULL;
    }
    work->heap.nextIwram = (void *)alternateNextAddress;
  } else {
    work->heap.nextEwram = (void *)primaryNextAddress;
  }
  return (void *)allocationAddress;
}

/// Reserved empty memory export; its historical purpose is not established.
UNUSED void RuntimeMemory_ReservedNoOp(void) {}
