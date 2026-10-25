#include "types.h"
#include "mem.h"
#include "ram_buffer.h"

struct Panel_02001804 {
  u8 unk00[8];
  u16 stride;
  u8 unk0a[2];
  s16 x;
  s16 y;
};

void Func_0200187c(struct Panel_02001804 *panel) {
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  register void *work asm("r0") = Runtime_BumpAllocateAlternatePool(0x300);
  u16 *tiles;
  s32 offset;
  register s32 row asm("r6");
  s32 column;

  offset = panel->y * 32 + panel->x;
  {
    register u32 vram asm("r2") = 0x06002000;
    __asm__ __volatile__("" : "+r"(offset));

    offset *= 2;
    tiles = (u16 *)(offset + vram);
  }
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  row = 0;
next_row:
  column = 0;
next_column:
  // One cell, the same tile in the map and in the shadow.
  {
    s16 tile = (row * 16 + column + 32) | 0xf000;

    *tiles++ = tile;
    *shadow++ = tile;
  }
  column++;
  if (column <= 15)
    goto next_column;
  tiles += 16;
  shadow += 16;
  row++;
  if (row <= 7)
    goto next_row;
  MemFree(work);
}
