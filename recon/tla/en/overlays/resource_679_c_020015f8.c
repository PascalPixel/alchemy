// Draft, differ 29 of 189 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 31 (tla-en); notes below may predate it.
// Draft, differ 133 of 189, skeleton gap 11 (tla-en, 2026-11-01)
// Draws Func_020017f8's cell markers (gData_02002aa0's 128 records): copies the
// eight marker pictures into their VRAM block, then for each live record on
// screen fills its OAM entry (the record's kind picks the picture and kind 2 is
// flipped; palette 15 when the marker is at its resting height and not in state
// 2, else 14) and draws it with Func_02001c3c.
// Settled forms: viewY is subtracted from both height and z and cancels in
// combine, but stays live in the stale flow data, spills, and leaves the
// native's otherwise unread store to sp; the block's offset is read once through
// the struct and once as u16 so the two ldrh stay apart.
// Left: the native holds the OAM entry's address in two registers (r7, copied to
// r2 for the first store and to r0 for the call). CSE crosses the kind == 2 skip
// and folds them into one pseudo here, for a separate oam and attr, for
// marker->oam repeated at the call, *attr++, and an inline helper. That one
// missing register is the rest of the gap: the palette then stays in r7 instead
// of ip, the 0 in ip instead of lr, and top and flip sit one register off. Two
// more lines are the prologue's order of sub sp and the hi-register moves.
// if/else or a ternary for flip is worse (gap 15).
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"
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
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 viewY = map->origin[1];
  struct CellMarkers *markers = &gData_02002aa0;
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[markers->block];
  u32 tile = entry->offset >> 5;
  u32 offset = ((u16 *)entry)[1];
  struct CellMarker *marker;
  s32 i;

  MARKER_COPY(0x0600f000, 0x06010000);
  MARKER_COPY(0x0600f400, 0x06010040);
  MARKER_COPY(0x0600f040, 0x06010080);
  MARKER_COPY(0x0600f440, 0x060100c0);
  MARKER_COPY(0x0600f080, 0x06010100);
  MARKER_COPY(0x0600f480, 0x06010140);
  MARKER_COPY(0x0600f080, 0x06010180);
  MARKER_COPY(0x0600f480, 0x060101c0);
  marker = markers->markers;
  for (i = 127; i >= 0; i--, marker++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 clear;
    u32 *attr;

    if (marker->state == 0)
      continue;
    dx = marker->x - viewX;
    height = marker->y - viewY;
    z = marker->z - viewZ - viewY;
    dy = z - height;
    if (marker->y == marker->rest && marker->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    y = dy >> 16;
    clear = 0;
    x = dx >> 16;
    left = x - 8;
    top = y - 8;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
     * order around it; as the plain statement, 31 of 189 instructions differ
     * in tla-en. */
    do {
      left &= 0x1ff;
    } while (0);
    kind = marker->kind;
    attr = marker->oam;
    top &= 255;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    attr[0] = clear;
    marker->oam[1] = top | (left << 16) | flip | 0x40000400;
    marker->oam[2] = palette | (tile + picture);
    Func_02001c3c(marker->oam, 0);
  }
}
