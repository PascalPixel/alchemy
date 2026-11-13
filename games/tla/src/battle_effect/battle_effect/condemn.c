/// @file
/// Condemn.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "module/battle_effect/battle_effect.h"
#include "resource_ids.h"

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
        /* FAKEMATCH(dead): i is 160 here, so i >> 31 folds to 0 and only
         * changes allocation; as row = 0, 477 of 549 instructions differ in
         * tla-en. */
        row = i >> 31;
        /* FAKEMATCH(dead): side = 0, never read, keeps the side's copy from
         * replacing the draws' index, so the test and the draws share one
         * register; without it, 31 of 549 instructions differ in tla-en. */
        side = 0;
      }
      if (frame <= 71) {
        draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                           gCondemn_PanelY[row * 7] + screenY, 57, 98);
      } else {
        if (frame <= 75)
          draw[effect->side](canvas, work->sheet, Data_081978f0[column * 7],
                             gCondemn_PanelY[row * 7] + screenY, 57, 98);
        draw[effect->side](canvas, work->sheet + PANEL_B,
                           Data_081978f0[column * 7 + 1],
                           gCondemn_PanelY[row * 7 + 1] + screenY, 99, 69);
        if ((u32)(frame - 72) <= 1)
          Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
        if ((u32)(frame - 74) <= 1)
          draw[effect->side](canvas, work->sheet + PANEL_C,
                             Data_081978f0[column * 7 + 2],
                             gCondemn_PanelY[row * 7 + 2] + screenY, 128, 91);
        if ((u32)(frame - 76) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_D,
                             Data_081978f0[column * 7 + 3],
                             gCondemn_PanelY[row * 7 + 3] + screenY, 128, 91);
        if ((u32)(frame - 78) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_E,
                             Data_081978f0[column * 7 + 4],
                             gCondemn_PanelY[row * 7 + 4] + screenY, 128, 59);
        if ((u32)(frame - 80) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_F,
                             Data_081978f0[column * 7 + 5],
                             gCondemn_PanelY[row * 7 + 5] + screenY, 122, 29);
        if ((u32)(frame - 82) <= 1)
          draw[effect->side](canvas, gMapCellBuffer + PANEL_G,
                             Data_081978f0[column * 7 + 6],
                             gCondemn_PanelY[row * 7 + 6] + screenY, 76, 25);
      }
    }

    // The strike: a cue at 68; at 72 the target is thrown, the screen
    // shakes and the results begin.
    if (frame == 68)
      Sound_PlayFar(212);
    if (frame == 72) {
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 0);
      work->shakeFrames = 8;
      BattlePlayback_StartFar(144);
    }
    BattleCamera_Shake(8, 8);
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
