/// @file
/// Ray.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define RAY_SPARK_COUNT 1024 ///< sparks in PARTICLES

#define RAY_PICTURES 1550 ///< the bolt pictures in the sheet
#define RAY_COUNT 4       ///< bolts

/// Four bolts close in, turning towards the centre for 16 frames and
/// scattering sparks from their roots; the sparks fly on until frame 64.
///
/// Abilities: Ray, Storm Ray, Destruct Ray (BATTLE_EFFECT_RAY).
void BattleEffect_RunRay(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 thrown;
  BlitterPair draw;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  // The motes for the sparks are in the battle sheet's slot.
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartTiledCanvas(1);
  // Destruct Ray stretches the canvas across the screen.
  if (work->args->variant == EFFECT_VARIANT_STRONGEST)
    REG_BG2PA = Q_8_8(0.5);
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + RAY_PICTURES, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->args->variant == EFFECT_VARIANT_STRONGEST) {
    if (work->args->side == EFFECT_SIDE_PARTY)
      REG_BG2X = -(16 << 8);
    else
      REG_BG2X = 16 << 8;
  } else {
    if (work->args->side == EFFECT_SIDE_PARTY)
      REG_BG2X = -(128 << 8);
  }

  // Every spark starts free (variant 0).
  for (i = 0; i != RAY_SPARK_COUNT; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
    SET_POSITION(spark, 64 << 8, 112 << 8, (Trig_Sin(angle) * speed) >> 16);
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ITEM_BREAK);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // The bolts, over the first 16 frames.
    if (frame < 16) {
      // Fill the canvas with level 16, not 0, on frames 2, 7 and 12.
      if (frame % 5 == 2)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
      for (bolt = 0; bolt != RAY_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 by a pixel a frame.
        thrown = 0;
        angle = ((frame << 11) + TRIG_TURN / 4) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -(((Trig_Cos(angle) * 8)) >> 16) - 8;
        if (work->args->variant == EFFECT_VARIANT_BASIC)
          draw[0](canvas,
                  work->sheet +
                      (Random16() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                      RAY_PICTURES,
                  x + (Random16() & 7) - 16, y, 24, 120);
        else
          draw[bolt & 1](canvas,
                         work->sheet +
                             (Random16() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                             RAY_PICTURES,
                         x + (Random16() & 7) - 16, y, 24, 120);

        // Throw sparks from the bolt's root, 112 pixels down its 120-pixel
        // picture: the first free ones, up to the variant's count (4, 6 or 8),
        // each living 32 to 39 frames.
        for (i = 0; i != RAY_SPARK_COUNT; i++) {
          struct Particle *spark = &PARTICLES[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - TRIG_TURN / 4;
            spark->x = px(x);
            spark->y = px(y + 112);
            spark->velocityX = (Trig_Sin(spread) * speed) >> 9;
            spark->velocityY = -(Trig_Cos(spread) * speed * 2) >> 7;
            spark->variant = (Random16() & 7) + 32;
            thrown++;
            if (thrown == gRay_Sparks[work->args->variant][1])
              break;
          }
        }
      }
      work->shakeFrames = 1;
    }

    // Move the sparks, bouncing them off the bottom (y 120) at half speed.
    for (i = 0; i != RAY_SPARK_COUNT; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, -px(1) / 32);
        if (spark->y > px(120)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < px(127) && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, x, y, size);
        }
      }
    }

    // The targets are hit 4 frames apart.
    if (frame >= 4 && frame < 96) {
      EFFECT_FOR_EACH_TARGET(i, work->args) {
        if (frame == i * 4 + 4)
          BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef RAY_SPARK_COUNT
#undef RAY_PICTURES
#undef RAY_COUNT
