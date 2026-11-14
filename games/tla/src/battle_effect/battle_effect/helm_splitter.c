/// @file
/// Helm Splitter.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// Effect kind 51: the caster dashes at the target while a swirl of flame grows
// over seven frames, then bursts into sparks that fall and bounce on the
// ground. Variant 0 is ☀️'s: the swirl rises at the caster and throws pink
// stars. Variant 1 is ⚓️'s own: the swirl, recoloured into the blue flame's
// palette, rises at the target and throws 32 skulls.
void BattleEffect_RunHelmSplitter(struct BattleEffectArgument *args) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectStep *sparks;
  struct EffectStep *spark;
  s32 *life;
  s32 frame;
  s32 i;

  // Set up the canvas and load the swirl and the spark sheets.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  if (args->variant == 0) {
    REG_BG2PA = Q_8_8(1);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  if (args->variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                               work->sheet + 0x3e80, 1, 0);
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
  BattleMotion_ApproachTargetFar(args->actor, args->actors[0], 16, px(7));

  for (frame = 0; frame != 70; frame++) {
    // Frames 8-23: the swirl grows, one cell every second frame.
    if (args->variant == 0) {
      EffectPosition_ProjectBattler(args->actor, &position);
      if (args->side == 0)
        REG_BG2X = (80 - position.x) << 8;
      else
        REG_BG2X = (32 - position.x) << 8;
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (args->side == 0) {
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
      EffectPosition_ProjectBattler(args->actors[0], &position);
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (args->side == 0) {
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
    if (frame == 18) {
      EffectPosition_ProjectBattler(args->actors[0], &position);
      BattlePlayback_StartFar(134);
      BattleEffect_TintBattler(args->actors[0], 7, 5, 0, 8);
      BattleMotion_ApplyKnockbackFar(args->actors[0], 6);
      work->shakeFrames = 4;
      for (i = 0; i != args->variant * 16 + 16; i++) {
        s32 force = (Random16() & 255) + 256;
        s32 angle = Random16() & 0xffff;

        spark = &sparks[i];
        if (args->variant == 0)
          spark->x = px(64);
        else
          spark->x = (position.x / 2) << 16;
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
        if (spark->y > 0x680000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
          s32 sx = spark->x >> 16;
          s32 sy = spark->y >> 16;

          if (args->variant == 0)
            draw[0](canvas, work->sheet + 0x3e80 + (((frame + i) / 4 % 6) << 8),
                    sx - 8, sy - 8, 16, 16);
          else
            draw[0](canvas, work->sheet + 0x3e80 + i % 3 * 576, sx - 12,
                    sy - 12, 24, 24);
        }
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}

// The sparks live in the map cell buffer's second 16 KB: 128 at the target,
// 128 thrown up at the start and 128 shed by the head.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// The pillar sheet's cell is 24 x 39; the faded copies follow it.
#define PILLAR_CELL_SIZE 936

// Returns the fixed IWRAM word copy entry, so the call loads its address before
// its arguments.
static inline s32 (*Iwram_GetCopyWords(void))(void *destination,
                                              const void *source, s32 size) {
  return Iwram_CopyWordsEntry;
}
