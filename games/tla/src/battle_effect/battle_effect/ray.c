/// @file
/// Ray.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define SPARK_COUNT 1024

#define BOLT_SHEET 0x60e ///< the bolt pictures in the sheet
#define BOLT_SIZE 2880   ///< one 24x120 bolt
#define BOLT_COUNT 4

// Four lightning bolts turn and close in on the targets, throwing sparks
// from their feet.

/// Four bolts close in, turning towards the centre and scattering sparks from
/// their roots.
///
/// Abilities: Ray, Storm Ray, Destruct Ray.
void BattleEffect_RunRay(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 xOffset;
  s32 count;
  DrawRectangle draw[2];
  struct ScreenPoint midpoint;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  xOffset = 0;
  if (args->side == EFFECT_SIDE_ENEMIES)
    BattleEffect_StartTiledCanvasFilled(0x2001);
  else
    BattleEffect_StartTiledCanvas(0x2001);
  if (args->variant == EFFECT_VARIANT_STRONGEST)
    REG_BG2PA = Q_8_8(0.5); // twice as wide
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCode2;
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  // Scroll the canvas: a fixed offset for the wide variant, else onto the
  // targets' midpoint.
  if (args->variant == EFFECT_VARIANT_STRONGEST) {
    if (work->args->side == EFFECT_SIDE_PARTY)
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
    struct Particle *spark = &SPARKS[i];
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
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
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
        if (args->variant == EFFECT_VARIANT_BASIC)
          draw[0](canvas,
                  work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
                  xOffset + x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](
              canvas, work->sheet + (Random16() & 3) * BOLT_SIZE + BOLT_SHEET,
              xOffset + x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's foot.
        for (i = 0; i != SPARK_COUNT; i++) {
          struct Particle *spark = &SPARKS[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - 0x4000;
            spark->x = px(x);
            spark->y = px(y + 112);
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
      struct Particle *spark = &SPARKS[i];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, -px(1) / 32);
        if (spark->y > px(120)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < px(127) && spark->y >= 0) {
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
      for (i = 0; i != args->targetCount; i++) {
        if (frame == i * 4 + 4)
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
