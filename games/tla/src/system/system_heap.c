/// @file
/// The heap: its two bump allocators and heap slots.
// Part of system.c.

#include "system_internal.h"

/// Empties the heap: clears the heap slots (both pool pointers and every slot)
/// and sets the pools' next addresses to gIwramHeap and gEwramHeap.
void Heap_Initialize(void) {
  struct HeapSlots *state = Ram_HeapSlots;

  Dma_Fill32(0, state, sizeof(struct HeapSlots));
  Ram_HeapSlots->nextIwram = gIwramHeap;
  Ram_HeapSlots->nextEwram = gEwramHeap;
}

/// Returns the IWRAM bytes left to allocate, up to the sound mixer's
/// workspace (IwramSoundMixWorkspace).
s32 Heap_GetRemainingIwram(void) {
  struct HeapSlots *state = Ram_HeapSlots;

  return (s32)IwramSoundMixWorkspace - (s32)state->nextIwram;
}

/// Returns the EWRAM bytes left to allocate, up to the end of EWRAM.
/// Nothing calls it or its far-call export.
s32 Heap_GetRemainingEwram(void) {
  struct HeapSlots *state = Ram_HeapSlots;

  return EWRAM_END - (s32)state->nextEwram;
}

/// Returns heap slot `slot`'s block, first allocating `size` bytes for it
/// (rounded up to words), from IWRAM when they fit and EWRAM otherwise.
void *HeapSlot_Alloc(s32 slot, s32 size) {
  struct HeapSlots *state;
  s32 alignedSize;
  u32 address;
  u32 nextAddress;
  u32 next;
  u32 block;

  state = Ram_HeapSlots;
  block = (u32)HEAP_SLOT(state, slot);
  if (block == 0) {
    block = (u32)state->nextIwram;
    alignedSize = (((u32)size + 3) >> 2) * 4;
    next = block + alignedSize;
    // A block ending exactly at the pool's end is refused too.
    if (next >= (u32)Ram_IwramHeapEnd) {
      address = (u32)state->nextEwram;
      nextAddress = address + alignedSize;
      if (nextAddress >= (u32)EWRAM_END) {
        return NULL;
      }
      state->nextEwram = (void *)nextAddress;
      HEAP_SLOT(state, slot) = (void *)address;
      return (void *)address;
    }
    state->nextIwram = (void *)next;
    HEAP_SLOT(state, slot) = (void *)block;
    return (void *)block;
  }
  return (void *)block;
}

/// As HeapSlot_Alloc, but tries EWRAM first, for blocks that need
/// room more than speed: the battle, shop and save state, among others.
void *HeapSlot_AllocEwram(s32 slot, s32 size) {
  struct HeapSlots *state;
  u32 alignedSize;
  u32 next;
  u32 address;
  u32 nextAddress;
  u32 block;

  state = Ram_HeapSlots;
  block = (u32)HEAP_SLOT(state, slot);
  if (block == 0) {
    alignedSize = (u32)size + 3;
    address = (u32)state->nextEwram;
    alignedSize = (alignedSize >> 2) * 4;
    next = address + alignedSize;
    if (next >= (u32)EWRAM_END) {
      address = (u32)state->nextIwram;
      nextAddress = address + alignedSize;
      if (nextAddress > (u32)(Ram_IwramHeapEnd - 1)) {
        return NULL;
      }
      state->nextIwram = (void *)nextAddress;
      HEAP_SLOT(state, slot) = (void *)address;
      return (void *)address;
    }
    state->nextEwram = (void *)next;
    HEAP_SLOT(state, slot) = (void *)address;
    return (void *)address;
  }
  return (void *)block;
}

/// Fills the free space of both heaps, from each pool's next address to its
/// end, with `value`.
void Heap_FillFreeWords(u32 value) {
  struct HeapSlots *state = Ram_HeapSlots;
  u32 address;

  address = (u32)state->nextIwram;
  Iwram_FillWordsEntry((void *)address, (u32)Ram_IwramHeapEnd - address, value);
  address = (u32)state->nextEwram;
  Iwram_FillWordsEntry((void *)address, (u32)EWRAM_END - address, value);
}

/// Allocates `size` bytes (rounded up to words) with no heap slot, from IWRAM
/// when they fit and EWRAM otherwise, and advances that pool's next address.
void *Heap_Alloc(s32 size) {
  struct HeapSlots *state = Ram_HeapSlots;
  u32 nextAddress;
  u32 next;
  u32 address;
  u32 words;
  u32 limit;

  words = (u32)size + 3;
  address = (u32)state->nextIwram;
  words >>= 2;
  limit = (u32)(Ram_IwramHeapEnd - 1);
  size = (s32)(words << 2);
  next = address + (u32)size;
  if (next > limit) {
    address = (u32)state->nextEwram;
    nextAddress = address + (u32)size;
    if (nextAddress >= (u32)EWRAM_END) {
      return NULL;
    }
    state->nextEwram = (void *)nextAddress;
  } else {
    state->nextIwram = (void *)next;
  }
  return (void *)address;
}

/// As Heap_Alloc, but tries EWRAM first. Called mostly by the battle code and
/// the credits for buffers too large to want IWRAM.
void *Heap_AllocEwram(s32 size) {
  struct HeapSlots *state = Ram_HeapSlots;
  u32 nextAddress;
  u32 next;
  u32 address;
  u32 words;

  words = (u32)size + 3;
  address = (u32)state->nextEwram;
  size = (s32)((words >> 2) << 2);
  next = address + (u32)size;
  if (next >= (u32)EWRAM_END) {
    address = (u32)state->nextIwram;
    nextAddress = address + (u32)size;
    if (nextAddress > (u32)(Ram_IwramHeapEnd - 1)) {
      return NULL;
    }
    state->nextIwram = (void *)nextAddress;
  } else {
    state->nextEwram = (void *)next;
  }
  return (void *)address;
}
