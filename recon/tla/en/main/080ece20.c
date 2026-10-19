// First draft of Func_080ece20: draws the world map picture on BG1, scrolled
// to the cursor in gWorldMapWork (resource 36, or 35 at the origin when the
// halfword at 0x640 is set): the palette, then 21 rows of 31 tiles from the
// decoded rows, wrapping the 24-row buffer and the 32-column screen. ☀️'s
// Map_ShowWorldMap (games/tbs/src/object/psynergy_effects.c) shows its map
// as one picture instead. Differ 140 of 149 (draft 147), most of it register
// choice that follows from one hoist: this draft keeps gMapCellBuffer in r11
// across the rows and column & 31 on the stack, where the native keeps column
// & 31 in r11, reloads gMapCellBuffer from the pool at each use and spills
// the unwrapped row around __modsi3. The clamps also take x and z in the
// other registers (x in r5 in the native). Shape otherwise as the listing.
// 2026-10-19: the forms that matched Func_080ecf74
// (games/tla/src/battle/effect/world_map_row.c: the tile in two steps, the cell
// as an index incremented with the tile, i + column) give 146 here: the same
// shape, but x and z, the tile and the cell pointer take other registers, and
// the native spills the unwrapped row around __modsi3.
// Now 108: the clamp bounds as locals (as in world_map_scroll.c), the tile in z
// once the clamps are done with it (the permuter's find, from 136), the cell as
// an index and the resources by their directory names. Left: register choice
// from the prologue (x and z take r6 and r5 here, r5 and r6 in the native) and
// the native's spill of the unwrapped row.
#include "types.h"
#include "dma.h"
#include "resource.h"
#include "ram_buffer.h"
#include "bg_scroll.h"

struct WorldMapWork {
  u8 padding00[0x62c];
  s32 *cursor;
  u8 padding630[0x10];
  s16 centered;
};

extern struct WorldMapWork gWorldMapWork;
extern char ResourceId_Data023, ResourceId_Data024;

void Func_080ece20(void) {
  struct WorldMapWork *work = &gWorldMapWork;
  s32 *cursor = work->cursor;
  s32 x = *cursor++ - (120 << 16);
  s32 z = *cursor - (80 << 16);
  s32 resource = (s32)&ResourceId_Data024;
  u32 *graphics;
  u32 column;
  u32 firstRow;
  u32 row;
  s32 bufferRow;
  s32 unwrapped;
  s32 cell;
  u32 i;
  s32 right;
  s32 bottom;

  if (work->centered != 0) {
    resource = (s32)&ResourceId_Data023;
    x = 0;
    z = 0;
  }
  graphics = Resource_GetTableEntry(resource);
  Dma_Set((u8 *)graphics + graphics[0], (void *)0x05000000, 0x84000078,
          (volatile u32 *)0x040000d4);
  *(u16 *)0x05000000 = 0;
  right = 272 << 16;
  bottom = 224 << 16;
  if (x < 0)
    x = 0;
  if (x > right)
    x = right;
  if (z < 0)
    z = 0;
  if (z > bottom)
    z = bottom;
  gBgScroll[1].x = x >> 16;
  gBgScroll[1].y = z >> 16;
  column = (u32)x >> 19;
  firstRow = (u32)z >> 19;
  for (row = 0; row <= 20; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    if (bufferRow > 23)
      bufferRow -= 24;
    z = (column & 31) + (unwrapped << 5);
    z = (z + 128) % 768 + 256;
    Resource_DecodeType01((u8 *)graphics + graphics[unwrapped + 1],
                          Ram_MapCellBuffer + (bufferRow << 12));
    cell = (bufferRow << 6) + column;
    for (i = 0; i <= 30; i++) {
      Dma_Set(Ram_MapCellBuffer + (cell << 6), (void *)(0x06000000 + (z << 6)),
              0x84000010, (volatile u32 *)0x040000d4);
      cell++;
      z++;
      if (i + column == 31)
        z -= 32;
    }
  }
}
