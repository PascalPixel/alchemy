#include "../../../../games/tla/src/scene/aqua_rock/local.h"

void Func_02001d30(struct ObjectRuntime *object) {
  struct ObjectRuntime *child = Import_ActorCreate(
      14, object->x, object->y + 0x40000, object->z + 0x40000);
  struct FieldSprite *parentSprite = object->animation;

  if (child != 0) {
    struct FieldSprite *sprite = child->animation;

    child->terrainHeight = object->terrainHeight;
    Func_02002918(child, gData_02002c44);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parentSprite->priority;
    }
  }
}
