/// @file
/// Meteor and Outer Space.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// The particle streams' modes (BattleAnim_RunParticleStreams).
enum ParticleStreamsMode {
  PARTICLE_STREAMS_METEOR,     ///< one spawned object rises
  PARTICLE_STREAMS_OUTER_SPACE ///< the user leaps, and two animated objects
                               ///< rise in the lightning bolt's colours
};

/// Adds per-channel steps to background colours 1 to 63, capped at 31.
///
/// Called once a frame to whiten the colours over a few frames: by
/// BattleAnim_RunParticleStreams from frame 36 of its burst, and by Tiamat's
/// BattleAnim_RunPaletteParticles. Rewrites background palette RAM.
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

/// Arms DMA 0 to write one halfword of the table at work->sheet + 0x1f80 into
/// background colour 0 at each horizontal blank, so the backdrop colour
/// changes line by line.
///
/// Runs each frame as a task (priority TASK_PASS_VBLANK + 0x7e) that
/// BattleAnim_RunParticleStreams adds for its rise and removes after it; the
/// rise rewrites the 160 colours, its heat lines, every frame. Stops DMA 0
/// and sets it up again, so the table is read from line 0.
void BattleAnim_ArmPaletteHBlankDma(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->sheet + 0x1f80, (void *)BG_PLTT, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// A formation rises out of a dissolving starfield and bursts into embers and
/// flashes, with one spawned object rising.
///
/// Abilities: Meteor (SUMMON_ANIM_METEOR).
///
/// BattleAnim_RunSummonEffect also plays it for a summon animation of 0, which
/// no row selects.
void BattleAnim_RunMeteor(struct BattleAnimArgs *effect) {
  BattleAnim_RunParticleStreams(effect, PARTICLE_STREAMS_METEOR);
}

/// The user leaps up, and two objects rise out of a dissolving starfield and
/// burst into embers and flashes, in the lightning bolt's colours.
///
/// Abilities: Outer Space (BATTLE_ANIM_OUTER_SPACE).
void BattleAnim_RunOuterSpace(struct BattleAnimArgs *effect) {
  BattleAnim_RunParticleStreams(effect, PARTICLE_STREAMS_OUTER_SPACE);
}

/// The burst's embers, 512 in PARTICLES after its 128 flashes.
#define PARTICLE_STREAMS_EMBERS (&PARTICLES[128])

#define PARTICLE_STREAMS_RISE_FRAMES 192 ///< frames, at most, of the rise
#define PARTICLE_STREAMS_BURST_FRAMES 54 ///< frames of the burst

/// The mote pictures in the sheet: two sets, MOTE_SET_BYTES apart.
#define PARTICLE_STREAMS_MOTE_CELLS 0x3200

/// The stream runs in two halves. The rise dissolves the battle map into a
/// starfield, then for up to PARTICLE_STREAMS_RISE_FRAMES lifts a formation of
/// objects while motes, orbiting sparks and falling drops are drawn into the
/// map buffer over a heat-tinted scanline palette. The burst then switches to
/// a blended half-scale canvas and throws embers, screen motes and growing
/// flashes from the origin for PARTICLE_STREAMS_BURST_FRAMES shaking frames.
///
/// Particle slots: the burst's flashes use PARTICLES 0 to 127 and its embers
/// the 512 after them (PARTICLE_STREAMS_EMBERS); work->particles holds the
/// rise's motes (0 to 31) and flames (32 to 47), then the burst's screen motes
/// (0 to 63), in whole pixels.
///
/// Called by BattleAnim_RunMeteor and BattleAnim_RunOuterSpace.
/// The rise shows Ram_MapGridCellBuffer on BG1 through
/// BattleAnim_FlushPendingGraphicsTransfer, scrolls BG2 with
/// BattleAnim_AdvanceBg2OnInterval and colours the backdrop with
/// BattleAnim_ArmPaletteHBlankDma, and removes all three after; it sets the
/// background view's active flag while it runs. The projection centre moves
/// down to line 240 from the dissolve on, until the burst's
/// BattleAnim_SetupBlendedDisplay puts it back at line 120; the burst's shake
/// then keeps it within 4 pixels of (120, 120) until
/// BattleAnim_EndCanvasLayer puts it back there at the end. The burst sets
/// BG2 back to the half-scale canvas and ends the canvas layer. Releases the
/// formation objects it spawned (Outer Space's on frame 48 of the burst).
void BattleAnim_RunParticleStreams(struct BattleAnimArgs *effect, s32 mode) {
  volatile u32 fill;
  u8 offsets[128];
  void **cache;
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleAnimWork *work;
  void *colors = (void *)BG_PLTT;

  /// Starts a flame in the first free particle slot from 32 to 47; called
  /// for each drop that reaches rows 94 and 95. A flame is never drawn.
  void ParticleStreams_SpawnFlame(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct Particle *flame = &work->particles[32 + i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        flame->velocityX = velocityX;
        break;
      }
    }
  }

  /// Starts a mote in the first free particle slot from 0 to 31. Nothing
  /// calls it: the rise starts its motes inline.
  UNUSED void ParticleStreams_SpawnMote(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct Particle *mote = &work->particles[i];

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
  struct ScreenPoint spot;
  struct ScreenPoint *screenPosition;
  struct ScreenPoint screen;
  struct SpriteScale scale;
  s32 frame;
  s32 i;

  cache = &gBattleAnimWork[1];
  canvas = cache[0];
  work = cache[-1];

  // Begin: in Outer Space the user leaps up with a sound. The stream's
  // direction is 1 when that user stands on ANIM_SIDE_PARTY, otherwise -1.
  work->effect = effect;
  ANIM_START_NARROW_CANVAS(BGCNT_WRAP);
  /* FAKEMATCH(temporary): mode tested through direction, so the arm reloads
   * mode where CSE would put 1; as if (mode == PARTICLE_STREAMS_OUTER_SPACE),
   * 1 of 1532 instructions differ in tbs-en. */
  direction = mode;
  if (direction == PARTICLE_STREAMS_OUTER_SPACE) {
    struct Object *animation =
        GetBattleObjectSlotFar(work->effect->user)->object;

    // The leap: 10 pixels a frame upwards, against a gravity of 0x91eb, about
    // 0.57 pixels a frame lost each frame.
    animation->velocityY = 0xa0000;
    animation->gravity = 0x91eb;
    BattleAnim_TintBattler(work->effect->user, -1, POSE_LEAP, -1, 0);
    Sound_PlaySongFar(SE_CRASH);
    direction = work->effect->side == ANIM_SIDE_PARTY ? mode : -1;
  } else {
    direction = -1;
  }
  BattleAnim_ConfigureEffectDisplay();
  // The backdrop and colour 1 go black: the wipe below
  // sweeps the canvas to colour 1, top to bottom.
  PLTT_BACKDROP = 0;
  *(u16 *)&BG_PLTT[1] = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleAnim_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  // The rising formation: two animated objects in Outer Space, otherwise one
  // spawned object.
  if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
    for (i = 0; i != 2; i++) {
      struct Sprite *animation = Sprite_CreateFar(483 + i * SPRITE_SECOND_PART);

      work->animations[i] = animation;
      if (animation != 0) {
        animation->flags = 0;
        Sprite_StartAnimFar(animation, 2);
        work->animations[i]->image.priority = 3;
      }
    }
  } else {
    BattleAnim_SpawnObjects(1, SPRITE_METEOR, 3);
  }
  // The starfield: load the flame sheet (and in Outer Space the bolt's
  // palette), fill the screen map, give palette entries 128 to 190 random
  // bright colours and scatter 256 random dots over the 128x128 canvas.
  Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work->sheet, 1, 1);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE)
    Iwram_CopyWords(colors,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  // The map picture, 32x16 tiles of 8x8 pixels (256x128, 0x2000 words), all
  // colour 1; its first BATTLE_BACKGROUND_BYTES are what BG1 shows.
  fill = CANVAS_FILL_WORD(1);
  Dma_Set((void *)&fill, Ram_MapGridCellBuffer, DMA_FILL_WORDS(0x2000),
          REG_DMA3);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapGridCellBuffer,
                  BATTLE_BACKGROUND_BYTES);
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

    // The tiled canvas byte for (x, y), as BattleAnim_WipeCanvas gives it.
    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
  // Dissolve: start the background scroll, then erase the map from the top
  // in ragged columns, each lagging by one of 128 random offsets of 0 to 63
  // rows. The erased band grows faster every frame until it passes row 191.
  gProjection.centerY = 240;
  BattleAnim_SelectLivingTargets(work->effect);
  // Every second frame BG2's reference point (20.8 fixed point) moves half a
  // pixel across, the way direction says, and one pixel down.
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Task_Add(BattleAnim_AdvanceBg2OnInterval, TASK_PASS_VBLANK + 0x7f);
  Task_Add(BattleAnim_FlushPendingGraphicsTransfer, TASK_PASS_VBLANK);
  for (i = 0; i != 128; i++)
    offsets[i] = Random16() & 63;
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    // The band erases 0, 2, 4, 6 and so on more rows each frame. A column
    // lagging the full 63 rows erases its last row, 127, at row 190.
    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          // Columns x and x + 128 share a lag. The map is tiled like the
          // canvas, but 32 tiles a row.
          if (y >= 0)
            if (y <= 127)
              Ram_MapGridCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                    (x & 7)] = 0;
        }
      }
      ANIM_PRESENT_FRAME(work);
    } while (limit <= 191);
  }
  // The rise: blend the map over the backdrop, colour the backdrop line by
  // line from the heat lines, and draw motes, orbit sparks and drops into the
  // map for up to PARTICLE_STREAMS_RISE_FRAMES frames. A or B skips ahead
  // after frame 16 in Outer Space, after frame 4 otherwise. The backdrop's
  // scroll is restored afterwards.
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    // One pointer serves both slot reads: two separate reads of gHeapSlots
    // make the function 0xe48 bytes long, where tbs-en's is 0xe38.
    void **slots = &gHeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
    struct BattleBackgroundWork *view = slots[0];

    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    BattleAnim_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, ANIM_CLIP_BOUNDS,
                            COMPOSITE_LIGHTEN);
    draw = slots[HEAP_SLOT_RAM_CODE - HEAP_SLOT_BATTLE_BACKGROUND];
    // Each copy darkens every pixel of the map by 2 levels (four bytes of 2
    // in one word, as the other darkening animations write theirs).
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
    Task_Add(BattleAnim_ArmPaletteHBlankDma, TASK_PASS_VBLANK + 0x7e);
    ANIM_FREE_PARTICLES(i, work->particles, 64);
    view->busy = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 angle = frame << 8;
      /* FAKEMATCH(pin): count in r2, so the division's +3 works on its copy in
       * r3; as a plain local, 5 of 1532 instructions differ in tbs-en. */
      register s32 count asm("r2") = work->frame;
      /* FAKEMATCH(pin): work's copy in r0, leaving r1 to the 0x778c offset; as
       * a plain local, 4 of 1532 instructions differ in tbs-en. */
      register u8 *base asm("r0") = (u8 *)work;
      s32 heat = count / 4;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH(barrier): count and base through an empty asm, which loads
       * count into r2 and copies it to r3; without it, 1 of 1532
       * instructions differ in tbs-en. */
      asm("" : "=r"(base) : "0"(base), "r"(count));
      line = (u16 *)(base + 0x1f80);
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 16)
          break;
      } else {
        if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 4)
          break;
      }
      {
        s32 hold;

        /* FAKEMATCH(barrier): hold set by an empty asm takes r7 across the
         * call, so heat takes r4 and its sp+8 save; without hold, 168 of 1532
         * instructions differ in tbs-en. */
        asm("" : "=r"(hold));
        /* FAKEMATCH(barrier): an empty asm that clobbers r6 before the call;
         * without it, 175 of 1532 instructions differ in tbs-en. */
        asm("" : : : "r6");
        if (frame == 0)
          Sound_PlaySongFar(SE_RUMBLE);
        /* FAKEMATCH(barrier): hold's use in an empty asm, which keeps it live
         * across the call; without it, 168 of 1532 instructions differ in
         * tbs-en. */
        asm("" : : "r"(hold));
      }
      // The heat lines, the 160 backdrop colours that
      // BattleAnim_ArmPaletteHBlankDma streams in one per scanline: black on
      // lines 0 to 14 and 135 to 159, and between them a ramp, blue first
      // and then green, that brightens as the rise goes on: a line's level
      // is (line - 16) / 4 plus a level every 4 frames, its blue that less
      // 32, its green that less 80 and its red half the green.
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        /* FAKEMATCH(temporary): blue holds the biased dividend and quotient,
         * keeping the r3 copy and shift; as level = green / 4 + heat, 5 of
         * 1532 instructions differ in tbs-en. */
        s32 blue = green;
        /* FAKEMATCH(pin): level in r2; as a plain local, 21 of 1532
         * instructions differ in tbs-en. */
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
      // Place the formation (both objects in Outer Space), drifting with the
      // stream and growing each frame: its 16.16 scale starts at 0.625 in
      // Outer Space and 1 otherwise, and gains 1/256 a frame.
      position.groundHeight = 0;
      position.height = 0xff0000;
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        size = angle + 0xa000;
        scale.x = size;
        scale.y = size;
        position.x = (x << 16) + 0x500000;
        position.y = (64 - y) << 16;
        Object_ApplyProjectedPlacementFar(work->animations[0], &position,
                                          &scale, 0);
        Object_ApplyProjectedPlacementFar(work->animations[1], &position,
                                          &scale, 0);
      } else {
        size = angle + 0x10000;
        scale.x = size;
        scale.y = size;
        position.x = (x << 16) + 0x600000;
        position.y = (96 - y) << 16;
        Object_ApplyProjectedPlacementFar(work->animations[0], &position,
                                          &scale, 0);
      }
      // A new mote rises from a random point on the lower half of a circle
      // of radius 30 * size beneath the formation.
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct Particle *mote = &work->particles[i];

        if (mote->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;

          mote->variant = 0;
          mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
          mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
          break;
        }
      }
      // The seven orbit sparks, 8x8, turning with the frame and lifting
      // with it. Angles are in 1/65536 turns: the orbit is tilted 1/32 turn
      // and turns once every 256 frames.
      // The sparks never project: moved 512 pixels along z, with the yaw
      // turning them at most 43 pixels from that, they lie behind the view,
      // nearer than gProjection's near depth, so Transform_ProjectPoint
      // leaves the transformed point, read back as 16.16 and centred by hand.
      screen.x = 0;
      screen.y = 0;
      screen.depth = 0x2000000;
      Transform_Reset();
      Transform_ApplyPosition(ScreenPoint_Coordinates(&screen));
      Transform_ApplyRoll(0x800);
      Transform_ApplyYaw(frame << 8);
      for (i = 0; i != 7; i++) {
        SET_VECTOR(vector, (gParticleStreams_OrbitPoints[i][0] / 2) << 16,
                   (gParticleStreams_OrbitPoints[i][1] + frame) << 16,
                   (gParticleStreams_OrbitPoints[i][2] / 2) << 16);
        Projection_Point(vector, screenPosition = &spot);
        screenPosition->x = Q_16_16_TO_INT(screenPosition->x) + 128;
        screenPosition->y = Q_16_16_TO_INT(screenPosition->y) + 60;
        draw(Ram_MapGridCellBuffer, work->sheet + 0x1f40, screenPosition->x - 4,
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
          draw(Ram_MapGridCellBuffer, work->sheet + 0x1c80,
               gParticleStreams_DropPoints[i][0] + x - 12, drop - 12, 24, 24);
        else if (drop <= 95)
          ParticleStreams_SpawnFlame(
              (gParticleStreams_DropPoints[i][0] + x) << 16, drop << 16, 1);
      }
      for (i = 0; i != 32; i++) {
        struct Particle *mote = &work->particles[i];

        // A rising mote shows six 32x32 cells, 1 KB each from the start of
        // the sheet, one a frame, moving 5 pixels up and 1.25 against the
        // stream each frame, then frees its slot.
        if (mote->variant >= 0) {
          draw(Ram_MapGridCellBuffer, work->sheet + (mote->variant << 10),
               Q_16_16_TO_INT(mote->x) - 16, Q_16_16_TO_INT(mote->y) - 16, 32,
               32);
          mote->x -= direction * 0x14000;
          mote->y -= 0x50000;
          mote->variant++;
          if (mote->variant == 6)
            mote->variant = -1;
        }
      }
      ANIM_PRESENT_FRAME(work);
      frame++;
      work->frame++;
    } while (frame != PARTICLE_STREAMS_RISE_FRAMES);
    WaitFrames(1);
    view->busy = 0;
    Task_Remove(BattleAnim_AdvanceBg2OnInterval);
    Task_Remove(BattleAnim_ArmPaletteHBlankDma);
    Task_Remove(BattleAnim_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }
  FreeHeapSlot(HEAP_SLOT_RAM_CODE);
  // The burst, PARTICLE_STREAMS_BURST_FRAMES frames: switch to the blended
  // half-scale display, load the blast
  // sheet and seed 32 screen motes, 128 flashes and 512 embers, the flashes
  // and embers leaving the origin with random velocities.
  BattleAnim_SetupBlendedDisplay();
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
  draw = gHeapSlots[HEAP_SLOT_RAM_CODE];
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  // Screen motes keep whole canvas pixels, not 16.16: x 0 to 127 and y 127
  // to 254, from the canvas's last row down, from where they rise.
  for (i = 0; i != 32; i++) {
    struct Particle *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  // Velocities in 16.16 pixels a frame: a flash up to 8 each way across and 8
  // up, an ember up to 16 each way across and 8 up.
  for (i = 0; i != 128; i++) {
    struct Particle *flash = &PARTICLES[i];

    SET_POSITION(flash, 0, 0, 0);
    SET_VELOCITY(flash, ((Random16() & 255) - 127) << 12,
                 (Random16() & 255) << 11, ((Random16() & 255) - 127) << 12);
    flash->variant = 0;
  }
  for (i = 0; i != 512; i++) {
    struct Particle *ember = &PARTICLE_STREAMS_EMBERS[i];

    SET_POSITION(ember, 0, 0, 0);
    SET_VELOCITY(ember, ((Random16() & 255) - 128) << 13,
                 (Random16() & 255) << 11, ((Random16() & 255) - 128) << 13);
    ember->variant = 0;
  }
  // Each copy refills the canvas with level 16.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

  // The burst shakes while the formation keeps climbing and growing. The
  // background brightens from frame 36; on frame 48 Outer Space's objects are
  // released and the battle shows the damage.
  for (frame = 0; frame != PARTICLE_STREAMS_BURST_FRAMES; frame++) {
    s32 size = (frame + 0x1d0) << 8;
    struct BattleCamera *camera = gBattleCamera;
    s32 elapsed = frame - 16;
    struct SpriteScale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      Sound_PlaySongFar(SE_SWITCH_MECHANISM);
    if (frame == 40)
      Sound_PlaySongFar(SE_CRASH);
    if (frame == 48) {
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        Sprite_DestroyFar(work->animations[0]);
        Sprite_DestroyFar(work->animations[1]);
        BattleObject_PutFar();
      }
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    ANIM_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != 64; i++) {
      // The first 64 embers, sized by depth and falling under gravity; one
      // that falls to height 20 restarts there with a new velocity.
      struct Particle *ember = &PARTICLE_STREAMS_EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        Projection_Point(&ember->x, &screen);
        screen.x >>= 1;
        if (screen.depth <= 159)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        // Mote size 9 at depth 160, one smaller every 64 units further.
        // UB: from depth 736 the size is 0, a picture 0 pixels wide, and its
        // cell is read from gBattleAnim_MoteCells[-1], the last halfword of
        // the blitter template, which ends at the table
        // (BattleAnim_BuildBlitter). No UBFIX form yet.
        size = 9 - (screen.depth - 160) / 64;
        draw(canvas,
             work->sheet +
                 (gBattleAnim_MoteCells[size - 1] + (i & 1) * MOTE_SET_BYTES) +
                 PARTICLE_STREAMS_MOTE_CELLS,
             screen.x - size / 2, screen.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(ember, 64, -0x2000);
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
      struct Particle *mote = &work->particles[i];
      // 64 screen motes rise 8 pixels a frame plus their index's low three
      // bits, wrapping back to row 128 above the top. Only 0 to 31 were
      // seeded; 32 to 63 keep what the slots held, such as a flame's 16.16
      // position, far off the canvas.
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet +
               (gBattleAnim_MoteCells[size - 1] + (i & 1) * MOTE_SET_BYTES) +
               PARTICLE_STREAMS_MOTE_CELLS,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }
    for (i = 0; i != 64; i++) {
      // The first 64 flashes, three more released each frame from frame 17:
      // seven growing cells, two frames each, then a restart at height 20
      // with a random depth and upward speed.
      struct Particle *flash = &PARTICLES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        Projection_Point(&flash->x, &screen);
        screen.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 cell = flash->variant / 2;

          draw(canvas, work->sheet + gParticleStreams_FlashCells[cell],
               screen.x - gParticleStreams_FlashSizes[cell] / 2,
               screen.y - gParticleStreams_FlashSizes[cell] / 2,
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
          // A gravity of 1, 1/65536 pixel a frame: a flash keeps its speed.
          Particle_AdvanceWithGravity3D(flash, 64, 1);
        }
      }
    }
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    // The formation keeps growing by 1/256 a frame, from a 16.16 scale of
    // about 1.81 in Outer Space (size) and 2 otherwise (grow).
    grow = (frame << 8) + 0x20000;
    position.groundHeight = 0;
    position.height = 0xff0000;
    if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
      scale.x = size;
      scale.y = size;
      position.x = (x << 16) + 0x600000;
      position.y = (96 - y) << 16;
      Object_ApplyProjectedPlacementFar(work->animations[0], &position, &scale,
                                        0);
      Object_ApplyProjectedPlacementFar(work->animations[1], &position, &scale,
                                        0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position.x = (x << 16) + 0x600000;
      position.y = (96 - y) << 16;
      Object_ApplyProjectedPlacementFar(work->animations[0], &position, &scale,
                                        0);
    }
    work->shakeFrames = 1;
    BattleCamera_Shake(8, 8);
    ANIM_PRESENT_FRAME(work);
  }
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  FreeHeapSlot(HEAP_SLOT_RAM_CODE);
  if (mode == PARTICLE_STREAMS_METEOR)
    Sprite_DestroyFar(work->animations[0]);
  BattleAnim_EndCanvasLayer();
}

#undef PARTICLE_STREAMS_EMBERS
#undef PARTICLE_STREAMS_RISE_FRAMES
#undef PARTICLE_STREAMS_BURST_FRAMES
#undef PARTICLE_STREAMS_MOTE_CELLS
