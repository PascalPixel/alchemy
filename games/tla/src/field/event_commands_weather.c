/// @file
/// Ground particles and lightning on the field.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "field/field_bank.h"

/* VIEW(event_commands): tag struct GroundParticle as struct
 * GroundParticle__event_commands; ground_mask.h has another definition; tried
 * with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/field/event_commands_weather.c:15: field `particles'
 * has incomplete type. */
struct GroundParticle__event_commands {
  u8 unused[4]; ///< no code names it
  u16 y : 8;
  u16 attr0 : 6;
  u16 shape : 2;
  u16 x : 9;
  u16 attr1 : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unusedAfterPalette[2]; ///< no code names it
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unusedAfterPosZ[4]; ///< no code names it
  u16 timer;
  u8 unusedAfterTimer[2]; ///< no code names it
};

struct GroundParticleWork {
  u8 unused[4]; ///< no code names it
  s32 tile;
  struct GroundParticle__event_commands particles[64];
};

/// Task of GroundParticles_Start: The Broken Seal's Weather_UpdateRain, kept
/// whole. Each particle plays its 16 frames of
/// gFieldEffect_GroundParticleFrames on a random spot near the leader, held
/// while FLAG_WEATHER_HELD is set.
void GroundParticles_Update(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  struct GroundParticleWork *work = Ram_HeapSlots->weather;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  s32 originZ = origin[1];
  struct GroundParticle__event_commands *particle = work->particles;
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Get(FLAG_WEATHER_HELD))
      particle->timer++;
    frame = &gFieldEffect_GroundParticleFrames[particle->timer * 5];
    x = (particle->posX - originX) / Q16_ONE + *frame++;
    y = (particle->posZ - particle->posY - originZ) / Q16_ONE + *frame++;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x800000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      particle->timer = 16;
    }
  }
}

/* VIEW(f0d6674): tag struct GroundParticleWork as struct
 * GroundParticleWork__f0d6674; event_commands.c has another definition; tried
 * with the definition the rest of the object has, Weather_UpdateRain moves: 1
 * of 167 instructions in tla-ja. */
struct GroundParticleWork__f0d6674 {
  u8 unused[4]; ///< no code names it
  // long, not s32: its own alias set lets the sprite stores pass its load.
  long tile;
  struct GroundParticle__event_commands particles[64];
};

/// The Broken Seal's routine with fixed sprites, and
/// GroundParticles_Update's sibling for the same 64 ground particles: each
/// counts down its frame (held while FLAG_WEATHER_HELD is set), is drawn at its
/// picture's offset (gGroundParticle_Offsets, pairs) with the work's tile,
/// shape 1 and size 2 when on screen, and when its count runs out is placed
/// again at a random point (x within -64 to +192, z within -128 to +128 pixels
/// of the map's focus), on the ground, for 12 frames.
void Weather_UpdateRain(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapGrid;
  struct GroundParticleWork__f0d6674 *work = Ram_HeapSlots->weather;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  struct GroundParticle__event_commands *particle = work->particles;
  s32 originZ = origin[1];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Get(FLAG_WEATHER_HELD))
      particle->timer++;
    frame = &gGroundParticle_Offsets[2 * particle->timer];
    x = (particle->posX - originX) / Q16_ONE + *frame++;
    y = (particle->posZ - particle->posY - originZ) / Q16_ONE + *frame++;
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

      x = focus[0] + (Random16() << 8) - px(64);
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      particle->timer = 12;
    }
  }
}

void PaletteFade_SetTarget(s32 value, s32 mode);
#define PaletteFade_SetTarget PaletteFade_SetTarget

/// Task for the storm: lightning flashes the BG colours three times, 5 frames
/// apart, with thunder; while lightning is on, the next burst comes 50 to 550
/// frames later. While FLAG_WEATHER_HELD is set no burst comes.
void Weather_UpdateLightning(void) {
  struct FieldBlendWork *work = Ram_HeapSlots->fieldBlend;
  struct PaletteFade *target = Ram_HeapSlots->paletteFade;
  s16 *timer = &work->timer;

  if (work->timer < 0)
    return;
  if (GameFlag_Get(FLAG_WEATHER_HELD))
    work->timer = 128;
  switch (work->timer--) {
  case 0:
    if (work->enabled != 0) {
      u32 a = Random16();
      u32 b = Random16();
      u32 draw = a * 24;
      u32 jitter = b * 100;
      work->timer = (((draw + a) << 4) >> 16) - (jitter >> 16) + 150;
      if (work->loud != 0)
        Sound_PlayFar(SE_THUNDER_LOUD);
      else
        Sound_PlayFar(SE_THUNDER);
    }
  case 5:
  case 10:
    PaletteFade_SetTarget(work, 1);
    Dma_Set(work->delta, target->delta, DMA_COPY_WORDS(672), REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    PaletteFade_SetTarget(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}
