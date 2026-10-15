/* Near miss, differ 70 of 511.
 *
 * The body of four effect kinds (Func_081445f8 and the next three wrappers
 * pass modes 0 to 3). Twin: The Broken Seal's BattleFx_RunMemberBurst, whose
 * modes 0 to 2 these are; The Lost Age adds mode 3 and reworks the effect:
 * the picture unpacked by Resource_LoadAndDecompress, the palette for mode 0
 * or 3 the pink burst's, Func_0815b634 in mode 3, Func_08144aac for the
 * caster's side; the background's wave moves BG1 (Func_081524b4 arms its
 * HBlank DMA) by an amplitude that swells and dies over the effect; motes are
 * pulled twice as hard across (x and z) and freed once they cross their
 * target, not once they are near it; in mode 3 each arrival pushes the
 * target back (BattleFx_StartActorMotion) with a cue on every other mote.
 * The edition level is "struct": the other editions' scaffolds hold this
 * function in another shape; check them before adopting.
 *
 * Left: order, and one form. The crossing test's first arm: the native
 * keeps target->x < 0 as a store-flag (lsrs #31, then cmp and bne); agscc
 * threads it into a branch on target->x (about 10 rows); the else arm
 * matches. &pos is movs r3, #80; add r3, sp in the native and add r3, sp,
 * #80 here; the rest are single swaps of a load with its neighbour.
 *
 * Tried (2026-10-15): the plain port 261; the BG1 scroll read once before
 * the line loop 143; the crossing test as a flag set in an if, else 0 (from
 * 123 as a ternary in the condition, 104 as a flag set after crossed = 0,
 * 81 with the arms swapped, 125 or 129 with the arms written as shifts);
 * the frame-80 phase as else-if with the call repeated for mode 3 (one
 * load of mode per test, as the native; 108 from 123 with mode == 0 ||
 * mode == 3, 129 as a ternary argument); sin * sway, not sway * sin (4
 * rows); the absolute value of x through a copy (no change).
 */
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
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void);      // BattleFx_EndCanvasLayer
void Func_08143000(void);      // ..ProcessPendingGraphicsTransfer
void Func_081434f8(void);      // ObjectGroup_TickMemberTimers
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void Func_08144aac(s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_081524b4(void); // arms the BG1 scroll's HBlank DMA
void Func_0815b634(void); // mode 3 only
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
s32 Battle_GetObjectTableValueFar(s32 unit);
void BattleFx_StartActorMotion(s32 id, s32 force, s32 acceleration,
                               s32 speedLimit, s32 velocityY, s32 percent);
void Audio_PlayCue(s32 cue);

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
  BattleFx_BeginCanvasLayer(1);
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
  Func_08144aac(effect->side, draw);

  // The motes start at the caster's body height and scatter every way at up
  // to two pixels a frame.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *source = GetBattleObjectSlotFar(effect->actor)->object;
    s32 height = Battle_GetObjectTableValueFar(effect->actor);

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

  Scheduler_AddOrUpdateCallback(Func_081524b4, 0xc80);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  Audio_PlayCue(146);

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
    Graphics_PrepareTransferInIwramWork(camera, camera + 3);

    // BG1 sways line by line, by up to 16 pixels at the effect's midpoint.
    // A negative scroll wraps into the halfword one short of 0x10000.
    row = work->bg2X;
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
      s32 half = Battle_GetObjectTableValueFar(effect->actors[k]) / 2;

      if (frame == k * 20 + 71) {
        if (mode == 0)
          Audio_PlayCue(134);
        else if (mode != 3)
          Audio_PlayCue(133);
      }
      if (frame == k * 20 + 70)
        Func_0814cd48(effect->actors[k], 7, 5, k, 26);
      if (frame > k * 20) {
        for (i = 0; i != Data_08197864[mode * 2]; i++) {
          struct EffectStep *mote = &PARTICLES[k * 128 + i];

          if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
            EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &pos);
            pos.x >>= 1;
            draw[0](canvas, work->sheet + i % 3 * MOTE_CELL, pos.x - 10,
                    pos.y - 16, 20, 32);
            BattleFxKernels_IntegrateVector3(mote, 62, 0);

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
              if (targetDistance <= moteDistance) {
                if (x < 0)
                  crossed = target->x < 0;
                else
                  crossed = target->x >= 0;
              } else {
                crossed = 0;
              }
              if (crossed) {
                mote->variant = -1;
                if (mode == 3) {
                  BattleFx_StartActorMotion(effect->actors[k], 1, 0x20000,
                                            0x20000, 0x20000, i * 8 + 100);
                  if ((i & 1) && frame != 80)
                    Audio_PlayCue(134);
                }
              }
            }
          }
        }
      }
    }
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Scheduler_RemoveCallback(Func_081524b4);
  Func_08143bb8();
}
