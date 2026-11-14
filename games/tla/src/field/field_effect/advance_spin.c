#include "types.h"
#include "fixed_math.h"
#include "system/main_routine_work.h"
#include "system/field_effect.h"
#include "field/event.h"

/// FieldEffect_AdvanceMotion with a fixed spin: moves the effect by its
/// velocity, damps the X and Z velocity, steps the scale and turns the sprite
/// 0x1000 each frame. The leader's object is looked up and not used. Overlays
/// 6aa, 6ab, 6ac and 6ad hold it.
void Effect_AdvanceMotionAndSpin(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX;
  s32 velocityZ;
  s32 scaleRateX;

  Actor_GetById(gMainRoutineWork.currentOwner);
  velocityX = effect->velocity.x;
  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;

  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += TRIG_TURN / 16;
}
