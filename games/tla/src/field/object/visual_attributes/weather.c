/// @file
/// Ground particles and lightning on the field.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/field.h"

/* VIEW(f0d6674): tag struct GroundParticleWork as struct
 * GroundParticleWork__f0d6674; visual_attributes.c has another definition;
 * tried with the definition the rest of the object has, Weather_UpdateRain
 * moves: 1 of 167 instructions in tla-ja. */
struct GroundParticleWork__f0d6674 {
  u8 unk00[4];
  // long, not s32: its own alias set lets the sprite stores pass its load.
  long tile;
  struct GroundParticle__visual_attributes particles[64];
};

/// The Broken Seal's routine with fixed sprites, and
/// GroundParticles_Update's sibling for the same 64 ground particles: each
/// counts down its frame (held while flag 358 is set), is drawn at its
/// picture's offset (Data_080f0b50, pairs) with the work's tile, shape 1 and
/// size 2 when on screen, and when its count runs out is placed again at a
/// random point (x within -64 to +192, z within -128 to +128 pixels of the
/// map's focus), on the ground, for 12 frames.
void Weather_UpdateRain(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct GroundParticleWork__f0d6674 *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  struct GroundParticle__visual_attributes *particle = work->particles;
  s32 originZ = origin[1];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Get(358))
      particle->timer++;
    frame = &Data_080f0b50[2 * particle->timer];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile;
      particle->shape = 1;
      particle->size = 2;
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x400000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      particle->timer = 12;
    }
  }
}

void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode);
#define BattleEffect_ApplyColorToTargetBuffer                                  \
  BattleEffect_ApplyColorToTargetBuffer

// Lightning: selects the palette according to the remaining duration.
void Weather_UpdateLightning(void) {
  struct FieldBlendWork *work = Ram_HeapSlots->unk70[2];
  struct BattleEffectBuffers *target = Ram_HeapSlots->block32;
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_Get(358))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    if (work->enabled != 0) {
      u32 a = Random16();
      u32 b = Random16();
      u32 draw = a * 24;
      u32 jitter = b * 100;
      work->timer = (((draw + a) << 4) >> 16) - (jitter >> 16) + 150;
      if (work->loud != 0)
        Sound_PlayFar(172);
      else
        Sound_PlayFar(171);
    }
  case 5:
  case 10:
    BattleEffect_ApplyColorToTargetBuffer(work, 1);
    Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    BattleEffect_ApplyColorToTargetBuffer(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}
