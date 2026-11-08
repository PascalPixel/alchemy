// Matches: differ 0 (tla-en, 2026-11-08); differ 0 as well in tla-ja, tla-de,
// tla-es, tla-fr and tla-it by alchemy-devtools diff, which leaves literal
// pools out, so the edition build (make compare-editions) still has to confirm
// each before adoption. Devices: two tagged dead stores (row = i >> 31, the
// twin's, and side = 0 in the party arm); the second position and the zero
// held in r8 are ordinary C, each measured at its comment.
// What closed it (2026-11-08): the test loaded the side once and gcse made a
// second register for the PANEL_B draw (its load is redone after the 72 to 75
// draw's call), then its last copy propagation pointed the first two draws
// back at the test's register, so both stayed live at the join and the copy
// (adds r3, r0, #0) stayed. A store to the tested local in either arm kills
// that copy at the join: the draws keep gcse's register, cse2 moves the test
// onto it and one register is left. Any local that is dead there works (amp
// reused, a fresh side, the store in either arm, any value, row = side = 0);
// without the store, 31. The --rtl dumps show it: 07.gcse "COPY-PROP:
// Replacing reg" for insns 509 and 571 is the line that must not appear.
// Tried before (10 at best): column = 0 or row = 0 hoisted, the else first,
// the test inverted or as !side, side through a local without the store, the
// draw through *(draw + side), once-only loops and blocks, goto, no else, and
// the permuter (over 30 minutes in all); none removed the copy.

// Effect kind 40. Twin: The Broken Seal's BattleEffect_RunCounterReveal, ported
// call for call and with its three devices (each measured here). The Lost
// Age's changes: the effect runs 104 frames, not 132, every cue 8 to 16
// frames earlier (jumps at 24 and 48, panels from 72); both pictures are
// unpacked by Resource_LoadAndDecompress (the crescents into the map buffer);
// the blitters are transparent (BattleEffect_LoadBlitter, mode 2); both
// callbacks at 0xc80; and the strike at frame 72 is Func_08118088 on the target
// with a shake of 8 and the results phase 144, where The Broken Seal throws the
// target by hand at 88 and drops it at 120; the screen shakes each frame.

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

// The blitter generator's modes, The Broken Seal's in reverse order.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ..ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 x, s32 y);           // Camera_ApplyShake
void Func_081180e8(s32 phase);              // BattlePlayback_StartFar
void Func_08118088(s32 actor, s32 variant); // ..ApplyVariantMotionFar
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffect_ArmBg2AffineHBlankDma(void);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 frames);
void Sound_PlayFar(s32 cue);

extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern u8 Data_081978f0[]; // each panel's x, by side (7 a side)
extern u8 Data_081978fe[]; // each panel's y
extern u8 gMapCellBuffer[];

// The seven panels of the counter picture: the first two in the work sheet,
// the rest in the map buffer.
#define PANEL_B 0x15d2
#define PANEL_C 0x3081
#define PANEL_D 0x0000
#define PANEL_E 0x2d80
#define PANEL_F 0x4b00
#define PANEL_G 0x58d2

// Counter reveal (kind 40): a shimmering image circles in, fades out and in
// again twice as it closes on the target, then splits into its panels one
// after another with a white flash at frame 72, when the target is thrown
// and the screen shakes. The background waves while the image fades.
void BattleEffect_RunCondemn(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  // Unused: the second position gives the stack frame its 72 bytes, as in
  // The Broken Seal's twin (one position: 4 of 549 differ, 2026-11-08).
  struct EffectPosition pos[2];
  s32 curve[2];
  s32 visible;
  s32 column;
  s32 row;
  s32 zero;
  u8 *cells;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // The zero is held in r8 through the setup calls, as in The Broken Seal's
  // twin (the literal 0 stored directly: 81 of 549 differ, 2026-11-08).
  zero = 0;

  // Set up: half-and-half blending, the image with its palette, the crescent
  // panels into the map buffer, two transparent blitters (the second
  // flipped), the wave's HBlank DMA and a transfer that clears the canvas.
  BattleEffect_StartTiledCanvas(0x2000);
  REG_BG2PA = 0x100;
  cells = gMapCellBuffer;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, cells, 1, 0);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 1;
  work->transferValue = zero;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The image starts off to the side, above the field.
  visible = 1;
  if (effect->side == 1)
    curve[0] = -0x500000;
  else
    curve[0] = 0x700000;
  curve[1] = -0x200000;

  for (frame = 0; frame != 104; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 amp;
    s32 rowBase;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame << 9;
    screenX = (curve[0] >> 16) + ((Trig_Sin(spin) << 4) >> 16) + 48;
    screenY = (curve[1] >> 16) + ((Trig_Cos(spin) << 2) >> 16) + 16;

    // It jumps closer at frame 24 and onto the target at frame 48, hidden
    // for the frame of each jump.
    if (frame == 24) {
      if (effect->side == 1)
        curve[0] = -0x200000;
      else
        curve[0] = 0x480000;
      curve[1] = 0x180000;
      visible = 0;
    }
    if (frame == 25) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }
    if (frame == 48) {
      EffectPosition_ProjectScaledBattler(effect->actors[0], pos);
      if (effect->side == 1)
        curve[0] = (pos[0].x - 128) << 16;
      else
        curve[0] = (pos[0].x - 64) << 16;
      curve[1] = 0;
      visible = 0;
    }
    if (frame == 49) {
      REG_BLDALPHA = 0x1010;
      visible = 1;
    }

    // Before each jump the image fades out over eight frames while the
    // background waves harder.
    scanline = work->bg2xLines;
    amp = 0;
    if (frame <= 23) {
      if (frame > 15) {
        amp = (frame * 2) - 32;
        REG_BLDALPHA = (31 - frame) | 0x1000;
      }
    } else if (frame <= 47) {
      if (frame > 31) {
        amp = (frame * 2) - 64;
        REG_BLDALPHA = (47 - frame) | 0x1000;
      }
    }
    if (amp < 0)
      amp = 0;

    i = 0;
    rowBase = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = rowBase - ((Trig_Sin(angle) * amp) >> 10);
      angle += 0x800;
    }

    // The image, then from frame 72 its panels, each shown for two frames.
    if (visible != 0) {
      s32 side = effect->side;

      if (side == 0) {
        column = 0;
        row = 0;
      } else {
        column = 1;
        /* FAKEMATCH(dead): i is 160 here, so this reads 0, as in The Broken
         * Seal's twin; as row = 0, 477 of 549 instructions differ in tla-en. */
        row = i >> 31;
        /* FAKEMATCH(dead): side is not read again; the store keeps gcse's copy
         * of the side from replacing the draws' index, so the test and the
         * draws share one register; as without it, 31 of 549 instructions
         * differ in tla-en. */
        side = 0;
      }
      if (frame <= 71) {
        draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                           Data_081978fe[row * 7] + screenY, 57, 98);
      } else {
        if (frame <= 75)
          draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                             Data_081978fe[row * 7] + screenY, 57, 98);
        draw[effect->side](canvas, work->sheet + PANEL_B,
                           Data_081978f0[column * 7 + 1],
                           Data_081978fe[row * 7 + 1] + screenY, 99, 69);
        if ((u32)(frame - 72) <= 1)
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if ((u32)(frame - 74) <= 1)
          draw[effect->side](canvas, work->sheet + PANEL_C,
                             Data_081978f0[column * 7 + 2],
                             Data_081978fe[row * 7 + 2] + screenY, 128, 91);
        if ((u32)(frame - 76) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_D,
                             Data_081978f0[column * 7 + 3],
                             Data_081978fe[row * 7 + 3] + screenY, 128, 91);
        if ((u32)(frame - 78) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_E,
                             Data_081978f0[column * 7 + 4],
                             Data_081978fe[row * 7 + 4] + screenY, 128, 59);
        if ((u32)(frame - 80) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_F,
                             Data_081978f0[column * 7 + 5],
                             Data_081978fe[row * 7 + 5] + screenY, 122, 29);
        if ((u32)(frame - 82) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_G,
                             Data_081978f0[column * 7 + 6],
                             Data_081978fe[row * 7 + 6] + screenY, 76, 25);
      }
    }

    // The strike: a cue at 68; at 72 the target is thrown, the screen
    // shakes and the results begin.
    if (frame == 68)
      Sound_PlayFar(212);
    if (frame == 72) {
      Func_08118088(effect->actors[0], 0);
      work->shakeFrames = 8;
      Func_081180e8(144);
    }
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  // The battle's own background (its id leads unk670) comes back.
  BattleBackground_LoadFar(1, *(u16 *)Ram_HeapSlots->battleWork->unk670, 24);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
