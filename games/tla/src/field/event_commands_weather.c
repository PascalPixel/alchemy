/// @file
/// The weather on the field: The Broken Seal's rain kept whole, this game's
/// rain, and the storm's lightning.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "fixed_math.h"
#include "flags.h"
#include "sounds.h"
#include "field/field.h"
#include "field/ground_mask.h"
#include "field/event.h"

/// The Broken Seal's rain each frame (GroundParticles_Start, which nothing
/// calls), kept whole: each drop falls from up to the right, splashes and
/// plays its GROUND_PARTICLE_FRAMES frames of
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

  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == GROUND_PARTICLE_FREED)
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
      Oam_AddSpriteAtDepthClamped(particle, OAM_DEPTH_WINDOW);
    }
    if (particle->timer == 0) {
      s32 *focus = map->origin;

      x = focus[0] + (Random16() << 8) - toFIXED(128);
      y = focus[2] + (Random16() << 8) - toFIXED(128);
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      particle->timer = GROUND_PARTICLE_FRAMES;
    }
  }
}

/// This game's rain each frame (Weather_StartRain), outside Mars Lighthouse
/// and on its ledges: each of
/// the 64 streaks falls down and to the right onto a spot on the ground, a pair
/// of gGroundParticles_Offsets a frame, and then starts again 12 frames up from
/// a random spot (x within -64 to +192, z within -128 to +128 pixels of the
/// map's focus). FLAG_WEATHER_HELD holds them where they are.
void Weather_UpdateRain(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct WeatherWork *work = Ram_HeapSlots->weather;
  s32 *origin = &map->viewX;
  s32 originX = origin[0];
  struct GroundParticle *particle = work->particles;
  s32 originZ = origin[1];
  u32 i;

  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == GROUND_PARTICLE_FREED)
      continue;
    if (GameFlag_Get(FLAG_WEATHER_HELD))
      particle->timer++;
    // XXX: a streak started at 12 frames reads its first offsets one pair past
    // the table's end; is it drawn a frame at its spot, by the next table's
    // first pair? A frame-by-frame recording of Mars Lighthouse's rain would
    // settle it.
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
      Oam_AddSpriteAtDepthClamped(particle, OAM_DEPTH_WINDOW);
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

/// The storm each frame: lightning flashes the BG colours three times, 5 frames
/// apart, with thunder; while lightning is on, the next burst comes 51 to 549
/// frames later. While FLAG_WEATHER_HELD is set no burst comes.
void Weather_UpdateLightning(void) {
  struct FieldBlendWork *work = Ram_HeapSlots->fieldBlend;
  struct PaletteFade *target = Ram_HeapSlots->paletteFade;
  if (work->timer < 0)
    return;
  if (GameFlag_Get(FLAG_WEATHER_HELD))
    work->timer = 128;
  switch (work->timer--) {
  case 0:
    // The burst ends: arm the next, 150 frames plus up to 399 less up to 99.
    if (work->enabled != 0) {
      work->timer = Random_Below(400) - Random_Below(100) + 150;
      if (work->loud != 0)
        Sound_PlayFar(SE_THUNDER_LOUD);
      else
        Sound_PlayFar(SE_THUNDER);
    }
    // fall through
  case 5:
  case 10:
    // A frame after each flash: fade back to the dark colours over 12 frames.
    PaletteFade_SetTarget((s32)work, 1);
    Dma_Set(work->delta, target->delta,
            DMA_COPY_WORDS(sizeof target->delta / 4), REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    // The flash: the lit colours, in one frame.
    PaletteFade_SetTarget((s32)work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}
