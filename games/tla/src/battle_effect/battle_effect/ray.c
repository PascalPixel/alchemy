/// @file
/// Ray.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define SPARK_COUNT 1024

#define BOLT_SHEET 0x60e // the bolt pictures in the sheet
#define BOLT_SIZE 2880   // one 24x120 bolt
#define BOLT_COUNT 4

// Four lightning bolts turn and close in on the targets, throwing sparks
// from their feet.

// Setup: 1,024 sparks wait at the canvas center. Frame loop: for 16
// frames the bolts circle in, their radius shrinking a pixel a frame, the
// canvas wiped every five frames, and each bolt throws the variant's
// number of sparks; the sparks fall and bounce off y = 120 until frame 64.
// The targets are hit from frame 4, four frames apart.
void BattleEffect_RunRay(struct BattleEffectArgument *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 xOffset;
  s32 count;
  DrawRectangle draw[2];
  struct EffectPosition midpoint;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  xOffset = 0;
  if (args->side == 0)
    BattleEffect_StartTiledCanvasFilled(0x2001);
  else
    BattleEffect_StartTiledCanvas(0x2001);
  if (args->variant == 2)
    REG_BG2PA = Q_8_8(0.5); // twice as wide
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

  // Scroll the canvas: a fixed offset for the wide variant, else onto the
  // targets' midpoint.
  if (args->variant == 2) {
    if (work->effect->side == 1)
      REG_BG2X = -0x1000;
    else
      REG_BG2X = 0x1000;
  } else {
    BattleEffect_GetTargetsMidpoint(&midpoint);
    midpoint.x -= 64;
    if (midpoint.x < 0) {
      xOffset = midpoint.x;
      midpoint.x = 0;
    } else if (midpoint.x > 112) {
      xOffset = midpoint.x - 112;
      midpoint.x = 112;
    }
    REG_BG2X = -midpoint.x << 8;
  }

  // The sparks wait at the center.
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &SPARKS[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
    SET_POSITION(spark, px(1) / 4, px(7) / 16, (Trig_Sin(angle) * speed) >> 16);
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ITEM_BREAK);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      BattlePlayback_StartFar(133);
    if (frame < 16) {
      // Wipe the canvas every five frames.
      if (frame % 5 == 2)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
      for (bolt = 0; bolt != BOLT_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 a pixel a frame.
        count = 0;
        angle = ((frame << 11) + TRIG_TURN / 4) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -((Trig_Cos(angle) << 3) >> 16) - 8;
        if (args->variant == 0)
          draw[0](canvas,
                  work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
                  xOffset + x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](
              canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
              xOffset + x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's foot.
        for (i = 0; i != SPARK_COUNT; i++) {
          struct EffectStep *spark = &SPARKS[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - 0x4000;
            spark->x = x << 16;
            spark->y = (y + 112) << 16;
            spark->velocityX = (Trig_Sin(spread) * speed) >> 9;
            spark->velocityY = -(Trig_Cos(spread) * speed * 2) >> 7;
            spark->variant = (Random16() & 7) + 32;
            count++;
            if (count == gRay_Sparks[args->variant][1])
              break;
          }
        }
      }
      work->shakeFrames = 1;
    }

    // The sparks fall and bounce off the bottom (y 120).
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *spark = &SPARKS[i];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, -0x800);
        if (spark->y > 0x780000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          draw[0](canvas, sheet + gBattleEffect_MotePictures[size - 1],
                  xOffset + x - size / 2, y - size, size, size * 2);
        }
      }
    }

    // The targets are hit four frames apart.
    if (frame >= 4 && frame < 96) {
      for (i = 0; i != args->count; i++) {
        if (frame == i * 4 + 4)
          BattleEffect_TintBattler(args->actors[i], 7, 5, i, 10);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  BattleEffect_EndCanvasLayer();
}
