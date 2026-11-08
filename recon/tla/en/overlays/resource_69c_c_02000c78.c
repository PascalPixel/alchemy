// Matches (tla-en, 2026-11-08, -mtune=gba; tla-ja, -de, -es, -fr and -it also differ 0).
// Twin of resource_6ad_c_02004938.c (adopted as mars_lighthouse_room_f/s4938.c), written in its
// form: z before the map origin, and i reused to hold mapX (22 to 0).
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"

extern const u16 gData_02002478[];
extern const s8 gData_02002484[][4];

struct Actor *Actor_GetById(s32 id);
void Func_02002330(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height, s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000c78(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
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
  x = (object->x + (gData_02002484[j][0] << 16)) >> 20;
  z = (object->z + (gData_02002484[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX; // i is reused to hold mapX.
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  Func_02002330(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
