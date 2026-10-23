/// @file
/// Dragon Driver and the Tiamat summon.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// The palette particles' modes (BattleAnim_RunPaletteParticles).
enum PaletteParticlesMode {
  PALETTE_PARTICLES_TIAMAT,       ///< also a formation of eight objects that
                                  ///< sweeps in and explodes at the end
                                  ///< (BattleAnim_RunImpactBurst)
  PALETTE_PARTICLES_DRAGON_DRIVER ///< the sparks and flashes thrown from the
                                  ///< user's side, in the LightningBolt colours
};

/// Embers rise from the ground while sparks and flashes fly from the user's
/// side, in the LightningBolt colours.
///
/// Abilities: Dragon Driver (BATTLE_ANIM_DRAGON_DRIVER).
void BattleAnim_RunDragonDriver(struct BattleAnimArgs *effect) {
  BattleAnim_RunPaletteParticles(effect, PALETTE_PARTICLES_DRAGON_DRIVER);
}
/// Embers rise from the ground while a formation of eight objects sweeps in,
/// and the formation explodes at the end.
///
/// Abilities: Tiamat (SUMMON_ANIM_TIAMAT).
void BattleAnim_RunTiamat(struct BattleAnimArgs *effect) {
  BattleAnim_RunPaletteParticles(effect, PALETTE_PARTICLES_TIAMAT);
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
/// Called by BattleAnim_RunTiamat and BattleAnim_RunDragonDriver.
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
/// sets work->shakeFrames, and tints and shakes the targets. Tiamat spawns the
/// eight formation objects and releases them after the impact burst.
///
/// mode: Tiamat's or Dragon Driver's (enum PaletteParticlesMode).
void BattleAnim_RunPaletteParticles(struct BattleAnimArgs *effect, s32 mode) {
  struct ScreenPoint origin;
  void **cursor;
  void *canvas;
  struct BattleCamera *camera;
  struct BattleAnimWork *work;
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
  work = cursor[HEAP_SLOT_BATTLE_ANIM - HEAP_SLOT_BATTLE_CANVAS];
  sheet = cursor[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = CANVAS_SLOT(cursor, HEAP_SLOT_RAM_CODE);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
  draw[1] = CANVAS_SLOT(cursor, HEAP_SLOT_RAM_CODE_2);

  // The flashes from FlashBurst, the streaks from EmberStreak (at sheet +
  // 0x3000).
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Decompress_Type01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  palette += 128;
  Decompress_Type01(palette, work->sheet + PALETTE_PARTICLES_STREAK_IMAGES);
  Decompress_Type01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA),
                    sheet);
  if (mode == PALETTE_PARTICLES_DRAGON_DRIVER)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  // Trails keep 75% of each pixel a frame.
  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Dragon Driver mirrors everything when the user is on the right (side 1).
  if (mode == PALETTE_PARTICLES_TIAMAT)
    dir = 1;
  else if (work->effect->side == 1)
    dir = -1;
  else
    dir = 1;
  // Dragon Driver's sparks and flashes start at a fixed point on the user's
  // side: the user's projected x is overwritten.
  if (mode == PALETTE_PARTICLES_DRAGON_DRIVER) {
    Projection_ScaledUnit(work->effect->user, &origin);
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
  ANIM_FREE_PARTICLES(i, work->particles, 64);
  for (i = 0; i != PALETTE_PARTICLES_STREAKS; i++) {
    struct Particle *streak =
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
  ANIM_FREE_PARTICLES(i, PARTICLES, PALETTE_PARTICLES_EMBERS);
  if (mode == PALETTE_PARTICLES_TIAMAT) {
    BattleAnim_SelectLivingTargets(work->effect);
    WaitFrames(1);
    BattleAnim_SpawnObjects(8, 377, 2);
  }

  for (frame = 0; frame != PALETTE_PARTICLES_FRAMES; frame++) {
    // A or B skips to frame 160.
    if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 48 && frame <= 159) {
      // The animations of frame 128 and the targets' reel of frame 138;
      // when the skip comes before frame 128, that frame's sound, shake and
      // shorter trails are skipped with it.
      if (mode == PALETTE_PARTICLES_TIAMAT) {
        Sprite_StartAnimFar(work->animations[0], 8);
        Sprite_StartAnimFar(work->animations[1], 9);
        Sprite_StartAnimFar(work->animations[3], 10);
        Sprite_StartAnimFar(work->animations[4], 11);
      }
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->targets[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, -1, 0);
        BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                       KNOCKBACK_HURL);
      }
      frame = 160;
    }
    ANIM_VIEW_FROM_CAMERA(camera);
    if (frame == 178)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);

    // From frame 128 the trails keep only half; over frames 160 to 175 the
    // canvas is filled each frame, with level 16 and on the last two frames
    // level 63, while the palette brightens; from 176 the pixels fade 2
    // levels a frame.
    if (frame == 128)
      work->transferValue = 50;
    if (frame == 176) {
      ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x02020202);
      palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
    } else if (frame >= 160 && frame <= 175) {
      ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0x10101010);
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
        struct Particle *grain = &PARTICLES[i];

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
          struct Particle *spark =
              &work->particles[PALETTE_PARTICLES_FIRST_SPARK + i];

          if (spark->variant == -1) {
            s32 speed = (Random16() & 255) + 128;
            s32 angle = (Random16() & 0x1fff) + 20000;

            if (mode == PALETTE_PARTICLES_TIAMAT) {
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
      Sound_PlaySongFar(SE_RUMBLE);
    if (frame == 128)
      Sound_PlaySongFar(SE_145);
    // Flashes: one a frame, thrown to the left (mirrored by dir), 65 to 110
    // degrees round from straight down, at 2 to 6 pixels a frame.
    if (frame >= 129 && frame <= 175) {
      s32 spawned = 0;

      for (i = 0; i != PALETTE_PARTICLES_FLASHES; i++) {
        struct Particle *flash = &work->particles[i];

        if (flash->variant == -1) {
          s32 speed = (Random16() & 255) + 128;
          s32 angle = (Random16() & 0x1fff) - 20000;

          if (mode == PALETTE_PARTICLES_TIAMAT) {
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
        struct Particle *flash = &work->particles[i];

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
      struct Particle *spark =
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
        struct Particle *grain = &PARTICLES[i];

        if (grain->variant >= 0) {
          u8 size = gTiamat_DropSizes[i & 3];
          s32 height = size * 2;

          draw[0](canvas, sheet + gBattleAnim_MoteCells[size - 1],
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
    if (mode == PALETTE_PARTICLES_TIAMAT && frame == 48)
      work->shakeFrames = 8;

    // Over frames 40 to 47 the formation moves 8 pixels left and 16 down a
    // frame; from 128 to 175 four of its objects play animations 8 to 11.
    if (frame >= 40 && frame <= 47) {
      focus.x += -(8 << 16);
      focus.y += 16 << 16;
    }
    if (mode == PALETTE_PARTICLES_TIAMAT) {
      if (frame == 128) {
        Sprite_StartAnimFar(work->animations[0], 8);
        Sprite_StartAnimFar(work->animations[1], 9);
        Sprite_StartAnimFar(work->animations[3], 10);
        Sprite_StartAnimFar(work->animations[4], 11);
      }
      if (frame == 176) {
        Sprite_StartAnimFar(work->animations[0], 0);
        Sprite_StartAnimFar(work->animations[1], 1);
        Sprite_StartAnimFar(work->animations[3], 3);
        Sprite_StartAnimFar(work->animations[4], 4);
      }
      BattleAnim_PlaceFormationObjects(SUMMON_FORMATION_TIAMAT, focus.x,
                                       focus.y);
    }
    if (frame == 138) {
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->targets[i],
                                  SPRITE_COLOR_RED_TINT, POSE_DAMAGE, -1, 0);
        BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                       KNOCKBACK_HURL);
      }
    }

    // Streaks: after their wait, fall 6 pixels across and 6 down a frame;
    // below y 55 they play six images, two frames each, over and over.
    if (frame <= 175) {
      for (i = 0; i != PALETTE_PARTICLES_STREAKS; i++) {
        struct Particle *streak =
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
            Sound_PlaySongFar(SE_136);
          draw[0](canvas, sheet + gBattleAnim_MoteCells[size - 1],
                  streak->x - size / 2, streak->y + 30, size, size * 2);
        } else {
          streak->variant++;
        }
      }
    }
    BattleCamera_Shake(8, 8);
    ANIM_END_FRAME(work);
  }

  // The impact burst builds its own blitters and transfer.
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  ANIM_RELEASE_BLITTERS();
  if (mode == PALETTE_PARTICLES_TIAMAT) {
    BattleAnim_RunImpactBurst(SUMMON_FORMATION_TIAMAT, focus.x, focus.y);
    for (i = 0; i != 8; i++)
      Sprite_DestroyFar(work->animations[i]);
  }
  BattleAnim_EndCanvasLayer();
}

#undef PALETTE_PARTICLES_FRAMES
#undef PALETTE_PARTICLES_STREAKS
#undef PALETTE_PARTICLES_FIRST_STREAK
#undef PALETTE_PARTICLES_FIRST_SPARK
#undef PALETTE_PARTICLES_EMBERS
#undef PALETTE_PARTICLES_FLASHES
#undef PALETTE_PARTICLES_SPARKS
#undef PALETTE_PARTICLES_STREAK_IMAGES
