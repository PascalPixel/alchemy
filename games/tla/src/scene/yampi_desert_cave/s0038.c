#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "macros.h"
#include "event_inline.h"

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];


s32 Func_02000038(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 sprite = ((struct ObjectAnimationState *)object->animation)->value28[0];
  s16 i;
  u32 kind;
  s32 w, h, x, z;
  s32 az;
  s32 bx, bz;
  s32 toX, toZ, fromX, fromZ;
  s32 layer1X, layer1Z;

  i = 0;
  if ((u16)sprite != gData_02001968[i]) {
  loop:
    /* FAKEMATCH(temporary): kind set to i, then stepped; as kind = (u16)(i +
     * 1), 12 of 160 instructions differ in tla-en. */
    kind = (u16)i;
    kind = (u16)(kind + 1);
    i = (s16)kind;
    if (kind <= 5 && (u16)sprite != gData_02001968[kind])
      goto loop;
  }
  kind = (u16)i;
  if (kind == 6)
    return 0;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  // i is reused: the footprint's second byte here, layers[1]'s x offset in
  // cells below.
  i = gData_02001974[kind][1];
  h = (ABS(i) + ABS(gData_02001974[kind][3])) >> 4;
  x = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  z = (object->z + (i << 16)) >> 20;
  i = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
  /* FAKEMATCH(temporary): the first call's last two arguments held in locals;
   * as direct arguments, 36 of 160 instructions differ in tla-en. */
  layer1X = (u16)i + x;
  layer1Z = (u16)az + z;
  Map_CopyCellAttributeRect(x, z, w, h, layer1X, layer1Z);
  /* FAKEMATCH(temporary): the second call's arguments held in locals; as direct
   * arguments, 10 of 160 instructions differ in tla-en. */
  fromX = (u16)(bx >> 20) + x;
  fromZ = (u16)(bz >> 20) + z;
  toX = x + 64;
  toZ = z + 64;
  Map_CopyCellAttributeRect(fromX, fromZ, w, h, toX, toZ);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}

#include "../../field/map/cellfill.c"
