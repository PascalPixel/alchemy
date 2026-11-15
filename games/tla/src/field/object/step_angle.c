#include "actor.h"

/// SC_CALL routine that turns an actor toward the angle kept in its timer: the
/// whole way when within 1/16 turn, otherwise 1/32 turn.
s32 Actor_StepAngle(struct Actor *object) {
  s32 delta = 0;

  if (object != NULL) {
    s32 target_angle = (u16)object->timer;
    s32 current_angle = object->facing;
    delta = (s16)(target_angle - current_angle);
    if (delta != 0) {
      if (delta > 4096)
        delta = 2048;
      if (delta < -4096)
        delta = -2048;
      object->facing = current_angle + delta;
    }
  }
  return delta;
}
