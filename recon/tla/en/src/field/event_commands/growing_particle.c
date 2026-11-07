/// @file
/// One frame of a sprite particle that grows as it drifts.
#include "types.h"
#include "effect_sprite.h"
#include "fixed_math.h"

/// A particle's position and velocity (16.16), integrated each frame, and its
/// age in frames.
struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

void Camera_WorldToScreen(s32 *position);
void BattleEffect_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);

/// Advances one particle by a frame. For its first 32 frames its sprite
/// steps through eight pictures of eight tiles from tiles, four frames each,
/// grows by a tenth of its size a frame to full size, is placed on screen at
/// the particle's position and drawn, and the particle moves on; after that
/// only its age counts on. Reached from Func_080e4730 and Func_080e4a34.
void Func_080e46ac(struct EffectSprite *sprite, struct EffectParticle *particle,
                   s32 unused, s32 tiles) {
  s32 position[3];
  s32 scale;
  s32 picture;

  if ((u32)particle->timer <= 31) {
    picture = (particle->timer >> 2) & 7;
    sprite->oam.index = tiles + picture * 8;
    scale = sprite->scaleX + 0x1999;
    sprite->scaleX = scale;
    if (scale > Q16_ONE)
      scale = Q16_ONE;
    sprite->scaleX = scale;
    sprite->scaleY = scale;
    position[0] = particle->x;
    position[1] = particle->y;
    position[2] = particle->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Func_080eb01c(sprite);
    BattleEffect_IntegrateVector2(&particle->x, 60, 0);
  }
  particle->timer++;
}
