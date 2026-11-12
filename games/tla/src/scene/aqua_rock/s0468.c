/// @file
/// Scene code of overlay 0x67b, in ROM order, from s0468's offset up to the next object the overlay links.

#include "local.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02000468(struct Actor *object) {
  struct FieldSprite *sprite;
  s32 angle;

  angle = object->angle;
  sprite = object->animation;
  sprite->rotation = angle + 0x4000;
}

s32 Func_02000478(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  object->speedLimit = 0x10000;
  object->acceleration = 0xccc;
  object->collisionFlags = 0;
  object->update = (void (*)(void))Func_02000468;
  return 0;
}

const u8 *Func_020004a4(void) {
  s16 scene = gMainRoutineWork.scene;

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

void Func_0200052c(struct Actor *object) {
  if ((u32)Random16Far() < 0x1000) {
    struct Actor *child;
    s32 spread = Random16Far();
    s32 x = object->x;

    x += spread * 48;
    child = Import_ActorCreate(0x2a2, x - 0x180000, object->y, object->z);
    if (child != 0) {
      Actor_SetSingleChildFlags(child, 0);
      child->update = (void (*)(void))Func_020004e4;
      child->motionFlags = 0;
    }
  }
}
