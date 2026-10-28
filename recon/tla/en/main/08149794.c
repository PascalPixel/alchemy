// Near miss, differ 42 of 457 (effect kind 18, the lightning pillars).

// ☀️ twin: BattleEffect_RunPlasma (games/tbs/src/effect/effect.c),
// shape 0.846, structure 1.000. ⚓️'s changes: blitters from
// BattleEffect_LoadBlitter(104, 0x03) (additive, ⚓️ mode 0) and (188, 0x17) (max,
// mirrored); the effect runs count * 7 + 42 frames where ☀️ runs
// count * 7 + 48; phase 134 begins at frame 24 where ☀️ begins it at 32;
// the pillar's cue is 134; Func_08118088 moves the struck targets.

// Same frame and instruction count; what is left is single-row order: the
// native builds the sheet offset 0x700 (movs r7, #224; lsls) before
// BattleEffect_StartCanvasLayer where agscc builds it after, the argument
// registers of the two BattleEffect_LoadBlitter calls in the other order, and pool
// offsets that move with them.

// Tried (differ):
// trial 1's port (effects trial, 2026-10-15)                          42
// the heap slots as Ram_HeapSlots fields in place of heap[23..47]
// with BattleEffect_LoadBlitter returning void                                 46
// the same with BattleEffect_LoadBlitter returning s32 (plan 0c)                 42
// the map cell buffer as the gMapCellBuffer symbol or as
// Ram_MapCellBuffer                                                 42
// greedy declaration-order scan, every local through every place     42
// alchemy permute, 5 minutes: 40 by holding the IWRAM copy entry in
// a local before the call, a forced temporary against the IWRAM
// call layer; not kept
// the BLDALPHA halfword pinned to r3 (one-pass block or barrier)    80
// (the sheet offset 0x700 then splits across the first call)

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"
#include "resource.h"

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define SPARK_MAX 1024
#define PILLAR_BASE_Y 104

// The camera work, as far as this effect reads it.
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

extern char ResourceId_LightningPillarSheet; // directory row 0x194
extern char ResourceId_ParticleSpritesA;
extern char ResourceId_VioletPaletteB; // directory row 0x122

extern u8 Data_081979da[];   // ☀️ gPlasma_Counts
extern u8 Data_081979cc[];   // ☀️ gPlasma_Columns, 7 for each side
extern u16 Data_081979c0[];  // ☀️ gPlasma_Sparks: sparks, shake
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);                  // ☀️ BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);                  // ☀️ ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void);                  // ☀️ ObjectGroup_CountDownMemberTimers
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);    // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Sound_PlayFar(s32 cue);

// Pillars of lightning strike the targets' row one after another, eight
// frames apart; each flashes the screen, throws a fan of sparks up from its
// base and shakes the camera, and the sparks bounce on the ground.
void BattleEffect_RunPlasma(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *variant;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  // Set up: the canvas layer, an additive blitter and a mirrored max one,
  // the pillar sheet and the particle sprites, and a violet palette for all
  // but the strongest variant.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER_ADDITIVE << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                            COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  if (effect->variant != 2) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Every spark starts at rest.
  variant = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
  for (k = 0; k != SPARK_MAX; k++) {
    *variant = 0;
    variant += 7;
  }
  sparks = (struct EffectStep *)Ram_MapCellBuffer;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  count = Data_081979da[effect->variant];

  for (frame = 0; frame != count * 8 - count + 42; frame++) {
    // The strongest variant pans the camera over the first 64 frames.
    if (effect->variant == 2 && frame <= 63) {
      struct CameraWork *camera = Ram_HeapSlots->cameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (effect->side == 1)
        camera->scroll -= speed;
      else
        camera->scroll += speed;
    }
    if (frame == 24)
      Func_081180e8(134);

    // Pillar i strikes at frame 8i: a flash, then its first cel for one
    // frame and its second for two.
    for (i = 0; i != count; i++) {
      start = i * 8;
      if (frame == start) {
        Sound_PlayFar(134);
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work->sheet, Data_081979cc[i + effect->side * 7] - 24,
                  0, 48, 112);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + 0x1500,
                  Data_081979cc[i + effect->side * 7] - 24, 0, 48, 112);

        // Throw sparks up from the pillar's base, within 90 degrees of
        // straight up, and shake the camera.
        if (frame == start + 2) {
          s32 thrown = 0;

          for (k = 0; k != SPARK_MAX; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;

              spark->x = Data_081979cc[i + effect->side * 7] << 16;
              spark->y = PILLAR_BASE_Y << 16;
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              thrown++;
              if (thrown == Data_081979c0[effect->variant * 2])
                break;
            }
          }
          work->shakeFrames = Data_081979c0[effect->variant * 2 + 1];
        }
      }

      // Four frames after each strike the targets recoil and flash.
      if (frame == start + 4) {
        for (k = 0; k != effect->count; k++) {
          Func_08118088(effect->actors[k], 1);
          Func_0814cd48(effect->actors[k], 7, 5, k, 8);
        }
      }
    }

    // The sparks fall and bounce at half speed off the ground, drawn
    // smaller as they die out.
    for (k = 0; k != SPARK_MAX; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > PILLAR_BASE_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = spark->x >> 16;
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = spark->y >> 16;
            size = spark->variant / 8 + 1;
            draw[k & 1](canvas, sprites + gData_08197410[size - 1],
                        x - size / 2, y - size, size, size * 2);
          }
        }
      }
    }

    Func_08158ce0(8, 16);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
