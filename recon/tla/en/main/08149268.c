// Near miss, differ 46 of 503 (effect kind 8, the falling crystals).

// ☀️ twin: BattleEffect_RunPrism (games/tbs/src/effect/effect.c),
// shape 0.800, structure 0.979. ⚓️'s changes: the counts table
// Data_081979b7 holds three bytes for each variant (crystals, frames and the
// frame that begins phase 134, where ☀️ begins it 80 frames before the
// end); the boulder sheet is unpacked into the work before the crystal
// sheet; the shard offsets gData_08197834 are words; the camera is
// Ram_HeapSlots->cameraWork; BattleEffect_FetchRectangleBlitters fetches the
// blitter pair.

// Same frame, instruction count and registers. The crystals are seeded
// with a counter of their own (k), which gives it r8 and the two masks r10
// and r11, as natively (98 -> 46). What is left: the native sets the first
// half of the work's transfer offset (movs r7, #239) before
// BattleEffect_StartCanvasLayer and keeps it across the setup calls, where
// alchemy-gcc builds it at its use, which moves the first literal pool and
// every pc offset across it; the rest are single rows where the native
// issues a load one slot earlier.

// Tried (differ):
// trial 1's port (effects trial, 2026-10-15)                        104
// alchemy permute, 5 minutes: cry->y read into a volatile long gives 61
// but adds an instruction (504), so it cannot reach 0; not kept
// the shared work struct and Ram_MapCellBuffer for the shards       104
// greedy declaration-order scan, every local through every place   104
// a separate counter k for the shard draw loop (414) or the actor
// loop (158); j declared before i (104)
// the seeding loop through i (64) or its own counter k (46, kept)

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "task.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

#define SHARDS_PER_CRYSTAL 16
#define GROUND_Y (56 << 16)
#define SHATTERED 1 // a crystal's z once it has hit the ground

extern char ResourceId_BoulderSheet; // directory row 0x14d
extern char ResourceId_CrystalSheet; // directory row 0x193

// ☀️ gPrism_Counts, with a third byte: crystals, frames and the frame
// that begins phase 134, for each variant.
extern u8 Data_081979b7[];
extern u32 gData_08197834[]; // ☀️ gPrism_ShardOffsets
extern u8 gData_0819781a[];  // ☀️ gPrism_ShardWidths
extern u8 gData_08197826[];  // ☀️ gPrism_ShardHeights
extern u8 gData_081977f8[];  // ☀️ gPrism_ShardStarts: x, y for each shard

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);         // ☀️ BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);         // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);         // ☀️ BattleEffect_CountDownBattlerTints
void Func_08158ce0(s32 x, s32 y); // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);    // ☀️ BattleEvent_StartPhaseFar
void BattleEffect_FetchRectangleBlitters(
    s32 side, DrawRectangle *draw); // ☀️ FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ Actor_UpdateMembers
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Sound_PlayFar(s32 cue);

// Crystals fall one after another, eight frames apart, onto the targets'
// ground and shatter into sixteen spinning shards each; the screen darkens
// over the last eight frames.
void BattleEffect_RunPrism(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct EffectStep *seed;
  struct EffectStep *cry;
  struct EffectStep *shards;
  struct EffectStep *shard;
  s32 x;
  s32 cell;
  s32 i;
  s32 j;
  s32 k;

  // Set up: the canvas layer, the two sheets and the blitter pair.
  shards = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The crystals wait above the screen, each eight frames behind the last,
  // and drift sideways so that they land over the targets.
  for (k = 0; k != Data_081979b7[effect->variant * 3]; k++) {
    seed = &work->particles[k];
    x = Random16() & 31;
    seed->y = -0x400000;
    if (effect->side == 1) {
      x = ((Random16() & 31) + 80) << 16;
      seed->velocityX = (Random16() & 63) << 12;
    } else {
      x = ((Random16() & 31) + 8) << 16;
      seed->velocityX = -(Random16() & 63) << 12;
    }
    seed->x = x - seed->velocityX * 18;
    seed->velocityY = 0;
    seed->z = 0;
    seed->variant = k * 8;
  }

  for (frame = 0; frame != Data_081979b7[effect->variant * 3 + 1]; frame++) {
    // The strongest variant turns the camera, slowing from frame 96.
    if (effect->variant == 2 && frame <= 103) {
      struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 95)
        speed = 192;
      else
        speed = 2496 - frame * 24;
      if (effect->side == 0)
        camera->yaw -= speed;
      else
        camera->yaw += speed;
    }
    if (frame == Data_081979b7[effect->variant * 3 + 2])
      Func_081180e8(134);

    // Darken over the last eight frames.
    if (frame == Data_081979b7[effect->variant * 3 + 1] - 8) {
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }

    if (frame <= Data_081979b7[effect->variant * 3 + 1] - 8) {
      for (i = 0; i != Data_081979b7[effect->variant * 3]; i++) {
        cry = &work->particles[i];
        if (cry->z == SHATTERED) {
          // A shattered crystal: its shards fly and spin, slowing down.
          for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
            cell = (j % 5) * 3 +
                   (shards[i * SHARDS_PER_CRYSTAL + j].variant / 96) % 3;
            shard = &shards[i * SHARDS_PER_CRYSTAL + j];
            draw[j <= 2](canvas, work->sheet + gData_08197834[cell] + 0x800,
                         HI(shard->x) - gData_0819781a[cell] / 2,
                         HI(shard->y) - gData_08197826[cell] / 2,
                         gData_0819781a[cell], gData_08197826[cell]);
            BattleEffectKernels_IntegrateVector2(shard, 64, 0x2000);
            shard->variant += shard->z;
            if (shard->z > 1 && (frame & 1))
              shard->z--;
          }
        } else if (frame >= cry->variant) {
          // A falling crystal (32x64), until it reaches the ground.
          draw[i & 1](canvas, work->sheet, HI(cry->x) - 16, HI(cry->y), 32, 64);
          BattleEffectKernels_IntegrateVector2(cry, 64, 0x10000);
          if (cry->y > GROUND_Y) {
            cry->z = SHATTERED;
            cry->y = GROUND_Y;
            for (j = 0; j != SHARDS_PER_CRYSTAL; j++) {
              shard = &shards[i * SHARDS_PER_CRYSTAL + j];
              shard->x = ((gData_081977f8[j * 2] - 40) << 16) + cry->x;
              shard->y = gData_081977f8[j * 2 + 1] << 16;
              shard->velocityX = ((Random16() & 127) - 64) << 11;
              shard->velocityY = -(Random16() & 127) << 11;
              if (i & 1) {
                shard->velocityX *= 2;
                shard->velocityY *= 2;
              }
              shard->z = 32;
              shard->variant = 0;
            }
            work->shakeFrames = 8;
            Sound_PlayFar(144);
            for (j = 0; j != effect->count; j++)
              Func_0814cd48(effect->actors[j], 7, 5, j, 4);
          }
        }
      }
    }

    Func_08158ce0(effect->variant * 2 + 4, effect->variant * 4 + 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
