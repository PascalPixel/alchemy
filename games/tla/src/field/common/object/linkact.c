#include "types.h"
#include "object_runtime.h"

void SceneActor_UpdateFacingTowardTarget(void);

/// Links the object to target and keeps it facing it.
void Func_080d4aa8(struct ObjectRuntime *object, struct ObjectRuntime *target) {
  object->linkedObject = target;
  object->update = SceneActor_UpdateFacingTowardTarget;
}
