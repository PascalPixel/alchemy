/// @file
/// A particle's sprite that grows as it animates.
// Part of radial.c.

#include "../radial_internal.h"

#include "macros.h"

/* VIEW(psynergy_effects_11): tag struct EffectSprite as struct
 * EffectSprite__psynergy_effects_11; effect_sprite.h has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/field/event_commands/radial/growing_sprite.c:22:
 * structure has no member named `tile'. */
struct EffectSprite__psynergy_effects_11 {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

void Camera_WorldToScreen(s32 *position);
struct Actor *Actor_GetById(u32 objectId);

/// Animates a particle's sprite through 8 tiles (4 frames each), grows its
/// scale by 0.1 a frame up to 1.0, draws it at the particle's projected
/// position and advances the particle.
void BattleEffect_UpdateGrowingParticleSprite(
    struct EffectSprite__psynergy_effects_11 *sprite,
    struct EffectParticle *particle, s32 tiles) {
  s32 position[3];
  s32 scale;

  sprite->tile = tiles + ((particle->timer >> 2) & 7) * 8;
  scale = sprite->scaleX + 0x1999;
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
