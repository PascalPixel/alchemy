#include "local.h"

s32 Func_02000478(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  object->speedLimit = 0x10000;
  object->acceleration = 0xccc;
  object->collisionFlags = 0;
  object->update = (void (*)(void))Func_02000468;
  return 0;
}

const u8 *Func_020004a4(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov67bRoom1)
    return gData_02002f04;
  if (scene == (s32)&SceneId_Ov67bRoom2)
    return gData_0200312c;
  return gData_02002eec;
}

void Func_020004e4(struct Actor *object) {
  if (Map_GetTerrainHeight(0, object->x, object->z) > object->y)
    object->y += 0x20000;
  else
    object->z -= 0x20000;
  if (object->y >> 20 == 23)
    Func_02002928(object);
  if (object->z >> 20 == 48)
    Func_02002928(object);
}
