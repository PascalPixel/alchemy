#include "types.h"
#include "object_runtime.h"

void SceneActor_UpdateFacingTowardTarget(void);

/// Makes the object watch target and keeps it facing it.
void Func_080d4aa8(struct ObjectRuntime *object, struct ObjectRuntime *target) {
  object->watchedObject = target;
  object->update = SceneActor_UpdateFacingTowardTarget;
}
