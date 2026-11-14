/// @file
/// Meteor and Outer Space.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The particle streams' modes (BattleEffect_RunParticleStreams).
enum ParticleStreamsMode {
  PARTICLE_STREAMS_METEOR,     ///< one spawned sprite falls
  PARTICLE_STREAMS_OUTER_SPACE ///< the user leaps, and two animated sprites
                               ///< fall in the lightning bolt's colours
};

/// The pictures in the flame sheet (ResourceId_FlameSheetB), each starting
/// where the one before ends: six 32x32 flames, then three 24x24 and three 8x8
/// glows, each glow brighter at its centre than the one before. The fall draws
/// only the six and the brightest glow of each size.
enum ParticleStreamsSheetPicture {
  /// The six 32x32 flames, in a row
  PARTICLE_STREAMS_FLAMES = 0,
  /// The brightest 24x24 glow
  PARTICLE_STREAMS_LARGE_GLOW = PARTICLE_STREAMS_FLAMES + 6 * 32 * 32 +
      2 * 24 * 24,
  /// The brightest 8x8 glow
  PARTICLE_STREAMS_SMALL_GLOW = PARTICLE_STREAMS_LARGE_GLOW + 24 * 24 +
      2 * 8 * 8,
  /// Past the sheet's end: the backdrop's 160 colours, one a line, that the
  /// fall writes there.
  PARTICLE_STREAMS_BACKDROP_LINES = PARTICLE_STREAMS_SMALL_GLOW + 8 * 8
};

/// Brightens background colours 1 to 63 by a step in each channel.
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

/// Streams the fall's backdrop colours into colour 0, one each line.
void BattleEffect_ArmPaletteHBlankDma(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->sheet + PARTICLE_STREAMS_BACKDROP_LINES, (void *)BG_PLTT,
          DMA_HBLANK_HALFWORD_PER_LINE, (volatile u32 *)channel);
}

/// The battle dissolves into a starfield and a meteor falls through it in
/// flames among falling fireballs, then bursts in embers and flashes.
///
/// Abilities: Meteor.
void BattleEffect_RunMeteor(struct BattleEffectParams *args) {
  BattleEffect_RunParticleStreams(args, PARTICLE_STREAMS_METEOR);
}

/// The Fusion Dragon leaps out of sight and falls back through a starfield in
/// flames, then bursts in embers and flashes, all in the lightning bolt's
/// colours.
///
/// Abilities: Outer Space, the Fusion Dragon's.
void BattleEffect_RunOuterSpace(struct BattleEffectParams *args) {
  BattleEffect_RunParticleStreams(args, PARTICLE_STREAMS_OUTER_SPACE);
}

/// The burst's embers, 512 in PARTICLES after its 128 flashes.
#define PARTICLE_STREAMS_EMBERS (&PARTICLES[128])

/// The mote pictures in the blast sheet, after its seven blast pictures
/// (BLAST_PICTURE_SIZES): two sets, MOTE_SET_BYTES apart.
#define PARTICLE_STREAMS_MOTE_PICTURES (16 * 16 + 32 * 32 + 5 * 48 * 48)

/// Meteor and Outer Space: the fall through the starfield, then the burst.
void BattleEffect_RunParticleStreams(struct BattleEffectParams *args,
                                     s32 mode) {
  volatile u32 fill;
  u8 offsets[128];
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;
  void *colors = (void *)BG_PLTT;

  /// Starts a landing where a fireball reaches the ground; it is never drawn.
  void ParticleStreams_SpawnLanding(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct Particle *landing = &work->particles[32 + i];

      if (landing->variant == -1) {
        landing->variant = 0;
        landing->x = x;
        landing->y = y;
        landing->velocityX = velocityX;
        break;
      }
    }
  }

  /// Starts a flame; the fall starts its own inline.
  UNUSED void ParticleStreams_SpawnFlame(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct Particle *flame = &work->particles[i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        break;
      }
    }
  }

  struct SpritePlacement position;
  s32 vector[3];
  struct ScreenPoint spot;
  struct ScreenPoint screen;
  struct SpriteScale scale;
  s32 frame;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  // In Outer Space the Fusion Dragon leaps out of sight.
  work->args = args;
  EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
  direction = mode;
  if (direction == PARTICLE_STREAMS_OUTER_SPACE) {
    struct Actor *actor = GetBattleObjectSlotFar(work->args->user)->actor;

    actor->velocityY = px(10);
    actor->gravity = px(57) / 100;
    BattleEffect_TintBattler(work->args->user, -1, POSE_LEAP, -1, 0);
    Sound_PlayFar(SE_CRASH);
    direction = work->args->side == EFFECT_SIDE_PARTY ? 1 : -1;
  } else {
    direction = -1;
  }
  BattleEffect_ConfigureEffectDisplay();
  // The backdrop and colour 1 go black for the wipe.
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_WipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
    for (i = 0; i != 2; i++) {
      struct Sprite *sprite = Sprite_CreateFar(SPRITE_BATTLE_FUSION_DRAGON +
                                               i * SPRITE_SECOND_PART);

      work->sprites[i] = sprite;
      if (sprite != 0) {
        sprite->flags = 0;
        Sprite_StartAnimationFar(sprite, 2);
        work->sprites[i]->image.priority = 3;
      }
    }
  } else {
    BattleEffect_SpawnObjects(1, SPRITE_METEOR, 3);
  }
  // The starfield.
  Resource_LoadAndDecompress(RESOURCE_FLAME_SHEET_B, work->sheet, 1, 1);
  if (mode == PARTICLE_STREAMS_OUTER_SPACE)
    Iwram_CopyWords(colors,
                    Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET), 128);
  fill = CANVAS_FILL_WORD(1);
  Dma_Set((void *)&fill, Ram_MapGridCellBuffer, DMA_FILL_WORDS(256 * 128 / 4),
          REG_DMA3);
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), Ram_MapGridCellBuffer,
                  BATTLE_BACKGROUND_BYTES);
  wait(1);
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

    BG_PLTT_COLOR(8, i) = RGB(red, green, blue);
  }
  fill = 0;
  Dma_Set((void *)&fill, canvas, DMA_FILL_WORDS(CANVAS_BYTES / 4), REG_DMA3);
  for (i = 0; i != 256; i++) {
    s32 x = Random16() & 127;
    s32 y = Random16() & 127;
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
  // The dissolve: the map is erased from the top in ragged columns.
  gProjection.centerY = 240;
  BattleEffect_SelectLivingTargets(work->args);
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Task_Add(BattleEffect_AdvanceBg2OnInterval, TASK_PASS_VBLANK + 127);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_VBLANK);
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
              Ram_MapGridCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                    (x & 7)] = 0;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (limit <= 191);
  }
  // The fall.
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    void **slots = &gHeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
    struct BattleBackgroundWork *view = slots[0];

    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 8, 7, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_LIGHTEN);
    draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
    Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_VBLANK + 126);
    EFFECT_FREE_PARTICLES(i, work->particles, 64);
    view->busy = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 growth = frame << 8;
      s32 count = work->frame;
      u8 *sheet = work->sheet;
      s32 dividend;
      s32 glow;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH(temporary): the division written out on a copy of count. */
      dividend = count;
      /* FAKEMATCH(dead): updates that cancel out end CSE's knowledge of count.
       */
      if (sheet) {
        count++;
        count--;
      }
      if (dividend < 0)
        dividend += 3;
      glow = dividend >> 2;
      /* FAKEMATCH(barrier): count and sheet through an empty asm clobbering r1
       * and r3. */
      __asm__("" : "=r"(sheet) : "0"(sheet), "r"(count) : "r1", "r3");
      line = (u16 *)(sheet + PARTICLE_STREAMS_BACKDROP_LINES);
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
         * call. */
        __asm__("" : "=r"(hold));
        /* FAKEMATCH(barrier): an empty asm that clobbers r6 before the call. */
        __asm__("" : : : "r6");
        if (frame == 0)
          Sound_PlayFar(SE_RUMBLE);
        /* FAKEMATCH(dead): updates of hold that cancel out, under a test of
         * hold. */
        if (hold) {
          hold++;
          hold--;
        }
      }
      // The backdrop: a blue glow that climbs and pales as the fall goes on.
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        s32 quarter = green / 4;
        s32 level = quarter + glow;
        s32 blue;

        /* FAKEMATCH(dead): updates of quarter that cancel out, under a test of
         * quarter, keep it live past level's sum. */
        if (quarter) {
          quarter++;
          quarter--;
        }
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
      // The falling sprite drifts with the stream and grows.
      position.groundHeight = 0;
      position.height = px(255);
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        size = growth + Q16_ONE * 5 / 8;
        scale.x = size;
        scale.y = size;
        position.x = px(x) + px(80);
        position.y = px(64 - y);
        Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale,
                                         0);
        Actor_ApplyProjectedPlacementFar(work->sprites[1], &position, &scale,
                                         0);
      } else {
        size = growth + Q16_ONE;
        scale.x = size;
        scale.y = size;
        position.x = px(x) + px(96);
        position.y = px(96 - y);
        Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale,
                                         0);
      }
      // A new flame below the falling sprite.
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct Particle *flame = &work->particles[i];

        if (flame->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + TRIG_TURN / 4;

          flame->variant = 0;
          flame->x = px(x + 96) + Trig_Sin(angle) * 30 / 65536 * size;
          flame->y = px(y) - Trig_Cos(angle) * 30 / 65536 * size;
          break;
        }
      }
      // Seven sparks orbit the falling sprite.
      screen.x = 0;
      screen.y = 0;
      screen.depth = px(512);
      Transform_Reset();
      Transform_ApplyPosition(ScreenPoint_Coordinates(&screen));
      Transform_ApplyRoll(TRIG_TURN / 32);
      Transform_ApplyYaw(frame << 8);
      for (i = 0; i != 7; i++) {
        SET_VECTOR(vector, px(gParticleStreams_OrbitPoints[i][0] / 2),
                   px(gParticleStreams_OrbitPoints[i][1] + frame),
                   px(gParticleStreams_OrbitPoints[i][2] / 2));
        Projection_Point(vector, &spot);
        spot.x = Q_16_16_TO_INT(spot.x) + 128;
        spot.y = Q_16_16_TO_INT(spot.y) + 60;
        draw(Ram_MapGridCellBuffer, work->sheet + PARTICLE_STREAMS_SMALL_GLOW,
             spot.x - 4, spot.y - 4, 8, 8);
      }
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      // Seven fireballs fall; one that reaches the ground becomes a landing.
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 fireballY = gParticleStreams_FireballPoints[i][1] + y;

        if (fireballY <= 93)
          draw(Ram_MapGridCellBuffer, work->sheet + PARTICLE_STREAMS_LARGE_GLOW,
               gParticleStreams_FireballPoints[i][0] + x - 12, fireballY - 12,
               24, 24);
        else if (fireballY <= 95)
          ParticleStreams_SpawnLanding(
              px(gParticleStreams_FireballPoints[i][0] + x), px(fireballY), 1);
      }
      for (i = 0; i != 32; i++) {
        struct Particle *flame = &work->particles[i];

        // A flame rises against the stream through its six pictures.
        if (flame->variant >= 0) {
          draw(Ram_MapGridCellBuffer,
               work->sheet + PARTICLE_STREAMS_FLAMES +
                   flame->variant * (32 * 32),
               Q_16_16_TO_INT(flame->x) - 16, Q_16_16_TO_INT(flame->y) - 16, 32,
               32);
          flame->x -= direction * (px(5) / 4);
          flame->y -= px(5);
          flame->variant++;
          if (flame->variant == 6)
            flame->variant = -1;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
      frame++;
      work->frame++;
    } while (frame != 192);
    wait(1);
    view->busy = 0;
    Task_Remove(BattleEffect_AdvanceBg2OnInterval);
    Task_Remove(BattleEffect_ArmPaletteHBlankDma);
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  // The burst: embers, rising motes and growing flashes from the impact.
  BattleEffect_SetupBlendedDisplay();
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BG2Y = -(16 << 8);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Resource_LoadAndDecompress(RESOURCE_BLAST_SHEET, work->sheet, 1, 0);
  for (i = 0; i != 32; i++) {
    struct Particle *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
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
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

  for (frame = 0; frame != 54; frame++) {
    s32 size = (frame + 464) << 8;
    struct BattleCamera *camera = gBattleCamera;
    s32 elapsed = frame - 16;
    struct SpriteScale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Palette_BrightenBgEntries(2, 2, 2);
    if (frame == 0)
      Sound_PlayFar(SE_SWITCH_MECHANISM);
    if (frame == 40)
      Sound_PlayFar(SE_CRASH);
    if (frame == 48) {
      if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
        Sprite_DestroyFar(work->sprites[0]);
        Sprite_DestroyFar(work->sprites[1]);
        BattleObject_PutFar();
      }
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    for (i = 0; i != 64; i++) {
      // Embers fall and bounce back up from the ground.
      struct Particle *ember = &PARTICLE_STREAMS_EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        Projection_Point(&ember->x, &screen);
        screen.x >>= 1;
        if (screen.depth <= 159)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        // UB: from depth 736 the size is 0, a picture 0 pixels wide, and its
        // picture is read from gBattleEffect_MotePictures[-1], the last
        // halfword of the blitter template, which ends at the table
        // (BattleEffect_BuildBlitter). No UBFIX form yet.
        size = 9 - (screen.depth - 160) / 64;
        draw(canvas,
             work->sheet +
                 (gBattleEffect_MotePictures[size - 1] +
                  (i & 1) * MOTE_SET_BYTES) +
                 PARTICLE_STREAMS_MOTE_PICTURES,
             screen.x - size / 2, screen.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(ember, 64, -px(1) / 8);
        if (ember->y <= px(20)) {
          ember->x = 0;
          ember->z = 0;
          ember->y = px(20);
          SET_VELOCITY(ember, ((Random16() & 63) - 32) << 15,
                       (Random16() & 63) << 13, ((Random16() & 63) - 32) << 15);
        }
      }
    }
    for (i = 0; i != 64; i++) {
      struct Particle *mote = &work->particles[i];
      // Motes rise up the screen and wrap round.
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet +
               (gBattleEffect_MotePictures[size - 1] +
                (i & 1) * MOTE_SET_BYTES) +
               PARTICLE_STREAMS_MOTE_PICTURES,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }
    for (i = 0; i != 64; i++) {
      // Flashes grow and rise, three more each frame.
      struct Particle *flash = &PARTICLES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        Projection_Point(&flash->x, &screen);
        screen.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 picture = flash->variant / 2;

          draw(canvas, work->sheet + gParticleStreams_FlashPictures[picture],
               screen.x - gParticleStreams_FlashSizes[picture] / 2,
               screen.y - gParticleStreams_FlashSizes[picture] / 2,
               gParticleStreams_FlashSizes[picture],
               gParticleStreams_FlashSizes[picture]);
        }
        flash->variant++;
        if (flash->variant == 14) {
          flash->y = px(20);
          flash->x = 0;
          flash->z = ((Random16() & 255) - 127) << 16;
          SET_VELOCITY(flash, 0, (Random16() & 255) << 11, 0);
          flash->variant = 0;
        } else {
          Particle_AdvanceWithGravity3D(flash, 64, 1);
        }
      }
    }
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + (2 << 16);
    position.groundHeight = 0;
    position.height = px(255);
    if (mode == PARTICLE_STREAMS_OUTER_SPACE) {
      scale.x = size;
      scale.y = size;
      position.x = px(x) + px(96);
      position.y = px(96 - y);
      Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale, 0);
      Actor_ApplyProjectedPlacementFar(work->sprites[1], &position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position.x = px(x) + px(96);
      position.y = px(96 - y);
      Actor_ApplyProjectedPlacementFar(work->sprites[0], &position, &scale, 0);
    }
    work->shakeFrames = 1;
    BattleCamera_Shake(8, 8);
    EFFECT_PRESENT_CANVAS(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  if (mode == PARTICLE_STREAMS_METEOR)
    Sprite_DestroyFar(work->sprites[0]);
  BattleEffect_EndCanvasLayer();
}

#undef PARTICLE_STREAMS_EMBERS
#undef PARTICLE_STREAMS_MOTE_PICTURES
