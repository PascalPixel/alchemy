// Matches: differ 0 (tla-en, 2026-11-08)
// Every edition measures differ 0 of 169: tla-en, tla-ja and tla-it at
// 080e2774; tla-de (080ef774), tla-es (080eb768) and tla-fr (080e7768) by
// --symbol Func_080e2774. Plain C, no device. What settled the last 10:
// TouchCopies laid out as Func_080e25e8 has it, a TouchCell array before the
// copies (a u8 pad gave the record stores a char alias set disjoint from the
// cell's s16, so sched2 hoisted the cellZ load above its address; 10 to 8),
// and both layer shifts named before either subtraction, so the shifted
// layer z overlaps the shifted x and takes r2 in place (8 to 0). Shape that
// mattered before: an index loop (a pointer loop lets GCSE carry the test's
// load of x into the body), the layer pointer reloaded after the branch, cz
// declared before cx but loaded after it (z takes r6), cellX computed before
// the cell's z is loaded, the offsets dx/dz with the copied coordinates
// written in the call and again in the record, and the count read once
// into n.
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
  struct TouchCell unk0000[1491];
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
  s32 layerX, layerZ;
  s32 left, top, i;

  if (layerIndex <= 2)
    layer += layerIndex;
  layerX = layer->x >> 20;
  layerZ = layer->z >> 20;
  left = x - layerX;
  top = z - layerZ;
  lift = -(owner->terrainHeight >> 20);

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
