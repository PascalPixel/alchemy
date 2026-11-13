/// @file
/// Titan Blade.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The sparks, in PARTICLES after the dust.
#define TITAN_BLADE_SWORD_SPARKS                                               \
  ((struct Particle *)(Ram_MapGridCellBuffer +                                 \
                       TITAN_BLADE_DUST * sizeof(struct Particle)))

#define TITAN_BLADE_FRAMES 102
#define TITAN_BLADE_SWORD_PICTURE 20000 ///< the sword picture in the sheet
#define TITAN_BLADE_FLASHES 32          ///< flashes scattered
#define TITAN_BLADE_RING 16             ///< of which drawn as the ring
#define TITAN_BLADE_DUST 684 ///< dust slots in PARTICLES, before the sparks
#define TITAN_BLADE_FIRST_DUST 256 ///< dust lit at once on frame 28
#define TITAN_BLADE_DRAWN_DUST 342 ///< dust moved and drawn
#define TITAN_BLADE_SPARKS 340     ///< sparks after the dust
/// The flash pictures in the sheet, after the 34x104 fire pillar.
#define TITAN_BLADE_FLASH_PICTURES (34 * 104)

/// A huge gold-hilted sword drops onto the first target and is driven into the
/// ground amid flashes, golden dust and sparks.
///
/// Abilities: Titan Blade.
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args) {
  void **slots;
  void **slot;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blitters[2];
  u8 *sheet;
  struct ScreenPoint screenPosition;
  s32 frame;
  s32 canvasCenterX;
  s32 step;
  s32 sparkOriginX;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  slot = slots;
  work = *slot++;
  canvas = *slot;
  sheet =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_BATTLE_SHEET);

  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Projection_Unit(work->args->targets[0], &screenPosition);
  canvasCenterX = screenPosition.x / 2;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  blitters[0] =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_TRANSPARENT);
  blitters[1] =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_RAM_CODE_2);

  Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet,
                             work->sheet + TITAN_BLADE_SWORD_PICTURE, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + TITAN_BLADE_FLASH_PICTURES, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The flashes.
  for (i = 0; i != TITAN_BLADE_FLASHES; i++) {
    struct Particle *flash = &work->particles[i];

    s32 scale = i * 2;
    s32 ang = Random16() & TRIG_ANGLE_MASK;

    flash->x = Trig_Sin(ang) * scale;
    flash->y = -(Trig_Cos(ang) * scale);
    flash->variant = i / 2 + 25;
  }

  // All the dust starts free (variant -1).
  for (i = 0; i != TITAN_BLADE_DUST; i++) {
    PARTICLES[i].variant = -1;
  }

  i = 0;
  // The sparks start at canvas y 88 below the target, thrown at 1 to 17
  // pixels a frame across and half that up or down (twice as fast across as
  // Ragnarok's and half as fast up), and live 32 to 39 frames.
  sparkOriginX = px(canvasCenterX);
  do {
    struct Particle *spark = &TITAN_BLADE_SWORD_SPARKS[i];

    s32 mag;
    s32 ang;

    mag = (Random16() & 0x1ff) + 32;
    ang = Random16() & TRIG_ANGLE_MASK;
    spark->x = sparkOriginX;
    spark->y = px(88);
    spark->velocityX = (Trig_Sin(ang) * mag) >> 5;
    spark->velocityY = -(Trig_Cos(ang) * mag) >> 6;
    spark->variant = (Random16() & 7) + 32;
    i++;
  } while (i != TITAN_BLADE_SPARKS);

  for (frame = 0; frame != TITAN_BLADE_FRAMES; frame++) {
    // The palette fades towards the blast colours over frames 25 to 47,
    // then towards the lightning colours from frame 57: the dust and sparks
    // turn from gold to pink and violet.
    if (frame >= 25 && frame <= 47) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    }
    if (frame > 56) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_LightningBoltSheet);
    }

    // Shakes of 8 frames from frames 8 and 48, and of 16 from the strike.
    if (frame == 8) {
      work->shakeFrames = 8;
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 60) {
      work->shakeFrames = 16;
    }

    if (frame == 4) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 32) {
      Sound_PlayFar(SE_SCATTER);
    }
    if (frame == 60) {
      Sound_PlayFar(SE_CRASH);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }

    if (frame > 55) {
      for (i = 0; i != TITAN_BLADE_RING; i++) {
        struct Particle *flash = &work->particles[i];

        s32 x;
        s32 life;
        s32 y;

        y = Q_16_16_TO_INT(flash->y);
        x = Q_16_16_TO_INT(flash->x) + canvasCenterX;
        life = flash->variant;
        if (life >= 0 && life <= 17) {
          blitters[0](canvas,
                      work->sheet +
                          (gTitanBlade_FlashPictures[life / 3] << 11) +
                          TITAN_BLADE_FLASH_PICTURES,
                      x - 16, y + 48, 32, 64);
        }
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
      }
    }

    // Dust at the target's feet.
    if (frame == 28) {
      for (i = 0; i != TITAN_BLADE_FIRST_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 mag;
          s32 ang;

          mag = Random16() & 63;
          ang = Random16() & TRIG_ANGLE_MASK;
          dust->x = ((Trig_Sin(ang) * mag) >> 3) + sparkOriginX;
          dust->y = ((Trig_Cos(ang) * mag) >> 2) + px(96);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
      }
    }

    step = frame - 32;
    if (step >= 0 && step <= 31) {
      s32 cnt;

      cnt = 0;
      for (i = 0; i != TITAN_BLADE_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 mag;
          s32 ang;

          mag = Random16() & 63;
          ang = Random16() & TRIG_ANGLE_MASK;
          dust->x = ((Trig_Sin(ang) * mag) >> 3) + sparkOriginX;
          dust->y = ((Trig_Cos(ang) * mag) >> 2) + px(96);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          cnt++;
          if (cnt == 16) {
            break;
          }
        }
      }
    }

    if (step >= 0 && step <= 31) {
      s32 phase = frame * 16;
      s32 x = canvasCenterX - 17;
      s32 width = 34;
      DrawRectangle blit = blitters[0];
      s32 high = 104;
      s32 size = (phase - 256) % high;

      blit(canvas, work->sheet, x, 4 - size, width, high);
      blit(canvas, work->sheet, x, 108 - size, width, size);
    }

    // Until frame 71, move and draw the first 342 dust motes; they swell from
    // frame 69, and one goes out once it falls below y 108.
    if (frame <= 71) {
      for (i = 0; i != TITAN_BLADE_DRAWN_DUST; i++) {
        struct Particle *dust = &PARTICLES[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0) {
            size += 2;
          }
          EFFECT_SWELL_MOTE(size, frame);
          EFFECT_DRAW_MOTE(blitters[0], canvas, sheet, Q_16_16_TO_INT(dust->x),
                           Q_16_16_TO_INT(dust->y), size);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          EFFECT_FALL_MOTE(dust->velocityY, gTitanBlade_DustGravity[i & 3],
                           frame);
          dust->velocityX = dust->velocityX * 62 / 64;
          dust->velocityY = dust->velocityY * 62 / 64;
          dust->variant++;
          if (dust->velocityY > 0 && Q_16_16_TO_INT(dust->y) > 108) {
            dust->variant = -1;
          }
        }
      }
    }

    // The 36x120 sword, until frame 95: it drops in over 10 frames, holds,
    // sinks slowly from frame 33 and plunges after frame 60.
    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = canvasCenterX - 18;
      high = 120;
      if (frame > 60) {
        top = frame * 8 - 450;
      } else if (frame > 32) {
        top = step / 2 + 16;
      } else if (frame <= 9) {
        top = frame * 16 - 128;
      } else {
        top = 16;
      }
      if (top + 120 > 108) {
        high = high - top - 12;
      }
      if (high > 0) {
        blitters[1](canvas, work->sheet + TITAN_BLADE_SWORD_PICTURE, x, top, 36,
                    high);
      }
    }

    // From frame 60 the sparks fly, falling by 1/8 pixel a frame each frame
    // with no damping and bouncing at half speed below y 108; a spark
    // shrinks from 8 pixels wide to 1 as its life runs out.
    if (frame > 59) {
      for (i = 0; i != TITAN_BLADE_SPARKS; i++) {
        struct Particle *spark = &TITAN_BLADE_SWORD_SPARKS[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          Particle_AdvanceWithGravity2D(spark, 64, px(1) / 8);
          y = spark->y;
          spark->variant--;
          if (y > px(108)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= px(127) - 1) && y >= 0) {
              u8 *picture;

              size = spark->variant / 5 + 1;
              picture = sheet + gBattleEffect_MotePictures[size - 1];
              x >>= 16;
              y >>= 16;
              x -= size / 2;
              y -= size;
              blitters[0](canvas, picture, x, y, size, size * 2);
            }
          }
        }
      }
    }

    // Every target flashes and recoils.
    if (frame == 68) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i], KNOCKBACK_JOLT);
      }
    }

    // The canvas flashes, filled at its brightest, as the sword lands and as
    // it strikes.
    if (frame == 9) {
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame == 60) {
      Iwram_FillWords(canvas, CANVAS_BYTES,
                      CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }

    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef TITAN_BLADE_SWORD_SPARKS
#undef TITAN_BLADE_FRAMES
#undef TITAN_BLADE_SWORD_PICTURE
#undef TITAN_BLADE_FLASHES
#undef TITAN_BLADE_RING
#undef TITAN_BLADE_DUST
#undef TITAN_BLADE_FIRST_DUST
#undef TITAN_BLADE_DRAWN_DUST
#undef TITAN_BLADE_SPARKS
#undef TITAN_BLADE_FLASH_PICTURES
