#include "local.h"

void Func_02001ea0(struct ObjectRuntime *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct ObjectRuntime *child =
        Engine_ObjectCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->animation;

      Func_02003ccc(child, gData_020040dc);
      child->motionFlags = 0;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->rotation = 0x4000;
        sprite->priority = 2;
      }
    }
  }
}
