// Draft, differ 6 of 60: overlay 692's copy of the function drafted in
// resource_68f_c_02002c1c.c (overlay 68f's Func_02002c1c, differ 6 there), whose
// header says what it is, what still differs and what was tried.
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"

extern u8 gMapShapeGrid[];

s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);

void Func_02004934(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = gMapShapeGrid + (Ram_HeapSlots->mapWork->layers[layer].cells -
                               (struct MapCell *)Ram_MapCellBuffer);
  s32 value = Func_02004d20(object->terrainId, object->x, object->z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  height += level;

  shape += object->x / 0x100000 + object->z / 0x100000 * 128;
  *shape = Func_02004ef0(object->terrainId, value, height);
}
