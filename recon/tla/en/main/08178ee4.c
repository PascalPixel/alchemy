/* Draft (tla-en): listing 525 instructions, draft 525, common 488, differ 74.
 * Written from the listing (☀️'s BattleFx_RunCounterReveal is only a
 * hint). A large sprite assembled from gMapCellBuffer cells sways on a
 * sine path over 108 frames, faded in and out through BLDALPHA, with two
 * blitters (heap slots 104 and 188) chosen by the effect's side.
 * Devices already in (each a one-pass block): one before the heap loads so
 * effect's spill comes first, as the native; one between BG2PA's pinned
 * 0x100 and its store so the pool load comes before the address.
 * Left:
 * - the native loads gMapCellBuffer into r6 before BattleFx_BeginTiledCanvas
 *   (here after it).
 * - both Func_081963ec calls set r1 before r0 in the native; sched2 breaks
 *   the tie by RTL order here (the two moves are symmetric in the
 *   dependence lists).
 * - the stores of the blitters' stack arguments and the side index's
 *   "lsls r1, r1, #2" are one row apart in several calls, and the
 *   halfword pool loads of the 16 in the BLDALPHA fades come one row
 *   earlier in the native.
 * Resource 220 needs its name in system/resource/directory.s when this is
 * adopted. The cell addresses are gMapCellBuffer offsets (the listing's
 * pool names them Data_020124de, Data_02011f20, Data_02012e6e and
 * Data_02014e9d).
 * 2026-10-15 (lane N2): rewritten with the work struct, named parts and
 * constants, comments and tagged devices; the same 525 instructions and
 * differ 74. */
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"

#define HEAP_SLOT_BLITTER 104           // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_BLITTER_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3; 2 << 4 | 3 is transparent and
// clipped, and | 4 mirrors it.
#define BLITTER_TRANSPARENT 2
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP_XY 3
#define FLIP_X 4

#define FIGURE_FRAMES 108
#define FIGURE_SWING_FRAME 30 // the first blow
#define FIGURE_HIT_FRAME 80   // the second, which starts the result phase

// The parts of the figure in Resource_Data0DC, unpacked into the map cells.
#define FIGURE_BODY 0            // 96x83
#define FIGURE_ARM_LOWERED 0x1f20 // 42x35
#define FIGURE_ARM_RAISED 0x24de  // 48x51
#define FIGURE_SWING 0x2e6e      // four cels, Data_081993d0 to d8
#define FIGURE_FIST 0x4e9d       // 32x29, then eight cels in Data_081993e0..f0

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_Data0DC;

// The swing's cels: widths, heights and offsets; the fist's: heights,
// y offsets and offsets.
extern const u8 gData_081993d0[];
extern const u8 gData_081993d4[];
extern const u16 gData_081993d8[];
extern const u8 gData_081993e0[];
extern const u8 gData_081993e8[];
extern const u16 gData_081993f0[];

void BattleFx_BeginTiledCanvas(s32 bgControl);
void Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
s32 Scheduler_RemoveCallback(s32 callback);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer,
                   s32 frames);
void Func_081963ec(s32 slot, s32 blitter);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Func_08143000(void);
void Func_08143bb8(void);

// A great figure strikes. It sweeps down on the target from above the far
// side, swaying on a small circle and fading in through the blend; its arm
// is raised, and at frame 30 it swings down through four cels and the
// screen flashes white; from frame 40 its fist falls through eight cels,
// and at frame 80 the blow lands and the result phase starts. From frame
// 92 it fades out and rises away; the battle background is reloaded at the
// end. On the far side the whole figure is drawn mirrored.
void Func_08178ee4(struct BattleEffectArgument *effect) {
  struct HeapSlots *heap;
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 *parts;
  DrawRectangle draw[2];
  struct EffectPosition target;
  s32 frame;
  s32 x;
  s32 y;

  /* FAKEMATCH: the native spills the effect argument before it loads the
   * heap slots; this one-pass block ends the block there. */
  do {
  } while (0);
  heap = Ram_HeapSlots;
  work = (struct BattleEffectWork *)heap->battleFxWork[0];
  canvas = heap->battleFxWork[1];
  parts = Ram_MapCellBuffer;
  BattleFx_BeginTiledCanvas(0x2001);
  {
    /* FAKEMATCH: the native loads BG2PA's 0x100 from the pool before it
     * starts the register address; pinned behind a one-pass block it does
     * the same (plain: one reordered pair). */
    register u16 scale __asm__("r3") = 0x100;

    do {
    } while (0);
    REG_BG2PA = scale;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DC, parts, 1, 1);
  Func_081963ec(HEAP_SLOT_BLITTER, BLITTER(BLITTER_TRANSPARENT, CLIP_XY));
  draw[0] = (DrawRectangle)heap->unk64[1];
  Func_081963ec(HEAP_SLOT_BLITTER_ALTERNATE,
                BLITTER(BLITTER_TRANSPARENT, CLIP_XY | FLIP_X));
  draw[1] = ((DrawRectangle *)heap)[47];
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback((s32)Func_08143000, 0xc80);
  EffectPosition_ApplyStepAndYOffset(effect->actors[0], &target);

  for (frame = 0; frame != FIGURE_FRAMES; frame++) {
    // It starts 100 pixels above the canvas and to the target's far side,
    // and comes down at it for 32 frames.
    if (frame == 0) {
      if (effect->side == 0)
        work->particles[0].x = (target.x + 40) << 16;
      else
        work->particles[0].x = (target.x - 168) << 16;
      work->particles[0].y = -100 << 16;
      if (effect->side == 0)
        work->particles[0].velocityX = -6 << 16;
      else
        work->particles[0].velocityX = 6 << 16;
      work->particles[0].velocityY = 8 << 16;
      REG_BLDALPHA = 0x1000;
    }
    if (frame < 32)
      BattleFxKernels_IntegrateVector2(&work->particles[0], 60, 0);

    // In over 32 frames, out over the last 16, rising away.
    if (frame <= 32) {
      s32 level = frame / 2;

      REG_BLDALPHA = ((16 - level / 2) << 8) | level;
    }
    if (frame > 91) {
      s32 level = FIGURE_FRAMES - frame;

      REG_BLDALPHA = ((16 - level / 2) << 8) | level;
      BattleFxKernels_IntegrateVector2(&work->particles[0], 60, -0x8000);
    }

    // The sway, and the canvas moved with the figure.
    x = HI(work->particles[0].x) + ((Trig_Sin(frame << 8) << 3) >> 16);
    y = HI(work->particles[0].y) + ((Trig_Cos(frame << 8) << 3) >> 16);
    REG_BG2X = -x << 8;

    // The body, and the arm raised until frame 48.
    if (effect->side == 0) {
      draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 25, y + 2,
                         96, 83);
      if ((u32)frame < 48)
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_RAISED, 31,
                           y + 22, 48, 51);
      else
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_LOWERED, 64,
                           y + 16, 42, 35);
    } else {
      draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_BODY, 7, y + 2,
                         96, 83);
      if ((u32)frame < 48)
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_RAISED, 49,
                           y + 22, 48, 51);
      else
        draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_ARM_LOWERED, 22,
                           y + 16, 42, 35);
    }

    // The swing, three frames a cel from frame 28.
    if (frame >= 28 && frame < 40) {
      s32 cel = (frame - 28) / 3;

      if (effect->side == 0) {
        draw[effect->side](
            canvas, Ram_MapCellBuffer + FIGURE_SWING + gData_081993d8[cel],
            46 - gData_081993d0[cel] / 2, y - gData_081993d4[cel] / 2 + 61,
            gData_081993d0[cel], gData_081993d4[cel]);
      } else {
        draw[effect->side](
            canvas, Ram_MapCellBuffer + FIGURE_SWING + gData_081993d8[cel],
            gData_081993d0[cel] / 2 - gData_081993d0[cel] + 82,
            y - gData_081993d4[cel] / 2 + 61, gData_081993d0[cel],
            gData_081993d4[cel]);
      }
    }

    // The fist: raised to frame 47, then falling six frames a cel from 56.
    if (frame > 39) {
      if (frame <= 47) {
        if (effect->side == 0)
          draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_FIST, 30,
                             y + 46, 32, 29);
        else
          draw[effect->side](canvas, Ram_MapCellBuffer + FIGURE_FIST, 66,
                             y + 46, 32, 29);
      } else {
        s32 cel = (frame - 56) / 6;

        if (cel < 0)
          cel = 0;
        if (cel <= 7) {
          if (effect->side == 0)
            draw[effect->side](
                canvas, Ram_MapCellBuffer + FIGURE_FIST + gData_081993f0[cel],
                63, y + gData_081993e8[cel] - 8, 32, gData_081993e0[cel]);
          else
            draw[effect->side](
                canvas, Ram_MapCellBuffer + FIGURE_FIST + gData_081993f0[cel],
                33, y + gData_081993e8[cel] - 8, 32, gData_081993e0[cel]);
        }
      }
    }

    if (frame == FIGURE_HIT_FRAME) {
      work->shakeFrames = 6;
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
    }
    if (frame == FIGURE_SWING_FRAME) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->shakeFrames = 3;
      Audio_PlayCue(212);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
    }

    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback((s32)Func_08143000);
  BattleBackground_LoadFar(
      1, *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670), 24);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Func_08143bb8();
}
