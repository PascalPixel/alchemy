/// @file
/// A field effect's motion, growth and spin each frame, with no drag.

#include "types.h"
#include "system/field_effect.h"
#include "field/sprite.h"

/// An effect's update routine: moves the effect by its velocity, which keeps
/// its speed, grows it at its rates and turns its sprite by its spin.
void FieldEffect_UpdateMotionScaleAndSpin(union FieldObject *object) {
  s32 x;
  s32 velocityX;
  struct Sprite *sprite;

  x = object->effect.x;
  velocityX = object->effect.velocity.x;
  sprite = object->effect.sprite;

  object->effect.x = x + velocityX;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  sprite->rotation += object->effect.spin;
}
