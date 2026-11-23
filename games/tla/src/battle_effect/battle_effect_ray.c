/// @file
/// Ray.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define RAY_FRAMES 64

#define RAY_SPARK_COUNT PARTICLE_POOL_COUNT ///< sparks in PARTICLES
/// The columns of a gRay_Sparks row, one row a variant.
enum {
  RAY_SPARKS_UNREAD,   ///< Unused: nothing reads it
  RAY_SPARKS_PER_BOLT, ///< sparks each bolt throws a frame
};

#define RAY_PICTURES 1550 ///< the bolt pictures in the sheet
#define RAY_COUNT 4       ///< bolts

/// The level the canvas flashes to every five frames while the bolts strike.
#define RAY_FLASH_LEVEL 16
/// How far Destruct Ray's canvas, at twice its width, is scrolled towards the
/// targets' side, in canvas pixels.
#define RAY_WIDE_CANVAS_SHIFT 16

/// Four bolts close in, turning towards the centre and scattering sparks from
/// their roots, while the canvas flashes.
///
/// Abilities: Ray, Storm Ray, Destruct Ray.
void BattleEffect_RunRay(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 edgeShift;
  s32 thrown;
  BlitterPair draw;
  struct ScreenPoint midpoint;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sheet = Ram_HeapSlots->battleSheet;
  edgeShift = 0;
  if (args->side == EFFECT_SIDE_ENEMIES)
    BattleEffect_StartTiledCanvasFilled(BGCNT_WRAP | BGCNT_PRIORITY(1));
  else
    BattleEffect_StartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
  if (args->variant == EFFECT_VARIANT_STRONGEST)
    REG_BG2PA = Q_8_8(0.5); // twice as wide
  else
    REG_BG2PA = Q_8_8(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET,
                             work->sheet + RAY_PICTURES, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);

  // Destruct Ray's wide canvas is scrolled towards the targets' side; the
  // others' is centred on the targets and kept on the screen, and where it
  // meets an edge the bolts shift by what is left over.
  if (args->variant == EFFECT_VARIANT_STRONGEST) {
    if (work->args->side == EFFECT_SIDE_PARTY)
      REG_BG2X = -(RAY_WIDE_CANVAS_SHIFT << 8);
    else
      REG_BG2X = RAY_WIDE_CANVAS_SHIFT << 8;
  } else {
    BattleEffect_GetTargetsMidpoint(&midpoint);
    midpoint.x -= CANVAS_SIZE / 2;
    if (midpoint.x < 0) {
      edgeShift = midpoint.x;
      midpoint.x = 0;
    } else if (midpoint.x > NARROW_CANVAS_PARTY_X) {
      edgeShift = midpoint.x - NARROW_CANVAS_PARTY_X;
      midpoint.x = NARROW_CANVAS_PARTY_X;
    }
    REG_BG2X = -midpoint.x << 8;
  }

  // Every spark waits to be thrown. The place and speeds set here are never
  // read: a spark is given its own as it is thrown.
  for (i = 0; i != RAY_SPARK_COUNT; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - DEGtoANG(90);
    SET_POSITION(spark, toFIXED(1) / 4, toFIXED(7) / 16,
                 FIXEDtoINT(Trig_Sin(angle) * speed));
    spark->velocityZ = FIXEDtoINT(-(Trig_Cos(angle) * speed * 2));
    spark->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ITEM_BREAK);

  for (frame = 0; frame != RAY_FRAMES; frame++) {
    if (frame == 20)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // The bolts, over the first 16 frames.
    if (frame < 16) {
      if (frame % 5 == 2)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(RAY_FLASH_LEVEL));
      for (bolt = 0; bolt != RAY_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The bolts close in from 32 pixels either side, a pixel a frame,
        // each turning at its own rate.
        thrown = 0;
        angle = (frame * (TRIG_TURN / 32) + DEGtoANG(90)) * bolt;
        x = FIXEDtoINT((32 - frame) * Trig_Sin(angle)) + 64;
        y = -FIXEDtoINT(Trig_Cos(angle) * 8) - 8;
        if (args->variant == EFFECT_VARIANT_BASIC)
          draw[0](canvas,
                  work->sheet +
                      (Random16() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                      RAY_PICTURES,
                  edgeShift + x + (Random16() & 7) - 16, y,
                  LIGHTNING_BOLT_WIDTH, LIGHTNING_BOLT_HEIGHT);
        else
          draw[bolt & 1](canvas,
                         work->sheet +
                             (Random16() & 3) * LIGHTNING_BOLT_PICTURE_BYTES +
                             RAY_PICTURES,
                         edgeShift + x + (Random16() & 7) - 16, y,
                         LIGHTNING_BOLT_WIDTH, LIGHTNING_BOLT_HEIGHT);

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
            if (thrown == gRay_Sparks[args->variant][RAY_SPARKS_PER_BOLT])
              break;
          }
        }
      }
      work->shakeFrames = 1;
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

          EFFECT_DRAW_MOTE(draw[0], canvas, sheet, edgeShift + x, y, size);
        }
      }
    }

    if (frame >= 4 && frame < 96) {
      EFFECT_FOR_EACH_TARGET(i, args) {
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

#undef RAY_FRAMES
#undef RAY_SPARK_COUNT
#undef RAY_PICTURES
#undef RAY_COUNT
#undef RAY_FLASH_LEVEL
#undef RAY_WIDE_CANVAS_SHIFT
