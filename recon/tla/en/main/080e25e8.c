// Draft, differ 162 of 145: copies the map cells an effect touched into a
// tile rectangle. For each cell in the event work's list at 0xcc0 (ending
// in -1) whose position relative to the given origin, the map layer's
// scroll and the controlled owner's terrain height falls inside width by
// height, Func_080201f0 copies that one tile, the cell's layer-2 entry
// clears its byte 2, and up to 64 copies are recorded in gData_02001000's
// list at 0x174c (count at 0x194c). Left: other. The native reads the
// cell's x afresh at the top of each pass and rebuilds the record's
// offsets from the base (0x194c, 0x174c + n * 8, 0x1750 + n * 8), keeping
// the four copied coordinates in r6, r8, r10, r7; this CSEs the first x
// into the loop and holds a single record pointer, so allocation differs
// throughout. Written over a byte array the draft measured 156; the
// struct is the native's addressing. A while loop measured the same.
#include "types.h"
#include "object_runtime.h"
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
struct ObjectRuntime *Object_GetById(u32 actor);
void Func_080201f0(s32 toX, s32 toZ, s32 width, s32 height, s32 fromX,
                   s32 fromZ);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

void Func_080e25e8(s32 toX, s32 toZ, s32 width, s32 height, s32 originX,
                   s16 originZ, s32 layerIndex) {
  u8 *event = Ram_HeapSlots->eventWork;
  s32 ownerHeight =
      Object_GetById(Party_GetLeaderActor())->terrainHeight >> 20;
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  struct TouchLayer *layer =
      &((struct TouchMap *)Ram_HeapSlots->mapWork)->layers[layerIndex];

  for (; cell->x != -1; cell++) {
    s32 x = cell->x - originX + (layer->x >> 20);
    s32 z = cell->z - originZ - ownerHeight + (layer->z >> 20);

    if (x >= 0 && x < width && z >= 0 && z < height) {
      s32 destX = toX + x;
      s32 destZ = toZ + z;
      s32 fromX = originX + x;
      s32 fromZ = originZ + z;

      Func_080201f0(destX, destZ, 1, 1, fromX, fromZ);
      Func_080dbb78(cell->x << 20, cell->z << 20, 2)->unk02 = 0;
      if (gData_02001000.count < 64) {
        struct TouchCopy *copy = &gData_02001000.copies[gData_02001000.count];

        copy->toX = destX;
        copy->toZ = destZ;
        copy->fromX = fromX;
        copy->fromZ = fromZ;
        copy->cellX = cell->x;
        copy->cellZ = cell->z;
        gData_02001000.count++;
      }
    }
  }
}
