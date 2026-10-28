/// @file
/// Bolt.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

/// The second blitter, made anew for each glint with its random flip.
#define BOLT_GLINT_BLITTER                                                     \
  ((DrawRectangle *)                                                           \
       gBattleEffectWork)[HEAP_SLOT_RAM_CODE_2 - HEAP_SLOT_BATTLE_EFFECT]

/// The columns of a gBolt_Counts row.
enum {
  BOLT_SPARKS,       ///< sparks thrown from each bolt's foot, at most the
                     ///< 128 of its target's run of PARTICLES
  BOLT_GLINTS,       ///< glints a frame around each target
  BOLT_GLINT_SPREAD, ///< how high and wide the glints spread, in pixels
  BOLT_STRIKES       ///< bolts drawn over each other
};

/// Where the bolt pictures start in the sheet: after the six glints that open
/// the tornado sheet (gBattleEffect_GlintCells), over its tornado strips.
#define BOLT_OFFSETS GLINT_SET_BYTES

/// A bolt of lightning strikes each target in turn, 8 frames apart.
///
/// Abilities: Bolt, Flash Bolt, Blue Bolt (BATTLE_EFFECT_BOLT).
///
/// Target i's turn starts on frame 8i with a flash of the canvas. The bolt
/// drops from the top, 64 pixels a frame down to y 104, and the screen shakes
/// the frame after; on frame 8i + 2 sparks fly up from its foot and glints
/// flicker there for 22 frames; on frame 8i + 4 the target recoils. The
/// battle moves on to the results on frame 24.
///
/// PARTICLES holds the sparks, 128 for each target. Sets work->shakeFrames on
/// each strike, and ends with the transfer task removed, the blitter
/// freed and the canvas layer closed; the glints' blitter is built and freed
/// for each glint.
///
/// args: the targets; variant picks the row of gBolt_Counts
///         and the glints' blend.
void BattleEffect_RunBolt(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  /* FAKEMATCH(dead): a pair whose second blitter, never used, fills four
   * bytes of the frame; as one DrawRectangle, 33 of 555 instructions differ
   * in tbs-en. */
  BlitterPair draw;
  u8 *sprites;
  struct ScreenPoint screen;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
  // The tornado sheet only for its glints.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + BOLT_OFFSETS, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  // A spark's variant is its life left, 0 when free. work->particles is
  // marked free too, though nothing here uses it.
  for (j = 0; j != 1024; j++)
    PARTICLES[j].variant = 0;
  EFFECT_FREE_PARTICLES(j, work->particles, 64);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(SE_ITEM_BREAK);
  for (frame = 0; frame != work->args->targetCount * 8 + 40; frame++) {
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    // The flash at each turn's start: the whole canvas set to level 16.
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == i * 8)
        Iwram_FillWords(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
    }
    // Each target's turn, 8 frames after the one before.
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      s32 start = i * 8;

      Projection_Unit(work->args->targets[i], &screen);
      screen.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 6);
        BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                       KNOCKBACK_BOUNCE);
      }

      // The bolt (24 wide) grows 64 pixels a frame and stops at 104; it is
      // drawn the variant's number of times over itself, each pass picking
      // one of the four pictures by target, frame and pass, so the picture
      // changes every 2 frames.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > 104)
          height = 104;
        for (j = 0; j != gBolt_Counts[work->args->variant][BOLT_STRIKES]; j++) {
          draw[0](canvas,
                  work->sheet +
                      (((i + frame + j) / 2) & 3) * LIGHTNING_BOLT_CELL_BYTES +
                      BOLT_OFFSETS,
                  screen.x - 12, 0, 24, height);
        }

        // Throw sparks up from the foot (y = 104), within a quarter turn
        // either side of straight up. With speed 64 to 575, a spark starts at
        // sin * speed / 32 pixels a frame across and cos * speed / 64 up (at
        // most 18 and 9), and lives 32 to 39 frames.
        if (frame == start + 2) {
          for (j = 0; j != gBolt_Counts[work->args->variant][BOLT_SPARKS];
               j++) {
            struct Particle *spark = &PARTICLES[i * 128 + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - TRIG_TURN / 4;

            spark->x = screen.x << 16;
            spark->y = 104 << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints for 22 frames: at random spots above the foot, flipped at
      // random, in the variant's blend (gBolt_GlintModes: brightest-wins,
      // or additive for Blue Bolt); the higher a glint, the narrower the band
      // it may fall in across.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != gBolt_Counts[work->args->variant][BOLT_GLINTS]; j++) {
          s32 kind = j & 3;
          s32 drop =
              Random16() % gBolt_Counts[work->args->variant][BOLT_GLINT_SPREAD];
          s32 spread =
              gBolt_Counts[work->args->variant][BOLT_GLINT_SPREAD] - drop + 1;
          s32 y =
              screen.y - drop - (gBattleEffect_GlintCellHeights[kind] >> 1) + 8;
          u32 random = Random16();
          s32 x = screen.x;

          x += random % spread;
          x -= spread / 2;
          x -= (gBattleEffect_GlintCellWidths[kind] >> 1);

          EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                              gBolt_GlintFlips[Random16() & 3] |
                                  EFFECT_CLIP_BOUNDS,
                              gBolt_GlintModes[work->args->variant]);
          BOLT_GLINT_BLITTER(canvas,
                             work->sheet + gBattleEffect_GlintCells[kind], x, y,
                             gBattleEffect_GlintCellWidths[kind],
                             gBattleEffect_GlintCellHeights[kind]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        }
      }
    }

    // Sparks fall at 1/16 pixel a frame per frame, bounce off the ground at
    // half speed and shrink from mote size 3 to 1 as they die out.
    for (j = 0; j != 1024; j++) {
      struct Particle *spark = &PARTICLES[j];

      if (spark->variant > 0) {
        spark->variant--;
        Particle_AdvanceWithGravity2D(spark, 60, Q16_ONE / 16);
        if (spark->y > 104 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, size);
        }
      }
    }
    BattleCamera_Shake(2, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef BOLT_GLINT_BLITTER
#undef BOLT_OFFSETS
