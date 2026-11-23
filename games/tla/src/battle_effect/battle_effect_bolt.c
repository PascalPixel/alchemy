/// @file
/// Bolt.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define BOLT_INTERVAL 8 ///< frames between one target's bolt and the next's
/// The run of PARTICLES each target's sparks are thrown from.
#define BOLT_SPARKS_PER_TARGET 128
/// The canvas line where the bolt ends and the sparks bounce.
#define BOLT_GROUND_Y 104
/// The level the canvas flashes to as each bolt starts.
#define BOLT_FLASH_LEVEL 16
/// The frames a spark keeps each size as it burns out.
#define BOLT_SPARK_SIZE_FRAMES 16

/// The second blitter, made anew for each glint with its random flip.
#define BOLT_GLINT_BLITTER ((DrawRectangle)Ram_HeapSlots->ramCodePaired)

/// The columns of a gBolt_Counts row, one row a variant.
enum {
  BOLT_SPARKS,       ///< sparks thrown from each bolt's foot, at most the
                     ///< BOLT_SPARKS_PER_TARGET of its target's run
  BOLT_GLINTS,       ///< glints a frame around each target
  BOLT_GLINT_SPREAD, ///< how high and wide the glints spread, in pixels
  BOLT_STRIKES,      ///< bolts drawn over each other
  BOLT_COUNT_COLUMNS
};
/// A variant's count in column column of gBolt_Counts.
#define BOLT_COUNT(variant, column)                                            \
  gBolt_Counts[(variant) * BOLT_COUNT_COLUMNS + (column)]

/// Where the bolt pictures start in the sheet, after the six glints.
#define BOLT_PICTURES GLINT_SET_BYTES

/// A bolt of lightning strikes each target in turn, and sparks fly up from
/// where it lands while glints flicker there.
///
/// Abilities: Bolt, Flash Bolt, Blue Bolt.
void BattleEffect_RunBolt(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  BlitterPair draw;
  u8 *sprites;
  struct ScreenPoint screen;
  s32 j;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  // The tornado sheet only for its glints.
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, work->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET,
                             work->sheet + BOLT_PICTURES, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  for (j = 0; j != PARTICLE_POOL_COUNT; j++)
    PARTICLES[j].variant = 0;
  EFFECT_FREE_PARTICLES(j, work->particles, 64);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ITEM_BREAK);

  for (frame = 0; frame != args->targetCount * BOLT_INTERVAL + 40; frame++) {
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The canvas flashes as each target's turn starts.
    EFFECT_FOR_EACH_TARGET(i, args) {
      if (frame == i * BOLT_INTERVAL)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                             CANVAS_FILL_WORD(BOLT_FLASH_LEVEL));
    }

    EFFECT_FOR_EACH_TARGET(i, args) {
      s32 start = i * BOLT_INTERVAL;

      EffectPosition_ProjectBattler(args->targets[i], &screen);
      screen.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        EFFECT_STRIKE_TARGET(args, i, 6, KNOCKBACK_BOUNCE);
      }

      // The bolt drops from the top, 64 pixels a frame, to the ground.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > BOLT_GROUND_Y)
          height = BOLT_GROUND_Y;
        for (j = 0; j != BOLT_COUNT(args->variant, BOLT_STRIKES); j++) {
          draw[0](canvas,
                  work->sheet +
                      (((i + frame + j) / 2) & 3) *
                          LIGHTNING_BOLT_PICTURE_BYTES +
                      BOLT_PICTURES,
                  screen.x - LIGHTNING_BOLT_WIDTH / 2, 0, LIGHTNING_BOLT_WIDTH,
                  height);
        }

        // Sparks fly up from its foot, within 90 degrees of straight up.
        if (frame == start + 2) {
          for (j = 0; j != BOLT_COUNT(args->variant, BOLT_SPARKS); j++) {
            struct Particle *spark = &PARTICLES[i * BOLT_SPARKS_PER_TARGET + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - DEGtoANG(90);

            spark->x = INTtoFIXED(screen.x);
            spark->y = toFIXED(BOLT_GROUND_Y);
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints flicker at the foot, each at a random place with a random
      // flip.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != BOLT_COUNT(args->variant, BOLT_GLINTS); j++) {
          s32 kind = j & 3;
          s32 drop = Random16() % BOLT_COUNT(args->variant, BOLT_GLINT_SPREAD);
          s32 spread = BOLT_COUNT(args->variant, BOLT_GLINT_SPREAD) - drop + 1;
          s32 y = screen.y - drop -
                  (gBattleEffect_GlintPictureHeights[kind] >> 1) + 8;
          u32 random = Random16();
          s32 x = screen.x;
          u32 flip;

          x += random % spread;
          x -= spread / 2;
          x -= gBattleEffect_GlintPictureWidths[kind] >> 1;
          flip = Random16();
          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                              gBolt_GlintFlips[flip & 3] | EFFECT_CLIP_BOUNDS,
                              gBolt_GlintModes[args->variant]);
          BOLT_GLINT_BLITTER(canvas,
                             work->sheet + gBattleEffect_GlintPictures[kind], x,
                             y, gBattleEffect_GlintPictureWidths[kind],
                             gBattleEffect_GlintPictureHeights[kind]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        }
      }
    }

    // Sparks fall, bounce off the ground at half speed, and shrink as they
    // burn out.
    for (j = 0; j != PARTICLE_POOL_COUNT; j++) {
      struct Particle *spark = &PARTICLES[j];

      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, toFIXED(1) / 16);
        if (spark->y > toFIXED(BOLT_GROUND_Y)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < toFIXED(127)) &&
                   spark->y >= 0) {
          s32 size = spark->variant / BOLT_SPARK_SIZE_FRAMES + 1;
          s32 x = FIXEDtoINT(spark->x);
          s32 y = FIXEDtoINT(spark->y);

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, size);
        }
      }
    }
    BattleCamera_Shake(2, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef BOLT_INTERVAL
#undef BOLT_SPARKS_PER_TARGET
#undef BOLT_GROUND_Y
#undef BOLT_FLASH_LEVEL
#undef BOLT_SPARK_SIZE_FRAMES
#undef BOLT_GLINT_BLITTER
#undef BOLT_COUNT
#undef BOLT_PICTURES
