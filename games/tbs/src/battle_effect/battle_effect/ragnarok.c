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
/// Abilities: Ragnarok.
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
  // The user takes its leap pose and animates fast until the end.
  Actor_SelectAnimationFar(user, POSE_LEAP);
  Actor_SetAnimationSpeedFar(user, ANIMATION_STEP_NORMAL * 3);

  Resource_LoadAndDecompress(RESOURCE_FLAME_BLADE_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             work->sheet + RAGNAROK_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);

  // Flashes and sparks for three columns, though only two strike: the third's
  // random draws are still taken.
  for (col = 0; col != RAGNAROK_SEEDED; col++) {
    for (i = 0; i != RAGNAROK_FLASHES; i++) {
      struct Particle *flash = &work->particles[col * RAGNAROK_FLASHES + i];

      s32 radius = i * 2;
      s32 angle = Random16() & TRIG_ANGLE_MASK;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
    }

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

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != RAGNAROK_FRAMES; frame++) {
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

    // The blade.
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

    if (frame > 16) {
      BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
    }

    // The two columns of light strike, with flashes and then sparks.
    for (col = 0; col != RAGNAROK_COLUMNS; col++) {
      s32 start = col * 8 + 16;

      if (frame == start) {
        work->shakeFrames = 12;
      }
      if (frame >= start) {
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

      // Sparks fall and bounce.
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

      // Every target flashes and recoils.
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
