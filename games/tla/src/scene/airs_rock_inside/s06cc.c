#include "types.h"
#include "actor.h"

s32 Func_02000690(s32 layer, s32 x, s32 z);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 GameFlag_Test(s32 flag);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02005df4(s32 x, s32 z, s32 terrain, s32 height);

/// Sets an object's cells by its flag (its action parameter): when the flag is
/// set it clears the kind byte of its layer 2 cell and its layer 0 cell, sinks
/// 4 pixels when not on its ground and takes sprite priority 3; otherwise it
/// marks both cells (0xff00 and 0xd480), rises 4 pixels and takes priority 1;
/// then it writes both cells back and moves the object's height
/// (Func_02005df4).
void Func_020006cc(struct Actor *object) {
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  s32 ground = Func_02000690(0, x, z);
  s32 cell = Func_02000690(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;
  s32 set = GameFlag_Test(object->action);
  u8 *flags = &object->priorityFlags;
  s32 px = x << 20;
  s32 pz = z << 20;

  if (set) {
    cell &= 0xffff00ff;
    ground = 0;
    if (object->terrainHeight >> 19 != height)
      height -= 4;
    // Through the field: through flags the byte, not the 2, would take the
    // result register.
    object->priorityFlags |= 2;
    Scene_SetActorSpritePriority(object, 3);
  } else {
    cell |= 0xff00;
    ground |= 0xd480;
    *flags &= ~2;
    height += 4;
    Scene_SetActorSpritePriority(object, 1);
  }
  Func_0200062c(0, x, z, ground);
  Func_0200062c(2, x, z, cell);
  Func_02005df4(px, pz, object->terrainId, height);
}
