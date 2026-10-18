// Part of effect.c, the battle effect object, compiled only through it.
// Meteor and Outer Space.

// Add per-channel steps to background colours 1 to 63, capped at 31.
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep) {
  u16 *color = (u16 *)&BG_PLTT[1];
  s32 i;

  for (i = 0; i != 63; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue += blueStep;
    green += greenStep;
    red += redStep;
    if (blue > 31)
      blue = 31;
    if (green > 31)
      green = 31;
    if (red > 31)
      red = 31;
    *color++ = RGB(red, green, blue);
  }
}

// Arms DMA 0 to write one halfword of the table at work->sheet + 0x1f80 into
// background colour 0 at each horizontal blank, so the backdrop colour
// changes line by line.
void BattleFx_ArmPaletteHBlankDma(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->sheet + 0x1f80, (void *)BG_PLTT, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

// Multi-stage particle streams and circling object scenes.
//
// The particle stream runs in two halves. The rise dissolves the battle map
// into a starfield, then for up to 192 frames lifts a formation of objects
// while motes, orbiting sparks and falling drops are drawn into the map
// buffer over a heat-tinted scanline palette. The burst then switches to a
// blended half-scale canvas and throws embers, screen motes and growing
// flashes from the origin for 54 shaking frames. The burst's flashes use the
// first 128 shared particle records and its embers the 512 records after
// them; the work's own particle slots hold the rising motes (0 to 31) and the
// flames (32 to 47).
//
// The other three effects are one frame loop each:
//
// - The circling scene (Ramses), over the desert backdrop: a formation
//   stands while an orb circles, drops three times throwing puffs, and flies
//   off; six rocks fall and bounce, and the orb's flight ends in a burst of
//   512 sparks from the shared particle records.
// - The dual stream (Dragon Cloud): a flame leaves the user along a stored
//   path with a fading trail of 19 copies behind it, then strikes the first
//   target and bursts into embers and flashes.
// - The fire burst shards (Annihilation): the user closes on the first
//   target, a fire streak flares and a burst of 64 shards scatters from the
//   target.
//
// Within a frame the order of the steps matters: later steps draw over
// earlier ones, and several of them consume random numbers.

// Abilities: Meteor (scene kind 11).
//
// A formation rises out of a dissolving starfield and bursts into embers and
// flashes: the particle stream in mode 0, with one spawned object rising.
DEFINE_EFFECT_KIND(BattleFx_RunMeteor, BattleFx_RunParticleStreams, 0)

// Abilities: Outer Space (kind 384).
//
// The user leaps up, and two objects rise out of a dissolving starfield and
// burst into embers and flashes: the particle stream in mode 1, in the
// lightning bolt's colours.
DEFINE_EFFECT_KIND(BattleFx_RunOuterSpace, BattleFx_RunParticleStreams, 1)

#define STREAM_EMBERS (&PARTICLES[128])

#define PARTICLE_STREAMS_RISE_FRAMES 192 // frames, at most, of the rise
#define PARTICLE_STREAMS_BURST_FRAMES 54 // frames of the burst

// The stream runs in two halves. The rise dissolves the battle map into a
// starfield, then for up to PARTICLE_STREAMS_RISE_FRAMES lifts a formation of
// objects while motes, orbiting sparks and falling drops are drawn into the
// map buffer over a heat-tinted scanline palette. The burst then switches to
// a blended half-scale canvas and throws embers, screen motes and growing
// flashes from the origin for PARTICLE_STREAMS_BURST_FRAMES shaking frames.
//
// Particle slots: the burst's flashes use PARTICLES 0 to 127 and its embers
// the 512 after them (STREAM_EMBERS); work->particles holds the rising motes
// (0 to 31) and the flames (32 to 47).
void BattleFx_RunParticleStreams(struct BattleEffectArgument *effect,
                                 s32 mode) {
  volatile u32 fill;
  u8 offsets[128];
  void **cache;
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;
  void *colors = (void *)BG_PLTT;

  // Start a flame in the first free particle slot from 32 to 47.
  void ParticleStreams_SpawnFlame(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct EffectStep *flame = &work->particles[32 + i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        flame->velocityX = velocityX;
        break;
      }
    }
  }

  // Start a mote in the first free particle slot from 0 to 31.
  UNUSED void ParticleStreams_SpawnMote(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct EffectStep *mote = &work->particles[i];

      if (mote->variant == -1) {
        mote->variant = 0;
        mote->x = x;
        mote->y = y;
        break;
      }
    }
  }

  struct SpritePlacement position;
  s32 vector[3];
  struct EffectPosition spot;
  struct EffectPosition *screenPosition;
  struct EffectPosition point;
  struct SpriteScale scale;
  s32 frame;
  s32 i;

  cache = &gBattleFxWork[1];
  canvas = cache[0];
  work = cache[-1];

  // Begin: in mode 1 the user leaps up with a sound. The stream's direction
  // is 1 when the mode 1 user stands on side 1, otherwise -1.
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0x2000);
  /* FAKEMATCH: test through direction so CSE retains the mode reload
   * in this arm. Testing mode directly replaces ldr r4,[sp,#76] with
   * movs r4,#1 (1532 instructions, two differing rows). 2026-10-13:
   * a switch on mode, side tested by if/else or `side != 1 ? -1 : mode`
   * differ the same 2; direction = -1 first and set in the arm 58. */
  direction = mode;
  if (direction == 1) {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actor)->object;

    object->velocityY = 0xa0000;
    object->verticalMotionStrength = 0x91eb;
    ObjectGroup_UpdateMembers(work->effect->actor, -1, 2, -1, 0);
    AudioCommand_PlayFar(SE_145);
    direction = work->effect->side == 1 ? mode : -1;
  } else {
    direction = -1;
  }
  BattleFx_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  BattleFx_WipeCanvas(0, 0);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  // The rising formation: two animated objects in mode 1, otherwise one
  // spawned object.
  if (mode == 1) {
    for (i = 0; i != 2; i++) {
      struct AnimationObject *object =
          ResourceObject_CreateFar(483 + i * 0x2001);

      work->objects[i] = object;
      if (object != 0) {
        object->flags = 0;
        AnimationObjects_SelectAnimationFar(object, 2);
        work->objects[i]->image.priority = 3;
      }
    }
  } else {
    BattleFx_SpawnObjects(1, 381, 3);
  }
  // The starfield: load the flame sheet (and in mode 1 the bolt's palette),
  // fill the screen map, give palette entries 128 to 190 random bright
  // colours and scatter 256 random dots over the 128x128 canvas.
  Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work, 1, 1);
  if (mode == 1)
    Iwram_CopyWords(colors,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  fill = 0x01010101;
  Dma_Set((void *)&fill, Ram_MapCellBuffer, DMA_FILL_WORDS(0x2000), REG_DMA3);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapCellBuffer, 0x7800);
  WaitFrames(1);
  REG_BLDCNT = 0;
  REG_BG2PA = Q_8_8(1);
  REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  for (i = 0; i != 63; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    ((u16 *)&BG_PLTT_COLOR(8, 0))[i] = RGB(red, green, blue);
  }
  fill = 0;
  Dma_Set((void *)&fill, canvas, DMA_FILL_WORDS(0x1000), REG_DMA3);
  for (i = 0; i != 256; i++) {
    s32 x = Random16() & 127;
    s32 y = Random16() & 127;
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), canvas, 0x4000);
  // Dissolve: start the background scroll, then erase the map from the top
  // in ragged columns, each lagging by one of 128 random offsets of 0 to 63
  // rows. The erased band grows faster every frame until it passes row 191.
  gProjection.centerY = 240;
  BattleFx_SelectLivingTargets(work->effect);
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Scheduler_AddOrUpdateCallback(Camera_AdvanceBg2Reference, 0x4ff);
  Scheduler_AddOrUpdateCallback(BattleFx_FlushPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  for (i = 0; i != 128; i++)
    offsets[i] = Random16() & 63;
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          if (y >= 0)
            if (y <= 127)
              Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                (x & 7)] = 0;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    } while (limit <= 191);
  }
  // The rise: blend the map over the backdrop, colour the backdrop line by
  // line from the heat lines, and draw motes, orbit sparks and drops into the
  // map for up to PARTICLE_STREAMS_RISE_FRAMES frames. A or B skips ahead
  // after frame 16 in mode 1, after frame 4 otherwise. The backdrop's scroll
  // is restored afterwards.
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    // Two independent absolute slot reads grow native EN from 0xe38 to 0xe48.
    void **slots = &gWorkSlot[HEAP_SLOT_BATTLE_BACKGROUND];
    struct BattlePresentationTransition *presentation = slots[0];

    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    BattleFx_BuildBlitter(HEAP_SLOT_BLITTER, 8, 7,
                          COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, COMPOSITE_MAX);
    draw = slots[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_BACKGROUND];
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN,
                        (s32)(Ram_MapBlocks + 0x202));
    Scheduler_AddOrUpdateCallback(BattleFx_ArmPaletteHBlankDma, 0x4fe);
    EFFECT_FREE_PARTICLES(i, work->particles, 64);
    presentation->active = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 angle = frame << 8;
      /* FAKEMATCH: count fixed in r2, as in the reference; as a hard register
       * it leaves the division temp canonical in cse, so the compare and +3 use
       * the temp as in the reference. Unpinned, it differs 10 (tbs-en,
       * 2026-10-18). */
      register s32 count asm("r2") = work->frame;
      /* FAKEMATCH: work's copy in r0; else it and the 0x778c reload swap r0/r1.
       * 2026-10-11: plain `heat = work->frame / 4` with a plain line pointer
       * differs 69: combine merges count's copy, the 0x778c reload and work
       * swap r0/r1, and the halfword zero becomes a literal-pool load once its
       * reload scratch moves. An ordinary base alone leaves 8 aligned
       * differences in 1532 instructions. Integer/word/halfword carriers, a
       * typed inline line helper and general or early-clobber barrier outputs
       * keep that residual. A two-minute exact-stack/branch search (seed 353,
       * nested helpers ignored) found no improvement in 1241 compiled
       * candidates. Changing count alone differs 10, both count and base differ
       * 74, and level alone differs 42. */
      register u8 *base asm("r0") = (u8 *)work;
      s32 heat = count / 4;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH: count read and base passed through; reading count after the
       * division keeps its load apart from the division temp (no combine);
       * through base the asm is not volatile, so no sched2 barrier, and the
       * mode reload lands after movs r2, #252 as in the reference (38 -> 2). */
      asm("" : "=r"(base) : "0"(base), "r"(count));
      line = (u16 *)(base + 0x1f80);
      if (mode == 1) {
        if ((gKeysRepeat & 3) && frame > 16)
          break;
      } else {
        if ((gKeysRepeat & 3) && frame > 4)
          break;
      }
      {
        s32 hold;

        /* FAKEMATCH: hold takes r7 across the call; with line in r5, hold in r7
         * and r6 clobbered, heat takes r4 and its sp+8 caller-save as in the
         * reference (222 -> 89); global.c ignores hard conflicts with r7, the
         * frame pointer, so r7 needs a pseudo. 2026-10-11: a plain call differs
         * 354, heat in r6 with no caller-save and the frame 280 instead of 284.
         */
        asm("" : "=r"(hold));
        /* FAKEMATCH: r6 is clobbered across the call. */
        asm("" : : : "r6");
        if (frame == 0)
          AudioCommand_PlayFar(SE_141);
        /* FAKEMATCH: hold's use after the call. */
        asm("" : : "r"(hold));
      }
      // The heat lines, the 160 backdrop colours that
      // BattleFx_ArmPaletteHBlankDma streams in one per scanline: black on
      // lines 0 to 14 and 135 to 159, and between them a ramp, blue first
      // and then green, that brightens as the rise goes on.
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        /* FAKEMATCH: reuse blue for the biased signed quotient. A
         * separate quotient coalesces with level; the explicit bias
         * keeps the native r3 copy and shift (four register rows). */
        s32 blue = green;
        /* FAKEMATCH: level in r2; unbinding it changes ten register/order rows.
         * 2026-10-11: the natural `level = green / 4 + heat` differs 18, a
         * level/blue r3/r2 swap only (local-alloc hands level r3 first); a use
         * of level after the first blue clamp matches, and moving green's
         * computation there differs 4; no plain statement survives to
         * allocation there. Ordinary level differs 42 in EN (1532
         * native/built). A generic output boundary after its sum reduces this
         * to 26, retaining the quotient/level r2/r3 exchange. A blue input
         * retains 26; also unbinding count differs 36, with either an input or
         * output count operand on the existing base boundary. Reversing the sum
         * or deriving green as blue - 48 retains 42. */
        register s32 level asm("r2");

        if (green < 0)
          blue += 3;
        blue >>= 2;
        level = blue + heat;
        blue = level - 32;
        green = level - 80;

        CLAMP(blue, 0, 31);
        CLAMP(green, 0, 31);
        *line++ = RGB(green >> 1, green, blue);
      }
      for (; i != 160; i++)
        *line++ = 0;
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      // Place the formation (both objects in mode 1), drifting with the
      // stream and growing each frame.
      position.groundHeight = 0;
      position.height = 0xff0000;
      if (mode == 1) {
        size = angle + 0xa000;
        scale.x = size;
        scale.y = size;
        position.x = (x << 16) + 0x500000;
        position.y = (64 - y) << 16;
        Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale,
                                          0);
        Object_ApplyProjectedPlacementFar(work->objects[1], &position, &scale,
                                          0);
      } else {
        size = angle + 0x10000;
        scale.x = size;
        scale.y = size;
        position.x = (x << 16) + 0x600000;
        position.y = (96 - y) << 16;
        Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale,
                                          0);
      }
      // A new mote rises from a random point on the lower half of a circle
      // of radius 30 * size beneath the formation.
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;

          mote->variant = 0;
          mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
          mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
          break;
        }
      }
      // The seven orbit sparks, 8x8, turning with the frame and lifting
      // with it.
      point.x = 0;
      point.y = 0;
      point.depth = 0x2000000;
      Render_ResetTransformState();
      SceneTransform_ApplyPosition(EffectPosition_Coordinates(&point));
      SceneTransform_ApplyRoll(0x800);
      SceneTransform_ApplyYaw(frame << 8);
      for (i = 0; i != 7; i++) {
        SET_VECTOR(vector, (gParticleStreams_OrbitPoints[i][0] / 2) << 16,
                   (gParticleStreams_OrbitPoints[i][1] + frame) << 16,
                   (gParticleStreams_OrbitPoints[i][2] / 2) << 16);
        EffectPosition_ApplyBaseAndYOffset(vector, screenPosition = &spot);
        screenPosition->x = HI(screenPosition->x) + 128;
        screenPosition->y = HI(screenPosition->y) + 60;
        draw(Ram_MapCellBuffer, work->sheet + 0x1f40, screenPosition->x - 4,
             screenPosition->y - 4, 8, 8);
      }
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      // The seven falling drops, 24x24; a drop reaching rows 94 and 95
      // becomes a flame instead.
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 drop = gParticleStreams_DropPoints[i][1] + y;

        if (drop <= 93)
          draw(Ram_MapCellBuffer, work->sheet + 0x1c80,
               gParticleStreams_DropPoints[i][0] + x - 12, drop - 12, 24, 24);
        else if (drop <= 95)
          ParticleStreams_SpawnFlame(
              (gParticleStreams_DropPoints[i][0] + x) << 16, drop << 16, 1);
      }
      for (i = 0; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        // A rising mote shows six 32x32 cells, moving up and against the
        // stream, then frees its slot.
        if (mote->variant >= 0) {
          draw(Ram_MapCellBuffer, work->sheet + (mote->variant << 10),
               HI(mote->x) - 16, HI(mote->y) - 16, 32, 32);
          mote->x -= direction * 0x14000;
          mote->y -= 0x50000;
          mote->variant++;
          if (mote->variant == 6)
            mote->variant = -1;
        }
      }
      EFFECT_PRESENT_FRAME(work);
      frame++;
      work->frame++;
    } while (frame != PARTICLE_STREAMS_RISE_FRAMES);
    WaitFrames(1);
    presentation->active = 0;
    Scheduler_RemoveCallback(Camera_AdvanceBg2Reference);
    Scheduler_RemoveCallback(BattleFx_ArmPaletteHBlankDma);
    Scheduler_RemoveCallback(BattleFx_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  // The burst: switch to the blended half-scale display, load the blast
  // sheet and seed 32 screen motes, 128 flashes and 512 embers, the flashes
  // and embers leaving the origin with random velocities.
  BattleFx_SetupBlendedDisplay();
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  draw = gWorkSlot[HEAP_SLOT_BLITTER];
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work, 1, 0);
  for (i = 0; i != 32; i++) {
    struct EffectStep *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  for (i = 0; i != 128; i++) {
    struct EffectStep *flash = &PARTICLES[i];

    SET_POSITION(flash, 0, 0, 0);
    SET_VELOCITY(flash, ((Random16() & 255) - 127) << 12,
                 (Random16() & 255) << 11, ((Random16() & 255) - 127) << 12);
    flash->variant = 0;
  }
  for (i = 0; i != 512; i++) {
    struct EffectStep *ember = &STREAM_EMBERS[i];

    SET_POSITION(ember, 0, 0, 0);
    SET_VELOCITY(ember, ((Random16() & 255) - 128) << 13,
                 (Random16() & 255) << 11, ((Random16() & 255) - 128) << 13);
    ember->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0x10101010);

  // The burst shakes while the formation keeps climbing and growing. The
  // background brightens from frame 36; on frame 48 the mode 1 objects are
  // released and the battle shows the damage.
  for (frame = 0; frame != PARTICLE_STREAMS_BURST_FRAMES; frame++) {
    s32 size = (frame + 0x1d0) << 8;
    struct BattleCamera *camera = gCameraWork;
    s32 elapsed = frame - 16;
    struct SpriteScale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      AudioCommand_PlayFar(SE_156);
    if (frame == 40)
      AudioCommand_PlayFar(SE_145);
    if (frame == 48) {
      if (mode == 1) {
        ResourceObject_ReleaseFar(work->objects[0]);
        ResourceObject_ReleaseFar(work->objects[1]);
        BattleActor_CommitPlacementFar();
      }
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != 64; i++) {
      // The first 64 embers, sized by depth and falling under gravity; one
      // that falls to height 20 restarts there with a new velocity.
      struct EffectStep *ember = &STREAM_EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        EffectPosition_ApplyBaseAndYOffset(&ember->x, &point);
        point.x >>= 1;
        if (point.depth <= 159)
          point.depth = 160;
        if (point.depth > 799)
          point.depth = 799;
        size = 9 - (point.depth - 160) / 64;
        draw(canvas,
             work->sheet +
                 (gBattleFx_MoteCells[size - 1] + (i & 1) * 770) + 0x3200,
             point.x - size / 2, point.y - size, size, size * 2);
        EffectStep_AdvanceWithGravity3D(ember, 64, -0x2000);
        if (ember->y <= 0x140000) {
          ember->x = 0;
          ember->z = 0;
          ember->y = 0x140000;
          SET_VELOCITY(ember, ((Random16() & 63) - 32) << 15,
                       (Random16() & 63) << 13, ((Random16() & 63) - 32) << 15);
        }
      }
    }
    for (i = 0; i != 64; i++) {
      struct EffectStep *mote = &work->particles[i];
      // 64 screen motes rise 8 pixels a frame plus their index's low three
      // bits, wrapping back to row 128 above the top.
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet + (gBattleFx_MoteCells[size - 1] + (i & 1) * 770) +
               0x3200,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }
    for (i = 0; i != 64; i++) {
      // The first 64 flashes, three more released each frame from frame 17:
      // seven growing cells, two frames each, then a restart at height 20
      // with a random depth and upward speed.
      struct EffectStep *flash = &PARTICLES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        EffectPosition_ApplyBaseAndYOffset(&flash->x, &point);
        point.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 cell = flash->variant / 2;

          draw(canvas, work->sheet + gParticleStreams_FlashCells[cell],
               point.x - gParticleStreams_FlashSizes[cell] / 2,
               point.y - gParticleStreams_FlashSizes[cell] / 2,
               gParticleStreams_FlashSizes[cell],
               gParticleStreams_FlashSizes[cell]);
        }
        flash->variant++;
        if (flash->variant == 14) {
          flash->y = 0x140000;
          flash->x = 0;
          flash->z = ((Random16() & 255) - 127) << 16;
          SET_VELOCITY(flash, 0, (Random16() & 255) << 11, 0);
          flash->variant = 0;
        } else {
          EffectStep_AdvanceWithGravity3D(flash, 64, 1);
        }
      }
    }
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + 0x20000;
    position.groundHeight = 0;
    position.height = 0xff0000;
    if (mode == 1) {
      scale.x = size;
      scale.y = size;
      position.x = (x << 16) + 0x600000;
      position.y = (96 - y) << 16;
      Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale, 0);
      Object_ApplyProjectedPlacementFar(work->objects[1], &position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position.x = (x << 16) + 0x600000;
      position.y = (96 - y) << 16;
      Object_ApplyProjectedPlacementFar(work->objects[0], &position, &scale, 0);
    }
    work->shakeFrames = 1;
    Camera_ApplyShake(8, 8);
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  if (mode == 0)
    ResourceObject_ReleaseFar(work->objects[0]);
  BattleFx_EndCanvasLayer();
}
