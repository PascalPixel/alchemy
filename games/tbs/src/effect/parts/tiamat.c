/// @file
/// Dragon Driver and the Tiamat summon.
// Part of effect.c.

#include "../effect_internal.h"

/// Embers rise from the ground while sparks and flashes fly from the user's
/// side, in the LightningBolt colours.
///
/// Abilities: Dragon Driver (BATTLE_FX_DRAGON_DRIVER).
void BattleFx_RunDragonDriver(struct BattleEffectArgument *effect) {
  BattleFx_RunPaletteParticles(effect, 1);
}
/// Embers rise from the ground while a formation of eight objects sweeps in,
/// and the formation explodes at the end.
///
/// Abilities: Tiamat (SCENE_EFFECT_TIAMAT).
void BattleFx_RunTiamat(struct BattleEffectArgument *effect) {
  BattleFx_RunPaletteParticles(effect, 0);
}
#define PALETTE_PARTICLES_FRAMES 208
/// Falling streaks, work->particles 24 to 39.
#define PALETTE_PARTICLES_STREAKS 16
/// The first streak in work->particles.
#define PALETTE_PARTICLES_FIRST_STREAK 24
#define PALETTE_PARTICLES_FIRST_SPARK 40 ///< the first spark in work->particles
#define PALETTE_PARTICLES_EMBERS 512     ///< embers in PARTICLES
#define PALETTE_PARTICLES_FLASHES 24     ///< flashes, work->particles 0 to 23
#define PALETTE_PARTICLES_SPARKS 24      ///< sparks, work->particles 40 to 63
/// The streak pictures in the sheet.
#define PALETTE_PARTICLES_STREAK_IMAGES 0x3000

/// Embers rise from the ground, and pressing A or B during frames 49 to 159
/// skips to frame 160. The targets reel on frame 138, the screen flares white
/// over frames 160 to 175 and fades out after, and the battle shows the damage
/// on frame 178.
///
/// Called by BattleFx_RunTiamat (mode 0) and BattleFx_RunDragonDriver
/// (mode 1).
///
/// Four kinds of particle, in canvas coordinates with y growing downwards,
/// 16.16 except the streaks, which are whole pixels:
///   PARTICLES[0..511]  embers: rise from y 112, speeding up; one a frame
///                      over frames 33 to 103, eight a frame over 104 to 175
///   particles[40..63]  sparks: one on each odd frame from 41 to 127,
///                      thrown up at a slant
///   particles[0..23]   flashes: one a frame over 129 to 175, thrown across
///   particles[24..39]  streaks: fall at a slant from the top and, below
///                      y 55, play their images over and over
/// An idle particle's variant is -1; a live one's counts its age up until it
/// goes out. A streak is never idle: its variant starts at minus its wait (0
/// to 31 frames) and counts up to 0, when it starts to fall.
///
/// Sets REG_BLDALPHA and REG_BG2CNT (the canvas in front, priority 0), loads
/// the flash and later the streak palette (or the lightning bolt's) into the
/// first 64 background colours and brightens them over frames 160 to 175,
/// sets work->shakeFrames, and tints and shakes the targets. Mode 0 spawns the
/// eight formation objects and releases them after the impact burst.
///
/// mode : 0 also brings in a formation of eight objects that sweeps in and,
///          at the end, explodes with BattleFx_RunImpactBurst;
///        1 throws the sparks and flashes from the user's side
///          (effect->side), in the LightningBolt colours.
void BattleFx_RunPaletteParticles(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct EffectPosition origin;
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

  cursor = &gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = cursor[0];
  camera = cursor[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_CANVAS];
  work = cursor[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
  sheet = cursor[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = CANVAS_SLOT(cursor, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = CANVAS_SLOT(cursor, HEAP_SLOT_BLITTER_ALTERNATE);

  // The flashes from FlashBurst, the streaks from EmberStreak (at sheet +
  // 0x3000).
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + PALETTE_PARTICLES_STREAK_IMAGES);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
  if (mode == 1)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  // Trails keep 75% of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Mode 1 mirrors everything when the user is on the right (side 1).
  if (mode == 0)
    dir = 1;
  else if (work->effect->side == 1)
    dir = -1;
  else
    dir = 1;
  // Mode 1's sparks and flashes start at a fixed point on the user's side:
  // the user's projected x is overwritten.
  if (mode == 1) {
    EffectPosition_ProjectScaledBattler(work->effect->actor, &origin);
    origin.x /= 2;
    origin.y = 66;
    if (work->effect->side == 1)
      origin.x = 76;
    else
      origin.x = 44;
  }

  // The formation's corner starts at (212, -60) and comes to rest at
  // (148, 68) after the sweep of frames 40 to 47.
  focus.x = 212 << 16;
  focus.y = -(60 << 16);
  EFFECT_FREE_PARTICLES(i, work->particles, 64);
  for (i = 0; i != PALETTE_PARTICLES_STREAKS; i++) {
    struct EffectStep *streak =
        &work->particles[PALETTE_PARTICLES_FIRST_STREAK + i];

    // Off the canvas at the top, x 128 to 255 (mirrored, -128 to -1), 65
    // to 72 lines above it.
    if (dir == 1)
      streak->x = (Random16() & 127) + 128;
    else
      streak->x = (Random16() & 127) - 128;
    streak->y = (Random16() & 7) - 72;
    // Frames to wait before falling.
    streak->variant = -(Random16() & 31);
  }
  EFFECT_FREE_PARTICLES(i, PARTICLES, PALETTE_PARTICLES_EMBERS);
  if (mode == 0) {
    BattleFx_SelectLivingTargets(work->effect);
    WaitFrames(1);
    BattleFx_SpawnObjects(8, 377, 2);
  }

  for (frame = 0; frame != PALETTE_PARTICLES_FRAMES; frame++) {
    // A or B skips to frame 160.
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159) {
      // The animations of frame 128 and the targets' reel of frame 138;
      // when the skip comes before frame 128, that frame's sound, shake and
      // shorter trails are skipped with it.
      if (mode == 0) {
        AnimationObjects_SelectAnimationFar(work->objects[0], 8);
        AnimationObjects_SelectAnimationFar(work->objects[1], 9);
        AnimationObjects_SelectAnimationFar(work->objects[3], 10);
        AnimationObjects_SelectAnimationFar(work->objects[4], 11);
      }
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, -1, 0);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
      }
      frame = 160;
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    if (frame == 178)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // From frame 128 the trails keep only half; over frames 160 to 175 the
    // canvas is filled each frame, with level 16 and on the last two frames
    // level 63, while the palette brightens; from 176 the pixels fade 2
    // levels a frame.
    if (frame == 128)
      work->transferValue = 50;
    if (frame == 176) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x02020202);
      palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
    } else if (frame >= 160 && frame <= 175) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0x10101010);
      if (frame > 173)
        work->transferValue = 0x3f3f3f3f;
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
      for (i = 0; i != PALETTE_PARTICLES_EMBERS; i++) {
        struct EffectStep *grain = &PARTICLES[i];

        if (grain->variant == -1) {
          grain->x = ((Random16() & 255) - 32) << 16;
          grain->y = 112 << 16;
          grain->velocityX = ((Random16() & 127) + gTiamat_DropSpeeds[i & 3])
                             << 9;
          grain->velocityY = -((Random16() & 127) + gTiamat_DropSpeeds[i & 3])
                             << 11;
          grain->variant = 0;
          spawned++;
          if (spawned == limit)
            break;
        }
      }
    }
    // Sparks: one on each odd frame, thrown up and to the right (mirrored
    // by dir), 110 to 155 degrees round from straight down, at 1/4 to 3/4
    // pixel a frame.
    if (frame >= 41 && frame <= 127) {
      s32 spawned = 0;

      if (frame & 1) {
        for (i = 0; i != PALETTE_PARTICLES_SPARKS; i++) {
          struct EffectStep *spark =
              &work->particles[PALETTE_PARTICLES_FIRST_SPARK + i];

          if (spark->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) + 20000;

            if (mode == 0) {
              spark->x = ((Random16() & 7) + 78) << 16;
              spark->y = 70 << 16;
            } else {
              spark->x = ((Random16() & 7) + origin.x - 8) << 16;
              spark->y = origin.y << 16;
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
      AudioCommand_PlayFar(SE_141);
    if (frame == 128)
      AudioCommand_PlayFar(SE_145);
    // Flashes: one a frame, thrown to the left (mirrored by dir), 65 to 110
    // degrees round from straight down, at 2 to 6 pixels a frame.
    if (frame >= 129 && frame <= 175) {
      s32 spawned = 0;

      for (i = 0; i != PALETTE_PARTICLES_FLASHES; i++) {
        struct EffectStep *flash = &work->particles[i];

        if (flash->variant == -1) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = (Random16() & 0x1fff) - 20000;

          if (mode == 0) {
            flash->x = 68 << 16;
            flash->y = 64 << 16;
          } else {
            flash->x = origin.x << 16;
            flash->y = origin.y << 16;
          }
          flash->velocityX = speed * Trig_Sin(angle) >> 6;
          flash->velocityY = speed * Trig_Cos(angle) >> 6;
          flash->variant = 0;
          spawned++;
          if (spawned == 1)
            break;
        }
      }
    }

    // Draw and move the particles.

    // Flashes: a new image every 4 frames; gone after 24 frames.
    if (frame <= 175) {
      for (i = 0; i != PALETTE_PARTICLES_FLASHES; i++) {
        struct EffectStep *flash = &work->particles[i];

        if (flash->variant >= 0) {
          draw[0](canvas, work->sheet + ((flash->variant >> 2) << 11),
                  Q_16_16_TO_INT(flash->x) - 16, Q_16_16_TO_INT(flash->y) - 32,
                  32, 64);
          flash->x += flash->velocityX * dir;
          flash->y += flash->velocityY;
          flash->variant++;
          if (flash->variant == PALETTE_PARTICLES_FLASHES)
            flash->variant = -1;
        }
      }
    }

    // Sparks: 1x2 dots that speed upwards; gone after 48 frames.
    for (i = 0; i != PALETTE_PARTICLES_SPARKS; i++) {
      struct EffectStep *spark =
          &work->particles[PALETTE_PARTICLES_FIRST_SPARK + i];
      s32 size = 1;

      if (spark->variant >= 0) {
        draw[1](canvas, gTiamat_SparkPixels, Q_16_16_TO_INT(spark->x),
                Q_16_16_TO_INT(spark->y) - size, size, size * 2);
        spark->x += spark->velocityX * dir;
        spark->y += spark->velocityY;
        spark->velocityY += -0x400;
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
      for (i = 0; i != PALETTE_PARTICLES_EMBERS; i++) {
        struct EffectStep *grain = &PARTICLES[i];

        if (grain->variant >= 0) {
          u8 size = gTiamat_DropSizes[i & 3];
          s32 height = size * 2;

          draw[0](canvas, sheet + gBattleFx_MoteCells[size - 1],
                  Q_16_16_TO_INT(grain->x) - size / 2,
                  Q_16_16_TO_INT(grain->y) - size, size, height);
          grain->x += grain->velocityX * dir;
          grain->y += grain->velocityY;
          if (frame > 128) {
            if (i & 1)
              grain->velocityX += -0x8000;
            else
              grain->velocityX += -0x2000;
          } else {
            grain->velocityY += -0x400;
          }
          grain->variant++;
          if (grain->variant == 256)
            grain->variant = -1;
        }
      }
    }

    // The shake and the formation.
    if (frame == 128)
      work->shakeFrames = 48;
    if (mode == 0 && frame == 48)
      work->shakeFrames = 8;

    // Over frames 40 to 47 the formation moves 8 pixels left and 16 down a
    // frame; from 128 to 175 four of its objects play animations 8 to 11.
    if (frame >= 40 && frame <= 47) {
      focus.x += -(8 << 16);
      focus.y += 16 << 16;
    }
    if (mode == 0) {
      if (frame == 128) {
        AnimationObjects_SelectAnimationFar(work->objects[0], 8);
        AnimationObjects_SelectAnimationFar(work->objects[1], 9);
        AnimationObjects_SelectAnimationFar(work->objects[3], 10);
        AnimationObjects_SelectAnimationFar(work->objects[4], 11);
      }
      if (frame == 176) {
        AnimationObjects_SelectAnimationFar(work->objects[0], 0);
        AnimationObjects_SelectAnimationFar(work->objects[1], 1);
        AnimationObjects_SelectAnimationFar(work->objects[3], 3);
        AnimationObjects_SelectAnimationFar(work->objects[4], 4);
      }
      BattleFx_PlaceFormationObjects(3, focus.x, focus.y);
    }
    if (frame == 138) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, -1, 0);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
      }
    }

    // Streaks: after their wait, fall 6 pixels across and 6 down a frame;
    // below y 55 they play six images, two frames each, over and over.
    if (frame <= 175) {
      for (i = 0; i != PALETTE_PARTICLES_STREAKS; i++) {
        struct EffectStep *streak =
            &work->particles[PALETTE_PARTICLES_FIRST_STREAK + i];

        if (streak->y > 55) {
          if (streak->variant >= 0 && streak->variant <= 11) {
            s32 cell = streak->variant / 2;

            draw[0](canvas,
                    work->sheet + gTiamat_StreakOffsets[cell] +
                        PALETTE_PARTICLES_STREAK_IMAGES,
                    streak->x - gTiamat_StreakWidths[cell] / 2,
                    streak->y + gTiamat_StreakDrops[cell],
                    gTiamat_StreakWidths[cell], gTiamat_StreakHeights[cell]);
          }
          streak->variant++;
          if (streak->variant == 12)
            streak->variant = 0;
        } else if (streak->variant == 0) {
          s32 size = 10;

          streak->x -= dir * 6;
          streak->y += 6;
          if (frame <= 47 && streak->y > 55)
            AudioCommand_PlayFar(SE_136);
          draw[0](canvas, sheet + gBattleFx_MoteCells[size - 1],
                  streak->x - size / 2, streak->y + 30, size, size * 2);
        } else {
          streak->variant++;
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  // The impact burst builds its own blitters and transfer.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  EFFECT_RELEASE_BLITTERS();
  if (mode == 0) {
    BattleFx_RunImpactBurst(3, focus.x, focus.y);
    for (i = 0; i != 8; i++)
      ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleFx_EndCanvasLayer();
}

#undef PALETTE_PARTICLES_FRAMES
#undef PALETTE_PARTICLES_STREAKS
#undef PALETTE_PARTICLES_FIRST_STREAK
#undef PALETTE_PARTICLES_FIRST_SPARK
#undef PALETTE_PARTICLES_EMBERS
#undef PALETTE_PARTICLES_FLASHES
#undef PALETTE_PARTICLES_SPARKS
#undef PALETTE_PARTICLES_STREAK_IMAGES
