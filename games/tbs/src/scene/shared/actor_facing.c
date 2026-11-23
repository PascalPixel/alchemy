#include "field/script_operands.h"

/// @file
/// Altin Peak, Bilibin, Imil, Kolima, McCoy's palace and garden, Sol Sanctum's
/// Elemental Star room, and Vale's Kraden, Babi, home and rooms scenes: people
/// who turn to keep someone in view.

/// An actor script call: an actor with a facing target turns towards
/// it, at most a sixteenth of a turn a frame.
static s32 Scene_TurnTowardTarget(struct Actor *actor) {
  s32 delta;
  u16 facing;
  s32 angle;
  struct Actor *target;

  target = actor->watchedActor;
  if (target != NULL) {
    actor->steeringFlags = ((u8)~ACTOR_FACES_MOTION & actor->steeringFlags);
    angle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                           target->x.fixed - actor->x.fixed);
    facing = actor->facing;
    delta = (s16)(angle - facing);
    if (delta != 0) {
      if (delta > FACING_STEP) {
        delta = FACING_STEP;
      }

      if (delta < -FACING_STEP) {
        delta = -FACING_STEP;
      }
      actor->facing = facing + delta;
    }
  }
  return 1;
}
