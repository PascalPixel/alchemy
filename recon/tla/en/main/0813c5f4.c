// Near miss, differ 44 of 580.
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 44. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 42).

// Twin: The Broken Seal's BattleEffect_RunFiveMode, the body of its five
// coloured streak effects. ⚓️ adds one thing: when each target's streaks
// start, modes 1 to 4 play their own cry. The rest is the twin with ⚓️'s
// calling conventions (the effect read from the argument, heap blocks as
// Ram_HeapSlots fields, BattleEffect_LoadBlitter with ⚓️'s mode numbers, callback
// order 0xc80, BattleEffectKernels_IntegrateVector3 for
// EffectStep_AdvanceWithGravity3D).

// Left: the native keeps the heap base 0x03000000 in r5 from the prologue
// to the alternate blitter's read after the side branch (adds r3, r5, #0);
// here its last use is the first blitter's read, which then lands in r5,
// and the join builds the base again (two instructions, and the two arms
// merge one store more). The rest are single rows where the native issues
// a load or a constant one slot earlier.

// Tried:
// the twin as ported                                         365
// alchemy permute, 10 minutes: the start frame kept in lift's local
// (tagged; as its own local 365, without lift 388)          56
// the heap base through a local (heap)                       no change
// the alternate blitter read once after the side branch      66

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_display.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "transform.h"
#include "resource.h"

// The blitters' heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define TASK_PASS_FRAME 0xc80

// Four signed bytes for each mode (The Broken Seal's gStreaks_Records).
extern s8 Data_08197533[];
#define RECORD(mode, field) Data_08197533[(mode) * 4 + (field)]
#define RECORD_ANIMATE 0 // cycle the streak through its three pictures
#define RECORD_HIT 1     // sound the hit and flash the target
#define RECORD_MOTION 2  // the target's recoil motion, or -1
#define RECORD_SHAKE 3   // tremble the target after the hit

// The struck target's sideways tremble, in pixels (gStreaks_Tremble).
extern s8 Data_08197547[];

#define STREAK_MAX 3 // streaks for each target

extern char ResourceId_FiveModeImage;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_VioletPaletteD;
extern char ResourceId_RedPaletteB;
extern char ResourceId_CyanPalette;

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 Task_Add(void *func, s32 order);
s32 Task_Remove(void *func);
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);

// Sonic Wave (mode 0), Shriek (1), Banshee Howl (2), Wicked Howl (3) and War
// Cry (4): effect kinds 300 to 305.

// Three streaks rise 20 pixels above the user and fly at each target in turn,
// 6 frames apart, reaching it in 24 frames; each target starts 32 frames
// after the one before. A streak is a 20x40 picture drawn twice, its upper
// half mirrored, and the canvas wobbles line by line on a sine wave while the
// transfer halves the old picture each frame into a trail. The mode chooses
// the colour, whether the streak animates, the hit's sound and flash, a
// recoil, and a tremble of the target.

void Func_0813c5f4(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  void *canvas;
  s32 k;
  DrawRectangle drawLower;
  DrawRectangle drawUpper;
  struct BattleCamera *camera;
  struct MotionObject *source;
  struct MotionObject *target;
  s32 resource;
  s32 frame;
  s32 j;
  s32 streaks = STREAK_MAX;
  s32 lift = 20 << 16;

  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  camera = heap->cameraWork;
  BattleEffect_StartCanvasLayer(1);

  // The upper half draws mirrored top to bottom; the far side mirrors
  // left to right as well.
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_Y));
    drawUpper = heap->unk64[1];
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
    drawLower = heap->unkb8[1];
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
    drawUpper = heap->unk64[1];
    BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                  BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    drawLower = heap->unkb8[1];
  }

  // The streak pictures, and the mode's colours.
  Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work->sheet, 0, 0);
  switch (mode) {
  case 0:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_VioletPaletteD;
    break;
  case 3:
    resource = (s32)&ResourceId_RedPaletteB;
    break;
  case 4:
  default:
    resource = (s32)&ResourceId_CyanPalette;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  work->transferMode = 2;   // each transfer halves the old picture,
  work->transferValue = 50; // leaving a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  source = GetBattleObjectSlotFar(effect->actor)->object;
  {
    s32 total = streaks * effect->count * 6 + 48;

    // Each streak flies from 20 pixels above the user to 20 pixels above
    // its target in 24 frames.
    for (k = 0; k != effect->count; k++) {
      target = GetBattleObjectSlotFar(effect->actors[k])->object;
      for (j = 0; j != streaks; j++) {
        struct EffectStep *streak = &work->particles[k * streaks + j];

        streak->x = source->x;
        streak->y = source->y + lift;
        streak->z = source->z;
        streak->velocityX = (target->x - streak->x) / 24;
        streak->velocityY = (target->y + lift - streak->y) / 24;
        streak->velocityZ = (target->z - streak->z) / 24;
        streak->variant = 0; // frames since it set out
      }
    }

    for (frame = 0; frame != total; frame++) {
      s32 *row = work->bg2xLines;

      // Wobble BG2's 160 lines on a sine wave, and fade the last 16
      // frames out.
      for (k = 0; k != 160; k++)
        *row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
      if (frame > total - 16)
        REG_BLDALPHA = (total - frame) | 0x1000;

      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      for (k = 0; k != effect->count; k++) {
        // Each target's streaks set out with the mode's cry.
        if (frame == k * 32) {
          if (mode == 1)
            Sound_PlayFar(192);
          if (mode == 2)
            Sound_PlayFar(189);
          if (mode == 3)
            Sound_PlayFar(148);
          if (mode == 4)
            Sound_PlayFar(138);
        }
        if (frame >= k * 32) {
          for (j = 0; j != streaks; j++) {
            /* FAKEMATCH: the streak's start frame is kept in lift's local,
             * which is dead by now. As a local of its own (start) or as an
             * expression, the allocation of every loop differs (differ 365
             * against 56): the merged local is what gives the native's
             * registers and its 88-byte stack frame. */
            lift = k * 32 + j * 6;
            if (frame >= lift) {
              struct EffectStep *streak = &work->particles[k * streaks + j];
              s32 step;

              // The picture changes every 8 frames, stopping at the sixth.
              EffectPosition_ProjectPoint((s32 *)streak, &position);
              position.x >>= 1;
              step = streak->variant / 8;
              if (step > 5)
                step = 5;
              if (RECORD(mode, RECORD_ANIMATE)) {
                u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              } else {
                u8 *cell = &work->sheet[step * 800 + 2 * 4800];

                drawUpper(canvas, cell, position.x - 10, position.y - 40, 20,
                          40);
                drawLower(canvas, cell, position.x - 10, position.y, 20, 40);
              }
              BattleEffectKernels_IntegrateVector3(streak, 64, 0);
              streak->variant++;
            }
          }

          // From 6 frames after the hit, tremble the target sideways for
          // 32 frames.
          if (RECORD(mode, RECORD_SHAKE) && frame >= k * 32 + 30 &&
              frame < k * 32 + 62) {
            struct MotionObject *struck =
                GetBattleObjectSlotFar(effect->actors[k])->object;

            struck->x += Data_08197547[(frame - k * 32 - 30) % 8] << 16;
            if (struck->x > 0)
              struck->x += 0x8000;
            else
              struck->x -= 0x8000;
            Func_0814cd48(effect->actors[k], -1, 5, -1, 0);
          }

          // The hit: a sound and two flashes, 16 frames apart. The first
          // hit lets the battle show its results.
          if (RECORD(mode, RECORD_HIT)) {
            if (frame == k * 32 + 24) {
              Sound_PlayFar(133);
              if (k == 0)
                Func_081180e8(-1);
              Func_0814cd48(effect->actors[k], 7, 5, k, 8);
            }
            if (frame == k * 32 + 40)
              Func_0814cd48(effect->actors[k], 7, 5, k, 8);
          }
          if (RECORD(mode, RECORD_MOTION) != -1 && frame == k * 32 + 24) {
            work->shakeFrames = 4;
            Func_08118088(effect->actors[k], RECORD(mode, RECORD_MOTION));
          }
        }
      }
      Func_08158ce0(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
