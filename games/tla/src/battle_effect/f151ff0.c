#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"
#include "effect_step.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);       // BattleEffect_TintBattler
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_08158ce0(s32 x, s32 y);          // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus
// The Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and
// COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

// A row of Data_081983a2, one for each variant.
enum { FIELD_MOTES, FIELD_PUFFS, FIELD_FRAMES };

#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)
#define MOTE_SHEET 0x3200 // the mote pictures in the sheet

extern char ResourceId_BlastSheet;
extern char ResourceId_FireStreakSheet;
extern u8 Data_081983a2[][3]; // gEruption_Counts
extern u16 gData_08197410[];  // gBattleEffect_MoteCells
extern u16 Data_081983ac[];   // gEruption_PuffCells
extern u16 Data_081983ba[];   // gEruption_PuffSizes

// A blast of motes and smoke puffs bursts from the near or far edge of the
// field toward the targets.

// Setup: mode 1 builds a grey ramp in the first 64 background colors and
// always plays variant 1; mode 0 copies the fire palette and plays the
// effect's variant. The puffs and motes all start at the side's edge with
// random velocities. Frame loop: the motes fly and fall under gravity,
// sized by depth; the puffs set out one a frame from frame 3 and grow
// through seven pictures. The targets are hit from frame 6, a frame apart;
// in mode 0 they also recoil. The variant chooses the number of motes and
// puffs and the length.
void Func_08151ff0(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  s32 variant;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  if (mode == 1) {
    for (i = 0; i != 64; i++) {
      s32 level = i / 2;

      BG_PLTT[i] = (level << 10) | (level << 5) | level;
    }
    variant = 1;
  } else {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet),
                    128);
    variant = effect->variant;
  }

  // Everything starts 100 pixels out on the user's side.
  for (i = 0; i != 32; i++) {
    struct EffectStep *puff = &work->particles[i];

    if (effect->side == 1)
      puff->x = 200 << 14;
    else
      puff->x = -(200 << 14);
    puff->y = 0;
    puff->z = 0;
    puff->velocityX = ((Random16() & 63) - 32) << 13;
    puff->velocityY = ((Random16() & 63) + 16) << 12;
    puff->velocityZ = ((Random16() & 63) - 32) << 13;
    puff->variant = 0;
  }
  for (i = 0; i != 1024; i++) {
    struct EffectStep *mote = &MOTES[i];

    if (effect->side == 1)
      mote->x = 200 << 14;
    else
      mote->x = -(200 << 14);
    mote->y = 0;
    mote->z = 0;
    mote->velocityX = ((Random16() & 63) - 32) << 13;
    mote->velocityY = ((Random16() & 31) + 8) << 13;
    mote->velocityZ = ((Random16() & 63) - 32) << 13;
    mote->variant = 0;
  }

  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != Data_081983a2[variant][FIELD_FRAMES]; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 2)
      Sound_PlayFar(144);
    if (frame == Data_081983a2[variant][FIELD_FRAMES] - 48)
      Func_081180e8(133);

    // The motes: depth 160-799 gives sizes 9 down to 1.
    for (i = 0; i != Data_081983a2[variant][FIELD_MOTES]; i++) {
      struct EffectStep *mote = &MOTES[i];

      if (mote->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint((s32 *)mote, &screen);
        screen.x >>= 1;
        screen.x += effect->side * 32 - 16;
        if (screen.depth < 160)
          screen.depth = 160;
        if (screen.depth > 799)
          screen.depth = 799;
        size = 9 - (screen.depth - 160) / 64;
        draw[0](canvas,
                work->sheet + (gData_08197410[size - 1] + (i & 1) * 770) +
                    MOTE_SHEET,
                screen.x - size / 2, screen.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(mote, 64, -0x2000);
      }
    }

    // The puffs: one more each frame; a picture every three frames.
    if (frame > 2) {
      for (i = 0; i != Data_081983a2[variant][FIELD_PUFFS]; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (i < frame && puff->y >= 0) {
          EffectPosition_ProjectPoint((s32 *)puff, &screen);
          screen.x >>= 1;
          screen.x += effect->side * 32 - 16;
          if (puff->variant >= 0 && puff->variant <= 20) {
            s32 cel = puff->variant / 3;
            u32 width;

            draw[0](canvas, work->sheet + Data_081983ac[cel],
                    screen.x - ((width = Data_081983ba[cel]) >> 1),
                    screen.y - (width >> 1), width, width);
          }
          if (puff->variant <= 20)
            puff->variant++;
          BattleEffectKernels_IntegrateVector3(puff, 64, -0x2000);
        }
      }
    }

    // Hit the targets a frame apart; in mode 0 they recoil too.
    if (mode == 0) {
      for (i = 0; i != effect->count; i++) {
        if (frame == i + 6) {
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
          Func_08118088(effect->actors[i], 2);
        }
      }
    } else {
      for (i = 0; i != effect->count; i++) {
        if (frame == i + 6)
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
      }
    }
    if (frame == 2)
      work->shakeFrames = 6;
    Func_08158ce0(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
