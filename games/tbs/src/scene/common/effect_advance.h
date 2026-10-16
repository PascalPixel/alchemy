#include "types.h"
#include "field_effect.h"

static void Effect_AdvanceMotion(union FieldObject *object) {
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
