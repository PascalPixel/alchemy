#include "trial_road.h"

void Func_0200480c(struct ObjectRuntime *object, struct ObjectRuntime *target) {
  Object_SetPositionAndResetMotion(object, target->x, target->y, target->z);
  Object_SetPositionAndResetMotion(target, 0, 0, 0);
}
