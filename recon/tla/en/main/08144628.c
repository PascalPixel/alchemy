// Draft, differ 69 of 511, skeleton gap 5/4 (tla-en, 2026-11-01)
//
// The body of four effect kinds (BattleEffect_RunDemonNight and the next three
// wrappers pass modes 0 to 3). Twin: The Broken Seal's
// BattleEffect_RunGatheringLights, whose modes 0 to 2 these are (same setup,
// three images and 20x32 draw, 128 slots per target, 20-frame turns, frame 80
// damage, frames 70 and 71 tint and sound, gravity call with 62, 0). The Lost
// Age adds mode 3 and reworks the effect: the picture unpacked by
// Resource_LoadAndDecompress, the palette for mode 0 or 3 the pink burst's,
// Func_0815b634 in mode 3, BattleEffect_FetchRectangleBlitters for the caster's
// side; the background's wave moves BG1 (BattleEffect_ArmBg1HBlankDma arms its
// HBlank DMA) by an amplitude that swells and dies over the effect; motes are
// pulled twice as hard across (x and z) and freed once they cross their target,
// not once they are near it; in mode 3 each arrival pushes the target back
// (BattleEffect_StartActorMotion) with a cue on every other mote. The edition
// level is "struct": the other editions' scaffolds hold this function in
// another shape; check them before adopting.

// Left: the listing is one instruction longer, which alone moves every pool
// load and branch offset after it (most of the 69 rows are those).
// - &pos: movs r3, #80 and add r3, sp in the native, add r3, sp, #80 here. Reload
//   writes the two instructions in both; the peephole joins them here because
//   they stay adjacent. The native's sched2 puts ldr r0, [sp, #20] between them,
//   which needs that load's priority above the add's (here 4 against 5).
// - moteDistance: the native compares and negates the copy (cmp r2, negs r2, r2);
//   CSE here puts x in its place. CSE keeps the copy only when the copy outlives
//   x: giving it a later use reproduces the form and wrecks allocation (310).
// - single swaps: str [sp, #36] with its neighbours, the two Transform_SetLookAt
//   argument loads, lsls r0, #19 with the pool load of the copy routine.

// Tried (2026-11-01, from the 36-of-511, gap 7/7 draft): the crossing test as a
// || of two && arms, each storing its sign test in crossed: both the store-flag
// and the threaded jumps now match (the assignment keeps the value alive; the
// plain tests, inline predicates, a ternary, reusing the x local give 90 or
// 100). moteDistance as a copy tested by if, an inline, __builtin_abs, or from
// mote->x with no x local (no change), shared with the wave loop's x (no
// change), the distance test repeated in each arm (86). A pointer to pos at
// any scope (79 or 129). The camera as a struct of eye and focus, the palette
// source in a local, the twin's [][2] table (no change); k * 20 in a local
// (171); the index spellings (69 to 71).

// Tried (2026-10-15): the plain port 261; the BG1 scroll read once before
// the line loop 143; the frame-80 phase as else-if with the call repeated for
// mode 3 (one load of mode per test, as the native; 108 from 123 with
// mode == 0 || mode == 3, 129 as a ternary argument); sin * sway, not
// sway * sin (4 rows).

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "bg_scroll.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// The motes live in the map buffer, 128 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);      // ..ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // BattleEffect_TintBattler
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Func_0815b634(void); // mode 3 only
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Sound_PlayFar(s32 cue);

extern char ResourceId_MemberBurstImage;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 Data_08197864[];

#define MOTES_PER_TARGET 128
#define MOTE_CELL (20 * 32)

// Member burst (kinds through four wrappers; mode picks the colours): from
// the caster's body, 128 motes per target scatter every way, and from 30
// frames on each is pulled toward the middle of its target's body, vanishing
// once it has crossed it. The background sways sideways, swelling and dying
// away over the effect. In mode 3 every mote that arrives also pushes its
// target back.
void Func_08144628(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition pos;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 *camera;
  s32 palette;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the mote picture, the mode's palette, and the max and additive
  // blitters for the caster's side.
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_MemberBurstImage, work->sheet, 0,
                             0);
  if (mode == 0 || mode == 3)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 1)
    palette = (s32)&ResourceId_MarsDjinnSheet;
  else
    palette = (s32)&ResourceId_MercuryDjinnSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
  if (mode == 3)
    Func_0815b634();
  BattleEffect_FetchRectangleBlitters(effect->side, draw);

  // The motes start at the caster's body height and scatter every way at up
  // to two pixels a frame.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *source = GetBattleObjectSlotFar(effect->actor)->object;
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    for (i = 0; i != MOTES_PER_TARGET; i++) {
      struct EffectStep *mote = &PARTICLES[k * 128 + i];

      mote->x = source->x;
      mote->y = height;
      mote->z = source->z;
      mote->velocityX = ((Random16() & 255) - 128) << 10;
      mote->velocityY = ((Random16() & 255) - 128) << 10;
      mote->velocityZ = ((Random16() & 255) - 128) << 10;
      mote->variant = 0;
    }
  }

  Task_Add(BattleEffect_ArmBg1HBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(146);

  for (frame = 0; frame != Data_08197864[mode * 2 + 1] + effect->count * 20;
       frame++) {
    s32 *row;
    s32 sway;
    s32 scrollX;

    if (frame == 80) {
      if (mode == 0)
        Func_081180e8(134);
      else if (mode == 3)
        Func_081180e8(134);
      else
        Func_081180e8(133);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // BG1 sways line by line, by up to 16 pixels at the effect's midpoint.
    // A negative scroll wraps into the halfword one short of 0x10000.
    row = work->bg2xLines;
    sway = Trig_Sin((frame << 15) /
                    (Data_08197864[mode * 2 + 1] + effect->count * 20)) *
               16 >>
           16;
    scrollX = gBgScroll[1].x;
    for (i = 0; i != 160; i++) {
      s32 x = (Trig_Sin((frame + i) << 10) * sway >> 16) + scrollX;

      if (x < 0)
        x += 0xffff;
      *row++ = x | 32 << 16;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 half = BattleUnit_GetEffectHeightFar(effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          Sound_PlayFar(134);
        else if (mode != 3)
          Sound_PlayFar(133);
      }
      if (frame == k * 20 + 70)
        Func_0814cd48(effect->actors[k], 7, 5, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != Data_08197864[mode * 2]; i++) {
          struct EffectStep *mote = &PARTICLES[k * 128 + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &pos);
            pos.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * MOTE_CELL, pos.x - 10,
                    pos.y - 16, 20, 32);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);

            // Thirty frames on, the mote is pulled toward the middle of its
            // target's body.
            if (frame > k * 20 + i + 30) {
              s32 x = mote->x;
              s32 dx = (target->x - x) >> 8;
              s32 dy = (target->y + half - mote->y) >> 9;
              s32 dz = (target->z - mote->z) >> 8;
              s32 targetDistance;
              s32 moteDistance;
              s32 crossed;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              // It has crossed once it is as far out from the middle as its
              // target is, on the target's side.
              targetDistance = target->x < 0 ? -target->x : target->x;
              moteDistance = x < 0 ? -x : x;
              if (targetDistance <= moteDistance &&
                  ((x < 0 && (crossed = target->x < 0)) ||
                   (x >= 0 && (crossed = target->x >= 0)))) {
                mote->variant = -1;
                if (mode == 3) {
                  BattleEffect_StartActorMotion(effect->actors[k], 1, 0x20000,
                                            0x20000, 0x20000, i * 8 + 100);
                  if ((i & 1) && frame != 80)
                    Sound_PlayFar(134);
                }
              }
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
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ArmBg1HBlankDma);
  BattleEffect_EndCanvasLayer();
}
