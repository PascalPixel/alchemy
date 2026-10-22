#include "types.h"
#include "field_effect.h"

// Babi Lighthouse, Crossbone Isle, Tret's interiors, the Tunnel Ruins and Venus
// Lighthouse.

/// An effect's update hook: move by its velocity, which loses an eighteenth of
/// its x and a sixteenth of its z each frame, and grow and spin at its rates.
static void FieldEffect_AdvanceMotion(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;

  effect->x += effect->velocity.x;
  effect->y += effect->velocity.y;
  effect->z += effect->velocity.z;
  effect->velocity.x -= effect->velocity.x / 18;
  effect->velocity.z -= effect->velocity.z / 16;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}
