// Differ 2 of 274 (tla-en, 2026-11-08, -mtune=gba).
// One sched2 order left at the end of the preheader: the listing stores the
// spilled d3c address (str r3, [sp, #12]) before `ldr r7, [sp, #24]`; here the
// load comes first. 2026-11-08 under -mtune=gba: point set before the header
// stores, at its declaration or in the for (2 to 4, or unchanged), i as s32
// (4), the 0x1b34 store after the -1 stores (26). Before, on 2026-11-07: for-init,
// do-while, event->points, barriers and the permuter (25,681 candidates) left
// it; an empty asm cost 36.
// 2026-11-08, round two: point set in the for, by the d3c store or in a
// once-only block, the locals reordered, i < 21, (u16)-1, point ending the
// list, and 7 permuter minutes (42,564 candidates) gave 2 to 212.
// Closed: pool -1 comes from the s16 pointer, movs -1 and 0xffff from the
// struct's fields; `point` is stepped after work's byte, and the second
// coordinate goes through its own cursor set after the first store. A hand
// index beside count breaks i's giv. The three `(field & 0xfff0) + 8` reads
// match under -mtune=gba.
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
  s16 *point;
  s16 *tile;

  work[0x1b34] = count;
  event->unk_d3c = 0xffff;
  event->unk_d3e = -1;
  event->unk_d40 = -1;
  event->unk_d42 = -1;
  point = points;
  for (i = 0; i <= 20; i++) {
    x = owner->x + (Data_080f0f64[i * 2] << 20);
    z = owner->z + (Data_080f0f64[i * 2 + 1] << 20);
    kind = Func_080dbda8(x, z, owner->terrainId);
    position[0] = x;
    position[2] = z;
    if (kind == 1 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      tile = point;
      *tile = x / 0x100000;
      tile++;
      *tile = z / 0x100000;
      count++;
      work[0x1b34] = kind;
      point += 2;
    }
    if (kind == 2 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d3c = x >> 16;
      event->unk_d3e = z >> 16;
      event->unk_d3c = (event->unk_d3c & 0xfff0) + 8;
      event->unk_d3e = (event->unk_d3e & 0xfff0) + 8;
      tile = point;
      *tile = x / 0x100000;
      tile++;
      *tile = z / 0x100000;
      point += 2;
      count++;
    }
    if (kind == 3 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d40 = x >> 16;
      event->unk_d42 = z >> 16;
      event->unk_d40 = (event->unk_d40 & 0xfff0) + 8;
      event->unk_d42 = (event->unk_d42 & 0xfff0) + 8;
      tile = point;
      *tile = x / 0x100000;
      tile++;
      *tile = z / 0x100000;
      point += 2;
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}
