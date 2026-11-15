/// @file
/// An actor script call that turns an actor toward an angle a step at a time.

#include "field/actor.h"
#include "fixed_math.h"

/// SC_CALL routine that turns an actor toward the angle kept in its timer: the
/// whole way when within 1/16 turn, otherwise 1/32 turn.
s32 Actor_StepAngle(struct Actor *actor) {
  s32 delta = 0;

  if (actor != NULL) {
    s32 targetAngle = (u16)actor->timer;
    s32 currentAngle = actor->facing;
    delta = (s16)(targetAngle - currentAngle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      actor->facing = currentAngle + delta;
    }
  }
  return delta;
}
