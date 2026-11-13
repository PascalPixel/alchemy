#include "types.h"
#include "system/field_effect.h"

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

  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;
}
