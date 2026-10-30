// Near miss, differ 2 of 272 (tla-en, 2026-10-30): one row. The native
// copies the frame counter's zero to sl (mov sl, r5) after the record's
// three stores, here before them. In sched2 the copy and the three stores
// all have priority 1 once the record's address is spilled (str r2,
// [sp, #12]); the stores have an output dependence on that spill (its slot
// has alias set 0, so it may alias them) and the copy has none, so the copy
// wins the last-scheduled-insn class test. The native's order needs the
// stores free of the spill, or the copy tied to it.
// Tried 2026-10-30 (differ): the empty asm after the stores, the one-pass
// block around frame = 0 or around record[2] (4 each, as before: the
// barrier also cuts the dependences that order the transfer value's offset
// and the 0xc80 shift before Task_Add); the three stores in one one-pass
// block (6); an empty one-pass block before record[2] (39); the record as
// an initialized array in a block before the loop (88); register-tied asm
// with a memory clobber on frame or after the stores (33 to 125);
// alchemy permute, 5 minutes, 3 jobs, from this draft: nothing below 2.
// Earlier (2026-10-26, differ): the record set before the transfer mode
// (51), frame = 0 apart from its loop (2), the record's stores in other
// orders (4, 6), the record declared later (6) or as struct EffectPosition
// (2), frame declared first (2), the loop as do while (2); an asm before
// the call as well (6); alchemy permute 5 minutes, 2 jobs (nothing below
// 2).

// Func_081539d0 is an unused effect (no caller).

// Twin: The Broken Seal's BattleEffect_RunSwirlingStars (effect.c), call for
// call. The Lost Age reads the effect from its argument, takes its heap
// blocks as Ram_HeapSlots fields and gives its task the priority 0xc80.
// ResourceId_StarDotSheet is row 0x14f of directory.s (asset
// BattleEffect_StarDotSheet), not yet named there.

// Before -mthumb-slow-loads (2026-10-15, differ 14) seven reordered pairs
// were left, each the native placing a load one row
// earlier (the particle base before the blitter call, the second loop's
// particle base, the cell offset table, the camera word) or a use of a
// loaded word one row later (frame's copy to r10 after the record's
// stores, the negations of the star's coordinates after the stores of the
// previous sums). Each follows from a load latency of 3 in sched2.

// Tried then: the particle pointer set before the blitter call (differ 32, r5
// and r6 trade); alchemy permute, 6 minutes, 2 jobs (2026-10-15): nothing
// below 14 (its best weighted candidate differs 20).

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"
#include "effect_step.h"
#include "graphics.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *pair);
void Func_080150e4(s32 angle);           // SceneTransform_ApplyRoll
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);

#define STARS ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_StarDotSheet;
extern u16 gData_08197410[]; // gBattleEffect_MoteCells

// Sixty-four stars swirl about a point in front of the camera and are
// drawn in.

// Setup: 256 stars are scattered up to 127 pixels each way. Frame loop
// (160 frames): four stars a frame join, each turning about one of four
// axes as fast as its number, sized by depth; thirty frames after joining
// a star accelerates toward the center by 1/256 of its distance a frame.
void Func_081539d0(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 record[3];
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct EffectStep *star;
  s32 i;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(effect->side ^ 1, draw);
  for (i = 0; i != 256; i++) {
    star = &STARS[i];
    star->x = ((Random16() & 0xff) - 127) << 16;
    star->y = ((Random16() & 0xff) - 127) << 16;
    star->z = ((Random16() & 0xff) - 127) << 16;
    star->velocityX = 0;
    star->velocityY = 0;
    star->velocityZ = 0;
    star->variant = 0;
  }
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  record[0] = 0;
  record[1] = 160 << 15;
  record[2] = 0;
  for (frame = 0; frame != 160; frame++) {
    s32 *camera;

    // The stars turn about a point 80 pixels in front of the camera.
    camera = Ram_HeapSlots->cameraWork;
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    SceneTransform_ApplyPosition(record);
    star = STARS;
    for (i = 0; i != 64; i++, star++) {
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        Transform_Push();
        switch (i & 3) {
        case 0:
          SceneTransform_ApplyYaw(frame * (i + 32) * 8);
          break;
        case 1:
          SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
          break;
        case 2:
          Func_080150e4(-frame * (i + 32) * 8);
          break;
        case 3:
          SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
          Func_080150e4(-frame * (i + 32) * 8);
          break;
        }
        EffectPosition_ProjectPoint((s32 *)star, &pos);
        pos.x >>= 1;
        Transform_Pop();

        // Depth 250-634 gives sizes 9 down to 3.
        if (pos.depth < 250)
          pos.depth = 250;
        if (pos.depth > 0x27a)
          pos.depth = 0x27a;
        size = 9 - (pos.depth - 250) / 64;
        draw[0](canvas,
                work->sheet + ((i & 3) * 770 + gData_08197410[size - 1]),
                pos.x - size / 2, pos.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(star, 60, 0);

        // Thirty frames in, the star is drawn toward the center.
        if (frame > i / 4 + 30) {
          s32 dx = -star->x >> 8;
          s32 dy = -star->y >> 8;
          s32 dz = -star->z >> 8;

          star->velocityX += dx;
          star->velocityY += dy;
          star->velocityZ += dz;
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
