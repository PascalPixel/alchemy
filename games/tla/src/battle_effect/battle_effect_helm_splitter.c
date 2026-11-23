/// @file
/// Helm Splitter.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define HELM_SPLITTER_FRAMES 70
#define HELM_SPLITTER_LEAP_FRAMES 16  ///< the user's leap at the target
#define HELM_SPLITTER_STRIKE_FRAME 18 ///< the swirl strikes the target
/// FireSwirlSheet's bytes; the sparks' pictures, PinkStarSheet's stars or
/// SkullSheet's skulls, follow them.
#define HELM_SPLITTER_SWIRL_BYTES 16000
#define HELM_SPLITTER_SPARK_PICTURES HELM_SPLITTER_SWIRL_BYTES
#define HELM_SPLITTER_SPARKS 16      ///< sparks the strike scatters, a variant
#define HELM_SPLITTER_SPARK_SLOTS 64 ///< spark slots moved and drawn
/// The side of each of PinkStarSheet's six square stars.
#define HELM_SPLITTER_STAR_SIZE 16

/// The user leaps at the first target and strikes through a fire swirl; the
/// target reels and sparks scatter, bouncing off the ground: stars for Helm
/// Splitter, and for Skull Splitter twice as many skulls from a swirl of blue
/// flame.
///
/// Abilities: Helm Splitter, Skull Splitter.
void BattleEffect_RunHelmSplitter(struct BattleEffectParams *args) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  DrawRectangle draw[2];
  struct ScreenPoint position;
  struct Particle *sparks;
  struct Particle *spark;
  s32 *life;
  s32 frame;
  s32 i;

  // Set up the canvas and load the swirl and the spark sprite sheets.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->variant == EFFECT_VARIANT_BASIC) {
    REG_BG2PA = Q_8_8(1);
  }
  Resource_LoadAndDecompress(RESOURCE_FIRE_SWIRL_SHEET, work->sheet, 1, 1);
  if (args->variant == EFFECT_VARIANT_BASIC) {
    Resource_LoadAndDecompress(RESOURCE_PINK_STAR_SHEET,
                               work->sheet + HELM_SPLITTER_SPARK_PICTURES, 1,
                               0);
    work->transferValue = CANVAS_FADE_HALF;
  } else {
    // Lift every drawn pixel of the swirl into the upper half of the palette,
    // where the blue flame's colours go.
    u8 *pixels = work->sheet;

    for (i = 0; i != HELM_SPLITTER_SWIRL_BYTES; i++) {
      if (pixels[i] != 0) {
        pixels[i] = pixels[i] * 2 + (CANVAS_LEVEL_BRIGHTEST + 1) / 2;
        if (pixels[i] > CANVAS_LEVEL_BRIGHTEST)
          pixels[i] = CANVAS_LEVEL_BRIGHTEST;
      }
    }
    Resource_LoadAndDecompress(
        RESOURCE_SKULL_SHEET, work->sheet + HELM_SPLITTER_SPARK_PICTURES, 1, 1);
    Iwram_CopyWordsEntry((void *)BG_PLTT,
                         Resource_GetTableEntry(RESOURCE_BLUE_FLAME_SHEET),
                         SHEET_PALETTE_BYTES);
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  }
  work->transferMode = CANVAS_COPY_AND_FADE;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  BattleEffect_FetchRectangleBlitters(args->side, draw);

  // No spark is alive yet.
  for (i = 0, life = &work->particles[0].variant;
       i != HELM_SPLITTER_SPARK_SLOTS; i++) {
    *life = 0;
    life += sizeof(struct Particle) / sizeof(*life);
  }
  sparks = work->particles;

  // The user leaps at the target.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0],
                                 HELM_SPLITTER_LEAP_FRAMES, toFIXED(7));

  for (frame = 0; frame != HELM_SPLITTER_FRAMES; frame++) {
    // The swirl grows, a picture every second frame.
    if (args->variant == EFFECT_VARIANT_BASIC) {
      EffectPosition_ProjectBattler(args->user, &position);
      if (args->side == EFFECT_SIDE_ENEMIES)
        REG_BG2X = (80 - position.x) << 8;
      else
        REG_BG2X = (32 - position.x) << 8;
      if (frame >= 8 && frame <= 23) {
        s32 picture = (frame - 8) / 2;

        if (picture > 6)
          picture = 6;
        if (args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[picture],
                  gHelmSplitter_PictureBiasX[picture] + 30,
                  gHelmSplitter_PictureBiasY[picture] + position.y - 60,
                  gHelmSplitter_PictureWidths[picture],
                  gHelmSplitter_PictureHeights[picture]);
        } else {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[picture],
                  -gHelmSplitter_PictureBiasX[picture] -
                      gHelmSplitter_PictureWidths[picture] + 98,
                  gHelmSplitter_PictureBiasY[picture] + position.y - 60,
                  gHelmSplitter_PictureWidths[picture],
                  gHelmSplitter_PictureHeights[picture]);
        }
      }
    } else {
      EffectPosition_ProjectBattler(args->targets[0], &position);
      if (frame >= 8 && frame <= 23) {
        s32 picture = (frame - 8) / 2;

        if (picture > 6)
          picture = 6;
        if (args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[picture],
                  position.x / 2 + gHelmSplitter_PictureBiasX[picture] - 16,
                  gHelmSplitter_PictureBiasY[picture] + position.y - 92,
                  gHelmSplitter_PictureWidths[picture],
                  gHelmSplitter_PictureHeights[picture]);
        } else {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[picture],
                  position.x / 2 - gHelmSplitter_PictureBiasX[picture] -
                      gHelmSplitter_PictureWidths[picture] + 16,
                  gHelmSplitter_PictureBiasY[picture] + position.y - 92,
                  gHelmSplitter_PictureWidths[picture],
                  gHelmSplitter_PictureHeights[picture]);
        }
      }
    }

    // The strike: the target flashes and reels, the screen shakes and the
    // swirl bursts into sparks.
    if (frame == HELM_SPLITTER_STRIKE_FRAME) {
      EffectPosition_ProjectBattler(args->targets[0], &position);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_BOUNCE);
      work->shakeFrames = 4;
      for (i = 0;
           i != args->variant * HELM_SPLITTER_SPARKS + HELM_SPLITTER_SPARKS;
           i++) {
        s32 force = (Random16() & 255) + 256;
        s32 angle = Random16() & TRIG_ANGLE_MASK;

        spark = &sparks[i];
        if (args->variant == EFFECT_VARIANT_BASIC)
          spark->x = toFIXED(64);
        else
          spark->x = INTtoFIXED(position.x / 2);
        spark->y = toFIXED(80);
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }

    // The sparks fall, bounce off the ground at y = 104 and fade out.
    for (i = 0; i != HELM_SPLITTER_SPARK_SLOTS; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, 0);
        if (spark->y > toFIXED(104)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < toFIXED(127)) &&
                   spark->y >= 0) {
          s32 sx = FIXEDtoINT(spark->x);
          s32 sy = FIXEDtoINT(spark->y);

          if (args->variant == EFFECT_VARIANT_BASIC)
            draw[0](canvas,
                    work->sheet + HELM_SPLITTER_SPARK_PICTURES +
                        ((frame + i) / 4 % 6) * HELM_SPLITTER_STAR_SIZE *
                            HELM_SPLITTER_STAR_SIZE,
                    sx - HELM_SPLITTER_STAR_SIZE / 2,
                    sy - HELM_SPLITTER_STAR_SIZE / 2, HELM_SPLITTER_STAR_SIZE,
                    HELM_SPLITTER_STAR_SIZE);
          else
            draw[0](canvas,
                    work->sheet + HELM_SPLITTER_SPARK_PICTURES +
                        i % 3 * PICTURE_BYTES(SKULL),
                    sx - SKULL_WIDTH / 2, sy - SKULL_HEIGHT / 2, SKULL_WIDTH,
                    SKULL_HEIGHT);
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef HELM_SPLITTER_FRAMES
#undef HELM_SPLITTER_LEAP_FRAMES
#undef HELM_SPLITTER_STRIKE_FRAME
#undef HELM_SPLITTER_SWIRL_BYTES
#undef HELM_SPLITTER_SPARK_PICTURES
#undef HELM_SPLITTER_SPARKS
#undef HELM_SPLITTER_SPARK_SLOTS
#undef HELM_SPLITTER_STAR_SIZE
