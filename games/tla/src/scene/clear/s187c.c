#include "types.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct Panel_02001804 {
  u8 unk00[8];
  u16 stride;
  u8 unk0a[2];
  s16 x;
  s16 y;
};

void Func_0200187c(struct Panel_02001804 *panel) {
  /* FAKEMATCH(pin): the shadow pointer in r5; as a plain local, 23 of 43
   * instructions differ in tla-en. */
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Heap_AllocEwram(0x300);
  u16 *tiles;
  s32 offset;
  /* FAKEMATCH(pin): the row in r6; as a plain local, 16 of 43 instructions
   * differ in tla-en. */
  register s32 row asm("r6");
  s32 column;

  offset = (panel->y * 32 + panel->x) * 2;
  tiles = (u16 *)(offset + 0x06002000);
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
  Heap_Release(work);
}
