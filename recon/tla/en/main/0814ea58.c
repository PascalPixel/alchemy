// Draft of Func_0814ea58, the body behind BattleFx_RunDrain (mode 0, or 1 when
// the variant is not 0) and BattleFx_RunTrueCollide (mode 2).

// Twin: The Broken Seal's BattleFx_RunDrain (effect.c), rebuilt
// around a mode. Mode 0 is The Broken Seal's variant 0 (the ice palette)
// and mode 1 its other variants (the blast palette). Mode 2 is new: it
// draws a 10-pixel disc (Func_0815b510) instead of the particle sheet,
// throws the motes twice as fast, fades a violet ramp through 16:6
// blending and trails at 50 instead of darkening. The mote size comes from
// Data_081982f4 by mode (6, 6, 10) where The Broken Seal draws 6.
// ResourceId_BlastSheet and ResourceId_VioletPaletteC are rows 0x184 and
// 0x165 of directory.s (assets BattleFx_BlastSheet and
// BattleFx_VioletPaletteC), not yet named there.

// Draft: differ 246 of 479 (draft 481).

// Left: register allocation in the member loops. The native keeps the
// mote counter in r8 and spills the member's buffer offset (sp+8), with
// the effect in r10 and half the height in r9; here the counter and the
// offset still trade places in the setup loop, so the frame differs by
// spills and the rest follows.

// Tried (2026-10-15):
// - the palette chosen by nested ?: (440); by an if-else chain into a
// local resource id: 440 with the native's branch order;
// - each member's motes through an offset and a walking pointer, as
// IceShardBursts: same code (440);
// - greedy declaration-order sweep: canvas before work, the camera
// before the sheet: 426;
// - the violet ramp: (i + 1) / 3 into its own temporary, so it is
// divided first as the native does: 407; a do-while with i + 1 as the
// next counter: 424;
// - a separate counter (j) for the frame loop's motes: 282 (the setup
// loop's counter alone: 413; both: 415).
// - greedy declaration-order sweep from there, two passes: 246 (the
// declarations as they stand).

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void); // ObjectGroup_TickMemberTimers
void BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_BeginPhaseFar
void Func_0815b510(u8 *map, s32 size, s32 scale);
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 actor);

// The Lost Age's blitter modes, BattleFx_LoadBlitter's high nibble: 3 minus The
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
extern u16 gData_08197410[]; // gBattleFx_MoteCellOffsets

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
  struct BattleCamera *camera;
  struct EffectPosition screen;
  s32 palette;
  DrawRectangle draw;
  s32 member;
  u8 *sheet;
  s32 frame;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleFx_BeginCanvasLayer(1);
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
    s32 offset;
    s32 half;
    struct EffectStep *mote;

    offset = member * MOTES_PER_MEMBER * sizeof(struct EffectStep);
    object = GetBattleObjectSlotFar(effect->actors[member])->object;
    half = Battle_GetObjectTableValueFar(effect->actors[member]) / 2;
    for (i = 0, mote = (struct EffectStep *)(Ram_MapCellBuffer + offset);
         i != MOTES_PER_MEMBER; i++, mote++) {
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

  BattleFx_LoadBlitter(104, BLIT_MODE(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  if (mode == 2) {
    // A violet ramp from color 1, blended 16:6, with a trail.
    for (i = 1; i != 64; i++) {
      s32 blue = 21 - (i + 1) / 3;

      BG_PLTT[i] = ((21 - i / 3) << 10) | blue;
    }
    REG_BLDALPHA = 0x610;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    // Each transfer lowers the canvas's pixels by 4: an afterimage.
    work->transferMode = 3;
    work->transferValue = 0x04040404;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  Audio_PlayCue(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    struct MotionObject *user;
    s32 half;

    user = GetBattleObjectSlotFar(effect->actor)->object;
    half = Battle_GetObjectTableValueFar(effect->actor) / 2;
    if (frame == 64)
      Func_081180e8(133);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    if (frame == 40)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == effect->count * 20 + 52)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);
    for (member = 0; member != effect->count; member++) {
      s32 offset;

      offset = member * MOTES_PER_MEMBER * sizeof(struct EffectStep);
      if (frame == member * 20)
        Func_0814cd48(effect->actors[member], 7, 5, member, 42);
      if (frame > member * 20) {
        struct EffectStep *mote;

        for (j = 0, mote = (struct EffectStep *)(Ram_MapCellBuffer + offset);
             j != 32; j++, mote++) {

          if (mote->variant >= 0) {
            u32 size = Data_081982f4[mode];

            // The canvas shows at twice the width: halve x.
            EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
            screen.x >>= 1;
            if (mode == 2) {
              draw(canvas, sheet, screen.x - size / 2, screen.y - size, size,
                   size * 2);
              BattleFxKernels_IntegrateVector3(mote, 61, 0);
            } else {
              draw(canvas, sheet + gData_08197410[size - 1],
                   screen.x - size / 2, screen.y - size, size, size * 2);
              BattleFxKernels_IntegrateVector3(mote, 62, 0);
            }

            // Home in on the user once ten frames out.
            if (frame > member * 20 + j + 10) {
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
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}
