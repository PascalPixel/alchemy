#include "types.h"
#include "dma.h"
#include "resource.h"
#include "ram_buffer.h"
#include "bg_scroll.h"
#include "field/world_map.h"

extern char ResourceId_Data023, ResourceId_Data024;

/// Draws the world map picture on BG1, scrolled to the cursor in gWorldMapWork
/// (resource 36, or 35 at the origin when the halfword at 0x640 is set): the
/// palette, then 21 rows of 31 tiles from the decoded rows, wrapping the 24-row
/// buffer and the 32-column screen.
void Func_080ece20(void) {
  struct WorldMapWork *work = &gWorldMapWork;
  u8 *buffer;
  s32 resource = (s32)&ResourceId_Data024;
  s32 *cursor = work->cursor;
  s32 x = *cursor++ - (120 << 16);
  s32 z = *cursor - (80 << 16);
  u32 *graphics;
  u32 column;
  u32 firstRow;
  u32 row;
  s32 bufferRow;
  s32 unwrapped;
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
  buffer = Ram_MapCellBuffer;
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
  x = (u32)x >> 19;
  column = x;
  firstRow = (u32)z >> 19;
  for (row = 0; row <= 20; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    /* FAKEMATCH(barrier): an empty asm reading x, which keeps it live over the
       row; as no asm, 78 of 149 instructions differ in tla-en. */
    asm("" : : "r"(x));
    if (bufferRow > 23)
      bufferRow -= 24;
    z = (column & 31) + (unwrapped << 5);
    /* FAKEMATCH(temporary): z reused for the tile; as a local of its own, 108
       of 149 instructions differ in tla-en. */
    z = (z + 128) % 768 + 256;
    Resource_DecodeType01((u8 *)graphics + graphics[unwrapped + 1],
                          buffer + (bufferRow << 12));
    /* FAKEMATCH(temporary): x reused for the cell pointer; as a local of its
       own, 125 of 149 instructions differ in tla-en. */
    x = bufferRow << 6;
    x += column;
    {
      s32 to;

      i = 0;
      to = 0x06000000;
      x <<= 6;
      x += (s32)buffer;
      /* FAKEMATCH(barrier): a tied empty asm holding VRAM's address and reading
         i; as the literal, 4 of 149 instructions differ in tla-en. */
      asm("" : "=r"(to) : "0"(to), "r"(i));
      for (; i <= 30; i++) {
        Dma_Set((void *)x, (void *)((z << 6) + to), 0x84000010,
                (volatile u32 *)0x040000d4);
        x += 64;
        z++;
        if (i + column == 31)
          z -= 32;
      }
    }
  }
}
