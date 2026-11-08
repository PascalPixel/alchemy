#include "room.h"

void Func_02005528(struct Actor *object, s32 mode);

s32 Func_020033cc(struct Actor *object) {
  s32 frame = object->action;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Func_02005528(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Func_020056c0(136);
      Func_02005480(object->watchedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->action / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02005480(object->watchedObject, 1);
    Func_020056c0(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->action++;
  return 1;
}

s32 Func_02003478(struct Actor *object) {
  s32 frame = object->action;

  if (frame == 0) {
    Func_020056c0(136);
    Func_02005480(object->watchedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Func_02005528(object, 1);
  } else if (frame <= 32) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->action / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02005480(object->watchedObject, 1);
    Func_020056c0(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->action++;
  return 1;
}
