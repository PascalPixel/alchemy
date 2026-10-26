/// @file
/// Kolima and Kolima Forest (SceneId_Kolima, SceneId_KolimaForestEntrance and
/// SceneId_KolimaForestTree): an effect that
/// stretches as it travels.
#include "action_script.h"
#include "types.h"
#include "actor.h"

/// An object script call: widen the object by 3/25 of its size a frame, and
/// release it once it has reached its destination.
static s32 FieldEffect_StretchUntilArrived(struct Object *interpreter) {
  struct Actor *streak = (struct Actor *)interpreter;
  streak->scaleX += 3 * Q16_ONE / 25;
  if (streak->targetX == ACTOR_NO_TARGET &&
      streak->targetY == ACTOR_NO_TARGET &&
      streak->targetZ == ACTOR_NO_TARGET) {
    Actor_Release(streak);
  }
  return 1;
}
