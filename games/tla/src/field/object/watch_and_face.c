#include "types.h"
#include "actor.h"

void SceneActor_UpdateFacingTowardTarget(void);

/// Makes the object watch target and keeps it facing it.
void Func_080d4aa8(struct Actor *object, struct Actor *target) {
  object->watchedObject = target;
  object->update = SceneActor_UpdateFacingTowardTarget;
}
