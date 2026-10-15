#include "mars_star.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200427c(struct ObjectRuntime *object, s32 mode);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);

s32 Func_02002b98(struct ObjectRuntime *object) {
  s32 frame = object->action;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Func_0200427c(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Func_0200446c(136);
      Func_020041cc(object->linkedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->action / 2) & 1;

      Animation_ApplyChildValues(object->linkedObject, half * 7);
    }
  } else {
    Func_020041cc(object->linkedObject, 1);
    Func_0200446c(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->linkedObject = 0;
    return 0;
  }
  object->action++;
  return 1;
}

s32 Func_02002c44(struct ObjectRuntime *object) {
  s32 frame = object->action;

  if (frame == 0) {
    Func_0200446c(136);
    Func_020041cc(object->linkedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Func_0200427c(object, 1);
  } else if (frame <= 32) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->action / 2) & 1;

      Animation_ApplyChildValues(object->linkedObject, half * 7);
    }
  } else {
    Func_020041cc(object->linkedObject, 1);
    Func_0200446c(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->linkedObject = 0;
    return 0;
  }
  object->action++;
  return 1;
}
