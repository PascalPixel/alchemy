// Draft, differ 12 of 479, skeleton gap 6 (tla-en, 2026-11-01)
// Func_0814ea58, the body behind BattleEffect_RunDrain (mode 0, or 1 when
// the variant is not 0) and BattleEffect_RunTrueCollide (mode 2).

// Twin: The Broken Seal's BattleEffect_RunDrain (effect.c), rebuilt
// around a mode. Mode 0 is The Broken Seal's variant 0 (the ice palette)
// and mode 1 its other variants (the blast palette). Mode 2 is new: it
// draws a 10-pixel disc (Func_0815b510) instead of the particle sheet,
// throws the motes twice as fast, fades a violet ramp through 16:6
// blending and trails at 50 instead of darkening. The mote size comes from
// Data_081982f4 by mode (6, 6, 10) where The Broken Seal draws 6.
// ResourceId_BlastSheet and ResourceId_VioletPaletteC are rows 0x184 and
// 0x165 of directory.s (assets BattleEffect_BlastSheet and
// BattleEffect_VioletPaletteC), not yet named there.

// Left: every register, stack slot and instruction agrees; four places are
// in a different order, and all four are sched2 ranking a spill store or
// reload differently from the ROM, with the RTL already in the ROM's order:
// - before the setup member loop, str [sp, #8] comes first in the ROM, ahead
//   of movs #36 (priority 2 to the store's 1 here);
// - the second Func_0814cd48 call: the ROM stores [sp] before negs and sets
//   r1 last, though it orders the first call as this draft does; the only
//   difference between the two blocks is the reload after the second call;
// - the frame member loop: str [sp, #12] before mov r9 in its preheader, and
//   movs r4, #20 after the three spill stores in its tail. The reload
//   registers (r3, r4, r0, r1, r2) prove the ROM's RTL has the 20 step between
//   the two offset steps, as here, so it is ranking and not source; moving the
//   step (start and offset locals at the top of the loop) fixes the preheader
//   and breaks the rotation (22);
// - the frame loop's end test: the ROM reloads frame straight after storing
//   it, where -mthumb-slow-loads makes that load wait, so count loads first.
// These are the compiler model's open questions (spill stores under-costed,
// a same-address reload too strict), out of this draft's scope. No device
// reaches them: the 20 step cannot be split from its reload by a barrier, and
// a device short of differ 0 stays out.

// Tried (2026-11-01):
// - The Broken Seal's form closed 234 to 16: one shared i in r8, every mote
//   `&MOTES[member * 128 + i]` inside its loop, declarations in spill-slot
//   order (work, canvas, frame, draw, member, sheet, camera), a u8 size so
//   gData_08197410[size - 1] shares size * 2;
// - the ramp's pointer set before i, stepped by hand: 16 to 12 (the build of
//   BG_PLTT + 1 comes first in the RTL);
// - offset and base locals, start = member * 20, the offset local inside the
//   if: no gain or worse (12, 12, 28, 45);
// - Func_0814cd48 returning s32, while and hand-stepped frame loops: 12;
//   swapped compares: 14; else if before the second tint: 70.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "task.h"
#include "effect_step.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "motobj.h"
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
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_081180e8(s32 phase);       // BattlePlayback_StartFar
void Func_0815b510(u8 *map, s32 size, s32 scale);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 actor);

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

#define MOTES_PER_MEMBER 128
#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_Data134; // the particle sheet
extern char ResourceId_IceBlockSheet;
extern char ResourceId_BlastSheet;
extern char ResourceId_VioletPaletteC;
extern u8 Data_081982f4[];   // the mote size by mode
extern u16 gData_08197410[]; // gBattleEffect_MoteCells

// Motes of light burst out of each target in turn, 20 frames apart, and
// are drawn into the user.

// Setup: each target's 128 motes start at the middle of its body with
// random velocities; the canvas darkens by 4 a transfer (in mode 2 it
// trails instead). Frame loop: a target flashes as its motes leave; ten
// frames after leaving (one frame later for each mote) a mote accelerates
// toward the middle of the user by 1/256 of the distance a frame and goes
// out within 16 pixels of it across. The user glows from frame 40 until 20
// frames before the end.
void Func_0814ea58(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  struct BattleCamera *camera;
  struct EffectPosition screen;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  if (mode == 2)
    Func_0815b510(sheet, 10, 0x10000);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 0)
    palette = (s32)&ResourceId_IceBlockSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_BlastSheet;
  else
    palette = (s32)&ResourceId_VioletPaletteC;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);

  // The motes leave the middle of each target's body.
  for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
    MOTES[i].variant = -1;

  for (member = 0; member != effect->count; member++) {
    struct MotionObject *object;
    s32 half;

    object = GetBattleObjectSlotFar(effect->actors[member])->object;
    half = BattleUnit_GetEffectHeightFar(effect->actors[member]) / 2;
    for (i = 0; i != MOTES_PER_MEMBER; i++) {
      struct EffectStep *mote = &MOTES[member * MOTES_PER_MEMBER + i];
      s32 speed;

      mote->x = object->x;
      mote->y = object->y + half;
      mote->z = object->z;
      mote->velocityX = ((Random16() & 0xff) - 128) << 10;
      mote->velocityY = ((Random16() & 0xff) - 128) << 10;
      mote->velocityZ = (speed = (Random16() & 0xff) - 128) << 10;
      if (mode == 2) {
        mote->velocityX <<= 1;
        mote->velocityY <<= 1;
        mote->velocityZ = speed << 11;
      }
      mote->variant = 0;
    }
  }

  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 2) {
    // A violet ramp from color 1, blended 16:6, with a trail.
    u16 *color = BG_PLTT + 1;

    for (i = 1; i != 64; i++) {
      s32 blue = 21 - (i + 1) / 3;

      *color++ = ((21 - i / 3) << 10) | blue;
    }
    REG_BLDALPHA = 0x610;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    // Each transfer lowers the canvas's pixels by 4: an afterimage.
    work->transferMode = 3;
    work->transferValue = 0x04040404;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 half;

    user = GetBattleObjectSlotFar(effect->actor)->object;
    half = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
    if (frame == 64)
      Func_081180e8(133);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 40)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == effect->count * 20 + 52)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);
    for (member = 0; member != effect->count; member++) {
      if (frame == member * 20)
        Func_0814cd48(effect->actors[member], 7, 5, member, 42);
      if (frame > member * 20) {
        for (i = 0; i != 32; i++) {
          struct EffectStep *mote = &MOTES[member * MOTES_PER_MEMBER + i];

          if (mote->variant >= 0) {
            u8 size = Data_081982f4[mode];

            // The canvas shows at twice the width: halve x.
            EffectPosition_ProjectPoint((s32 *)mote, &screen);
            screen.x >>= 1;
            if (mode == 2) {
              draw(canvas, sheet, screen.x - size / 2, screen.y - size, size,
                   size * 2);
              BattleEffectKernels_IntegrateVector3(mote, 61, 0);
            } else {
              draw(canvas, sheet + gData_08197410[size - 1],
                   screen.x - size / 2, screen.y - size, size, size * 2);
              BattleEffectKernels_IntegrateVector3(mote, 62, 0);
            }

            // Home in on the user once ten frames out.
            if (frame > member * 20 + i + 10) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = (user->x - mote->x) >> 8;
              dy = (user->y + half - mote->y) >> 8;
              dz = (user->z - mote->z) >> 8;
              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((dx > -0x1000 && dx < 0x1000) &&
                  (dz > -0x1000 && dz < 0x1000))
                mote->variant = -1;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
