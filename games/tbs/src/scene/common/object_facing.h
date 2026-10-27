#include "action_script.h"
#include "types.h"
#include "actor.h"

// Altin Peak, Bilibin, Imil, Kolima, McCoy's palace and garden, Sol Sanctum's
// Elemental Star room, and Vale's Kraden, Babi, home and interiors maps.

/// An object script call: an actor with a facing target turns towards
/// it, at most a sixteenth of a turn a frame.
static s32 Scene_TurnTowardTarget(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s32 delta;
  u16 old;
  s32 angle;
  struct Actor *target;

  target = actor->linkedActor;
  if (target != 0) {
    actor->steeringFlags = (0xFE & actor->steeringFlags);
    angle = (u16)ArcTan2(target->z.fixed - actor->z.fixed,
                         target->x.fixed - actor->x.fixed);
    old = actor->facing;
    delta = (s16)(angle - old);
    if (delta != 0) {
      if (delta > 0x1000) {
        delta = 0x1000;
      }

      if (delta < -0x1000) {
        delta = -0x1000;
      }
      actor->facing = (old + delta);
    }
  }
  return 1;
}
