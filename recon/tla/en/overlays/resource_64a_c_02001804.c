// Draft of Func_02001804 and Func_0200187c (overlay 64A), both far off: the
// native inner loops rebuild the tile from the row and column on every pass
// and reload the 0xfffff000 mask from the pool inside the loop, and
// Func_0200187c keeps the work block in r0 across the loops; this C has the
// tile strength-reduced, the mask hoisted and the block saved. The source
// probably computed the tile through something the loop pass could not see
// through.
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

void Func_020021f8(s32 source, void *work);

void Func_02001804(struct Panel_02001804 *panel, s32 source) {
  u16 *shadow = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Runtime_BumpAllocateAlternatePool(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  Func_020021f8(source, work);
  offset = (panel->y * 32 + panel->x) * 2;
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  tiles = (u16 *)(0x06002000 + offset);
  for (row = 0; row < 8; row++) {
    for (column = 0; column < 16; column++) {
      s16 tile = (panel->stride * row + column) | 0xf000;

      *tiles++ = tile;
      *shadow++ = tile;
    }
    tiles += 16;
    shadow += 16;
  }
  MemFree(work);
}

void Func_0200187c(struct Panel_02001804 *panel) {
  u16 *shadow = (u16 *)Ram_HeapSlots->windowTiles;
  void *work = Runtime_BumpAllocateAlternatePool(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  offset = (panel->y * 32 + panel->x) * 2;
  shadow = (u16 *)((u8 *)shadow + offset + 8);
  tiles = (u16 *)(0x06002000 + offset);
  for (row = 0; row < 8; row++) {
    for (column = 0; column < 16; column++) {
      s16 tile = (row * 16 + column + 32) | 0xf000;

      *tiles++ = tile;
      *shadow++ = tile;
    }
    tiles += 16;
    shadow += 16;
  }
  MemFree(work);
}
