/// @file
/// Part of effect.c, the battle effect object, compiled only through it.
/// Ray.

#include "../effect_internal.h"

// Closing bolts: four lightning bolts that close in from around the
// target, and the sparks they throw.

#define RAY_SPARK_COUNT 1024

#define RAY_CELLS 0x60e // the bolt pictures in the sheet
#define RAY_COUNT 4

/// Abilities: Ray, Storm Ray, Destruct Ray (kind 17).
///
/// Four bolts turn in towards the centre for 16 frames, scattering sparks
/// from their roots; the sparks fly on until frame 64.
///
/// effect: the targets. variant 2 shows the canvas at double width;
///         variant 0 draws every bolt with the maximum blend, the others
///         alternate it with additive. The variant also sets the sparks
///         each bolt throws a frame (gRay_Sparks).
void BattleFx_RunRay(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 thrown;
  BlitterPair draw;
  s32 i;

  // The work, the canvas, and BG2's scale and position.
  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginTiledCanvas(1);
  if (work->effect->variant == 2)
    REG_BG2PA = Q_8_8(0.5); // double width
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + RAY_CELLS, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->effect->variant == 2) {
    if (work->effect->side == 1)
      REG_BG2X = -0x1000;
    else
      REG_BG2X = 0x1000;
  } else {
    if (work->effect->side == 1)
      REG_BG2X = -0x8000;
  }

  // Set up the sparks.
  for (i = 0; i != RAY_SPARK_COUNT; i++) {
    struct EffectStep *spark = &PARTICLES[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - 0x4000;
    SET_POSITION(spark, 0x4000, 0x7000, (Trig_Sin(angle) * speed) >> 16);
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  AudioCommand_PlayFar(SE_ITEM_BREAK);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      EFFECT_SHOW_DAMAGE(SE_133); // start playing the results
    if (frame < 16) {
      // Clear the canvas every 5 frames.
      if (frame % 5 == 2)
        Iwram_FillWords(canvas, CANVAS_BYTES, 0x10101010);
      for (bolt = 0; bolt != RAY_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 by a pixel a frame.
        thrown = 0;
        angle = ((frame << 11) + 0x4000) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -(((Trig_Cos(angle) * 8)) >> 16) - 8;
        if (work->effect->variant == 0)
          draw[0](canvas,
                  work->sheet + (Random16() & 3) * LIGHTNING_BOLT_CELL_BYTES +
                      RAY_CELLS,
                  x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](canvas,
                         work->sheet +
                             (Random16() & 3) * LIGHTNING_BOLT_CELL_BYTES +
                             RAY_CELLS,
                         x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's root.
        for (i = 0; i != RAY_SPARK_COUNT; i++) {
          struct EffectStep *spark = &PARTICLES[i];

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
            thrown++;
            if (thrown == gRay_Sparks[work->effect->variant][1])
              break;
          }
        }
      }
      work->shakeFrames = 1;
    }

    // Move the sparks, bouncing them off the bottom (y 120).
    for (i = 0; i != RAY_SPARK_COUNT; i++) {
      struct EffectStep *spark = &PARTICLES[i];

      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, -0x800);
        if (spark->y > 0x780000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, x, y, size);
        }
      }
    }

    // The targets are hit 4 frames apart.
    if (frame >= 4 && frame < 96) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 4 + 4)
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
      }
    }
    Camera_ApplyShake(2, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef RAY_SPARK_COUNT
#undef RAY_CELLS
#undef RAY_COUNT
