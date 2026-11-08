// Matches (tla-en, 2026-11-08, -mtune=gba): the point list is indexed by count
// (points[count * 2]) instead of stepped through a cursor, which settles the
// preheader's spill order; the four stores before the loop are plain -1.
//
// Looks at 21 map cells around the effect owner, at the tile offsets
// of Data_080f0f64 (signed x, z byte pairs), on the owner's layer. Each cell
// of kind 1, 2 or 3 that Func_080cda84 accepts for the effect's id is listed
// as a tile in the event work's point list at 0xcc0, which ends with -1, -1;
// a kind-1 cell also stores its kind in the heap slot 0x5c work's byte
// 0x1b34, and the last kind-2 and kind-3 cells are kept in pixels, centred
// on their tile, at 0xd3c and 0xd40.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct PointWork {
  u8 unk_0[0xcc0];
  s16 points[62];
  u16 unk_d3c;
  u16 unk_d3e;
  u16 unk_d40;
  u16 unk_d42;
};

extern const s8 Data_080f0f64[];

s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);

void Func_080e2264(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct PointWork *event = Ram_HeapSlots->eventWork;
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s16 *points = event->points;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 position[3];
  s32 x, z;
  s32 kind;
  u32 i;
  s32 count = 0;

  work[0x1b34] = count;
  event->unk_d3c = -1;
  event->unk_d3e = -1;
  event->unk_d40 = -1;
  event->unk_d42 = -1;
  for (i = 0; i <= 20; i++) {
    x = owner->x + (Data_080f0f64[i * 2] << 20);
    z = owner->z + (Data_080f0f64[i * 2 + 1] << 20);
    kind = Func_080dbda8(x, z, owner->terrainId);
    position[0] = x;
    position[2] = z;
    if (kind == 1 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
      work[0x1b34] = kind;
    }
    if (kind == 2 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d3c = x >> 16;
      event->unk_d3e = z >> 16;
      event->unk_d3c = (event->unk_d3c & 0xfff0) + 8;
      event->unk_d3e = (event->unk_d3e & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
    if (kind == 3 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d40 = x >> 16;
      event->unk_d42 = z >> 16;
      event->unk_d40 = (event->unk_d40 & 0xfff0) + 8;
      event->unk_d42 = (event->unk_d42 & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}
