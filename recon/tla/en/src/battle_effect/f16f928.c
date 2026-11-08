#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "task.h"
#include "resource.h"
#include "heap_release.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The map cell buffer holds 1,024 more particles while an effect runs.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list; the packed loader argument is mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define STREAKS 64

// The splash: six 24x48 pictures in the work sheet after the streak.
#define SPLASH_OFFSET 0x3200
#define SPLASH_SIZE (24 * 48)

extern char ResourceId_CyanPalette;
extern char ResourceId_SmokeSheet;

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Palette_StepFadeTransfer(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 blitter); // BattleEffect_BuildBlitter, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 mask, u32 range);   // Camera_ApplyShake
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleMotion_ApproachTargetFar(s32 actor, s16 target, s32 frames,
                                    s32 speed);
void EffectPosition_ProjectBattler(s32 actor, struct EffectPosition *position);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void SummonEffect_RunCanvasMode3(struct BattleEffectArgument *effect);
void Sound_PlayFar(s32 cue);

// Hurricane: driving rain slants down over the target, each drop bursting
// into a splash where it lands, while the target is struck eight times; the
// user rushes in at frame 32. The canvas then hands over to the summon
// canvas effect in mode 3.
//
// Setup: canvas layer 1 at half blend and full width, centred on the
// target; one additive blitter; the cyan palette; a 4x64 streak drawn into
// the work sheet as a slanted line, leaning with the wind; the splash
// pictures after it.
// Frame loop (68 frames): 64 streaks start above the screen at frame 0 and
// fall in turn, one more each frame; a streak that falls below line 48
// starts its splash.
void BattleEffect_RunHurricane(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2];
  struct EffectPosition position;
  struct EffectStep *streak;
  struct EffectStep *splash;
  s32 size;
  s32 frame;
  s32 i;

  size = 0x3c00;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      104, BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Iwram_CopyWords((void *)BG_PLTT,
                  ValueLr1((s32 (*)())Resource_GetTableEntry, (s32)((s32)&ResourceId_CyanPalette)), 128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EffectPosition_ProjectBattler(effect->actors[0], &position);
  {
    /* FAKEMATCH(temporary): x computed before the register's address is built;
     * as REG_BG2X = (64 - position.x) << 8, 10 of 376 instructions differ in
     * tla-en. */
    s32 x = 64 - position.x;

    REG_BG2X = x << 8;
  }

  // The streak: one pixel of colour 24 a row, stepping a pixel sideways
  // every 16 rows against the wind.
  Iwram_ClearWords(work->sheet, 256);
  for (i = 0; i != STREAKS; i++) {
    if (effect->side == 0)
      work->sheet[i * 4 - i / 16 + 3] = 24;
    else
      work->sheet[i * 4 + i / 16] = 24;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SPLASH_OFFSET, 1, 0);
  REG_BG2PA = 0x100;

  for (frame = 0; frame != 68; frame++) {
    // Frame 0: the streaks wait above the screen, drifting with the wind.
    if (frame == 0) {
      Sound_PlayFar(170);
      for (i = 0; i != 256; i++)
        PARTICLES[i].variant = -1;
      for (i = 0; i != STREAKS; i++) {
        streak = &work->particles[i];
        streak->y = ((Random16() & 31) - 64) << 16;
        if (effect->side == 0) {
          streak->x = ((Random16() & 63) + 40) << 16;
          streak->velocityX = -(1 << 16);
        } else {
          streak->x = ((Random16() & 63) + 24) << 16;
          streak->velocityX = 1 << 16;
        }
        streak->velocityY = 16 << 16;
        streak->variant = -1;
      }
    }
    if (frame == 32)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8,
                                     3 << 16);
    if (frame == 46) {
      Func_081180e8(134);
      Func_08118088(effect->actors[0], 4);
    }

    // Each splash rises and spreads through its six pictures, four frames
    // apiece.
    splash = PARTICLES;
    for (i = 0; i != 256; i++, splash++) {
      if (splash->variant >= 0 && splash->variant < 24) {
        blit[0](canvas,
                work->sheet + SPLASH_OFFSET + splash->variant / 4 * SPLASH_SIZE,
                HI(splash->x) - 12, HI(splash->y) - 32, 24, 48);
        BattleEffectKernels_IntegrateVector2(splash, 56, -0x4000);
        splash->variant++;
      }
    }

    // The target is struck every four frames from frame 6.
    for (i = 0; i != 8; i++) {
      if (frame == i * 4 + 6) {
        Func_0814cd48(effect->actors[0], 7, 5, 0, 3);
        Sound_PlayFar(132);
      }
    }

    // Streak i falls from frame i, shortened as it reaches the ground at
    // line 112; below line 48 it starts splash i.
    for (i = 0; i != 32; i++) {
      s32 y;
      s32 height;
      s32 x;

      streak = &work->particles[i];
      if (frame < i)
        continue;
      y = HI(streak->y);
      if (y > 111)
        continue;
      height = 64;
      if (y > 48)
        height = 112 - y;
      blit[0](canvas, work->sheet, HI(streak->x), y, 4, height);
      x = streak->x + streak->velocityX;
      streak->x = x;
      streak->y += streak->velocityY;
      if (streak->y >> 16 > 48) {
        /* FAKEMATCH(temporary): landing set to the buffer, then advanced; as
         * &PARTICLES[i], 162 of 376 instructions differ in tla-en. */
        struct EffectStep *landing = PARTICLES;

        landing += i;
        if (landing->variant == -1) {
          landing->y = 112 << 16;
          landing->variant = 0;
          landing->x = x;
          landing->velocityX = (Random16() % 12 - 6) << 15;
          landing->velocityY = (-Random16() & 7) << 10;
        }
      }
    }

    Func_08158ce0(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Clear the canvas and its characters and hand the layer over.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  /* FAKEMATCH(barrier): the clear routine's load kept after HeapSlot_Release;
   * as plain calls with no barrier, 2 of 376 instructions differ in tla-en. */
  __asm__("");
  Iwram_ClearWords((void *)0x06004000, size);
  Iwram_ClearWords(canvas, size);
  Task_Remove(Palette_StepFadeTransfer);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  ValueLr1((s32 (*)())SummonEffect_RunCanvasMode3, (s32)(effect));
}
