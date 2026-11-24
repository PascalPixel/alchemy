/// @file
/// A field effect's motion and spin each frame.

#include "types.h"
#include "fixed_math.h"
#include "system/main_routine_state.h"
#include "system/field_effect.h"
#include "field/event.h"

/// FieldEffect_AdvanceMotion with a fixed spin: moves the effect by its
/// velocity, which loses drag, grows it at its rates and turns its sprite a
/// sixteenth of a turn each frame. Nothing calls it.
UNUSED void FieldEffect_AdvanceMotionAndSpin(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX;
  s32 velocityZ;
  s32 scaleRateX;

  // DEAD: the leader looked up and not used; the ROM keeps the call.
  Actor_GetById(gMainRoutineState.leaderActorId);
  velocityX = effect->velocity.x;
  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;

  effect->velocity.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
  effect->velocity.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += TRIG_TURN / 16;
}
