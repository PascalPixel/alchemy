// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 6 of
// 467: the native issues the work pointer's copy to r9 before the third heap
// load, and in both bursts adds the speed's 32 between the x store and the y
// load. An empty asm reading the speed after the x store disturbs the whole
// allocation (227). A greedy device search found nothing lower.
// Draft of BattleEffect_RunPlumeEdge, effect kind 85 (Plume Edge), written from the
// listing on The Broken Seal's canvas skeleton. No close twin (shape
// BattleEffect_RunImpactBurst 0.566, call order BattleEffect_RunJupiter
// 0.677); the pillar's two-piece scroll is BattleEffect_RunNereid'.

// Listing 467 instructions, draft 467, differ 36.
// Left: order rows only. The native moves the work pointer to r9 before
// it loads the sheet slot, starts REG_BLDALPHA's address before loading
// its value, adds the 32 to each spark's speed before it loads the
// position's y (both bursts), and sets r1 (32) after the flash's y; it
// also places the first literal pool straight after the
// GetBattleObjectSlotFar call, one instruction earlier than here, which
// moves every pool offset after it (about 20 rows).
// Tried (differ):
// first draft                                                        94
// locals declared in the order of the native's stack slots
// (canvas, frame, sheet, target, pillarY)                          60
// work read before canvas                                            52
// HI as *(s16 *)((u8 *)&v + 2), (s32)((s16 *)&v)[1], a struct of
// two halves                                                       52
// the cell table through a local pointer before the spark loop       51
// the spark loop stepped by pointer (step++): loop.c then hoists
// the cell table into r10 as the native does                       36
// HI through an inline helper (BattleEffect_RunStoneJustice's device)               135
// alchemy permute 8 minutes, 2 jobs: 25 only with a forced
// temporary for the FlashBurstSheet id taken before the pillar
// load, 33 with an unsigned char picture index (a false type);
// neither kept

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "task.h"
#include "resource.h"
#include "mem.h"
#include "motobj.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The map cell buffer holds 1,024 more particles while an effect runs.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_Data0DA;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_Data134;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_FlashBurstSheet;

// Spark cells of the Data134 sheet, by size - 1 (byte offsets).
extern u16 gData_08197410[];

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);            // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames, s32 percent);
void BattleMotion_ApproachTargetFar(s32 actor, s16 target, s32 frames,
                                    s32 speed);
void EffectPosition_ProjectBattler(
    s32 actor, struct EffectPosition *position);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Sound_PlayFar(s32 cue);

// Plume Edge: the user dashes at the target, which a fan of white flashes
// throws back; a pillar of light scrolls up through it for half a second and
// then bursts into 256 sparks with a white flash of the whole canvas.
//
// Setup: canvas layer 0 at half blend, the two rectangle blitters, the
// pillar and flash sheets in the work sheet, the spark sheet in the shared
// sheet and the light palette over the background's.
// Frame loop (80 frames): the fan of 64 flashes at frame 0, the user's dash
// at frames 18 and 21, the spark burst at frame 32, then the sparks fall.
void BattleEffect_RunPlumeEdge(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  struct MotionObject *target;
  s32 pillarY;
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectStep *step;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  pillarY = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4710, 0,
                             0);
  Iwram_CopyWords(
      (void *)BG_PLTT,
      Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x2000, 1, 0);
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;

  for (frame = 0; frame != 80; frame++) {
    EffectPosition_ProjectBattler(effect->actors[0], &position);

    // Frame 0: 64 flashes fan out upward from the target, one more each
    // frame, and the target turns white and stops bouncing.
    if (frame == 0) {
      for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = (Random16() & 0x7fff) - 0x4000;

        step = &work->particles[i];
        step->x = (position.x / 2) << 16;
        step->y = position.y << 16;
        step->velocityX = (Trig_Sin(angle) * speed) >> 8;
        step->velocityY = -(Trig_Cos(angle) * speed * 2) >> 8;
        step->variant = -i;
      }
      target->verticalMotionStrength = 0;
      target->y = 0;
      pillarY = position.y;
      Func_0814cd48(effect->actors[0], -1, 5, 0, -1);
      work->shakeFrames = 4;
      Sound_PlayFar(144);
    }

    // Frame 32: the pillar bursts into 256 sparks, the target is knocked
    // back and flashes, and the whole canvas flashes white.
    if (frame == 32) {
      for (i = 0; i != 256; i++) {
        s32 speed = (Random16() & 0x3ff) + 32;
        s32 angle = Random16();

        step = &PARTICLES[256 + i];
        step->x = (position.x / 2) << 16;
        step->y = (position.y - 16) << 16;
        step->velocityX = (Trig_Sin(angle) * speed) >> 7;
        step->velocityY = -(Trig_Cos(angle) * speed * 2) >> 7;
        step->variant = (Random16() & 7) + 48;
      }
      Func_08118088(effect->actors[0], 4);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      work->shakeFrames = 8;
      Func_081180e8(145);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }

    // The target is pushed down 2 pixels a frame for the first 24 frames,
    // while the user closes in on it.
    if (frame >= 0 && frame < 24)
      target->y += 2 << 16;
    if (frame == 18)
      BattleEffect_SetApproachMotion(effect->actor, effect->actors[0], 8, 80);
    if (frame == 21)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 12,
                                     8 << 16);

    // Sparks fall and shrink: a spark is drawn at its life / 6 pixels wide,
    // two pixels at the least.
    step = PARTICLES;
    for (i = 0; i != 512; i++, step++) {
      if (step->variant >= 0) {
        s32 size = step->variant / 6;

        if (size < 2)
          size = 2;
        step->variant--;
        BattleEffectKernels_IntegrateVector2(step, 60, 0x2000);
        draw[0](canvas, sheet + gData_08197410[size - 1],
                HI(step->x) - size / 2, HI(step->y) - size, size, size * 2);
      }
    }

    // Each flash rises for 18 frames through the six 32x64 pictures of the
    // flash sheet, three frames apiece.
    for (i = 0; i != 32; i++) {
      step = &work->particles[i];
      step->variant++;
      if (step->variant >= 0 && step->variant < 18) {
        s32 picture = step->variant / 3;

        BattleEffectKernels_IntegrateVector2(step, 60, -0x2000);
        draw[0](canvas, work->sheet + 0x2000 + picture * 0x800,
                HI(step->x) - 16, HI(step->y) - 32, 32, 64);
      }
    }

    // The pillar scrolls up 8 pixels a frame over the target, drawn in
    // two pieces of its 104-pixel picture.
    if (frame >= 0 && frame < 32) {
      s32 scroll = (frame * 8) % 104;

      draw[0](canvas, work->sheet, position.x / 2 - 17, pillarY - scroll - 96,
              34, 104);
      draw[0](canvas, work->sheet, position.x / 2 - 17, pillarY - scroll + 8,
              34, scroll);
    }

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(104);
  HeapSlot_Release(188);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
