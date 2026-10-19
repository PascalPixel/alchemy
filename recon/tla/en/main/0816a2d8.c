// Draft, differ 342 of 469 (441 here): Func_0816a2d8, the body of five effect
// kinds (variant_a.c's wrappers pass variants 1-5), The Broken Seal's
// BattleFx_RunFlashingStrike rebuilt.

// Twin: ☀️ BattleFx_RunFlashingStrike (games/tbs/src/effect/effect.c), a body of
// two kinds there; The Lost Age keeps its frame (the 120 x 120 picture in four
// frames of four, the canvas flooded with colour 0x3f for four frames, the hit
// at frame 20) and adds:
// - six picture sets, with variant 4's colours rotated in the BG palette;
// - variant 5, a 120 x 60 picture drawn twice, its lower half mirrored
// through the second blitter;
// - variant 3 hands the canvas to SummonFx_RunCanvasMode2 instead of
// closing it, and variant 4 animates the user instead of moving it;
// - a blend of 16/16 (BLDCNT cleared but for variants 3 to 5);
// - two blitters, flipped by side, where ☀️ builds one copy blitter.

// Left: 28 instructions short, all in the frame loop. The native keeps 120 in
// r8, 60 in r11, the frame in r10 and the canvas in r9, with both blitters in
// r6 and r7; here 120 takes r6 and the blitters r7 and r11, and the frames
// 8 to 11 and 12 to 15 of the reflected variant share one tail through r5,
// where the native loads each address from the pool twice and only the last
// case holds its address in r5. In variant 4's palette loop the native
// holds the two masks in ip and lr, here r6 and r7.

// Tried (differ): the approach target as s16, which loads variant 3's user
// twice (391, then 369 as s32); the palette channels written as one
// expression of a u16 (394), red first as Func_08166b10 has them (368), blue
// first (342), red in the store expression (346); alchemy permute 8 minutes,
// 2 jobs: 320 with a temporary holding 120 for the sizes (not kept, a
// device of no known source form).

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "system.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// BattleFx_LoadBlitter's argument: The Lost Age's blitter mode in the high nibble,
// the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define BLIT_ADDITIVE 0

// Each picture is four frames; the reflected variant's frames are half as
// tall.
#define PICTURE_SIZE 120
#define FRAME_BYTES (PICTURE_SIZE * PICTURE_SIZE)
#define HALF_FRAME_BYTES (PICTURE_SIZE * PICTURE_SIZE / 2)

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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void Palette_StepFadeTransfer(void); // Palette_StepFadeTransfer
s32 BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter(slot,
                                               // 7, 7, flags, mode)
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_08118088(s32 actor,
                   s32 variant);  // BattleMotion_ApplyVariantMotionFar
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void BattleFx_EndCanvasLayer(void);         // BattleFx_EndCanvasLayer
void SummonFx_RunCanvasMode2(struct BattleEffectArgument *effect);
void Audio_PlayCue(s32 cue);
void Object_SetMode(struct MotionObject *object, s32 mode);
s32 ObjectDispatch_SetAnimationSpeedFar(struct MotionObject *object,
                                           s32 value);
// The target is a whole word here: variant 3 passes the user twice, loaded once
// and copied, where an s16 parameter would load it again halved.
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames,
                                    s32 jumpVelocity);

// A picture flashes up over the target in four frames of four, the canvas
// floods with one colour, and the target is struck.
//
// Setup: open the canvas, load the variant's four frames (two into the
// sheet, two into the map cell buffer), move the user up to the target, and
// build the blitters. Each frame shows the picture's frame for its time,
// floods the canvas from frame 16 to 19, and shakes the camera; results at
// frame 18, the hit at 20.
void Func_0816a2d8(struct BattleEffectArgument *effect, s32 variant) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw;
  DrawRectangle mirror;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(0);
  if ((u32)(variant - 3) > 2)
    REG_BLDCNT = 0;
  REG_BLDALPHA = 0x1010;

  // The pictures: frames 1 and 2 in the sheet, 3 and 4 in the map cell
  // buffer (variant 5's four half frames all there).
  if (variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, Ram_MapCellBuffer,
                               1, 1);
  } else if (variant == 4) {
    u16 *color;
    s32 i;

    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, Ram_MapCellBuffer,
                               1, 1);
    // Turn the first 64 BG colours: red to blue, blue to green, green to red.
    color = (u16 *)BG_PLTT;
    for (i = 0; i != 64; i++) {
      s32 blue = (*color >> 10) & 0x1f;
      s32 green = (*color >> 5) & 0x1f;
      s32 red = *color & 0x1f;

      *color++ = (red << 10) | (blue << 5) | green;
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
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

  // The user closes on the target in 16 frames (variant 3 jumps in place,
  // variant 4 strikes a pose instead and waits 8).
  if (variant == 1 || variant == 5)
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16,
                                   0x80000);
  else if (variant == 3)
    BattleMotion_ApproachTargetFar(effect->actor, effect->actor, 16, 0xa0000);
  else if (variant == 4) {
    struct MotionObject *user = GetBattleObjectSlotFar(effect->actor)->object;

    Object_SetMode(user, 2);
    ObjectDispatch_SetAnimationSpeedFar(user, 48);
  } else
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
  if (variant == 4)
    WaitFrames(8);
  else
    WaitFrames(16);

  // The blitters face the target: one for the picture, one flipped upside
  // down for its reflection.
  if (effect->side == 1)
    BattleFx_LoadBlitter(HEAP_BLITTER,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                             COMPOSITE_FLIP_X));
  else
    BattleFx_LoadBlitter(HEAP_BLITTER,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (effect->side == 1)
    BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                             COMPOSITE_FLIP));
  else
    BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                             COMPOSITE_FLIP_Y));
  mirror = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  Audio_PlayCue(SE_REVEAL);
  for (frame = 0; frame != REVEAL_FRAMES; frame++) {
    if (variant == 5) {
      // Half the picture, then its reflection below it.
      if (frame <= 3) {
        draw(canvas, Ram_MapCellBuffer, 0, 0, PICTURE_SIZE, PICTURE_SIZE / 2);
        mirror(canvas, Ram_MapCellBuffer, 0, PICTURE_SIZE / 2, PICTURE_SIZE,
               PICTURE_SIZE / 2);
      } else if (frame <= 7) {
        draw(canvas, Ram_MapCellBuffer + HALF_FRAME_BYTES, 0, 0, PICTURE_SIZE,
             PICTURE_SIZE / 2);
        mirror(canvas, Ram_MapCellBuffer + HALF_FRAME_BYTES, 0,
               PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      } else if (frame <= 11) {
        draw(canvas, Ram_MapCellBuffer + HALF_FRAME_BYTES * 2, 0, 0,
             PICTURE_SIZE, PICTURE_SIZE / 2);
        mirror(canvas, Ram_MapCellBuffer + HALF_FRAME_BYTES * 2, 0,
               PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      } else if (frame <= 15) {
        draw(canvas, Ram_MapCellBuffer + HALF_FRAME_BYTES * 3, 0, 0,
             PICTURE_SIZE, PICTURE_SIZE / 2);
        mirror(canvas, Ram_MapCellBuffer + HALF_FRAME_BYTES * 3, 0,
               PICTURE_SIZE / 2, PICTURE_SIZE, PICTURE_SIZE / 2);
      }
    } else {
      if (frame <= 3)
        draw(canvas, work->sheet, 0, 0, PICTURE_SIZE, PICTURE_SIZE);
      else if (frame <= 7)
        draw(canvas, work->sheet + FRAME_BYTES, 0, 0, PICTURE_SIZE,
             PICTURE_SIZE);
      else if (frame <= 11)
        draw(canvas, Ram_MapCellBuffer, 0, 0, PICTURE_SIZE, PICTURE_SIZE);
      else if (frame <= 15)
        draw(canvas, Ram_MapCellBuffer + FRAME_BYTES, 0, 0, PICTURE_SIZE,
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
    Func_08158ce0(16, 16);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  if (variant == 3) {
    // Clear the canvas and its tiles and hand the layer to the next scene.
    Iwram_ClearWords((void *)0x06004000, 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    SummonFx_RunCanvasMode2(effect);
  } else {
    if (variant == 4)
      ObjectDispatch_SetAnimationSpeedFar(
          GetBattleObjectSlotFar(effect->actor)->object, 16);
    BattleFx_EndCanvasLayer();
  }
}
