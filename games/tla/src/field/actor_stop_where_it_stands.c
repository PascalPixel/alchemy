/// @file
/// Stopping an actor where it stands.

#include "types.h"
#include "field/field_event.h"
#include "macros.h"

/// Stops an actor where it stands: no movement target, no speed, and its timer
/// back to 0.
void Actor_StopWhereItStands(struct Actor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;

  SET_VELOCITY(actor, 0, 0, 0);

  actor->timer = 0;
}
