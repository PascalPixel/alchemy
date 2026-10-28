// Near miss of Func_08153c30, an unused effect (no caller): differ 16 of
// 291 (draft 291, 2026-10-18).

// Twin: The Broken Seal's BattleEffect_RunTargetBursts (effect.c), call for
// call. The Lost Age loads its two blitters through BattleEffect_LoadBlitter into heap
// slots 104 and 188 (BLIT_ADDITIVE 0 and BLIT_MAX 1 where The Broken Seal
// has 3 and 2), reads the effect from its argument and gives its task the priority
// 0xc80. Resources not yet named in directory.s, by row and asset: 0x161
// BattleEffect_SmokeSheet, 0x12d BattleEffect_TargetBurstImage and 0x17f
// BattleEffect_PinkBurstSheet.

// Left: eight rows, each a pair the native issues in the other order: the
// particle cursor's setup before the loop counters, the record's address
// after the camera word, object->z before its store, the cell's 9x before
// the frame word.

// Tried (2026-10-15): the lane's rewrite (named modes, the burst offset as
// a constant): 22 -> 20.
// Tried (2026-10-18): BattleEffect_LoadBlitter returning s32, as defined: 20 ->
// 18; an empty asm before the heap reads (tagged): 18 -> 16; alchemy permute
// 5 minutes (from 18): 16 only by one block around the whole body (not
// kept).

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define BURST_IMAGE 0x1b00 // the burst pictures, after the smoke

extern char ResourceId_SmokeSheet;
extern char ResourceId_TargetBurstImage;
extern char ResourceId_PinkBurstSheet;

// Bursts of smoke rise from each target in turn, eight frames apart.

// Setup: 64 puffs, six a target, each but the sixth thrown out at a random
// heading. Frame loop (96 frames): for 40 frames from its start a
// target's puffs rise 40 pixels above it through six 24x48 pictures; the
// sixth, drawn ADDITIVE, is the burst itself and falls faster.
void Func_08153c30(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  s32 frame;
  DrawRectangle rectangleB;
  DrawRectangle rectangleA;
  struct EffectStep *step;
  s32 i;
  struct EffectPosition screen;
  s32 record[3];

  /* FAKEMATCH: stores the argument before the 0x03000000 base is started; the
   * native's first instruction after sub sp is the argument's store (differ
   * 18 without it). */
  __asm__("");
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage,
                             work->sheet + BURST_IMAGE, 0, 0);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)0x05000000, palette, 128);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, CLIP));
  rectangleA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, CLIP));
  rectangleB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  for (i = 0; i != 64; i++) {
    s32 angle;
    s32 magnitude;

    step = &work->particles[i];
    angle = Random16() & 0xffff;
    magnitude = Random16() & 0xff;
    step->x = 0;
    step->y = 0;
    step->z = 0;
    if (i % 6 == 5) {
      step->velocityX = 0;
      step->velocityY = 0;
    } else {
      step->velocityX = (Trig_Sin(angle) * magnitude) >> 7;
      step->velocityY = (Trig_Cos(angle) * magnitude) >> 9;
    }
    step->velocityZ = 0;
    step->variant = 0;
  }

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != 96; frame++) {
    s32 member;
    s32 *camera;

    camera = Ram_HeapSlots->cameraWork;
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *memberObject;

      // The puffs rise from 40 pixels above the target.
      memberObject = GetBattleObjectSlotFar(effect->actors[member])->object;
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      record[0] = memberObject->x;
      record[1] = 160 << 13;
      record[2] = memberObject->z;
      SceneTransform_ApplyPosition(record);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct EffectStep *p;

        for (i = 0; i != 6; i++) {
          s32 cel;

          // A picture every six frames, holding the last.
          p = &work->particles[member * 6 + i];
          cel = p->variant / 6;
          if (cel > 5)
            cel = 5;
          EffectPosition_ProjectPoint((s32 *)p, &screen);
          screen.x >>= 1;
          if (i == 5) {
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            rectangleA(canvas, work->sheet + cel * 0x480 + BURST_IMAGE,
                       screen.x - 12, screen.y - 36, 24, 48);
          } else {
            BattleEffectKernels_IntegrateVector3(p, 60, 0x200);
            rectangleB(canvas, work->sheet + cel * 0x480, screen.x - 12,
                       screen.y - 36, 24, 48);
          }
          p->variant += 1;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
