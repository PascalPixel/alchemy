#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"

extern char ResourceId_WindowTiles;
extern u16 *gData_030011c4;

/// Blanks the debug text screen (screen block 4), homes the debug text cursor
/// and points BG0 at the screen.
// As ☀️'s DebugText_Clear.
void Func_08014b70(void) {
  volatile u32 fill;
  u32 blank;

  /* FAKEMATCH(one-pass): the fill word set in a block of its own, so it loads
   * from the pool before the prologue's sub sp; as fill = 0xf000f000, 2 of 22
   * instructions differ in tla-en. */
  do {
    blank = 0xf000f000;
  } while (0);
  fill = blank;
  Dma_Set((const void *)&fill, (void *)0x06002000, 0x85000140, REG_DMA3);
  /* FAKEMATCH(one-pass): the cursor store in a block of its own, so BG0CNT's
   * address is built after it; as a plain store, 4 of 22 instructions differ
   * in tla-en. */
  do {
    gData_030011c4 = (u16 *)0x06002000;
  } while (0);
  {
    s32 control = 0x400;

    *(volatile u16 *)0x04000008 = control;
  }
}

void Ui_LoadWindowGraphics(void) {
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles),
          (void *)0x06000000, 0x84000800, REG_DMA3);
  Dma_Set((const void *)gUi_WindowPalette, (void *)0x050001e0, 0x80000010,
          REG_DMA3);
  /* FAKEMATCH: the do-while keeps the backdrop store's value before its
   * address. */
  do {
    s32 black = 0;
    PLTT_BACKDROP = black;
  } while (0);
  {
    volatile u16 *pal;
    s32 color;

    color = 0x4180;
    pal = (volatile u16 *)0x050001e8;
    *pal = color;
    color = 0x3960;
    pal++;
    *pal = color;
    color = 0x3140;
    pal++;
    *pal = color;
    color = 0x2920;
    pal++;
    *pal = color;
    color = 0x49a0;
    pal++;
    *pal = color;
    color = 0x51c0;
    pal++;
    *pal = color;
    color = 0x59e0;
    pal++;
    *pal = color;
  }
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)0x05000200,
          0x800000e0, REG_DMA3);
}

void Palette_LoadBasicObjColors(void) {
  Dma_Set((const void *)gSystem_BasicColorPalette, (void *)0x05000200,
          0x800000e0, REG_DMA3);
}

void Runtime_InitializeHeap(void) {
  struct HeapSlots *work = Ram_HeapSlots;

  Dma_Fill32(0, work, sizeof(struct HeapSlots));
  Ram_HeapSlots->nextIwram = gIwramHeap;
  Ram_HeapSlots->nextEwram = gEwramHeap;
}

extern u8 IwramSoundMixWorkspace[];

s32 Runtime_GetRemainingIwram(void) {
  struct HeapSlots *work = Ram_HeapSlots;

  return (s32)IwramSoundMixWorkspace - (s32)work->nextIwram;
}

s32 Runtime_GetRemainingEwram(void) {
  struct HeapSlots *work = Ram_HeapSlots;

  return 0x02040000 - (s32)work->nextEwram;
}

void *Runtime_AllocateHeapBlock(s32 kind, s32 size) {
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
      if (next_address >= 0x02040000U) {
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

void *Runtime_AllocateBlock(s32 kind, s32 size) {
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
    /* FAKEMATCH: load the heap head before shifting the alignment value. */
    __asm__("" : : "r"(address), "r"(aligned_size));
    aligned_size = (aligned_size >> 2) * 4;
    next = address + aligned_size;
    if (next >= 0x02040000U) {
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

void Runtime_FillFreeHeapWords(u32 value) {
  u32 *state = (u32 *)Ram_HeapSlots;
  u32 address;

  address = state[1];
  Iwram_FillWords((void *)address, (u32)Ram_IwramHeapEnd - address, value);
  address = state[0];
  Iwram_FillWords((void *)address, 0x02040000U - address, value);
}

void *MemAlloc(s32 size) {
  u32 *state = (u32 *)Ram_HeapSlots;
  u32 other;
  u32 next;
  u32 address;
  u32 words;
  u32 limit;
  /* FAKEMATCH: the bound captured in r4 keeps the heap and head registers;
   * as plain C, 6 instructions differ in tla-en. */
  register u32 held asm("r4");

  words = (u32)size + 3;
  address = state[1];
  /* FAKEMATCH: keep the unshifted value and incoming size live through the
   * heap-head load. */
  __asm__("" : : "r"(address), "r"(words), "r"(size));
  words >>= 2;
  /* FAKEMATCH: capture the bound before the final alignment shift. */
  __asm__("" : "=r"(held), "+r"(words) : "0"((u32)(Ram_IwramHeapEnd - 1)));
  limit = held;
  size = (s32)(words << 2);
  next = address + (u32)size;
  if (next > limit) {
    address = state[0];
    other = address + (u32)size;
    if (other >= 0x02040000U) {
      return NULL;
    }
    state[0] = other;
    goto finish;
  }
  state[1] = next;
finish:
  return (void *)address;
}

void *Runtime_BumpAllocateAlternatePool(s32 size) {
  u32 *state = (u32 *)Ram_HeapSlots;
  u32 other;
  u32 next;
  u32 address;
  u32 words;

  /* FAKEMATCH: the one-pass block keeps the heap-head load between the size's
   * add and its shifts; as plain statements the ARM9TDMI schedule lifts both
   * shifts above the load (differ 4). */
  do {
    words = (u32)size + 3;
    address = state[0];
  } while (0);
  size = (s32)((words >> 2) << 2);
  next = address + (u32)size;
  if (next >= 0x02040000U) {
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
