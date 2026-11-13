/// @file
/// Ragnarok.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// Where the flash graphics start: after the 64x128 sword image.
#define RAGNAROK_FLASH_PICTURES (64 * 128)

#define RAGNAROK_FRAMES 80
#define RAGNAROK_COLUMNS 2  ///< columns that strike
#define RAGNAROK_SEEDED 3   ///< columns given particles; the third is unused
#define RAGNAROK_FLASHES 16 ///< flashes kept per column
#define RAGNAROK_RING 12    ///< of which drawn as the ring
#define RAGNAROK_SPARKS 340 ///< sparks kept per column
#define RAGNAROK_DRAWN_SPARKS 256 ///< of which flown

/// A huge flaming blade swings down over the targets and light bursts over
/// them twice.
///
/// Abilities: Ragnarok (BATTLE_EFFECT_RAGNAROK).
///
/// The sword is drawn for 40 frames: it swings down over 10 frames, pauses
/// until frame 20, then creeps on. The bursts come at frames 16 and 24 and
/// scatter sparks six frames later, as the targets recoil; the battle shows the
/// damage on frame 40. Each burst stands at a strike column of
/// gRagnarok_StrikeColumns, the first two of three per side. work->particles
/// holds 16 flashes a column and PARTICLES 340 sparks a column.
///
/// Leaves args->variant at EFFECT_VARIANT_STRONGER, and the user in its
/// action pose with its animation back at the usual speed.
void BattleEffect_RunRagnarok(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *sheet;
  struct Actor *user;
  DrawRectangle blit[2];
  s32 frame;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);

  user = GetBattleObjectSlotFar(args->user)->actor;
  // Nothing in the effect or the battle reads the variant afterwards.
  // XXX (no evidence): why it is forced to the stronger one; the store
  // changes nothing the game shows, so only the author's intent would say.
  args->variant = EFFECT_VARIANT_STRONGER;
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(work->args->side, blit);
  // The user takes its leap pose and animates fast until the end: an
  // effect step of 48, three times the usual 16.
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL * 3);

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + RAGNAROK_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  // Prepare 16 flashes and 340 sparks a column, for three columns although
  // only two strike: the third's random draws are still taken.
  for (col = 0; col != RAGNAROK_SEEDED; col++) {
    // Flashes: i * 2 pixels from the center, lifetime 25-32.
    for (i = 0; i != RAGNAROK_FLASHES; i++) {
      struct Particle *flash = &work->particles[col * RAGNAROK_FLASHES + i];

      s32 radius = i * 2;
      s32 angle = Random16() & TRIG_ANGLE_MASK;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
    }

    // Sparks: random direction and speed from column y = 88, lifetime 32-39;
    // speeds of 0.5 to 8.5 pixels a frame (speed / 64), doubled vertically.
    for (i = 0; i != RAGNAROK_SPARKS; i++) {
      struct Particle *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & TRIG_ANGLE_MASK;
      spark->x = px(gRagnarok_StrikeColumns[work->args->side][col]);
      spark->y = px(88);
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }

  // Each transfer keeps 3/4 of every canvas level, for an afterimage.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != RAGNAROK_FRAMES; frame++) {
    // Sounds and the first shake; the battle shows the damage on frame 40.
    if (frame == 4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 8) {
      work->shakeFrames = 8;
    }
    if (frame == 18) {
      Sound_PlayFar(SE_CRASH);
    }
    if (frame == 40) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    // Frames 0 to 39, the blade: it swings down 16 pixels and 10 across a
    // frame for 10 frames, pauses until frame 20, then creeps on, 2 pixels
    // down and 1 across a frame. It is cut off at line 104.
    if (frame <= 39) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (work->args->side == EFFECT_SIDE_PARTY) {
        if (frame <= 9) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > 20) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame <= 9) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > 20) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104) {
        height = height - top - 24;
      }
      if (height > 0) {
        blit[0](canvas, work->sheet, x - 32, top, 64, height);
      }
    }

    // From frame 17 the palette fades towards the blast colours.
    if (frame > 16) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    }

    // The two columns, from frames 16 and 24: a 12-frame shake and the
    // flashes at their start, the sparks from 6 frames later.
    for (col = 0; col != RAGNAROK_COLUMNS; col++) {
      s32 start = col * 8 + 16;

      if (frame == start) {
        work->shakeFrames = 12;
      }
      if (frame >= start) {
        // Large flash (picture 0) for the first two frames, then 12 small
        // flashes. Each small one shows for the last 18 frames of its life,
        // stepping through gRagnarok_FlashPictures' six entries three frames
        // apiece: pictures 1 to 4, then 5 for six frames.
        if (frame < start + 2) {
          blit[0](canvas, work->sheet + RAGNAROK_FLASH_PICTURES,
                  gRagnarok_StrikeColumns[work->args->side][col] - 16, 56, 32,
                  64);
        }
        for (i = 0; i != RAGNAROK_RING; i++) {
          struct Particle *flash = &work->particles[col * RAGNAROK_FLASHES + i];

          s32 x;
          s32 life;
          s32 y;

          y = Q_16_16_TO_INT(flash->y);
          x = Q_16_16_TO_INT(flash->x) +
              gRagnarok_StrikeColumns[work->args->side][col];
          life = flash->variant;
          if (life >= 0 && life <= 17) {
            blit[0](canvas,
                    work->sheet +
                        gRagnarok_FlashPictures[life / 3] * (32 * 64) +
                        RAGNAROK_FLASH_PICTURES,
                    x - 16, y + 56, 32, 64);
          }
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
        }
      }

      // Sparks gain 1/16 pixel a frame of downward speed each frame,
      // undamped, and bounce below y = 108 at half speed; draw only within
      // x = 0-126.
      if (frame > start + 5) {
        for (i = 0; i != RAGNAROK_DRAWN_SPARKS; i++) {
          struct Particle *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            Particle_AdvanceWithGravity2D(spark, 64, px(1) / 16);
            spark->variant--;
            y = spark->y;
            if (y > px(108)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x < px(127)) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 drawY = y >> 16;
                s32 drawX = x >> 16;

                // Size follows lifetime, 1 to 8 pixels; alternate sparks take
                // the brighter pixel or add.
                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = sheet + gBattleEffect_MotePictures[size - 1];
                drawX -= size / 2;
                drawY -= size;
                blit[lane](canvas, picture, drawX, drawY, size, size * 2);
              }
            }
          }
        }
      }

      // As the column's sparks start to fly, every target flashes and
      // recoils.
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        if (frame == start + 6) {
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
          BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                         KNOCKBACK_HURL);
        }
      }
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL);
  EFFECT_FINISH_CANVAS();
}

#undef RAGNAROK_FLASH_PICTURES
#undef RAGNAROK_FRAMES
#undef RAGNAROK_COLUMNS
#undef RAGNAROK_SEEDED
#undef RAGNAROK_FLASHES
#undef RAGNAROK_RING
#undef RAGNAROK_SPARKS
#undef RAGNAROK_DRAWN_SPARKS
