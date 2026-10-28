// Draft, differ 134 of 160: Yampi Desert Cave, finds a pushed object's kind
// by its sprite and copies its footprint between the map layers, then marks
// its cells. Left: the kind search's loop form and register choice.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001848(s32 x, s32 z, s32 w, s32 h, s32 toX, s32 toZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 w, s32 h, s32 value);

s32 Func_02000038(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = Object_GetById(id);
  s16 sprite = ((struct ObjectAnimationState *)object->animation)->value28[0];
  s16 i;
  u16 kind;
  s32 w, h, x, z;
  u16 ax, az;
  s32 bx, bz;

  i = 0;
  if ((u16)sprite != gData_02001968[i]) {
  loop:
    i++;
    if ((u16)i <= 5 && (u16)sprite != gData_02001968[(u16)i])
      goto loop;
  }
  kind = i;
  if (kind == 6)
    return 0;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
  x = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  z = (object->z + (gData_02001974[kind][1] << 16)) >> 20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
  Func_02001848(x, z, w, h, ax + x, az + z);
  Func_02001848((u16)(bx >> 20) + x, (u16)(bz >> 20) + z, w, h, x + 64, z + 64);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}
