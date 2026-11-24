/// @file
/// Ray.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define RAY_FRAMES 64 ///< frames of the bolts' approach

#define RAY_SPARK_COUNT PARTICLE_POOL_COUNT ///< sparks in PARTICLES

#define RAY_PICTURES 1550 ///< where the bolt pictures start in the sheet
#define RAY_COUNT 4       ///< bolts
/// The bolts turn a 32nd of a turn a frame, each a quarter turn more than the
/// one before.
#define RAY_BOLT_TURN (TRIG_TURN / 32)
/// The strongest variant shows the canvas twice as wide, scrolled this many
/// canvas pixels toward the targets' side.
#define RAY_WIDE_CANVAS_SHIFT 16
/// Where the canvas is shown over targets on the party's side: from screen x
/// 128.
#define RAY_PARTY_CANVAS_X 128
/// The column of a gRay_Sparks row that says how many sparks each bolt throws
/// a frame; nothing reads the first.
#define RAY_SPARKS_PER_BOLT 1

/// Four bolts close in, turning towards the centre and scattering sparks from
/// their roots.
///
/// Abilities: Ray, Storm Ray, Destruct Ray.
void BattleEffect_RunRay(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  u8 *motes;
  s32 bolt;
  s32 thrown;
  BlitterPair draw;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  motes = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartTiledCanvas(1);
  if (state->args->variant == EFFECT_VARIANT_STRONGEST)
    REG_BG2PA = Q_8_8(0.5);
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET,
                             state->sheet + RAY_PICTURES, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, motes, 0, 0);
  if (state->args->variant == EFFECT_VARIANT_STRONGEST) {
    if (state->args->side == EFFECT_SIDE_PARTY)
      REG_BG2X = -(RAY_WIDE_CANVAS_SHIFT << 8);
    else
      REG_BG2X = RAY_WIDE_CANVAS_SHIFT << 8;
  } else {
    if (state->args->side == EFFECT_SIDE_PARTY)
      REG_BG2X = -(RAY_PARTY_CANVAS_X << 8);
  }

  for (i = 0; i != RAY_SPARK_COUNT; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - DEGtoANG(90);
    SET_POSITION(spark, 64 << 8, 112 << 8, FIXEDtoINT(Trig_Sin(angle) * speed));
    spark->velocityZ = FIXEDtoINT(-(Trig_Cos(angle) * speed * 2));
    spark->variant = 0;
  }

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ITEM_BREAK);

  for (frame = 0; frame != RAY_FRAMES; frame++) {
    if (frame == 20)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // The bolts, over the first 16 frames.
    if (frame < 16) {
      if (frame % 5 == 2)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
      for (bolt = 0; bolt != RAY_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        thrown = 0;
        angle = (frame * RAY_BOLT_TURN + DEGtoANG(90)) * bolt;
        x = FIXEDtoINT((32 - frame) * Trig_Sin(angle)) + 64;
        y = -FIXEDtoINT(Trig_Cos(angle) * 8) - 8;
        if (state->args->variant == EFFECT_VARIANT_BASIC)
          draw[0](canvas,
                  state->sheet +
                      (Random16() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                      RAY_PICTURES,
                  x + (Random16() & 7) - 16, y, LIGHTNING_BOLT_WIDTH,
                  LIGHTNING_BOLT_HEIGHT);
        else
          draw[bolt & 1](canvas,
                         state->sheet +
                             (Random16() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                             RAY_PICTURES,
                         x + (Random16() & 7) - 16, y, LIGHTNING_BOLT_WIDTH,
                         LIGHTNING_BOLT_HEIGHT);

        // Sparks fly from the bolt's root.
        for (i = 0; i != RAY_SPARK_COUNT; i++) {
          struct Particle *spark = &PARTICLES[i];

          if (spark->variant == 0) {
            s32 speed;
            s32 spread;

            speed = (Random16() & 0x1ff) + 128;
            spread = (Random16() & 0x7fff) - DEGtoANG(90);
            spark->x = INTtoFIXED(x);
            spark->y = INTtoFIXED(y + 112);
            spark->velocityX = (Trig_Sin(spread) * speed) >> 9;
            spark->velocityY = -(Trig_Cos(spread) * speed * 2) >> 7;
            spark->variant = (Random16() & 7) + 32;
            thrown++;
            if (thrown ==
                gRay_Sparks[state->args->variant][RAY_SPARKS_PER_BOLT])
              break;
          }
        }
      }
      state->shakeFrames = 1;
    }

    // Sparks bounce off the bottom.
    for (i = 0; i != RAY_SPARK_COUNT; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, -toFIXED(1) / 32);
        if (spark->y > toFIXED(120)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < toFIXED(127) && spark->y >= 0) {
          s32 x = FIXEDtoINT(spark->x);
          s32 y = FIXEDtoINT(spark->y);
          s32 size = spark->variant / 8 + 1;

          EFFECT_DRAW_MOTE(draw[0], canvas, motes, x, y, size);
        }
      }
    }

    // The targets are hit 4 frames apart.
    if (frame >= 4 && frame < 96) {
      EFFECT_FOR_EACH_TARGET(i, state->args) {
        if (frame == i * 4 + 4)
          BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 10);
      }
    }
    BattleCamera_Shake(2, 4);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef RAY_SPARK_COUNT
#undef RAY_PICTURES
#undef RAY_COUNT
#undef RAY_BOLT_TURN
#undef RAY_WIDE_CANVAS_SHIFT
#undef RAY_PARTY_CANVAS_X
#undef RAY_FRAMES
#undef RAY_SPARKS_PER_BOLT
