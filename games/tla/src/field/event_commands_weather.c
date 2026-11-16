/// @file
/// Ground particles and lightning on the field.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "field/field.h"
#include "field/ground_mask.h"

/// Task of GroundParticles_Start: The Broken Seal's Weather_UpdateRain, kept
/// whole. Each particle plays its 16 frames of
/// gFieldEffect_GroundParticleFrames on a random spot near the leader, held
/// while FLAG_WEATHER_HELD is set.
void GroundParticles_Update(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct WeatherWork *work = Ram_HeapSlots->weather;
  s32 *origin = &map->viewX;
  s32 originX = origin[0];
  s32 originZ = origin[1];
  struct GroundParticle *particle = work->particles;
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
    x = (particle->posX - originX) / FIXED_ONE + *frame++;
    y = (particle->posZ - particle->posY - originZ) / FIXED_ONE + *frame++;
    if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = map->origin;

      x = focus[0] + (Random16() << 8) - toFIXED(128);
      y = focus[2] + (Random16() << 8) - toFIXED(128);
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      particle->timer = 16;
    }
  }
}

/// The Broken Seal's routine with fixed sprites, and
/// GroundParticles_Update's sibling for the same 64 ground particles: each
/// counts down its frame (held while FLAG_WEATHER_HELD is set), is drawn at its
/// picture's offset (gGroundParticles_Offsets, pairs) with the work's tile,
/// shape 1 and size 2 when on screen, and when its count runs out is placed
/// again at a random point (x within -64 to +192, z within -128 to +128 pixels
/// of the map's focus), on the ground, for 12 frames.
void Weather_UpdateRain(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct WeatherWork *work = Ram_HeapSlots->weather;
  s32 *origin = &map->viewX;
  s32 originX = origin[0];
  struct GroundParticle *particle = work->particles;
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
    frame = &gGroundParticles_Offsets[2 * particle->timer];
    x = (particle->posX - originX) / FIXED_ONE + *frame++;
    y = (particle->posZ - particle->posY - originZ) / FIXED_ONE + *frame++;
    if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase;
      particle->shape = 1;
      particle->size = 2;
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = map->origin;

      x = focus[0] + (Random16() << 8) - toFIXED(64);
      y = focus[2] + (Random16() << 8) - toFIXED(128);
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
