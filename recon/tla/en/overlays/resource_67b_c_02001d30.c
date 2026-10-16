// differ 8: the listing loads the child's sprite between the load and the
// store of terrainHeight and the pool address after them, and the parent
// sprite's attribute byte before the child's. Assigning the sprite after the
// terrain copy moves the parent sprite out of r8 (differ 28).
#include "../../../../games/tla/src/scene/aqua_rock/local.h"

void Func_02001d30(struct ObjectRuntime *object) {
  struct ObjectRuntime *child = Engine_ObjectCreate(
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
