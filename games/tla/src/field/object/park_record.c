#include "types.h"
#include "field_event.h"
#include "macros.h"

void SceneActor_ParkRecord(struct Actor *actor) {
  actor->targetX = TARGET_UNSET;
  actor->targetY = TARGET_UNSET;
  actor->targetZ = TARGET_UNSET;

  SET_VELOCITY(actor, 0, 0, 0);

  actor->timer = 0;
}
