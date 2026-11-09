#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

extern const u16 gData_020022d0[];
extern const s8 gData_020022dc[][4];

void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000af0(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)object->animation)->record;
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

  if ((u16)shape != gData_020022d0[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gData_020022d0[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gData_020022dc[j][0]) + ABS(gData_020022dc[j][2])) >> 4;
  height = (ABS(gData_020022dc[j][1]) + ABS(gData_020022dc[j][3])) >> 4;
  x = (object->x + (gData_020022dc[j][0] << 16)) >> 20;
  z = (object->z + (gData_020022dc[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX;
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
