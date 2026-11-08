// Differ 19 of 189 (tla-en, 2026-11-08, -mtune=gba); 8 November round: the device is still
// needed (29 without it, any global works); top masked before the kind load (87), attr
// set before kind (21), the store through marker->oam (121), a copy of attr for the
// store, attr outside the loop (25), the store in its own once-only block, and an
// 8-minute permute (best 25) found nothing below 19. Earlier: a dead self-update of a
// global after left &= 0x1ff (tagged; any global works, a local or a second
// once-only block does not) puts i and viewY back in r8 and sl, from 29. Left:
// only the missing copy of the OAM address (below). Without the once-only block
// 21; the update before kind 19, after attr 89; a separate oam and attr 19; an
// 8-minute permute from 19 found nothing better.
// Before: differ 29, allocation only: i declared first (29), z as
// z - viewY - viewZ (157), attr before kind (31); direct gData_02002aa0 (29), view
// without its pointer (57), permuter 5 minutes (35).
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
// more lines were the hi-register moves, now closed.
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
    // A once-only block: its end holds the order around the statement (31 of 189
    // without it).
    do {
      left &= 0x1ff;
      /* FAKEMATCH(dead): the form kept; as nothing, 29 of 189 instructions differ in tla-en. */
      Ram_HeapSlots->mapWork += 0;
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
