// Matches: differ 0 (tla-en, 2026-11-01). Copies the map cells an effect
// touched into a tile rectangle. For each cell in the event work's list at
// 0xcc0 (ending in -1) whose position relative to the given origin, the map
// layer's scroll and the controlled owner's terrain height falls inside width
// by height, Func_080201f0 copies that one tile, the cell's layer-2 entry
// clears its byte 2, and up to 64 copies are recorded in gData_02001000's
// list at 0x174c (count at 0x194c). No S2 device. What mattered: an index
// loop (a pointer loop lets GCSE carry the test's load of x into the body);
// x and z taken from the origin first and the scrolls added after; a `map`
// local; `owner` live across the cell address, with the height read after it
// (that keeps r0 busy, so the 0xcc0 reload takes r2 and the layer's scratch
// registers follow). The last pair, the cellZ address before the load of z:
// struct TouchCell must be a member type of struct TouchCopies, whose alias
// set then covers it, so the load gets a true dependence on the cellX store
// and sched2 delays it. As `u8 unk0000[0x174c]` the draft differs by that
// pair, 2 of 145. Where the cell type really sits in this work area is
// unknown: the leading array stands for it and its size is only the offset.
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

void Func_080e25e8(s32 toX, s32 toZ, s32 width, s32 height, s32 originX,
                   s16 originZ, s32 layerIndex) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 ownerHeight = owner->terrainHeight >> 20;
  struct TouchMap *map = (struct TouchMap *)Ram_HeapSlots->mapWork;
  struct TouchLayer *layer = &map->layers[layerIndex];
  s32 i;

  for (i = 0; cell[i].x != -1; i++) {
    s32 x = cell[i].x - originX;
    s32 z = cell[i].z - originZ - ownerHeight;

    x += layer->x >> 20;
    z += layer->z >> 20;

    if (x >= 0 && x < width && z >= 0 && z < height) {
      s32 n;

      Func_080201f0(toX + x, toZ + z, 1, 1, originX + x, originZ + z);
      Func_080dbb78(cell[i].x << 20, cell[i].z << 20, 2)->unk02 = 0;
      n = gData_02001000.count;
      if (n < 64) {
        gData_02001000.copies[n].toX = toX + x;
        gData_02001000.copies[n].toZ = toZ + z;
        gData_02001000.copies[n].fromX = originX + x;
        gData_02001000.copies[n].fromZ = originZ + z;
        gData_02001000.copies[n].cellX = cell[i].x;
        gData_02001000.copies[n].cellZ = cell[i].z;
        gData_02001000.count++;
      }
    }
  }
}
