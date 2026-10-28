#include "types.h"
#include "field_event.h"
#include "macros.h"

void SceneActor_ParkRecord(struct FieldActor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  SET_VELOCITY(actor, 0, 0, 0);

  actor->timer = 0;
}
