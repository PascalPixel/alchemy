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

  /* FAKEMATCH(signature): gData_02001000 typed as 1491 TouchCells before the
   * copies, which it does not hold, so the cell's z is read after the last
   * copy's address is built; as u8 unk0000[0x174c], 2 of 169 instructions
   * differ in tla-en. */
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
