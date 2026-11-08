// Matches: differ 0 (tla-en, 2026-11-08); differ 0 in tla-ja, tla-de, tla-es,
// tla-fr and tla-it too, with one tagged dead device.
// The missing copy of the OAM address came from attr's second set: the entry
// is written through *attr++ and the call takes marker->oam again, so GCSE
// cannot reuse attr for the call and keeps the address in a second pseudo (the
// native's r7, copied to r2 for the stores). attr[0] with marker->oam[1] and
// [2], a separate oam and attr, marker->oam repeated, an inline helper and
// once-only blocks around attr all folded the two into one pseudo (19 to 31).
// The block's order then settles the rest: left masked, top masked, attr, kind
// (with the device: attr before top 2, attr after kind 12 to 45, kind before
// top 20, top before left 16, attr before left 94).
// The device: a dead self-update of a global after left &= 0x1ff lengthens the
// lives that cross it (3 insns in the allocators' view; reload drops the pair)
// so viewZ outranks i (r8 before sl) and y outranks left. The once-only block
// that used to hold it is no longer needed. Without the device this form is
// 82: i set before the copies 114, marker set before them 104, marker set in
// the loop's header or marker++ before i-- 82, a loop from labels 120.
// Lead for removing the device, differ 16 with none: i = 127 set before marker,
// then "y = dy >> 16; x = dx >> 16; clear = 0; left; top;" and "kind; top &=
// 255; left &= 0x1ff; attr; flip = 0; picture;" gives the native's allocation;
// what remains is scheduling: the masks interleave with the kind load (255 takes
// r2 because kind is loaded first) and i's mov sl precedes marker's adds. All
// 30 orders of the five coordinate lines and all 360 orders of the six block
// lines (kind before picture) were measured on that form, none below 16; the
// device placed anywhere in it gives 30 or worse.
// Settled forms: viewY is subtracted from both height and z and cancels in
// combine, but stays live in the stale flow data, spills, and leaves the
// native's otherwise unread store to sp; the block's offset is read once through
// the struct and once as u16 so the two ldrh stay apart; flip is set before an
// if with no else (a ternary or if/else gives 37).
// Draws Func_020017f8's cell markers (gData_02002aa0's 128 records): copies the
// eight marker pictures into their VRAM block, then for each live record on
// screen fills its OAM entry (the record's kind picks the picture and kind 2 is
// flipped horizontally; palette 14 at priority 3 when the marker is at its
// resting height and not in state 2, else priority 2) and draws it with
// Func_02001c3c.
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
    left &= 0x1ff;
    /* FAKEMATCH(dead): the form kept; as nothing, 82 of 189 instructions differ in tla-en. */
    Ram_HeapSlots->mapWork += 0;
    top &= 255;
    attr = marker->oam;
    kind = marker->kind;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    *attr++ = clear;
    *attr++ = top | (left << 16) | flip | 0x40000400;
    *attr = palette | (tile + picture);
    Func_02001c3c(marker->oam, 0);
  }
}
