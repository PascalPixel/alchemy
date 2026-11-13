/// @file
/// The heap: its two bump allocators and heap slots.
// Part of system.c.

#include "../system_internal.h"

void Heap_Initialize(void) {
  struct HeapSlots *work = Ram_HeapSlots;

  Dma_Fill32(0, work, sizeof(struct HeapSlots));
  Ram_HeapSlots->nextIwram = gIwramHeap;
  Ram_HeapSlots->nextEwram = gEwramHeap;
}

s32 Heap_GetRemainingIwram(void) {
  struct HeapSlots *work = Ram_HeapSlots;

  return (s32)IwramSoundMixWorkspace - (s32)work->nextIwram;
}

s32 Heap_GetRemainingEwram(void) {
  struct HeapSlots *work = Ram_HeapSlots;

  return EWRAM_END - (s32)work->nextEwram;
}

void *HeapSlot_Alloc(s32 kind, s32 size) {
  u32 *allocator_state;
  s32 kind_offset;
  s32 aligned_size;
  u32 address;
  u32 next_address;
  u32 next;
  u32 cached_address;

  allocator_state = (u32 *)Ram_HeapSlots;
  kind_offset = kind;
  cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
  if (cached_address == 0) {
    cached_address = allocator_state[1];
    aligned_size = (((u32)size + 3) >> 2) * 4;
    next = cached_address + aligned_size;
    if (next >= (u32)Ram_IwramHeapEnd) {
      address = allocator_state[0];
      next_address = address + aligned_size;
      if (next_address >= (u32)EWRAM_END) {
        return NULL;
      }
      allocator_state[0] = next_address;
      *(u32 *)((u8 *)allocator_state + kind_offset) = address;
      return (void *)address;
    }
    allocator_state[1] = next;
    *(u32 *)((u8 *)allocator_state + kind_offset) = cached_address;
    return (void *)cached_address;
  }
  return (void *)cached_address;
}

void *HeapSlot_AllocEwram(s32 kind, s32 size) {
  u32 *allocator_state;
  s32 kind_offset;
  u32 aligned_size;
  u32 next;
  u32 address;
  u32 next_address;
  u32 cached_address;

  allocator_state = (u32 *)Ram_HeapSlots;
  kind_offset = kind;
  cached_address = *(u32 *)((u8 *)allocator_state + kind_offset);
  if (cached_address == 0) {
    aligned_size = (u32)size + 3;
    address = allocator_state[0];
    aligned_size = (aligned_size >> 2) * 4;
    next = address + aligned_size;
    if (next >= (u32)EWRAM_END) {
      address = allocator_state[1];
      next_address = address + aligned_size;
      if (next_address > (u32)(Ram_IwramHeapEnd - 1)) {
        return 0;
      }
      allocator_state[1] = next_address;
      *(u32 *)((u8 *)allocator_state + kind_offset) = address;
      return (void *)address;
    }
    allocator_state[0] = next;
    *(u32 *)((u8 *)allocator_state + kind_offset) = address;
    return (void *)address;
  }
  return (void *)cached_address;
}

void Heap_FillFreeWords(u32 value) {
  u32 *state = (u32 *)Ram_HeapSlots;
  u32 address;

  address = state[1];
  Iwram_FillWords((void *)address, (u32)Ram_IwramHeapEnd - address, value);
  address = state[0];
  Iwram_FillWords((void *)address, (u32)EWRAM_END - address, value);
}

void *Heap_Alloc(s32 size) {
  u32 *state = (u32 *)Ram_HeapSlots;
  u32 other;
  u32 next;
  u32 address;
  u32 words;
  u32 limit;

  words = (u32)size + 3;
  address = state[1];
  words >>= 2;
  limit = (u32)(Ram_IwramHeapEnd - 1);
  size = (s32)(words << 2);
  next = address + (u32)size;
  if (next > limit) {
    address = state[0];
    other = address + (u32)size;
    if (other >= (u32)EWRAM_END) {
      return NULL;
    }
    state[0] = other;
    goto finish;
  }
  state[1] = next;
finish:
  return (void *)address;
}

void *Heap_AllocEwram(s32 size) {
  u32 *state = (u32 *)Ram_HeapSlots;
  u32 other;
  u32 next;
  u32 address;
  u32 words;

  words = (u32)size + 3;
  address = state[0];
  size = (s32)((words >> 2) << 2);
  next = address + (u32)size;
  if (next >= (u32)EWRAM_END) {
    address = state[1];
    other = address + (u32)size;
    if (other > (u32)(Ram_IwramHeapEnd - 1)) {
      return NULL;
    }
    state[1] = other;
    goto finish;
  }
  state[0] = next;
finish:
  return (void *)address;
}
