/// @file
/// Ray.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

#define RAY_SPARK_COUNT 1024 ///< sparks in PARTICLES

#define RAY_CELLS 0x60e ///< the bolt pictures in the sheet
#define RAY_COUNT 4     ///< bolts

/// Four bolts close in, turning towards the centre for 16 frames and
/// scattering sparks from their roots; the sparks fly on until frame 64.
///
/// Abilities: Ray, Storm Ray, Destruct Ray (BATTLE_ANIM_RAY).
///
/// The targets flash white 4 frames apart from frame 4. Writes REG_BG2PA and
/// REG_BG2X, sets work->shakeFrames over the first 16 frames.
///
/// effect: the targets. Destruct Ray shows the canvas at double width; Ray
///         draws every bolt with the maximum blend, the others alternate it
///         with additive. The variant also sets the sparks each bolt throws
///         a frame (gRay_Sparks).
void BattleAnim_RunRay(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 bolt;
  s32 thrown;
  BlitterPair draw;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  cursor = slots;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  // The motes for the sparks are in the battle sheet's slot.
  sheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleAnim_StartTiledCanvas(1);
  // Destruct Ray stretches the canvas across the screen; the others show it
  // at its own width over the targets' half (on ANIM_SIDE_PARTY, moved 128
  // pixels). BG2X is 20.8: Destruct Ray's canvas moves 16 texels towards the
  // targets.
  if (work->effect->variant == ANIM_VARIANT_STRONGEST)
    REG_BG2PA = Q_8_8(0.5);
  else
    REG_BG2PA = Q_8_8(1);
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                    COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + RAY_CELLS, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->effect->variant == ANIM_VARIANT_STRONGEST) {
    if (work->effect->side == ANIM_SIDE_PARTY)
      REG_BG2X = -0x1000;
    else
      REG_BG2X = 0x1000;
  } else {
    if (work->effect->side == ANIM_SIDE_PARTY)
      REG_BG2X = -0x8000;
  }

  // Every spark starts free (variant 0). A bolt that throws it overwrites x,
  // y and their velocities; z and velocityZ, set only here, are never read,
  // but the two random numbers each spark takes here still advance the
  // sequence.
  for (i = 0; i != RAY_SPARK_COUNT; i++) {
    struct Particle *spark = &PARTICLES[i];
    s32 speed;
    s32 angle;

    speed = (Random16() & 0x3ff) + 256;
    angle = (Random16() & 0x7fff) - 0x4000;
    SET_POSITION(spark, 0x4000, 0x7000, (Trig_Sin(angle) * speed) >> 16);
    spark->velocityZ = -(Trig_Cos(angle) * speed * 2) >> 16;
    spark->variant = 0;
  }

  ANIM_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlaySongFar(SE_ITEM_BREAK);

  for (frame = 0; frame != 64; frame++) {
    if (frame == 20)
      ANIM_SHOW_DAMAGE(SE_IMPACT);
    // The bolts, over the first 16 frames.
    if (frame < 16) {
      // Fill the canvas with level 16, not 0, on frames 2, 7 and 12.
      if (frame % 5 == 2)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
      for (bolt = 0; bolt != RAY_COUNT; bolt++) {
        s32 angle;
        s32 x;
        s32 y;

        // The radius across shrinks from 32 by a pixel a frame; bolt n starts
        // a quarter turn further round than bolt n - 1 and turns n times as
        // fast. Each frame a bolt picks one of four pictures and jitters up
        // to 7 pixels across.
        thrown = 0;
        angle = ((frame << 11) + 0x4000) * bolt;
        x = (((32 - frame) * Trig_Sin(angle)) >> 16) + 64;
        y = -(((Trig_Cos(angle) * 8)) >> 16) - 8;
        if (work->effect->variant == ANIM_VARIANT_BASIC)
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

        // Throw sparks from the bolt's root, 112 pixels down its 120-pixel
        // picture: the first free ones, up to the variant's count (4, 6 or
        // 8), each living 32 to 39 frames. They leave upwards, in the upper
        // half circle, at up to 1.25 pixels a frame across and 10 up.
        for (i = 0; i != RAY_SPARK_COUNT; i++) {
          struct Particle *spark = &PARTICLES[i];

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

    // Move the sparks, bouncing them off the bottom (y 120) at half speed;
    // a spark is drawn only inside the canvas, and shrinks as it ages. The
    // gravity is negative, 1/32 pixel a frame upwards each frame, so the
    // sparks keep rising, and they keep 60/64 of their speed.
    for (i = 0; i != RAY_SPARK_COUNT; i++) {
      struct Particle *spark = &PARTICLES[i];

      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, -0x800);
        if (spark->y > 0x780000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;
          s32 size = spark->variant / 8 + 1;

          ANIM_DRAW_MOTE(draw[0], canvas, sheet, x, y, size);
        }
      }
    }

    // The targets are hit 4 frames apart. The bound of 96 is past the last
    // frame (63), so it never limits.
    if (frame >= 4 && frame < 96) {
      ANIM_FOR_EACH_TARGET(i, work->effect) {
        if (frame == i * 4 + 4)
          BattleAnim_TintBattler(work->effect->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 10);
      }
    }
    BattleCamera_Shake(2, 4);
    ANIM_END_FRAME(work);
  }

  ANIM_FINISH_CANVAS();
}

#undef RAY_SPARK_COUNT
#undef RAY_CELLS
#undef RAY_COUNT
