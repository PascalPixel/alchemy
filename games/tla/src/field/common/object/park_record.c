#include "types.h"
#include "field_event.h"

void SceneActor_ParkRecord(struct FieldActor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  actor->velocityX = 0;
  actor->velocityY = 0;
  actor->velocityZ = 0;

  actor->timer = 0;
}
