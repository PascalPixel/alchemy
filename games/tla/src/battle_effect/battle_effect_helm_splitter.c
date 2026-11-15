/// @file
/// Helm Splitter.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define HELM_SPLITTER_FRAMES 70

#define HELM_SPLITTER_STRIKE_FRAME 18 ///< the swirl strikes the target
/// The pink star effect, after the swirl.
#define HELM_SPLITTER_STAR_PICTURES 16000
#define HELM_SPLITTER_STAR_SIZE 16 ///< PinkStarSheet's six stars

/// The user leaps at the first target and strikes through a fire swirl; the
/// target reels and sparks scatter, bouncing off the ground.
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

  // Set up the canvas and load the swirl and the spark sheets.
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  if (args->variant == EFFECT_VARIANT_BASIC) {
    REG_BG2PA = Q_8_8(1);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  if (args->variant == EFFECT_VARIANT_BASIC) {
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                               work->sheet + HELM_SPLITTER_STAR_PICTURES, 1, 0);
    work->transferValue = 50;
  } else {
    // Lift every drawn pixel of the swirl into the upper half of the palette,
    // where the blue flame's colours go.
    u8 *pixels = work->sheet;

    for (i = 0; i != 16000; i++) {
      if (pixels[i] != 0) {
        pixels[i] = pixels[i] * 2 + 32;
        if (pixels[i] > 63)
          pixels[i] = 63;
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet,
                               work->sheet + 0x3e80, 1, 1);
    Iwram_CopyWordsEntry(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
    work->transferValue = 75;
  }
  work->transferMode = CANVAS_COPY_AND_FADE;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_FetchRectangleBlitters(args->side, draw);

  // No spark is alive yet.
  for (i = 0, life = &work->particles[0].variant; i != 64; i++) {
    *life = 0;
    life += 7;
  }
  sparks = work->particles;

  // The caster dashes at the target.
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 16, px(7));

  for (frame = 0; frame != HELM_SPLITTER_FRAMES; frame++) {
    // Frames 8-23: the swirl grows, one cell every second frame.
    if (args->variant == EFFECT_VARIANT_BASIC) {
      EffectPosition_ProjectBattler(args->user, &position);
      if (args->side == EFFECT_SIDE_ENEMIES)
        REG_BG2X = (80 - position.x) << 8;
      else
        REG_BG2X = (32 - position.x) << 8;
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[cell],
                  gHelmSplitter_PictureBiasX[cell] + 30,
                  gHelmSplitter_PictureBiasY[cell] + position.y - 60,
                  gHelmSplitter_PictureWidths[cell],
                  gHelmSplitter_PictureHeights[cell]);
        } else {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[cell],
                  -gHelmSplitter_PictureBiasX[cell] -
                      gHelmSplitter_PictureWidths[cell] + 98,
                  gHelmSplitter_PictureBiasY[cell] + position.y - 60,
                  gHelmSplitter_PictureWidths[cell],
                  gHelmSplitter_PictureHeights[cell]);
        }
      }
    } else {
      EffectPosition_ProjectBattler(args->targets[0], &position);
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (args->side == EFFECT_SIDE_ENEMIES) {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[cell],
                  position.x / 2 + gHelmSplitter_PictureBiasX[cell] - 16,
                  gHelmSplitter_PictureBiasY[cell] + position.y - 92,
                  gHelmSplitter_PictureWidths[cell],
                  gHelmSplitter_PictureHeights[cell]);
        } else {
          draw[0](canvas, work->sheet + gHelmSplitter_Pictures[cell],
                  position.x / 2 - gHelmSplitter_PictureBiasX[cell] -
                      gHelmSplitter_PictureWidths[cell] + 16,
                  gHelmSplitter_PictureBiasY[cell] + position.y - 92,
                  gHelmSplitter_PictureWidths[cell],
                  gHelmSplitter_PictureHeights[cell]);
        }
      }
    }

    // Frame 18: the hit. The target flashes and reels, the screen shakes and
    // the swirl bursts into sparks.
    if (frame == HELM_SPLITTER_STRIKE_FRAME) {
      EffectPosition_ProjectBattler(args->targets[0], &position);
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_BOUNCE);
      work->shakeFrames = 4;
      for (i = 0; i != args->variant * 16 + 16; i++) {
        s32 force = (Random16() & 255) + 256;
        s32 angle = Random16() & TRIG_ANGLE_MASK;

        spark = &sparks[i];
        if (args->variant == EFFECT_VARIANT_BASIC)
          spark->x = px(64);
        else
          spark->x = px(position.x / 2);
        spark->y = px(80);
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }

    // The sparks fall, bounce off the ground at y = 104 and fade out.
    for (i = 0; i != 64; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0);
        if (spark->y > px(104)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
          s32 sx = Q_16_16_TO_INT(spark->x);
          s32 sy = Q_16_16_TO_INT(spark->y);

          if (args->variant == EFFECT_VARIANT_BASIC)
            draw[0](canvas,
                    work->sheet + HELM_SPLITTER_STAR_PICTURES +
                        (((frame + i) / 4 % 6) << 8),
                    sx - 8, sy - 8, HELM_SPLITTER_STAR_SIZE, 16);
          else
            draw[0](canvas, work->sheet + 0x3e80 + i % 3 * PICTURE_BYTES(SKULL),
                    sx - 12, sy - 12, SKULL_WIDTH, SKULL_HEIGHT);
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

/// The sparks live in the map cell buffer's second 16 KB: 128 at the target,
/// 128 thrown up at the start and 128 shed by the head.
#define SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

/// The pillar sheet's cell is 24 x 39; the faded copies follow it.
#define PILLAR_CELL_SIZE 936

// Returns the fixed IWRAM word copy entry, so the call loads its address before
// its arguments.
static inline s32 (*Iwram_GetCopyWords(void))(void *destination,
                                              const void *source, s32 size) {
  return Iwram_CopyWordsEntry;
}
