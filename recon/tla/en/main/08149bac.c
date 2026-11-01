// Draft, differ 14 of 783, skeleton gap 7 (tla-en, 2026-11-01)
// BattleEffect_RunSparkGroups, the body of the spark-group effects through
// BattleEffect_RunNova (kind 0), BattleEffect_RunDynamite (kind 1) and
// BattleEffect_RunThrash (kind 2).

// ☀️ twin: BattleEffect_RunSparkGroups
// (games/tbs/src/battle_effect/battle_effect/plasma.c), confirmed statement by
// statement. ⚓️'s changes: kind 1 also starts a palette fade (work->fadeFrames
// 24, work->fadeStep 0); the blitter pair comes from
// BattleEffect_FetchRectangleBlitters(0, draw) where ☀️ loads max and additive
// blitters itself; the effect runs count * 8 + 40 frames (☀️ + 56); kinds 1
// and 2 play cue 145 with phase 145 at frame 2; the variant 3 wipe goes through
// the IWRAM call layer; the transfer callback runs at 0xc80.

// Left: instruction order only, no device, every register agrees. Two blocks:
// - the end of the set-up group loop: the listing has ldr i, movs 224, ...,
//   adds i, lsls row, str giv, str i, adds row; here the row runs ahead of the
//   stores (10 instructions);
// - the spark's draw call: the listing stores size to [sp, #0] before
//   [sp, #4], here after the reload of &draw (4 instructions).
// Both are sched2 only. --rtl shows the stores' priority too low by the same
// amount: with a store then a load of memory costing a full load (3), as
// loads in the same blocks do, the sched2 ranking gives the listing's order in
// both blocks. That is the load-after-store model of -mthumb-slow-loads (K2),
// a compiler question for Pascal, not something this source can change.

// What closed the rest (differ 386 to 14), each a plain form:
// - `spark` and `point` are one variable per loop, not shared ones: the two
//   loops of each hold them in different registers;
// - `sparks`, set once from gMapCellBuffer, gets no register, so reload
//   rematerializes it at each use and that moves the rotation of reload
//   registers (the listing's pool load into r5 in the second spark loop);
// - both flash loops index particles[i * FLASH_POINTS + j], the second one
//   keeping i * 16 as a giv through the frame.

// Tried, no gain (differ 14 unless noted):
// asm("") at the end of the set-up group body; a one-pass block there;
// asm("") before the spark draw (16); a height local for size * 2 (203);
// s32 for the spark's x and y, either or both, in either order; a spelled-out
// size++; &graphics[...]; speed in two statements (258); sparks set at the top
// of the function; ☀️'s &sparks[...] + j spelling (same bytes as sparks + ...).
// A barrier cannot give the listing's order: the row's first shift sits
// between the increment and the two stores, on both sides of any barrier.

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "fixed_math.h"
#include "resource.h"

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

#define FLASH_POINTS 16 // flash points kept for each group
#define FLASH_DRAWN 12  // of which the first 12 are drawn
#define FLASH_FRAMES 18 // six cels of three frames
#define GROUND_Y (108 << 16)

// The kinds of spark group, chosen by the effect's entry.
enum {
  SPARKS_FIXED,     // from a fixed point, canvas at priority 1
  SPARKS_ORANGE,    // from a lower point, orange, with a palette fade
  SPARKS_FROM_ACTOR // from the caster, lightning colours, sparks rising
};

// The camera work, as far as this effect reads it.
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

extern char ResourceId_FlashBurstSheet;    // directory row 0x13e
extern char ResourceId_ParticleSpritesA;   // directory row 0x134
extern char ResourceId_OrangePaletteB;     // directory row 0x148
extern char ResourceId_LightningBoltSheet; // directory row 0x188

// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];
extern u8 Data_08197a1a[];   // ☀️ gPlasma_FlashCells
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

// The sparks live in the map cell buffer.
extern struct EffectStep gMapCellBuffer[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);                  // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
void BattleEffect_FetchRectangleBlitters(
    s32 side, DrawRectangle *draw); // ☀️ FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Sound_PlayFar(s32 cue);

#define SHAPE(n) Data_081979de[effect->variant][n]
#define SPARK_COUNT SHAPE(0)
#define GROUP_COUNT SHAPE(1)
#define GROUP_X(i) SHAPE((i) + 2)

// One to three groups of sparks burst one after another, eight frames
// apart: each opens with a big flash, a ring of small flashes and a spray
// of sparks that fall (or, from the caster, rise) and bounce; six frames
// after each burst the targets flash and recoil.
void BattleEffect_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct EffectStep *sparks;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];

  // The burst's origin on the canvas.
  if (kind == SPARKS_FIXED) {
    BattleEffect_StartCanvasLayer(1);
    originX = 60;
    originY = 48;
  } else if (kind == SPARKS_ORANGE) {
    BattleEffect_StartCanvasLayer(0);
    originX = 60;
    originY = 64;
    work->fadeFrames = 24;
    work->fadeStep = 0;
  } else {
    BattleEffect_StartCanvasLayer(0);
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    originX = pos.x / 2;
    originY = pos.y + 48;
  }

  // The flash sheet, the particle sprites and the kind's palette.
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (kind == SPARKS_ORANGE) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (kind == SPARKS_FROM_ACTOR) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  }

  sparks = gMapCellBuffer;

  for (i = 0; i != GROUP_COUNT; i++) {
    // The group's ring of flash points, wider and later further out.
    for (j = 0; j != FLASH_POINTS; j++) {
      struct EffectStep *point;
      s32 radius;
      s32 angle;

      point = &work->particles[i * FLASH_POINTS + j];
      radius = j * 2;
      angle = Random16() & 0xffff;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }

    // Its sparks, thrown in every direction from the group's centre.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark;
      s32 speed;
      s32 angle;

      spark = sparks + i * SPARK_COUNT + j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & 0xffff;
      if (effect->side == 1)
        spark->x = (originX - GROUP_X(i) + 28) << 16;
      else
        spark->x = (originX + GROUP_X(i) - 28) << 16;
      spark->y = originY << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != GROUP_COUNT * 8 + 40; frame++) {
    struct CameraWork *camera = Ram_HeapSlots->cameraWork;

    // The strongest variant pans the camera; the next one wipes the canvas
    // at frame 4.
    if (effect->variant == 2 && frame <= 51) {
      if (effect->side == 0)
        camera->scroll += 256;
      else
        camera->scroll -= 256;
    }
    if (effect->variant == 3 && frame == 4)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);

    if (kind == SPARKS_ORANGE || kind == SPARKS_FROM_ACTOR) {
      if (frame == 2) {
        Sound_PlayFar(145);
        Func_081180e8(145);
      }
    } else {
      if (frame == 2)
        Sound_PlayFar(145);
      if (frame == 24)
        Func_081180e8(134);
    }

    for (i = 0; i != GROUP_COUNT; i++) {
      start = i * 8;
      if (frame == start)
        work->shakeFrames = 12;

      // The big flash, for two frames.
      if (frame >= start && frame < start + 2) {
        if (effect->side == 1)
          draw[0](canvas, work->sheet, originX - GROUP_X(i) + 12, originY - 32,
                  32, 64);
        else
          draw[0](canvas, work->sheet, originX + GROUP_X(i) - 44, originY - 32,
                  32, 64);
      }

      // The ring of small flashes, each counting down through six cels.
      if (frame >= start) {
        for (j = 0; j != FLASH_DRAWN; j++) {
          struct EffectStep *point;
          s32 x;
          s32 y;

          point = &work->particles[i * FLASH_POINTS + j];
          y = HI(point->y) + originY;
          if (effect->side == 1)
            x = HI(point->x) + originX - GROUP_X(i) + 28;
          else
            x = HI(point->x) + originX + GROUP_X(i) - 28;
          if (point->variant >= 0 && point->variant < FLASH_FRAMES)
            draw[0](canvas,
                    work->sheet + (Data_08197a1a[point->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }

      // From the sixth frame the sparks fly, bounce off the ground at half
      // speed, and shrink as they die out.
      if (frame > start + 5) {
        s32 gravity;

        if (kind == SPARKS_FROM_ACTOR)
          gravity = -0x1000;
        else
          gravity = 0x1000;
        for (j = 0; j != SPARK_COUNT; j++) {
          struct EffectStep *spark = sparks + i * SPARK_COUNT + j;
          if (spark->variant > 0) {
            BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
            spark->variant--;
            if (spark->y > GROUND_Y) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
              s16 y = spark->y >> 16;
              s16 x = spark->x >> 16;
              s32 size = spark->variant / 5 + 1;

              draw[j & 1](canvas, graphics + gData_08197410[size - 1],
                          x - size / 2, y - size, size, size * 2);
            }
          }
        }
      }

      // The targets flash and recoil six frames after the burst.
      for (j = 0; j != effect->count; j++) {
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[j], 7, 5, j, 10);
          Func_08118088(effect->actors[j], 4);
        }
      }
    }

    Func_08158ce0(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
