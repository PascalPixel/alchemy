/// @file
/// Bolt.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"

#define BOLT_INTERVAL 8

#define SPARKS_PER_TARGET 128
#define GROUND_Y 104
#define CUE_THUNDER 138

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.

// The second blitter, loaded again for each glint.
#define DRAW_GLINT ((DrawRectangle)Ram_HeapSlots->unkb8[1])

void BattleEffect_RunBolt(struct BattleEffectArgument *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sprites;
  struct EffectPosition pos;
  s32 j;

  // Set up: the canvas layer, the bolt and spark sheets, and every spark
  // and particle free.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + 0xc56, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  for (j = 0; j != SPARK_COUNT; j++)
    SPARKS[j].variant = 0;
  EFFECT_FREE_PARTICLES(j, work->particles, 64);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  Sound_PlayFar(CUE_THUNDER);

  for (frame = 0; frame != args->count * BOLT_INTERVAL + 40; frame++) {
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The sky flashes as each bolt starts.
    for (i = 0; i != args->count; i++) {
      if (frame == i * BOLT_INTERVAL)
        Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(16));
    }

    for (i = 0; i != args->count; i++) {
      s32 start = i * BOLT_INTERVAL;

      EffectPosition_ProjectBattler(args->actors[i], &pos);
      pos.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        BattleEffect_TintBattler(args->actors[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 6);
        BattleMotion_ApplyKnockbackFar(args->actors[i], 6);
      }

      // The bolt, 24 wide, grows 64 pixels a frame down to the ground.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > GROUND_Y)
          height = GROUND_Y;
        for (j = 0; j != gBolt_Counts[args->variant * 4 + 3]; j++) {
          draw[0](canvas,
                  work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
                  pos.x - 12, 0, 24, height);
        }

        // Sparks fly up from the ground within 90 degrees of straight up.
        if (frame == start + 2) {
          for (j = 0; j != gBolt_Counts[args->variant * 4]; j++) {
            struct EffectStep *spark = &SPARKS[i * SPARKS_PER_TARGET + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - TRIG_TURN / 4;

            spark->x = pos.x << 16;
            spark->y = GROUND_Y << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints: at random places above the target's feet, each with a
      // random palette and flip.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != gBolt_Counts[args->variant * 4 + 1]; j++) {
          s32 kind = j & 3;
          s32 drop = Random16() % gBolt_Counts[args->variant * 4 + 2];
          s32 spread = gBolt_Counts[args->variant * 4 + 2] - drop + 1;
          s32 y =
              pos.y - drop - (gBattleEffect_GlintPictureHeights[kind] >> 1) + 8;
          u32 r = Random16();
          s32 x = pos.x;
          u32 flip;

          x += r % spread;
          x -= spread / 2;
          x -= gBattleEffect_GlintPictureWidths[kind] >> 1;
          flip = Random16();

          /* FAKEMATCH(dead): an empty once-only loop. */
          do {
          } while (0);
          BattleEffect_BuildBlitter(188, 7, 7,
                                    gBolt_GlintFlips[flip & 3] |
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                                    gBolt_GlintModes[args->variant]);
          DRAW_GLINT(canvas, work->sheet + gBattleEffect_GlintPictures[kind], x,
                     y, gBattleEffect_GlintPictureWidths[kind],
                     gBattleEffect_GlintPictureHeights[kind]);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        }
      }
    }

    // Sparks fall, bounce off the ground at half speed, and shrink as they
    // burn out.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark = &SPARKS[j];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > GROUND_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < px(127)) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                  x - size / 2, y - size, size, size * 2);
        }
      }
    }
    BattleCamera_Shake(2, 8);
    EFFECT_END_FRAME(work);
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
