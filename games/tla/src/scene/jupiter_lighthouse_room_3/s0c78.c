/// @file
/// Rewrites the map cells under a pushable actor: its sprite shape picks one of
/// the six footprints in gData_02002484 (four signed bytes each, in sixteenths
/// of a cell); the cells under it are copied from the map's spare area
/// (Map_CopyCellAttributeRect) and byte 2 of both collision layers there is
/// filled with 255. Returns 1, or 0 for a shape with no footprint. The twin of
/// overlay 6ad's Func_02004938.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

extern const u16 gData_02002478[];
extern const s8 gData_02002484[][4];

void Map_FillCellRectByte2(s32 layer, s32 x, s32 y, u32 width, u32 height,
                           s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000c78(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_GetById(id);
  s16 shape = *(s16 *)((struct FieldSprite *)actor->animation)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gData_02002478[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gData_02002478[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gData_02002484[j][0]) + ABS(gData_02002484[j][2])) >> 4;
  height = (ABS(gData_02002484[j][1]) + ABS(gData_02002484[j][3])) >> 4;
  x = (actor->x + (gData_02002484[j][0] << 16)) >> 20;
  z = (actor->z + (gData_02002484[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX; // i is reused to hold mapX.
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  Map_CopyCellAttributeRect(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
