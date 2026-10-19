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

// Unpacks source into a scratch block, then writes a 16 by 8 block of tiles
// at the panel's cell into the BG map at 0x06002000 and into the window
// tilemap's shadow: palette 15, the tile of each cell the row times the
// panel's stride plus the column. The loops are the listing's: no loop pass
// touches them, so the tile is rebuilt and the mask reloaded on every pass.
void Func_02001804(struct Panel_02001804 *panel, s32 sourceArg) {
  /* FAKEMATCH(pin): the shadow pointer in r5; as a plain local, 7 of 53
   * instructions differ in tla-en. */
  register u16 *shadow asm("r5") = (u16 *)Ram_HeapSlots->windowTiles;
  /* FAKEMATCH(pin): the source copied into r6 after the shadow load; as the
   * plain parameter, 2 of 53 instructions differ in tla-en. */
  register s32 source asm("r6") = sourceArg;
  void *work = Runtime_BumpAllocateAlternatePool(0x300);
  u16 *tiles;
  s32 offset;
  s32 row;
  s32 column;

  Func_020021f8(source, work);
  offset = panel->y * 32 + panel->x;
  {
    /* FAKEMATCH(temporary): the map's base loaded before offset doubles,
     * with the barrier; as 0x06002000 + offset, 2 of 53 instructions
     * differ in tla-en. */
    u32 vram = 0x06002000;
    /* FAKEMATCH(barrier): an empty asm on offset that keeps the base's load
     * ahead of the doubling; without it, 2 of 53 instructions differ in
     * tla-en. */
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
    s16 tile = (panel->stride * row + column) | 0xf000;

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
