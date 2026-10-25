// Draft, differ 13 of 100: Izumo Ruins' pillar cell update.
#include "../../../../games/tla/src/scene/izumo_ruins/izumo_ruins.h"

void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_02001480(s32 x, s32 z, s32 layer, s32 height);

void Func_02000f30(struct ObjectRuntime *object) {
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 lower = Func_02000eac(0, column, row);
  s32 upper = Func_02000eac(2, column, row);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  if (GameFlag_Test(object->action)) {
    upper |= 0xff00;
    lower &= ~0x80;
    if (object->terrainHeight >> 19 != height)
      height -= 4;
    Object_SetSpritePriority(object, 3);
  } else {
    if (lower >> 8 != 232)
      upper = (upper & 0xff) | ((lower >> 8) << 8);
    else
      upper = (upper & 0xff) | (232 << 8);
    lower |= 0x80;
    height += 4;
    Object_SetSpritePriority(object, 2);
  }
  Func_02000ee8(0, column, row, lower);
  Func_02000ee8(2, column, row, upper);
  Func_02001480(column << 20, row << 20, object->terrainId, height);
}
