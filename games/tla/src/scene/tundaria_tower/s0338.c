#include "tundaria_tower.h"

s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_02003030(s32 x, s32 z, s32 terrain, s32 height);

/// Sets the cell under the object by the flag its action names: with the flag
/// set, bit 7 of the layer 0 attribute and the high byte of the layer 2
/// attribute are cleared and the sprite goes to priority 3; without it they are
/// set and the sprite goes to priority 2. Func_02003030 then gets the cell, the
/// object's terrain and the terrain height in half cells, 4 lower or higher.
void Func_02000338(struct ObjectRuntime *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 attr0 = MapCell_GetAttributes(0, x, z);
  s32 attr2 = MapCell_GetAttributes(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  if (GameFlag_Test(object->action)) {
    attr2 &= 0xffff00ff;
    attr0 &= ~128;
    if ((object->terrainHeight >> 19) != height)
      height -= 4;
    Object_SetSpritePriority(object, 3);
  } else {
    attr2 = (attr2 & 0xff) | 0xff00;
    attr0 |= 128;
    height += 4;
    Object_SetSpritePriority(object, 2);
  }
  MapCell_SetAttributes(0, x, z, attr0);
  MapCell_SetAttributes(2, x, z, attr2);
  Func_02003030(x << 20, z << 20, object->terrainId, height);
}
