/// @file
/// Bolt.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define BOLT_INTERVAL 8

#define SPARKS_PER_TARGET 128
#define GROUND_Y 104
#define CUE_THUNDER 138

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.
#define BLITTER_MAX (1 << 4)

// The second blitter, loaded again for each glint.
#define DRAW_GLINT ((DrawRectangle)Ram_HeapSlots->unkb8[1])

void BattleEffect_RunBolt(struct BattleEffectArgument *effect) {
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
  BattleEffect_LoadBlitter(104,
                           BLITTER_MAX | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + 0xc56, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  for (j = 0; j != SPARK_COUNT; j++)
    SPARKS[j].variant = 0;
  for (j = 0; j != 64; j++)
    work->particles[j].variant = -1;
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(CUE_THUNDER);

  for (frame = 0; frame != effect->count * BOLT_INTERVAL + 40; frame++) {
    if (frame == 24)
      BattlePlayback_StartFar(PHASE_RESULTS);

    // The sky flashes as each bolt starts.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * BOLT_INTERVAL)
        Iwram_FillWordsEntry(canvas, 0x4000, 0x10101010);
    }

    for (i = 0; i != effect->count; i++) {
      s32 start = i * BOLT_INTERVAL;

      EffectPosition_ProjectBattler(effect->actors[i], &pos);
      pos.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 6);
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 6);
      }

      // The bolt, 24 wide, grows 64 pixels a frame down to the ground.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > GROUND_Y)
          height = GROUND_Y;
        for (j = 0; j != gBolt_Counts[effect->variant * 4 + 3]; j++) {
          draw[0](canvas,
                  work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
                  pos.x - 12, 0, 24, height);
        }

        // Sparks fly up from the ground within 90 degrees of straight up.
        if (frame == start + 2) {
          for (j = 0; j != gBolt_Counts[effect->variant * 4]; j++) {
            struct EffectStep *spark = &SPARKS[i * SPARKS_PER_TARGET + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - 0x4000;

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
        for (j = 0; j != gBolt_Counts[effect->variant * 4 + 1]; j++) {
          s32 kind = j & 3;
          s32 drop = Random16() % gBolt_Counts[effect->variant * 4 + 2];
          s32 spread = gBolt_Counts[effect->variant * 4 + 2] - drop + 1;
          s32 y = pos.y - drop - (gData_08197498[kind] >> 1) + 8;
          u32 r = Random16();
          s32 x = pos.x;
          u32 flip;

          x += r % spread;
          x -= spread / 2;
          x -= gData_08197492[kind] >> 1;
          flip = Random16();

          // An empty once-only loop ends CSE's block, so the two reads of each
          // size table's address stay apart; without it, 172 of 552
          // instructions differ in tla-en.
          do {
          } while (0);
          BattleEffect_BuildBlitter(188, 7, 7,
                                    gBolt_GlintFlips[flip & 3] |
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                                    gBolt_GlintModes[effect->variant]);
          DRAW_GLINT(canvas, work->sheet + gBattleEffect_GlintCells[kind], x, y,
                     gData_08197492[kind], gData_08197498[kind]);
          HeapSlot_Release(188);
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
        } else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          draw[0](canvas, sprites + gBattleEffect_MoteCells[size - 1],
                  x - size / 2, y - size, size, size * 2);
        }
      }
    }
    BattleCamera_Shake(2, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
