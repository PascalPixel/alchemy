/// @file
/// Dragon Driver and the Tiamat summon.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Who breathes the dragon's fire (BattleEffect_RunPaletteParticles).
/// ★ Named from what the player sees: Tiamat's fire (its render)
enum DragonFireMode {
  DRAGON_FIRE_TIAMAT,       ///< Tiamat lands beside the targets and breathes
                            ///< the fire, then bursts away
                            ///< (BattleEffect_RunImpactBurst)
  DRAGON_FIRE_DRAGON_DRIVER ///< no dragon is drawn: the breath starts just in
                            ///< front of the targets, in the LightningBolt
                            ///< colours
};

/// Fireballs rain on the targets and a blast of fire sweeps across them, in
/// the LightningBolt colours.
///
/// Abilities: Dragon Driver (BATTLE_EFFECT_DRAGON_DRIVER), the Fusion Dragon's.
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args) {
  BattleEffect_RunPaletteParticles(args, DRAGON_FIRE_DRAGON_DRIVER);
}
/// Tiamat lands while fireballs rain on the targets, breathes fire across
/// them, and bursts away.
///
/// Abilities: Tiamat (SUMMON_EFFECT_TIAMAT).
void BattleEffect_RunTiamat(struct BattleEffectParams *args) {
  BattleEffect_RunPaletteParticles(args, DRAGON_FIRE_TIAMAT);
}
#define DRAGON_FIRE_FRAMES 208
/// Falling fireballs, work->particles 24 to 39.
#define DRAGON_FIRE_FIREBALLS 16
/// The first fireball in work->particles.
#define DRAGON_FIRE_FIRST_FIREBALL 24
#define DRAGON_FIRE_FIRST_SPARK 40 ///< the first spark in work->particles
#define DRAGON_FIRE_EMBERS 512     ///< embers in PARTICLES
#define DRAGON_FIRE_BREATH 24      ///< puffs of breath, work->particles 0 to 23
#define DRAGON_FIRE_SPARKS 24      ///< sparks, work->particles 40 to 63
/// The flames a fireball burns with where it lands, in the sheet.
#define DRAGON_FIRE_FLAME_IMAGES 12288

/// Fireballs rain on the targets and burn where they land while embers rise
/// from the ground; pressing A or B during frames 49 to 159 skips to frame 160.
/// A blast of fire sweeps across the targets from frame 129, they reel on
/// frame 138, the screen flares white over frames 160 to 175 and fades out
/// after, and the battle shows the damage on frame 178.
///
/// Called by BattleEffect_RunTiamat and BattleEffect_RunDragonDriver.
///
/// Four kinds of particle, in canvas coordinates with y growing downwards,
/// 16.16 except the fireballs, which are whole pixels:
///   PARTICLES[0..511]  embers: rise from y 112, speeding up; one a frame
///                      over frames 33 to 103, eight a frame over 104 to 175
///   particles[40..63]  sparks: one on each odd frame from 41 to 127,
///                      thrown up at a slant
///   particles[0..23]   breath: one puff a frame over 129 to 175, thrown
///                      across the targets
///   particles[24..39]  fireballs: fall at a slant from the top and, below
///                      y 55, burn with their flames over and over
/// An idle particle's variant is -1; a live one's counts its age up until it
/// goes out. A fireball is never idle: its variant starts at minus its wait (0
/// to 31 frames) and counts up to 0, when it starts to fall.
///
/// Sets REG_BLDALPHA and REG_BG2CNT (the canvas in front, priority 0), loads
/// the breath's and later the flames' palette (or the lightning bolt's) into
/// the first 64 background colours and brightens them over frames 160 to 175,
/// sets work->shakeFrames, and tints and shakes the targets. Tiamat spawns her
/// eight sprites and releases them after the impact burst.
///
/// mode: Tiamat's or Dragon Driver's (enum DragonFireMode).
void BattleEffect_RunPaletteParticles(struct BattleEffectParams *args,
                                      s32 mode) {
  struct ScreenPoint origin;
  void **cursor;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleEffectWork *work;
  BlitterPair draw;
  s32 frame;
  u8 *sheet;
  struct Point2D focus;
  s32 dir;
  u8 *palette;
  s32 i;

  cursor = &gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = cursor[0];
  camera = cursor[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_CANVAS];
  work = cursor[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
  sheet = cursor[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = CANVAS_SLOT(cursor, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = CANVAS_SLOT(cursor, HEAP_SLOT_RAM_CODE_2);

  // The breath from FlashBurst, the fireballs' flames from EmberStreak (at
  // sheet + 12288).
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  palette += 128;
  Decompress_TaggedLz(palette, work->sheet + DRAGON_FIRE_FLAME_IMAGES);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                      sheet);
  if (mode == DRAGON_FIRE_DRAGON_DRIVER)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  // Trails keep 75% of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // Dragon Driver is drawn mirrored when its targets are the party
  // (EFFECT_SIDE_PARTY), as when the Fusion Dragon casts it.
  if (mode == DRAGON_FIRE_TIAMAT)
    dir = 1;
  else if (work->args->side == EFFECT_SIDE_PARTY)
    dir = -1;
  else
    dir = 1;
  // Dragon Driver's sparks and breath start at a fixed point just in front of
  // the targets, wherever the user stands.
  if (mode == DRAGON_FIRE_DRAGON_DRIVER) {
    Projection_ScaledUnit(work->args->user, &origin);
    /* FAKEMATCH(dead): the halved x is replaced below; without the halving,
     * 63 of 987 instructions differ in tbs-en. */
    origin.x /= 2;
    origin.y = 66;
    if (work->args->side == EFFECT_SIDE_PARTY)
      origin.x = 76;
    else
      origin.x = 44;
  }

  // Tiamat's corner starts at (212, -60) and comes to rest at (148, 68)
  // after she swoops down over frames 40 to 47.
  focus.x = px(212);
  focus.y = -px(60);
  EFFECT_FREE_PARTICLES(i, work->particles, 64);
  for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
    struct Particle *fireball =
        &work->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

    // Off the canvas at the top, x 128 to 255 (mirrored, -128 to -1), 65
    // to 72 lines above it.
    if (dir == 1)
      fireball->x = (Random16() & 127) + 128;
    else
      fireball->x = (Random16() & 127) - 128;
    fireball->y = (Random16() & 7) - 72;
    // Frames to wait before falling.
    fireball->variant = -(Random16() & 31);
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, DRAGON_FIRE_EMBERS);
  if (mode == DRAGON_FIRE_TIAMAT) {
    BattleEffect_SelectLivingTargets(work->args);
    wait(1);
    BattleEffect_SpawnObjects(8, SPRITE_TIAMAT, 2);
  }

  for (frame = 0; frame != DRAGON_FIRE_FRAMES; frame++) {
    // A or B skips to frame 160.
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159) {
      // Tiamat's open mouth of frame 128 and the targets' reel of frame 138;
      // when the skip comes before frame 128, that frame's sound, shake and
      // shorter trails are skipped with it.
      if (mode == DRAGON_FIRE_TIAMAT) {
        Sprite_StartAnimationFar(work->sprites[0], 8);
        Sprite_StartAnimationFar(work->sprites[1], 9);
        Sprite_StartAnimationFar(work->sprites[3], 10);
        Sprite_StartAnimationFar(work->sprites[4], 11);
      }
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HURL);
      }
      frame = 160;
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 178)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // From frame 128 the trails keep only half; over frames 160 to 175 the
    // canvas is filled each frame, with level 16 and on the last two frames at
    // its brightest, while the palette brightens; from 176 the pixels fade 2
    // levels a frame.
    if (frame == 128)
      work->transferValue = CANVAS_FADE_HALF;
    if (frame == 176) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
      palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
    } else if (frame >= 160 && frame <= 175) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));
      if (frame > 173)
        work->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
      Palette_BrightenBgEntries(2, 2, 2);
    }

    // Start new particles.

    // Embers: from y 112 at x -32 to 223, moving up; a random 0 to 127 plus
    // a base from gTiamat_DropSpeeds by index gives the speed, in 128ths of
    // a pixel a frame across and 32nds up, so they climb four times as fast
    // as they drift.
    if (frame >= 33 && frame <= 175) {
      s32 spawned = 0;
      s32 limit = 1;

      if (frame > 103)
        limit = 8;
      for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
        struct Particle *ember = &PARTICLES[i];

        if (ember->variant == -1) {
          ember->x = ((Random16() & 255) - 32) << 16;
          ember->y = px(112);
          ember->velocityX = ((Random16() & 127) + gTiamat_DropSpeeds[i & 3])
                             << 9;
          ember->velocityY = -((Random16() & 127) + gTiamat_DropSpeeds[i & 3])
                             << 11;
          ember->variant = 0;
          spawned++;
          if (spawned == limit)
            break;
        }
      }
    }
    // Sparks: one on each odd frame, from Tiamat's head (or in front of
    // Dragon Driver's targets), thrown up and to the right (mirrored by dir),
    // 110 to 155 degrees round from straight down, at 1/4 to 3/4 pixel a
    // frame.
    if (frame >= 41 && frame <= 127) {
      s32 spawned = 0;

      if (frame & 1) {
        for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
          struct Particle *spark =
              &work->particles[DRAGON_FIRE_FIRST_SPARK + i];

          if (spark->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) + 20000;

            if (mode == DRAGON_FIRE_TIAMAT) {
              spark->x = ((Random16() & 7) + 78) << 16;
              spark->y = px(70);
            } else {
              spark->x = ((Random16() & 7) + origin.x - 8) << 16;
              spark->y = px(origin.y);
            }
            spark->velocityX = speed * Trig_Sin(angle) >> 9;
            spark->velocityY = speed * Trig_Cos(angle) >> 9;
            spawned++;
            spark->variant = 0;
            if (spawned == 1)
              break;
          }
        }
      }
    }
    if (frame == 48)
      Sound_PlayFar(SE_RUMBLE);
    if (frame == 128)
      Sound_PlayFar(SE_CRASH);
    // Breath: one puff a frame, from Tiamat's mouth (or in front of Dragon
    // Driver's targets), thrown to the left across the targets (mirrored by
    // dir), 65 to 110 degrees round from straight down, at 2 to 6 pixels a
    // frame.
    if (frame >= 129 && frame <= 175) {
      s32 spawned = 0;

      for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
        struct Particle *puff = &work->particles[i];

        if (puff->variant == -1) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = (Random16() & 0x1fff) - 20000;

          if (mode == DRAGON_FIRE_TIAMAT) {
            puff->x = px(68);
            puff->y = px(64);
          } else {
            puff->x = px(origin.x);
            puff->y = px(origin.y);
          }
          puff->velocityX = speed * Trig_Sin(angle) >> 6;
          puff->velocityY = speed * Trig_Cos(angle) >> 6;
          puff->variant = 0;
          spawned++;
          if (spawned == 1)
            break;
        }
      }
    }

    // Draw and move the particles.

    // Breath: a new image every 4 frames; gone after 24 frames.
    if (frame <= 175) {
      for (i = 0; i != DRAGON_FIRE_BREATH; i++) {
        struct Particle *puff = &work->particles[i];

        if (puff->variant >= 0) {
          draw[0](canvas, work->sheet + ((puff->variant >> 2) << 11),
                  Q_16_16_TO_INT(puff->x) - 16, Q_16_16_TO_INT(puff->y) - 32,
                  32, 64);
          puff->x += puff->velocityX * dir;
          puff->y += puff->velocityY;
          puff->variant++;
          if (puff->variant == DRAGON_FIRE_BREATH)
            puff->variant = -1;
        }
      }
    }

    // Sparks: 1x2 dots that speed upwards; gone after 48 frames.
    for (i = 0; i != DRAGON_FIRE_SPARKS; i++) {
      struct Particle *spark = &work->particles[DRAGON_FIRE_FIRST_SPARK + i];
      /* FAKEMATCH(temporary): the dot's size held in a local; as 1 and 2 in
       * the call, 16 of 987 instructions differ in tbs-en. */
      s32 size = 1;

      if (spark->variant >= 0) {
        draw[1](canvas, gTiamat_SparkPixels, Q_16_16_TO_INT(spark->x),
                Q_16_16_TO_INT(spark->y) - size, size, size * 2);
        spark->x += spark->velocityX * dir;
        spark->y += spark->velocityY;
        spark->velocityY += -px(1) / 64;
        spark->variant++;
        if (spark->variant == 48)
          spark->variant = -1;
      }
    }

    // Embers: speed upwards by 1/64 pixel a frame each frame until frame 128,
    // then keep their climb and curve away to the left (to the right when
    // dir is -1), the odd ones by 1/2 pixel a frame each frame and the even
    // by 1/8; gone after 256 frames.
    if (frame <= 175) {
      for (i = 0; i != DRAGON_FIRE_EMBERS; i++) {
        struct Particle *ember = &PARTICLES[i];

        if (ember->variant >= 0) {
          u8 size = gTiamat_DropSizes[i & 3];

          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  Q_16_16_TO_INT(ember->x) - size / 2,
                  Q_16_16_TO_INT(ember->y) - size, size, size * 2);
          ember->x += ember->velocityX * dir;
          ember->y += ember->velocityY;
          if (frame > 128) {
            if (i & 1)
              ember->velocityX += -px(1) / 2;
            else
              ember->velocityX += -px(1) / 8;
          } else {
            ember->velocityY += -px(1) / 64;
          }
          ember->variant++;
          if (ember->variant == 256)
            ember->variant = -1;
        }
      }
    }

    // The shake, and Tiamat.
    if (frame == 128)
      work->shakeFrames = 48;
    if (mode == DRAGON_FIRE_TIAMAT && frame == 48)
      work->shakeFrames = 8;

    // Over frames 40 to 47 Tiamat swoops 8 pixels left and 16 down a frame.
    // From 128 to 175 her head, neck, lower jaw and chest (sprites 0, 1, 3
    // and 4) show the open-mouthed pictures 8 to 11 of SPRITE_TIAMAT, then
    // their own again.
    if (frame >= 40 && frame <= 47) {
      focus.x += -px(8);
      focus.y += px(16);
    }
    if (mode == DRAGON_FIRE_TIAMAT) {
      if (frame == 128) {
        Sprite_StartAnimationFar(work->sprites[0], 8);
        Sprite_StartAnimationFar(work->sprites[1], 9);
        Sprite_StartAnimationFar(work->sprites[3], 10);
        Sprite_StartAnimationFar(work->sprites[4], 11);
      }
      if (frame == 176) {
        Sprite_StartAnimationFar(work->sprites[0], 0);
        Sprite_StartAnimationFar(work->sprites[1], 1);
        Sprite_StartAnimationFar(work->sprites[3], 3);
        Sprite_StartAnimationFar(work->sprites[4], 4);
      }
      BattleEffect_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT, focus.x,
                                         focus.y);
    }
    if (frame == 138) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_RED_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_HURL);
      }
    }

    // Fireballs: after their wait, fall 6 pixels across and 6 down a frame,
    // a glowing ball whose fading trail streaks behind it; below y 55 they
    // burn with six flames, two frames each, over and over.
    if (frame <= 175) {
      for (i = 0; i != DRAGON_FIRE_FIREBALLS; i++) {
        struct Particle *fireball =
            &work->particles[DRAGON_FIRE_FIRST_FIREBALL + i];

        if (fireball->y > 55) {
          if (fireball->variant >= 0 && fireball->variant <= 11) {
            s32 cell = fireball->variant / 2;

            draw[0](canvas,
                    work->sheet + gTiamat_StreakOffsets[cell] +
                        DRAGON_FIRE_FLAME_IMAGES,
                    fireball->x - gTiamat_StreakWidths[cell] / 2,
                    fireball->y + gTiamat_StreakDrops[cell],
                    gTiamat_StreakWidths[cell], gTiamat_StreakHeights[cell]);
          }
          fireball->variant++;
          if (fireball->variant == 12)
            fireball->variant = 0;
        } else if (fireball->variant == 0) {
          /* FAKEMATCH(temporary): the ball's size held in a local; as 10 and
           * 20 in the call, 302 of 987 instructions differ in tbs-en. */
          s32 size = 10;

          fireball->x -= dir * 6;
          fireball->y += 6;
          if (frame <= 47 && fireball->y > 55)
            Sound_PlayFar(SE_WHOOSH);
          draw[0](canvas, sheet + gBattleEffect_MoteCells[size - 1],
                  fireball->x - size / 2, fireball->y + 30, size, size * 2);
        } else {
          fireball->variant++;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  // The impact burst builds its own blitters and transfer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  if (mode == DRAGON_FIRE_TIAMAT) {
    BattleEffect_RunImpactBurst(SUMMON_FORMATION_TIAMAT, focus.x, focus.y);
    for (i = 0; i != 8; i++)
      Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef DRAGON_FIRE_FRAMES
#undef DRAGON_FIRE_FIREBALLS
#undef DRAGON_FIRE_FIRST_FIREBALL
#undef DRAGON_FIRE_FIRST_SPARK
#undef DRAGON_FIRE_EMBERS
#undef DRAGON_FIRE_BREATH
#undef DRAGON_FIRE_SPARKS
#undef DRAGON_FIRE_FLAME_IMAGES
