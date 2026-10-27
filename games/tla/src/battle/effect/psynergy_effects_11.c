#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "iwram_call.h"
#include "macros.h"

struct EffectSprite {
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

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct RiseWork {
  u8 unk0000[0x18a8];
  struct ObjectRuntime *object;
  s16 done;
  s16 timer;
};

struct AnchorWork {
  u8 unk0000[0x1930];
  struct ObjectRuntime *anchor;
};

struct ModeEvent {
  u8 unk000[352];
  u16 cue;
};

void Func_080eb01c(struct EffectSprite *sprite);
void Camera_WorldToScreen(s32 *position);
void BattleEffect_IntegrateVector3(struct EffectParticle *particle, s32 frames,
                               s32 flags);
struct ObjectRuntime *Object_GetById(s32 id);
void WaitTicks(s32 ticks);
s32 Trig_Sin(s32 angle);

/// Animates a particle's sprite through 8 tiles (4 ticks each), grows its
/// scale by 0.1 a frame up to 1.0, draws it at the particle's projected
/// position and advances the particle.
void BattleEffect_UpdateGrowingParticleSprite(struct EffectSprite *sprite, struct EffectParticle *particle,
                   s32 tiles) {
  s32 position[3];
  s32 scale;

  sprite->tile = tiles + ((particle->timer >> 2) & 7) * 8;
  scale = sprite->scaleX + 0x1999;
  sprite->scaleX = scale;
  if (scale > 0x10000)
    scale = 0x10000;
  sprite->scaleX = scale;
  sprite->scaleY = scale;
  SET_VECTOR(position, particle->x, particle->y, particle->z);
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  Func_080eb01c(sprite);
  BattleEffect_IntegrateVector3(particle, 60, 0);
}
