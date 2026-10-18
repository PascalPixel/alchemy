#include "object_runtime.h"
#include "macros.h"

void ObjectMotion_ResetTargetsAndVelocity(u32 objectId) {
  struct ObjectRuntime *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    object->actionFlags = (u8)(object->actionFlags | 1);
    Object_ResetMotion(object);
    object->targetX = 0x80000000;
    object->targetY = 0x80000000;
    object->targetZ = 0x80000000;
    SET_VELOCITY(object, 0, 0, 0);
    object->unk56 = 0;
  }
}
