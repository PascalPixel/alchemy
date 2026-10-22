/// @file
/// Kolima and Kolima Forest (SceneId_Kolima, SceneId_KolimaForestEntrance and
/// SceneId_KolimaForestTree): an effect that
/// stretches as it travels.
#include "script.h"
#include "types.h"
#include "field_obj.h"

/// An object script call: widen the object by 3/25 of its size a frame, and
/// release it once it has reached its destination.
static s32 FieldFx_StretchUntilArrived(struct ScriptInterpreter *interpreter) {
  struct FieldActor *streak = (struct FieldActor *)interpreter;
  streak->scaleX += 3 * Q16_ONE / 25;
  if (streak->targetX == ACTOR_NO_TARGET &&
      streak->targetY == ACTOR_NO_TARGET &&
      streak->targetZ == ACTOR_NO_TARGET) {
    Engine_ObjectDispatchRelease(streak);
  }
  return 1;
}
