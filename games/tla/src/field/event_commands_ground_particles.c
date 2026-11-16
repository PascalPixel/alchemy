/// @file
/// The ground particles, the rising particles and the floating motes.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/object.h"

/// Unused: nothing calls GroundParticles_StartFar. The Broken Seal's
/// Weather_StartRain, kept whole: 64 sprites placed at the leader's position on
/// the terrain, their tiles decoded into VRAM, the blend set, and
/// GroundParticles_Update scheduled to move them. The Lost Age's rain is
/// Weather_StartRain.
void GroundParticles_Start(void) {
  struct WeatherWork *work;
  s32 *leader;
  struct GroundParticle *particle;
  // words is the decode buffer, then each particle's attribute cursor.
  u32 *words;
  u32 i;
  // The fill's value; as a literal 0 the fill differs.
  u32 zero = 0;

  work = HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct WeatherWork));
  leader = Ram_HeapSlots->mapGrid->origin;
  Scene_QueueMusic(SE_AMBIENT_WATER);
  particle = work->particles;
  Dma_Fill32(zero, work, sizeof(struct WeatherWork));
  words = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gFieldEffect_GroundParticleTiles, words);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 768, words);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  for (i = 0; i < 64; i++) {
    s32 x;
    s32 z;

    words = (u32 *)particle;
    *words++ = 0;
    *words++ = OBJ_SIZE_16X16 | OBJ_MODE(1);
    *words = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(0, x, z);
    particle->timer = (i & 15) + 1;
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
  struct RisingParticleWork *work = Ram_HeapSlots->weather;
  struct RisingParticle *particle = work->particles;
  struct FieldCoordinate screen[3];
  u32 i;
  s32 x;
  s32 y;

  for (i = 0; i < 64; i++, particle++) {
    if (particle->timer == 0) {
      x = Random16() * 200 + FIXED_ONE * 288;
      y = FIXED_ONE * 80 - Random16() * 20;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
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
        particle->sprite.attributes.tile = work->tile + 16;
      else if (particle->timer < 90)
        particle->sprite.attributes.tile = work->tile + 8;
      else
        particle->sprite.attributes.tile = work->tile;
      particle->sprite.attributes.tile =
          ((gFrameCount >> 3) & 1) * 4 + particle->sprite.attributes.tile;
      particle->sprite.attributes.x = x;
      particle->sprite.attributes.shape = 0;
      particle->sprite.attributes.size = 1;
      particle->sprite.attributes.priority = 3;
      particle->sprite.attributes.blendMode = 1;
      particle->sprite.attributes.y = y;
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    particle->timer--;
  }
}

/// Starts the rising particles: allocates and clears heap slot
/// HEAP_SLOT_WEATHER, decodes the particle tiles into VRAM, sets each of the 64
/// particles' sprite words and places it on the ground at a random point of the
/// strip (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2
/// frames, and schedules the update.
void RisingParticles_Start(void) {
  struct RisingParticleWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, 2068);
  struct RisingParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r2, built after the copy of work.
   */
  register u32 zero asm("r2");
  s32 priority;
  u8 *tiles;
  u32 i;

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 2068);
  tiles = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gFieldEffect_MoteTiles, tiles);
  work->vramEntry = VramCache_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 768, tiles);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  for (i = 0; i < 64; i++, particle++) {
    u32 *sprite = particle->sprite.words;
    s32 x;
    FIXED z;

    *sprite++ = 0;
    *sprite++ = OBJ_SIZE_16X16 | OBJ_MODE(1);
    *sprite = OBJ_PRIORITY(1) | OBJ_PALETTE(13);
    x = Random16() * 200 + toFIXED(288);
    z = toFIXED(80) - Random16() * 20;
    particle->posX = x;
    particle->posZ = z;
    particle->posY = MapGrid_GetTerrainHeightFar(0, x, z);
    particle->drift = 0;
    particle->timer = i * 2;
  }
  priority = TASK_PASS_FRAME;
  Task_Add(RisingParticles_Update, priority);
}

/// The Broken Seal's routine with 64 motes in HEAP_SLOT_WEATHER, started by
/// FieldMotes_Start.
void FieldMotes_Update(void) {
  struct WeatherWork *work = Ram_HeapSlots->weather;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u32 spawned = 0;
  u32 i;
  struct GroundParticle *mote;

  for (i = 0, mote = work->particles; i < 64; i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != (u16)-1) {
      s32 *camera = &map->viewX;
      s32 cameraX = camera[0];
      s32 cameraZ = camera[1];
      s16 age = mote->timer;
      u32 firstJitterDraw;
      u32 secondJitterDraw;

      if (GameFlag_Get(FLAG_WEATHER_HELD)) {
        mote->timer++;
        mote->delay--;
      }
      firstJitterDraw = Random16();
      secondJitterDraw = Random16();
      x = FIXEDtoINT(mote->posX - cameraX) +
          (((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
      y = FIXEDtoINT(mote->posZ - mote->posY - cameraZ) - (u16)age;
      if (x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
        if (mote->timer < 60) {
          mote->tile = work->tileBase + 16;
          mote->delay += 3;
        } else if (mote->timer < 90) {
          mote->tile = work->tileBase + 8;
          mote->delay++;
        } else {
          mote->tile = work->tileBase;
        }
        if ((gFrameCount >> 3) & 1)
          mote->tile += 4;
        mote->x = x;
        mote->y = y - (mote->delay >> 2);
        mote->shape = 0;
        mote->size = 1;
        Oam_AddSpriteAtDepthClamped(mote, 240);
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
      mote->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}
