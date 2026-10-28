/// @file
/// The heap: its two bump allocators and heap slots.
// Part of system.c.

#include "../system_internal.h"

// The heap: two bump allocators, one in IWRAM up to Ram_IwramHeapEnd and one
// in EWRAM up to its end (EWRAM_END), and 62 heap slots for long-lived
// blocks (enum HeapSlot). Nothing is freed one block at a time.

/// Empties the heap: clears the 64 words of gHeapSlots (both pool pointers and
/// every heap slot) and sets the pools' next addresses to gIwramHeap and
/// gEwramHeap. Called by System_Initialize, by MainRoutine_Run and by
/// the debug modes that start over, so every earlier block is forgotten.
void Heap_Initialize(void) {
  union HeapState *work = (union HeapState *)gHeapSlots;

  Dma_Fill32(0, work, 0x100);
  work->heap.nextIwram = gIwramHeap;
  work->heap.nextEwram = gEwramHeap;
}

/// Returns the IWRAM bytes left to allocate, up to gCartridgeResetMarker
/// (0x03007800), the same end the allocators test. Battle_Run calls
/// it around each action and ignores the result.
s32 Heap_GetRemainingIwram(void) {
  union HeapState *work = (union HeapState *)gHeapSlots;

  return (s32)(*(u8 (*)[]) & gCartridgeResetMarker) - (s32)work->heap.nextIwram;
}

/// Returns the EWRAM bytes left to allocate, up to the end of EWRAM.
/// Battle_Run calls it around each action and ignores the result.
s32 Heap_GetRemainingEwram(void) {
  union HeapState *work = (union HeapState *)gHeapSlots;

  return EWRAM_END - (s32)work->heap.nextEwram;
}

/// Returns heap slot `kind`'s block, first allocating `size` bytes for it
/// (rounded up to words), from IWRAM when they fit and EWRAM otherwise. A new
/// block advances that pool's next address and is stored in the slot, so
/// later calls with the same `kind` return it until HeapSlot_Release
/// frees it. Returns NULL when neither pool has room. Called throughout the
/// game for long-lived work blocks, and by wait() for each frame's OAM
/// list.
///
/// A slot that already has a block returns it whatever `size` asks for, so a
/// caller that allocates also gets an earlier caller's block. The block is
/// not cleared. It lives until HeapSlot_Release(kind), which also
/// releases every block taken after it from the same pool, or until
/// Heap_Initialize.
void *HeapSlot_Alloc(s32 kind, s32 size) {
  union HeapState *work;
  s32 alignedSize;
  u32 address;
  u32 nextAddress;
  u32 next;
  u32 cachedAddress;

  work = (union HeapState *)gHeapSlots;
  cachedAddress = (u32)work->slots[kind];
  if (cachedAddress == 0) {
    cachedAddress = (u32)work->heap.nextIwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = cachedAddress + alignedSize;
    // A block ending exactly at the pool's end is refused too
    if (next >= (u32)Ram_IwramHeapEnd) {
      address = (u32)work->heap.nextEwram;
      nextAddress = address + alignedSize;
      if (nextAddress >= EWRAM_END) {
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

/// As HeapSlot_Alloc, but tries EWRAM first, for blocks that need
/// room more than speed: the battle, shop and save work, among others.
void *HeapSlot_AllocEwram(s32 kind, s32 size) {
  union HeapState *work;
  u32 alignedSize;
  u32 next;
  u32 address;
  u32 nextAddress;
  u32 cachedAddress;

  work = (union HeapState *)gHeapSlots;
  cachedAddress = (u32)work->slots[kind];
  if (cachedAddress == 0) {
    address = (u32)work->heap.nextEwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = address + alignedSize;
    if (next >= EWRAM_END) {
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

/// Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM
/// when they fit and EWRAM otherwise, and advances that pool's next address.
/// Returns NULL when neither has room. Called for short-lived buffers (decode
/// buffers, RAM copies of code). The buffer is not cleared. Heap_Release(p)
/// sets its pool's next address back to `p`, which also frees everything
/// allocated after it there, so buffers are freed newest first.
void *Heap_Alloc(s32 size) {
  union HeapState *work = (union HeapState *)gHeapSlots;
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
    if (nextAddress >= EWRAM_END) {
      return NULL;
    }
    work->heap.nextEwram = (void *)nextAddress;
  } else {
    work->heap.nextIwram = (void *)next;
  }
  return (void *)allocationAddress;
}

/// As Heap_Alloc, but tries EWRAM first. Called mostly by the battle code and
/// the credits for buffers too large to want IWRAM.
void *Heap_AllocEwram(s32 size) {
  union HeapState *work = (union HeapState *)gHeapSlots;
  u32 alternateNextAddress;
  u32 primaryNextAddress;
  u32 allocationAddress;
  u32 alignedWords = ((u32)size + 3) >> 2;

  allocationAddress = (u32)work->heap.nextEwram;
  size = alignedWords << 2;
  primaryNextAddress = allocationAddress + size;
  if (primaryNextAddress >= EWRAM_END) {
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

// XXX (no evidence): what it was for; nothing in either game calls it, and The
// Lost Age's heap code has no empty routine in its place.
/// Unused: reserved empty memory export; nothing calls it.
UNUSED void Heap_ReservedNoOp(void) {}
