// Draft, differ 238 of 169: other, mostly allocation. The native stack frame is
// 44 bytes against 36: it keeps the left and top edges, the scroll, the
// lift and the three stack arguments on the stack and the four copied
// coordinates in r8 to r11; this draft holds the edges in registers and
// shifts layer 0's scroll once where the native shifts it again for the
// edges.
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

void Func_080e2774(s32 x, s32 z, s16 layerIndex, s16 width, s16 height,
                   s32 toX, s32 toZ) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner =
      Actor_GetById(Party_GetLeaderActor());
  struct TouchLayer *layer =
      ((struct TouchMap *)Ram_HeapSlots->mapWork)->layers;
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 layerX = layer->x;
  s32 layerZ = layer->z;
  s32 scrollX = layerX >> 20;
  s32 scrollZ = layerZ >> 20;
  s32 left, top, lift;

  if (layerIndex <= 2) {
    layerX = layer[layerIndex].x;
    layerZ = layer[layerIndex].z;
  }
  left = x - (layerX >> 20);
  top = z - (layerZ >> 20);
  lift = -(owner->terrainHeight >> 20);

  for (; cell->x != -1; cell++) {
    s32 cellX = cell->x - scrollX;
    s32 cellZ = cell->z + lift - scrollZ;

    if (cellX >= left && cellX < left + width && cellZ >= top &&
        cellZ < top + height) {
      s32 destX = toX + (cellX - left);
      s32 destZ = toZ + (cellZ - top);
      s32 fromX = x + (cellX - left);
      s32 fromZ = z + (cellZ - top);

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
