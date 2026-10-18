// Part of psynergy_effects.c, compiled only through it.
// Ground particles, motes, sparkles and lightning on the field.

#include "../psynergy_effects_internal.h"

// Scheduler callback: animates the ground particles. Each plays a 16-frame
// animation (gFieldFx_GroundParticleFrames, 5 halfwords a frame: x and y
// offsets, tile, shape and size) on a random spot of ground within 128 pixels
// of the leader, then starts again elsewhere. They freeze while flag 0x166 is
// set.
void FieldFx_UpdateGroundParticles(void) {
  struct MapScrollWork *view = gMapWork[0];
  struct GroundParticleWork *work = gMapWork[21];
  s32 *camera = &view->viewX;
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  for (i = 0; i < FIELD_WEATHER_PARTICLE_COUNT; i++, particle++) {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    // BUG: freezing a timer of 16 indexes past the 16-frame table. Which
    // frame should be held is not established, so keep the original timing.
    if (GameFlag_TestFar(0x166))
      particle->timer++;
    frame = &gFieldFx_GroundParticleFrames[5 * particle->timer];
    x = (particle->posX - cameraX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - cameraZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Runtime_PushSlotEntry(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *leader = view->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      particle->timer = 16;
    }
  }
}

// Scheduler callback for the storm: every 150 to 550 frames lightning flashes
// the palette three times (5 frames apart) with a thunderclap, louder with
// loud set. Flag 358 holds the next flash back.
void FieldFx_UpdateLightning(void) {
  struct FieldBlendWork *work = gFieldBlendWork[0];
  struct BattleEffectBuffers *target = gFieldBlendWork[2];
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_TestFar(358))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    if (work->enabled != 0) {
      u32 intervalDraw = Random16();
      u32 intervalJitter = Random16();
      work->timer =
          ((intervalDraw * 400) >> 16) - ((intervalJitter * 100) >> 16) + 150;
      if (work->loud != 0)
        AudioCommand_PlayFar(SE_172);
      else
        AudioCommand_PlayFar(SE_171);
    }
    // fall through
  case 5:
  case 10:
    // Flash: jump to the lit palette and fade back over 12 frames.
    BattleFx_ApplyColorToTargetBuffer(work, 1);
    Dma_Set(work->delta, target->delta, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2a0,
            REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    BattleFx_ApplyColorToTargetBuffer(work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}

// Starts the ground particles (FieldFx_UpdateGroundParticles) with sound 170:
// loads their tiles and puts 32 of them under the leader with staggered
// timers, drawn semi-transparent.
void FieldFx_StartGroundParticles(void) {
  struct GroundParticleWork *work;
  s32 *leader;
  struct GroundParticle *particle;
  u32 *words;
  u32 i;

  /* FAKEMATCH(temporary): one pointer, words, holds the fill's zero, the
   * decode buffer and each particle's cell cursor, all in r6 as the bytes
   * have them; as Dma_Fill32(0, work, ...), 4 of 93 instructions differ in
   * tbs-en. */
  work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
  leader = ((struct MapScrollWork *)gMapWork[0])->origin;
  BattleFx_SetQueuedSoundAndPlay(SE_170);
  words = 0;
  particle = work->particles;
  Dma_Fill32((u32)words, work, sizeof(struct GroundParticleWork));
  words = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_GroundParticleTiles, words);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, words);
  Runtime_ReleaseHeapBlock(14);
  for (i = 0; i < FIELD_WEATHER_PARTICLE_COUNT; i++) {
    s32 x;
    s32 z;

    // Semi-transparent, 16 by 16, priority 1, palette 13.
    words = (u32 *)particle;
    *words++ = 0;
    *words++ = 0x40000400;
    *words = 0xd400;
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}

// A particle's first word, seen as a value or a link.
union DustWord {
  u32 value;
  void *link;
};

// Scheduler callback: the floating motes. Each mote appears on the ground
// within 128 pixels of the leader (at most 8 new ones a frame), rises a pixel
// a frame, ever faster for its last 90 frames, jittering sideways and
// changing tiles as it ages, and lasts 120 frames or until it leaves the
// screen. They freeze while flag 0x166 is set.
void FieldMotes_Update(void) {
  struct GroundParticleWork *work = WORK_SLOT(29);
  struct MapScrollWork *map = WORK_SLOT(8);
  u32 spawned = 0;
  u32 i;
  struct GroundParticle *mote = work->particles;

  for (i = 0; i < FIELD_WEATHER_PARTICLE_COUNT; i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != 0xffff) {
      s32 *camera = &map->viewX;
      s32 cameraX = camera[0];
      s32 cameraZ = camera[1];
      u16 age = mote->timer;
      u32 firstJitterDraw;
      u32 secondJitterDraw;

      if (GameFlag_TestFar(0x166)) {
        mote->timer++;
        mote->delay--;
      }
      firstJitterDraw = Random16();
      secondJitterDraw = Random16();
      x = ((mote->posX - cameraX) >> 16) +
          (((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
      y = ((mote->posZ - mote->posY - cameraZ) >> 16) - age;
      if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
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
        Runtime_PushSlotEntry(mote, 240);
      } else {
        mote->timer = 0;
      }
    }
    if (spawned < 8 && mote->timer == 0) {
      s32 *leader = map->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      mote->posZ = y;
      mote->posX = x;
      mote->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}

// Starts the floating motes (FieldMotes_Update): loads their tiles and puts
// 32 of them under the leader with staggered timers, drawn semi-transparent.
void FieldMotes_Start(void) {
  struct GroundParticleWork *work;
  struct GroundParticle *mote;
  u8 *tiles;
  u32 i;
  u32 size;

  size = 0x410;
  work = Runtime_AllocateBlock(29, size);
  mote = work->particles;
  /* FAKEMATCH(temporary): size reused for the cleared word keeps the zero in
   * r1, the register size was passed in; as Dma_Fill32(0, work, ...), 4 of
   * 83 instructions differ in tbs-en. */
  size = 0;
  Dma_Fill32(size, work, sizeof(*work));
  tiles = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_MoteTiles, tiles);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, tiles);
  Runtime_ReleaseHeapBlock(14);
  for (i = 0; i < FIELD_WEATHER_PARTICLE_COUNT; i++) {
    s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the stores in r1; as an s32 * cursor and an x of
     * its own, 16 of 83 instructions differ in tbs-en. */
    u32 p = (u32)mote;
    s32 z;
    *(s32 *)p = 0;
    p += sizeof(s32);
    *(s32 *)p = 0x40000400;
    p += sizeof(s32);
    *(s32 *)p = 0xd400;
    p = pos[0];
    z = pos[2];
    mote->posX = (s32)p;
    mote->posZ = z;
    mote->posY = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
    mote->timer = (i & 15) + 1;
    mote++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Scheduler_AddOrUpdateCallback(FieldMotes_Update, 0xc80);
}

// Scheduler callback: the sparkles. Now and then (1 frame in 256) a sparkle
// appears on the ground within 128 pixels of the leader, and up to three more
// follow on the same spot, each 4 frames shorter. Each plays
// gFieldFx_SparkleFrames, flipping every other frame. No new bursts start
// while the work's stopped is set; flag 0x166 freezes them.
void FieldEffect_UpdateSparkles(void) {
  struct GroundParticleWork *work;
  u32 spawned;
  u32 i;
  struct MapScrollWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct GroundParticle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = WORK_SLOT(29);
  spawned = 0;
  map = WORK_SLOT(8);
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->particles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_TestFar(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gFieldFx_SparkleFrames[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->posY - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
        /* FAKEMATCH(volatile): gFrameCount read through a cast that drops its
         * volatile, which keeps the size and flip stores together; as
         * gFrameCount, 3 of 235 instructions differ in tbs-en. */
        frames = *(unsigned long *)&gFrameCount;
        sparkle->y = sy;
        sparkle->affineIndex = ((frames >> 1) & 1) << 3;
        Runtime_PushSlotEntry(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        // The rest of a burst, on the first sparkle's spot.
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        // A burst starts at a random spot.
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->posY = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i < FIELD_WEATHER_PARTICLE_COUNT);
}

// Starts the sparkles (FieldEffect_UpdateSparkles): loads their tiles and
// clears the 32 sparkles.
void FieldEffect_InitSparkles(void) {
  u32 size = 0x410;
  struct GroundParticleWork *work = Runtime_AllocateBlock(29, size);
  struct GroundParticle *sparkle = work->particles;
  u8 *tiles;
  u32 i;
  s32 clear;

  /* FAKEMATCH(temporary): size reused for the fill word keeps the zero in r1,
   * the register size was passed in; as Dma_Fill32(0, work, ...), 4 of 74
   * instructions differ in tbs-en. */
  size = 0;
  Dma_Fill32(size, work, sizeof(*work));
  tiles = Runtime_AllocateBlock(14, 0x400);
  Resource_DecodeByteLz(gFieldFx_SparkleTiles, tiles);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x200, tiles);
  Runtime_ReleaseHeapBlock(14);
  i = 0;
  clear = 0;
  do {
    struct MapScrollWork *view = gMapWork[0];
    s32 *leader;
    /* FAKEMATCH(temporary): one integer cursor for the attribute stores and
     * the height's x keeps the view load order and the leader in r2; as a
     * u32 * cursor and an x of its own, 14 of 74 instructions differ in
     * tbs-en. */
    u32 attr;
    s32 z;

    attr = (u32)sparkle;
    *(u32 *)attr = clear;
    attr += sizeof(union DustWord);
    leader = view->origin;
    *(u32 *)attr = 0x40000400;
    attr += sizeof(union DustWord);
    *(u32 *)attr = 0xd400;
    attr = leader[0];
    z = leader[2];
    sparkle->posX = clear;
    sparkle->posZ = clear;
    sparkle->posY = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
    sparkle->timer = (i & 15) + 1;
    i++;
    sparkle++;
  } while (i < FIELD_WEATHER_PARTICLE_COUNT);
  Scheduler_AddOrUpdateCallback(FieldEffect_UpdateSparkles, 0xc80);
}

// Starts the storm's lightning (FieldFx_UpdateLightning): builds the dark
// (0x10003) and lit (0x10005) palettes and their 12-step fade, and arms the
// first flash in 600 frames, repeating.
void FieldFx_StartLightning(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;

  work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  buffers = gPaletteBufferWork;
  Dma_Fill32(0, work, sizeof(struct FieldBlendWork));
  BattleFx_BuildBuffer(0x10003, buffers, work, 1);
  BattleFx_BuildBuffer(0x10005, buffers, work->to, 1);
  BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
  BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 600;
  work->enabled = 1;
  Scheduler_AddOrUpdateCallback(FieldFx_UpdateLightning, 0xc80);
}

// Stops the lightning: no flash for 0x7fff frames, and none after.
void FieldFx_StopLightning(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 0x7FFF;
  state->enabled = 0;
}

// One last flash of lightning in 12 frames, then no more.
void FieldFx_LastLightning(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

// Restarts the lightning: the next flash in 128 frames, repeating.
void FieldFx_ResumeLightning(void) {
  struct FieldBlendWork *state =
      Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

// Flashes the palette once, 120 frames from now, from palette from to palette
// to (BattleFx_BuildBuffer modes), with the storm's flash callback.
void BattleFx_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  s16 *target;

  work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
  buffers = gPaletteBufferWork;
  Dma_Fill32(0, work, sizeof(struct FieldBlendWork));
  BattleFx_BuildBuffer(from, buffers, work, 1);
  target = work->to;
  BattleFx_BuildBuffer(to, buffers, target, 1);
  BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
  BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
  work->timer = 120;
  work->enabled = 0;
  Scheduler_AddOrUpdateCallback(FieldFx_UpdateLightning, 0xc80);
}
