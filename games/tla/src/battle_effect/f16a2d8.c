#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_release.h"
#include "system.h"
#include "iwram_call.h"
#include "task.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode in the high
// nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define BLIT_ADDITIVE 0

// Each of the four pictures is PICTURE_SIZE pixels square; the reflected
// variant's are half as tall.
#define PICTURE_SIZE 120
#define PICTURE_BYTES (PICTURE_SIZE * PICTURE_SIZE)
#define HALF_PICTURE_BYTES (PICTURE_SIZE * PICTURE_SIZE / 2)

#define REVEAL_FRAMES 21
#define FLOOD_FRAME 16 // the pictures end and the canvas floods
#define RESULTS_FRAME 18
#define HIT_FRAME 20

#define PHASE_RESULTS 134
#define SE_REVEAL 212

extern char ResourceId_ArcFragmentsSheet;
extern char ResourceId_RevealMaskA;
extern char ResourceId_RevealMaskB;
extern char ResourceId_RevealMaskC;
extern char ResourceId_RevealMaskD;
extern char ResourceId_RevealMaskE;
extern char ResourceId_RevealMaskF;

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Palette_StepFadeTransfer(void);
s32 BattleEffect_LoadBlitter(
    s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                 // 7, 7, flags, mode)
void Func_081180e8(s32 phase);   // BattlePlayback_StartFar
void Func_08118088(s32 actor,
                   s32 variant);  // BattleMotion_ApplyVariantMotionFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_EndCanvasLayer(void);
void SummonEffect_RunCanvasMode2(struct BattleEffectArgument *effect);
void Sound_PlayFar(s32 cue);
void Actor_SetMode(struct MotionObject *object, s32 mode);
s32 Actor_SetAnimationSpeedFar(struct MotionObject *object, s32 value);
// The target is a whole word here: variant 3 passes the user twice, loaded once
// and copied, where an s16 parameter would load it again halved.
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames,
                                    s32 jumpVelocity);

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

// A picture flashes up over the target (four pictures, four frames each), the
// canvas floods with one colour, and the target is struck.
//
// Setup: open the canvas, load the variant's four pictures (two into the
// sheet, two into the map cell buffer), move the user up to the target, and
// build the blitters. Each frame shows the current picture for its time,
// floods the canvas from frame 16 to 19, and shakes the camera; results at
// frame 18, the hit at 20.
void Func_0816a2d8(struct BattleEffectArgument *effect, s32 variant) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  if ((u32)(variant - 3) > 2)
    REG_BLDCNT = 0;
  REG_BLDALPHA = 0x1010;

  // The pictures: 1 and 2 in the sheet, 3 and 4 in the map cell
  // buffer (variant 5's four half pictures all there).
  if (variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 4) {
    s32 i;

    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, Ram_MapCellBuffer,
                               1, 1);
    // Turn the first 64 BG colours: red to blue, blue to green, green to red.
    for (i = 0; i != 64; i++) {
      s32 blue = (((u16 *)BG_PLTT)[i] >> 10) & 0x1f;
      s32 green = (((u16 *)BG_PLTT)[i] >> 5) & 0x1f;
      s32 red = ((u16 *)BG_PLTT)[i] & 0x1f;

      ((u16 *)BG_PLTT)[i] = (red << 10) | (blue << 5) | green;
    }
  } else if (variant == 3) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 5) {
    Resource_LoadAndDecompress((s32)&ResourceId_ArcFragmentsSheet,
                               Ram_MapCellBuffer, 1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, Ram_MapCellBuffer,
                               1, 1);
  }
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The user closes on the target in 16 frames (variant 3 jumps in place,
  // variant 4 strikes a pose instead and waits 8).
  if (variant == 1 || variant == 5)
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16,
                                   0x80000);
  else if (variant == 3)
    BattleMotion_ApproachTargetFar(effect->actor, effect->actor, 16, 0xa0000);
  else if (variant == 4) {
    struct MotionObject *user = GetBattleObjectSlotFar(effect->actor)->object;

    Actor_SetMode(user, 2);
    Actor_SetAnimationSpeedFar(user, 48);
  } else
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
  if (variant == 4)
    wait(8);
  else
    wait(16);

  // The blitters face the target: one for the picture, one flipped upside
  // down for its reflection.
  if (effect->side == 1)
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                        COMPOSITE_CLIP_Y |
                                                        COMPOSITE_FLIP_X));
  else
    BattleEffect_LoadBlitter(
        HEAP_BLITTER,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (effect->side == 1)
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                        COMPOSITE_CLIP_Y |
                                                        COMPOSITE_FLIP));
  else
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                             BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                        COMPOSITE_CLIP_Y |
                                                        COMPOSITE_FLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  Sound_PlayFar(SE_REVEAL);
  for (frame = 0; frame != REVEAL_FRAMES; frame++) {
    if (variant == 5) {
      // Half the picture, then its reflection below it.
      if (frame <= 3) {
        draw[0](canvas, Ram_MapCellBuffer, 0, 0, PICTURE_SIZE,
                PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer, 0, PICTURE_SIZE / 2, PICTURE_SIZE,
                PICTURE_SIZE / 2);
      } else if (frame <= 7) {
        draw[0](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES, 0, 0,
                PICTURE_SIZE, PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES, 0,
                PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      } else if (frame <= 11) {
        draw[0](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 2, 0, 0,
                PICTURE_SIZE, PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 2, 0,
                PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      } else if (frame <= 15) {
        draw[0](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 3, 0, 0,
                PICTURE_SIZE, PICTURE_SIZE / 2);
        draw[1](canvas, Ram_MapCellBuffer + HALF_PICTURE_BYTES * 3, 0,
                PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      }
    } else {
      if (frame <= 3)
        draw[0](canvas, work->sheet, 0, 0, PICTURE_SIZE, PICTURE_SIZE);
      else if (frame <= 7)
        draw[0](canvas, work->sheet + PICTURE_BYTES, 0, 0, PICTURE_SIZE,
                PICTURE_SIZE);
      else if (frame <= 11)
        draw[0](canvas, Ram_MapCellBuffer, 0, 0, PICTURE_SIZE, PICTURE_SIZE);
      else if (frame <= 15)
        draw[0](canvas, Ram_MapCellBuffer + PICTURE_BYTES, 0, 0, PICTURE_SIZE,
                PICTURE_SIZE);
    }

    // The flood: four frames of the canvas filled with colour 0x3f.
    if ((u32)(frame - FLOOD_FRAME) <= 3)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    if (frame == RESULTS_FRAME)
      Func_081180e8(PHASE_RESULTS);
    if (frame == HIT_FRAME) {
      work->shakeFrames = 8;
      Func_08118088(effect->actors[0], 4);
    }
    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(HEAP_BLITTER);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  if (variant == 3) {
    // Clear the canvas and its tiles and hand the layer to the next scene.
    /* FAKEMATCH(wrapper): both clears through ClearWords_, which sets the size
     * for each call and holds the routine; as direct Iwram_ClearWords calls, 14
     * of 469 instructions differ in tla-en. */
    ClearWords_((void *)0x06004000, 0x3c00);
    ClearWords_(canvas, 0x3c00);
    Task_Remove(Palette_StepFadeTransfer);
    SummonEffect_RunCanvasMode2(effect);
  } else {
    if (variant == 4)
      Actor_SetAnimationSpeedFar(GetBattleObjectSlotFar(effect->actor)->object,
                                 16);
    BattleEffect_EndCanvasLayer();
  }
}
