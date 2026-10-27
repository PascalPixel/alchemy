#include "action_script.h"
#include "types.h"
#include "actor.h"

// Altin Peak, Bilibin, Imil, Kolima, McCoy's palace and garden, Sol Sanctum's
// Elemental Star room, and Vale's Kraden, Babi, home and interiors maps.

/// An object script call: an actor with a facing target turns towards
/// it, at most a sixteenth of a turn a frame.
static s32 Scene_TurnTowardTarget(struct Actor *interpreter) {
  struct Actor *object = interpreter;
  s32 delta;
  u16 old;
  s32 angle;
  struct Actor *target;

  target = object->linkedActor;
  if (target != 0) {
    object->steeringFlags = (0xFE & object->steeringFlags);
    angle = (u16)ArcTan2(target->z.fixed - object->z.fixed,
                         target->x.fixed - object->x.fixed);
    old = object->facing;
    delta = (s16)(angle - old);
    if (delta != 0) {
      if (delta > 0x1000) {
        delta = 0x1000;
      }

      if (delta < -0x1000) {
        delta = -0x1000;
      }
      object->facing = (old + delta);
    }
  }
  return 1;
}
