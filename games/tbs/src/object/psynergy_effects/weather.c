/// @file
/// Ground particles, motes, sparkles and lightning on the field.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Scheduler callback: animates the ground particles. Each plays a 16-frame
/// animation (gFieldFx_GroundParticleFrames, 5 halfwords a frame: x and y
/// offsets, tile, shape and size) on a random spot of ground within 128 pixels
/// of the leader, then starts again elsewhere. The timer counts down, so the
/// rows play from the last: each comes down slantwise onto its spot and stays
/// there for its last seven frames. They freeze while FLAG_WEATHER_HELD is set.
/// Vale runs them with the lightning while FLAG_VALE_STORM is set
/// (vale_gate.c).
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
    if (GameFlag_TestFar(FLAG_WEATHER_HELD))
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

      // A new spot up to 128 pixels either way of the leader. The height
      // call passes pixels where Map_GetTerrainHeight takes 16.16, as in
      // ObjectMotion_SetHorizontalPositionWithTerrain (see the XXX there).
      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
      particle->timer = 16;
    }
  }
}

/// Scheduler callback for the storm: lightning flashes the BG colours three
/// times, 5 frames apart. The timer counts down to a burst's end: the lit
/// colours show at 11, 6 and 1, each fading back over 12 frames from the
/// next frame on. While enabled is set, the end of a burst plays a
/// thunderclap (louder with loud set) and sets the timer to 51 to 549 frames
/// for the next. FLAG_WEATHER_HELD holds the next burst back, and a timer below
/// 0 stops the lightning.
void FieldFx_UpdateLightning(void) {
  struct FieldBlendWork *work = gFieldBlendWork[0];
  struct BattleEffectBuffers *target =
      gFieldBlendWork[HEAP_SLOT_PALETTE_BUFFER - HEAP_SLOT_FIELD_BLEND];
  s16 *timer = &work->timer;

  if (*timer < 0)
    return;
  if (GameFlag_TestFar(FLAG_WEATHER_HELD))
    work->timer = 128;
  switch ((*timer)--) {
  case 0:
    // The burst ends: arm the next, 150 frames plus up to 399 less up to 99.
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
    // A frame after each flash: fade back to the dark colours over 12 frames.
    BattleFx_ApplyColorToTargetBuffer((s32)work, 1);
    Dma_Set(work->delta, target->delta, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2a0,
            REG_DMA3);
    target->duration = 12;
    target->step = 0;
    break;
  case 1:
  case 6:
  case 11:
    // The flash: the lit colours, in one frame.
    BattleFx_ApplyColorToTargetBuffer((s32)work->to, 1);
    target->duration = 1;
    target->step = 0;
    break;
  }
}

/// Starts the ground particles (FieldFx_UpdateGroundParticles) with sound 170:
/// loads their tiles and puts 32 of them under the leader with staggered
/// timers, drawn semi-transparent. Their work is heap block 29, which the
/// motes and sparkles also take. SE_170 is queued
/// (BattleFx_SetQueuedSoundAndPlay), so it plays again wherever the queued
/// sound is replayed, as after a Psynergy effect.
void FieldFx_StartGroundParticles(void) {
  struct GroundParticleWork *work;
  s32 *leader;
  struct GroundParticle *particle;
  u32 *words;
  u32 i;

  /* FAKEMATCH(temporary): words holds the fill's zero, the decode buffer and
   * each cell cursor, all in r6; as Dma_Fill32(0, work, ...), 4 of 93
   * instructions differ in tbs-en. */
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

/// A particle's first word, seen as a value or a link.
union DustWord {
  u32 value;
  void *link;
};

/// Scheduler callback: the floating motes. Each mote is given a spot of
/// ground within 128 pixels of the leader (at most 8 new ones a frame),
/// starts 120 pixels above it on screen and comes down a pixel a frame,
/// slowing to 3/4 pixel a frame for its last 90 frames and 1/4 for its last
/// 60, jittering sideways and changing tiles as it ages. It lasts 120
/// frames, ending about 52 pixels above its spot, or until it leaves the
/// screen. They freeze while FLAG_WEATHER_HELD is set. Nothing in ☀️ starts them
/// (FieldMotes_Start has only its overlay veneer).
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

      if (GameFlag_TestFar(FLAG_WEATHER_HELD)) {
        mote->timer++;
        mote->delay--;
      }
      // x jitters by -1 or 0, and y is the frames left in pixels above the
      // spot; a quarter of the delay, which builds up as it ages, lifts it
      // further.
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

/// Starts the floating motes (FieldMotes_Update): loads their tiles and puts
/// 32 of them under the leader with staggered timers, drawn semi-transparent.
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

/// Scheduler callback: the sparkles. Now and then (1 frame in 256) a sparkle
/// appears on the ground within 128 pixels of the leader, and up to three more
/// follow on the same spot, each 4 frames shorter. Each plays
/// gFieldFx_SparkleFrames from the row its timer starts at down to row 0,
/// coming down onto the spot, flipping every other frame. No new bursts start
/// while the work's stopped is set; FLAG_WEATHER_HELD freezes them. Sol
/// Sanctum's eruption scene starts them (sol_sanctum_eruption.c).
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
      if (GameFlag_TestFar(FLAG_WEATHER_HELD) != 0)
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
#ifdef UBFIX
        frames = gFrameCount;
#else
        // UB: this drops volatile and reads a u32 as unsigned long.
        /* FAKEMATCH(volatile): the unsigned long alias combines the size
         * and flip stores; as gFrameCount or a u32 alias, 3 of 235
         * instructions differ in tbs-en. UBFIX reads the declared type. */
        frames = *(unsigned long *)&gFrameCount;
#endif
        sparkle->y = sy;
        sparkle->affineIndex = ((frames >> 1) & 1) << 3;
        Runtime_PushSlotEntry(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        // The rest of a burst, on the first sparkle's spot. The height is
        // read at sx >> 16 and sy >> 16, the last live sparkle's screen
        // point shifted down 16 bits (0, or -1 off the left or top), where
        // Map_GetTerrainHeight takes 16.16 pixels; the burst's first sparkle
        // reads it the same way (see the XXX in
        // ObjectMotion_SetHorizontalPositionWithTerrain).
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

/// Starts the sparkles (FieldEffect_UpdateSparkles): loads their tiles and
/// clears the 32 sparkles.
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
    /* FAKEMATCH(temporary): one integer cursor for the stores and the x keeps
     * the leader in r2; as a u32 * cursor and an x of its own, 14 of 74
     * instructions differ in tbs-en. */
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

/// Starts the storm's lightning (FieldFx_UpdateLightning): builds the dark
/// (0x10003, PALETTE_TRANSFORM_WARM) and lit (0x10005, PALETTE_TRANSFORM_PALE)
/// BG colours and their 12-step fade, and arms the first burst to end in 600
/// frames, repeating. The work is heap block HEAP_SLOT_FIELD_BLEND, which the
/// other lightning calls look up again. Vale's storm scenes call it.
void FieldFx_StartLightning(void) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;

  work = Runtime_AllocateBlock(HEAP_SLOT_FIELD_BLEND,
                               sizeof(struct FieldBlendWork));
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

/// Stops the lightning: no flash for 0x7fff frames, and none after.
void FieldFx_StopLightning(void) {
  struct FieldBlendWork *state = Runtime_AllocateBlock(
      HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  state->timer = 0x7FFF;
  state->enabled = 0;
}

/// One last burst of lightning, its first flash two frames from now (timer
/// 12; FieldFx_UpdateLightning flashes at 11), then no more. Vale's storm and
/// eruption scenes call it.
void FieldFx_LastLightning(void) {
  struct FieldBlendWork *state = Runtime_AllocateBlock(
      HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  state->timer = 12;
  state->enabled = 0;
}

/// Restarts the lightning: the next burst ends in 128 frames, its first flash
/// 118 frames from now, and bursts repeat.
void FieldFx_ResumeLightning(void) {
  struct FieldBlendWork *state = Runtime_AllocateBlock(
      HEAP_SLOT_FIELD_BLEND, sizeof(struct FieldBlendWork));
  state->timer = 128;
  state->enabled = 1;
}

/// Flashes the palette with the storm's callback: one burst of three flashes
/// of palette to, each fading back to palette from (BattleFx_BuildBuffer
/// modes) on the BG colours, ending 120 frames from now, and no more after.
/// Vale's eruption calls it with the warm and red transforms
/// (vale_eruption.c).
void BattleFx_StartBufferBlend(s32 from, s32 to) {
  struct FieldBlendWork *work;
  struct BattleEffectBuffers *buffers;
  s16 *target;

  work = Runtime_AllocateBlock(HEAP_SLOT_FIELD_BLEND,
                               sizeof(struct FieldBlendWork));
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
