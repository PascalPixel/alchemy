#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"

s32 Func_02003010(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02003180(s32 layer, s32 shape, s32 height);

void Func_02002c1c(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid) +
              (Ram_HeapSlots->mapWork->layers[layer].cells -
               (struct MapCell *)Ram_MapCellBuffer);
  s32 value = Func_02003010(object->terrainId, object->x, object->z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  height += level;

  shape += object->x / 0x100000 + object->z / 0x100000 * 128;
  *shape = Func_02003180(object->terrainId, value, height);
}
