/// @file
/// A particle's sprite that grows as it animates.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"

#include "macros.h"

void Camera_WorldToScreen(s32 *position);

/// Animates a particle's sprite through 8 tiles (4 frames each), grows its
/// scale by 0.1 a frame up to 1.0, draws it at the particle's projected
/// position and advances the particle.
void BattleEffect_UpdateGrowingParticleSprite(
    struct EffectSprite *sprite, struct FieldPsynergyParticle *particle,
    s32 tiles) {
  s32 position[3];
  s32 scale;

  sprite->oam.index = tiles + ((particle->timer >> 2) & 7) * 8;
  scale = sprite->scaleX + Q16_ONE / 10;
  sprite->scaleX = scale;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  sprite->scaleX = scale;
  sprite->scaleY = scale;
  SET_VECTOR(position, particle->x, particle->y, particle->z);
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  EffectSprite_Draw(sprite);
  BattleEffect_IntegrateVector3(particle, 60, 0);
}
