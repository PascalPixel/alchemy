// Draft, differ 189 of 189: draws Func_020017f8's cell markers (gData_02002aa0's
// 128 records): copies the eight marker pictures into their VRAM block,
// then for each live record on screen fills its OAM entry (the record's
// kind picks the picture and kind 2 is flipped; palette 15 when the marker
// is at its resting height and not in state 2, else 14) and draws it with
// Func_02001c3c. Left: a rough reading: the native builds each transfer's control and VRAM addresses from the literal pool afresh and keeps the scroll and tile in r8, r9 and r11 and the loop count in r10; the alignment is poor.
#include "types.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[128];
};

extern struct CellMarkers gData_02002aa0;
void Func_02001c3c(u32 *oam, s32 depth);

#define MARKER_COPY(from, to)                                                  \
  Dma_Set((void *)(from), (void *)(offset + (to)), 0x84000010,                 \
          (volatile u32 *)0x040000d4)

void Func_020015f8(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 scrollX = *(s32 *)(map + 0xe4) & 0xffff0000;
  s32 scrollZ = *(s32 *)(map + 0xe8) & 0xffff0000;
  s32 lift = (*(s32 **)map)[1];
  struct CellMarker *marker;
  u32 offset = gResourceTableEntries[gData_02002aa0.block].offset;
  u32 tile = offset >> 5;
  s32 i, x, y, palette, flip;

  MARKER_COPY(0x0600f000, 0x06010000);
  MARKER_COPY(0x0600f400, 0x06010040);
  MARKER_COPY(0x0600f040, 0x06010080);
  MARKER_COPY(0x0600f440, 0x060100c0);
  MARKER_COPY(0x0600f080, 0x06010100);
  MARKER_COPY(0x0600f480, 0x06010140);
  MARKER_COPY(0x0600f080, 0x06010180);
  MARKER_COPY(0x0600f480, 0x060101c0);
  marker = gData_02002aa0.markers;
  for (i = 127; i >= 0; i--, marker++) {
    if (marker->state == 0)
      continue;
    x = (marker->x - scrollX) >> 16;
    y = ((marker->z - scrollZ) - marker->y) >> 16;
    if (marker->y == marker->rest && marker->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    flip = marker->kind == 2 ? 0x10000000 : 0;
    marker->oam[0] = 0;
    marker->oam[1] =
        ((y - 8) & 255) | (((x - 8) & 0x1ff) << 16) | flip | 0x40000400;
    marker->oam[2] = palette | (tile + marker->kind * 4);
    Func_02001c3c(marker->oam, 0);
  }
}
