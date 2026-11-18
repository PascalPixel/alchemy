/// @file
/// A field effect's motion each frame.

#include "types.h"
#include "system/field_effect.h"
#include "field/sprite.h"

/// A drifting effect loses a FIELD_EFFECT_DRAG_X-th of its x speed and a
/// FIELD_EFFECT_DRAG_Z-th of its z speed each frame.
#define FIELD_EFFECT_DRAG_X 18
#define FIELD_EFFECT_DRAG_Z 16

/// An effect's update routine: move by its velocity, which loses an eighteenth
/// of its x and a sixteenth of its z each frame, and grow and spin at its
/// rates.
void FieldEffect_AdvanceMotion(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX;
  s32 velocityZ;
  s32 scaleRateX;
  struct FieldSprite *sprite;

  velocityX = effect->velocity.x;
  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;

  effect->velocity.x = velocityX - velocityX / FIELD_EFFECT_DRAG_X;
  effect->velocity.z = velocityZ - velocityZ / FIELD_EFFECT_DRAG_Z;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;
}

#undef FIELD_EFFECT_DRAG_X
#undef FIELD_EFFECT_DRAG_Z
