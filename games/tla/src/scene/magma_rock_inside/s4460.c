#include "local.h"

void Func_02004460(struct ObjectRuntime *object, struct ObjectRuntime *target) {
  Object_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Object_SetPositionAndResetMotion(target, 0, 0, 0);
}
