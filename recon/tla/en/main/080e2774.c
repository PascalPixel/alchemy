// Differ 10 of 169 (tla-en, 2026-11-08, -mtune=gba): cellX computed before the
// cell's z is loaded (20 to 10) settles the loop top. Left: the top edge
// (layer z shifted in r3, not in place in r2) and the record's cellZ load
// issued before its address. Tried with it: top before lift (14), lift first
// (71), declarations reordered (10 to 32), cellZ as lift + cz - scrollZ (12),
// top as z then -= (10), the record from cx or cz (41, 27); 20 permuter
// minutes found nothing below 10. Earlier: top first (40), cellZ as
// cz - scrollZ + lift (26), cellZ before cellX (36).
// 8 November: the top edge through a layer-z local, shifted in place or not,
// and every spelling of z - (layer z >> 20) stay at 10 (14 with top before
// lift): the shifted layer z is a block-local pseudo that takes r3 by order,
// while the native's shares r2; the record's cellZ through casts, a
// TouchCopy pointer (84) or a once-only block (66) does not move its load.
// Another 8 permuter minutes found nothing below 10.
// Near miss, differ 20 of 169 (tla-en, 2026-10-31): allocation and
// scheduling only. The top edge: the native shifts layer z in place in r2
// and reloads z into r3; this shifts into r3 and reloads z into r2 (lift
// computed between the edges measured 20, after them 24). The loop top:
// the native reloads lift into r4 and the x scroll into r2 before loading
// the cell's x; this reloads the scroll into r3 first. The record: the
// native forms the cellZ address before loading the cell's z; this loads
// first. Shape that mattered: an index loop (a pointer loop lets GCSE carry
// the test's load of x into the body, 70), the layer pointer reloaded after
// the branch (shifting saved layerX/layerZ lets PRE share the shift with
// the scroll), cz declared before cx but loaded after it (z takes r6), the
// offsets dx/dz with the copied coordinates written in the call and again
// in the record so CSE keeps them (named locals left destZ in r10, 38),
// and the count read once into n.
// Func_080e25e8's sibling with the rectangle given in map cells. For
// each cell in the event work's list at 0xcc0 (ending in -1), taken relative
// to map layer 0's scroll and the controlled owner's terrain height, that
// falls inside the width by height rectangle at (x, z) on the chosen layer
// (layer 0 above 2), Func_080201f0 copies that one tile to (toX, toZ) plus
// its offset, the cell's layer-2 entry clears its byte 2, and up to 64
// copies are recorded in gData_02001000's list at 0x174c (count at 0x194c).
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct TouchCell {
  s16 x;
  s16 z;
};

struct TouchLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[40];
};

struct TouchMap {
  u8 unk000[264];
  struct TouchLayer layers[1];
};

struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unk06[2];
};

struct MapCell {
  u8 unk00[2];
  u8 unk02;
};

struct TouchCopies {
  u8 unk0000[0x174c];
  struct TouchCopy copies[64];
  s8 count;
};

extern struct TouchCopies gData_02001000;

s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(u32 actor);
void Func_080201f0(s32 toX, s32 toZ, s32 width, s32 height, s32 fromX,
                   s32 fromZ);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

void Func_080e2774(s32 x, s32 z, s16 layerIndex, s16 width, s16 height, s32 toX,
                   s32 toZ) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchLayer *layer =
      ((struct TouchMap *)Ram_HeapSlots->mapWork)->layers;
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 lift;
  s32 scrollX = layer->x >> 20;
  s32 scrollZ = layer->z >> 20;
  s32 left, top, i;

  if (layerIndex <= 2)
    layer += layerIndex;
  left = x - (layer->x >> 20);
  lift = -(owner->terrainHeight >> 20);
  top = z - (layer->z >> 20);

  for (i = 0; cell[i].x != -1; i++) {
    s32 cz, cx, cellX, cellZ;

    cx = cell[i].x;
    cellX = cx - scrollX;
    cz = cell[i].z;
    cellZ = cz + lift - scrollZ;

    if (cellX >= left && cellX < left + width && cellZ >= top &&
        cellZ < top + height) {
      s32 dx = cellX - left;
      s32 dz = cellZ - top;
      s32 n;

      Func_080201f0(toX + dx, toZ + dz, 1, 1, x + dx, z + dz);
      Func_080dbb78(cx << 20, cz << 20, 2)->unk02 = 0;
      n = gData_02001000.count;
      if (n < 64) {
        gData_02001000.copies[n].toX = toX + dx;
        gData_02001000.copies[n].toZ = toZ + dz;
        gData_02001000.copies[n].fromX = x + dx;
        gData_02001000.copies[n].fromZ = z + dz;
        gData_02001000.copies[n].cellX = cell[i].x;
        gData_02001000.copies[n].cellZ = cell[i].z;
        gData_02001000.count++;
      }
    }
  }
}
