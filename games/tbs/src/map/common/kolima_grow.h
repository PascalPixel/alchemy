/// @file
/// Kolima and Kolima Forest (MapId_Kolima, MapId_KolimaForestEntrance and
/// MapId_KolimaForestTree): an effect that
/// stretches as it travels.
#include "action_script.h"
#include "types.h"
#include "object_event.h"

/// An object script call: widen the object by 3/25 of its size a frame, and
/// release it once it has reached its destination.
static s32 FieldEffect_StretchUntilArrived(struct Object *interpreter) {
  struct ObjectEvent *streak = (struct ObjectEvent *)interpreter;
  streak->scaleX += 3 * Q16_ONE / 25;
  if (streak->targetX == OBJ_EVENT_NO_TARGET &&
      streak->targetY == OBJ_EVENT_NO_TARGET &&
      streak->targetZ == OBJ_EVENT_NO_TARGET) {
    ObjectEvent_Release(streak);
  }
  return 1;
}
