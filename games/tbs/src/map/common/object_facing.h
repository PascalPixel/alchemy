#include "action_script.h"
#include "types.h"
#include "object.h"

// Altin Peak, Bilibin, Imil, Kolima, McCoy's palace and garden, Sol Sanctum's
// Elemental Star room, and Vale's Kraden, Babi, home and interiors maps.

/// An object script call: an object event with a facing target turns towards
/// it, at most a sixteenth of a turn a frame.
static s32 Map_TurnTowardTarget(struct Object *interpreter) {
  struct Object *obj = interpreter;
  s32 delta;
  u16 old;
  s32 angle;
  struct Object *target;

  target = obj->linkedObject;
  if (target != 0) {
    obj->steeringFlags = (0xFE & obj->steeringFlags);
    angle = (u16)ArcTan2(target->z - obj->z, target->x - obj->x);
    old = obj->facing;
    delta = (s16)(angle - old);
    if (delta != 0) {
      if (delta > 0x1000) {
        delta = 0x1000;
      }

      if (delta < -0x1000) {
        delta = -0x1000;
      }
      obj->facing = (old + delta);
    }
  }
  return 1;
}
