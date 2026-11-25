/// @file
/// The ground particles, the rising particles and the floating motes.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"
#include "field/weather.h"

/// mote.png's six pictures, four tiles each: two of each size, the motes
/// shrinking as they rise and flickering between the two.
#define MOTE_PICTURE_TILES SPRITE_TILES(16, 16)
#define MOTE_LARGE 0
#define MOTE_MEDIUM 2
#define MOTE_SMALL 4

/// Unused: nothing calls GroundParticles_StartFar. The Broken Seal's rain,
/// kept whole where this game has its own Weather_StartRain: 64 sprites placed
/// at the leader's position on the terrain, their tiles decoded into VRAM, the
/// blend set, and GroundParticles_Update scheduled to move them.
void GroundParticles_Start(void) {
  struct WeatherState *state;
  s32 *leader;
  struct WeatherParticle *particle;
  // words is the decode buffer, then each particle's attribute cursor.
  u32 *words;
  u32 i;
  u32 fill = 0;

  state = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct WeatherState));
  leader = ((struct MapState *)Ram_HeapSlots->mapGrid)->origin;
  Scene_QueueMusic(SE_AMBIENT_WATER);
  particle = state->particles;
  Dma_Fill32(fill, state, sizeof(struct WeatherState));
  words = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gWeather_ParticleTiles, words);
  state->entry = VramCache_FindFreeEntry();
  state->tileBase =
      VramCache_Load(state->entry, 6 * WEATHER_PICTURE_BYTES, words);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
    s32 x;
    s32 z;

    words = (u32 *)particle;
    *words++ = 0;
    *words++ = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    *words = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(MAPGRID_LAYER_GROUND, x, z);
    particle->timer = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
    particle++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(GroundParticles_Update, TASK_PASS_FRAME);
}

/// Task of RisingParticles_Start: updates the 64 rising particles; each starts
/// on the ground at a random point of the strip for 120 frames, drifts right by
/// a random amount each frame, rises with its age, and is drawn at priority 3,
/// its tile stepping back by 8 at 90 and 60 frames left and flickering between
/// two tiles every 8 frames.
void RisingParticles_Update(void) {
  struct RisingParticleState *state = Ram_HeapSlots->weather;
  struct RisingParticle *particle = state->particles;
  struct FieldCoordinate screen[3];
  u32 i;
  s32 x;
  s32 y;

  for (i = 0; i < ARRAY_COUNT(state->particles); i++, particle++) {
    if (particle->timer == 0) {
      x = Random16() * 200 + FIXED_ONE * 288;
      y = FIXED_ONE * 80 - Random16() * 20;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(MAPGRID_LAYER_GROUND, x, y);
      particle->timer = 120;
      particle->drift = 0;
    }
    screen[0].fixed = particle->posX + particle->drift;
    screen[1].fixed = particle->posY;
    screen[2].fixed = particle->posZ;
    Camera_WorldToScreen(screen);
    x = screen[0].part[0].integer;
    y = screen[2].part[0].integer - particle->timer - 8;
    particle->drift += Random16() + FIXED_ONE / 2;
    if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
      if (particle->timer < 60)
        particle->sprite.attributes.tile =
            state->tile + MOTE_SMALL * MOTE_PICTURE_TILES;
      else if (particle->timer < 90)
        particle->sprite.attributes.tile =
            state->tile + MOTE_MEDIUM * MOTE_PICTURE_TILES;
      else
        particle->sprite.attributes.tile =
            state->tile + MOTE_LARGE * MOTE_PICTURE_TILES;
      particle->sprite.attributes.tile =
          ((gFrameCount >> 3) & 1) * MOTE_PICTURE_TILES +
          particle->sprite.attributes.tile;
      particle->sprite.attributes.x = x;
      particle->sprite.attributes.shape = OBJ_SHAPE_SQUARE;
      particle->sprite.attributes.size = OAM_SIZE_16X16;
      particle->sprite.attributes.priority = 3;
      particle->sprite.attributes.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
      particle->sprite.attributes.y = y;
      Oam_AddSpriteAtDepthClamped(particle, OAM_DEPTH_WINDOW);
    }
    particle->timer--;
  }
}

/// Starts the rising particles, as the Northern Reaches does: allocates and
/// clears heap slot
/// HEAP_SLOT_WEATHER, decodes the particle tiles into VRAM, sets each of the 64
/// particles' sprite words and places it on the ground at a random point of the
/// strip (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2
/// frames, and schedules the update.
void RisingParticles_Start(void) {
  struct RisingParticleState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct WeatherState));
  struct RisingParticle *particle = state->particles;
  volatile u32 fill;
  s32 priority;
  u8 *tiles;
  u32 i;
  FIXED z;

  /* FAKEMATCH(temporary): z, holding the fill's 0 before it is a particle's
   * z; as 0 in the fill. */
  z = 0;
  Dma_Fill32Cell(fill, z, state, sizeof(struct WeatherState));
  tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gFieldEffect_MoteTiles, tiles);
  state->vramEntry = VramCache_FindFreeEntry();
  state->tile =
      VramCache_Load(state->vramEntry, 6 * WEATHER_PICTURE_BYTES, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  for (i = 0; i < ARRAY_COUNT(state->particles); i++, particle++) {
    u32 *sprite = particle->sprite.words;
    s32 x;

    *sprite++ = 0;
    *sprite++ = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    *sprite = OBJ_PRIORITY(1) | OBJ_PALETTE(WEATHER_PALETTE);
    x = Random16() * 200 + toFIXED(288);
    z = toFIXED(80) - Random16() * 20;
    particle->posX = x;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(MAPGRID_LAYER_GROUND, x, z);
    particle->drift = 0;
    particle->timer = i * 2;
  }
  priority = TASK_PASS_FRAME;
  Task_Add(RisingParticles_Update, priority);
}

/// Task of FieldMotes_Start: moves and draws the 64 motes in HEAP_SLOT_WEATHER.
/// Up to eight a frame start within 128 pixels of the point the view follows
/// and live 120 frames, rising, shrinking at 90 and 60 frames left and
/// flickering every 8 frames; FLAG_WEATHER_HELD holds them where they are.
void FieldMotes_Update(void) {
  struct WeatherState *state = Ram_HeapSlots->weather;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u32 spawned = 0;
  u32 i;
  struct WeatherParticle *mote;

  for (i = 0, mote = state->particles; i < WEATHER_PARTICLE_COUNT;
       i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != GROUND_PARTICLE_FREED) {
      s32 cameraX = MapState_View(map)[0];
      s32 cameraZ = MapState_View(map)[1];
      s16 age = mote->timer;
      u32 firstJitterDraw;
      u32 secondJitterDraw;

      if (GameFlag_Get(FLAG_WEATHER_HELD)) {
        mote->timer++;
        mote->delay--;
      }
      // x jitters by -1 or 0, and y is the frames left in pixels above the
      // spot; a quarter of the delay, which builds up as it ages, lifts it
      // further.
      firstJitterDraw = Random16();
      secondJitterDraw = Random16();
      x = FIXEDtoINT(mote->posX - cameraX) +
          (((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
      y = FIXEDtoINT(mote->posZ - mote->posY - cameraZ) - (u16)age;
      if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
        if (mote->timer < 60) {
          mote->tile = state->tileBase + MOTE_SMALL * MOTE_PICTURE_TILES;
          mote->delay += 3;
        } else if (mote->timer < 90) {
          mote->tile = state->tileBase + MOTE_MEDIUM * MOTE_PICTURE_TILES;
          mote->delay++;
        } else {
          mote->tile = state->tileBase + MOTE_LARGE * MOTE_PICTURE_TILES;
        }
        if ((gFrameCount >> 3) & 1)
          mote->tile += MOTE_PICTURE_TILES;
        mote->x = x;
        mote->y = y - (mote->delay >> 2);
        mote->shape = OBJ_SHAPE_SQUARE;
        mote->size = OAM_SIZE_16X16;
        Oam_AddSpriteAtDepthClamped(mote, OAM_DEPTH_WINDOW);
      } else {
        mote->timer = 0;
      }
    }
    if (spawned < 8 && mote->timer == 0) {
      s32 *leader = map->origin;

      x = leader[0] + (Random16() << 8) - toFIXED(128);
      y = leader[2] + (Random16() << 8) - toFIXED(128);
      mote->posX = x;
      mote->posZ = y;
      mote->posY = MapGrid_GetTerrainHeightFar(MAPGRID_LAYER_GROUND, x, y);
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}

#undef MOTE_PICTURE_TILES
#undef MOTE_LARGE
#undef MOTE_MEDIUM
#undef MOTE_SMALL
