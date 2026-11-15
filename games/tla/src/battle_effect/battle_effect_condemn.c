/// @file
/// Condemn.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

/// The seven panels of the counter picture: the first two in the work sheet,
/// the rest in the map buffer.
#define PANEL_B 0x15d2
#define PANEL_C 0x3081
#define PANEL_D 0x0000
#define PANEL_E 0x2d80
#define PANEL_F 0x4b00
#define PANEL_G 0x58d2

/// A shimmering image circles in, fading out and in as it closes on the first
/// target, then splits into its panels with a white flash, and the target is
/// thrown back while the screen shakes.
///
/// Abilities: Condemn.
void BattleEffect_RunCondemn(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  // Unused: the second position gives the stack frame its 72 bytes,.
  struct ScreenPoint pos[2];
  s32 curve[2];
  s32 visible;
  s32 column;
  s32 row;
  s32 zero;
  u8 *cells;
  s32 frame;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;

  zero = 0;

  // Set up: half-and-half blending, the image with its palette, the crescent
  // panels into the map buffer, two transparent blitters (the second
  // flipped), the wave's HBlank DMA and a transfer that clears the canvas.
  BattleEffect_StartTiledCanvas(0x2000);
  REG_BG2PA = Q_8_8(1);
  cells = gMapGridCellBuffer;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, cells, 1, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, zero);

  // The image starts off to the side, above the field.
  visible = 1;
  if (args->side == EFFECT_SIDE_PARTY)
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
    screenX =
        (Q_16_16_TO_INT(curve[0])) + (Q_16_16_TO_INT(Trig_Sin(spin) << 4)) + 48;
    screenY =
        (Q_16_16_TO_INT(curve[1])) + (Q_16_16_TO_INT(Trig_Cos(spin) << 2)) + 16;

    // It jumps closer at frame 24 and onto the target at frame 48, hidden
    // for the frame of each jump.
    if (frame == 24) {
      if (args->side == EFFECT_SIDE_PARTY)
        curve[0] = -0x200000;
      else
        curve[0] = 0x480000;
      curve[1] = 0x180000;
      visible = 0;
    }
    if (frame == 25) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      visible = 1;
    }
    if (frame == 48) {
      EffectPosition_ProjectScaledBattler(args->targets[0], pos);
      if (args->side == EFFECT_SIDE_PARTY)
        curve[0] = px(pos[0].x - 128);
      else
        curve[0] = px(pos[0].x - 64);
      curve[1] = 0;
      visible = 0;
    }
    if (frame == 49) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      visible = 1;
    }

    // Before each jump the image fades out over eight frames while the
    // background waves harder.
    scanline = work->bg2xLines;
    amp = 0;
    if (frame <= 23) {
      if (frame > 15) {
        amp = (frame * 2) - 32;
        REG_BLDALPHA = BLEND_ALPHA(31 - frame, BLEND_WEIGHT_MAX);
      }
    } else if (frame <= 47) {
      if (frame > 31) {
        amp = (frame * 2) - 64;
        REG_BLDALPHA = BLEND_ALPHA(47 - frame, BLEND_WEIGHT_MAX);
      }
    }
    if (amp < 0)
      amp = 0;

    i = 0;
    rowBase = (6 - screenX) << 8;
    angle = frame << 11;
    for (; i != 160; i++) {
      *scanline++ = rowBase - ((Trig_Sin(angle) * amp) >> 10);
      angle += TRIG_TURN / 32;
    }

    // The image, then from frame 72 its panels, each shown for two frames.
    if (visible != 0) {
      s32 side = args->side;

      if (side == 0) {
        column = 0;
        row = 0;
      } else {
        column = 1;
        /* FAKEMATCH(dead): i is 160 here. */
        row = i >> 31;
        /* FAKEMATCH(dead): side = 0, never read. */
        side = 0;
      }
      if (frame <= 71) {
        draw[args->side](canvas, work->sheet, gCondemn_PictureX[column * 7],
                         gCondemn_PictureY[row * 7] + screenY, 57, 98);
      } else {
        if (frame <= 75)
          draw[args->side](canvas, work->sheet, gCondemn_PictureX[column * 7],
                           gCondemn_PictureY[row * 7] + screenY, 57, 98);
        draw[args->side](canvas, work->sheet + PANEL_B,
                         gCondemn_PictureX[column * 7 + 1],
                         gCondemn_PictureY[row * 7 + 1] + screenY, 99, 69);
        if ((u32)(frame - 72) <= 1)
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
        if ((u32)(frame - 74) <= 1)
          draw[args->side](canvas, work->sheet + PANEL_C,
                           gCondemn_PictureX[column * 7 + 2],
                           gCondemn_PictureY[row * 7 + 2] + screenY, 128, 91);
        if ((u32)(frame - 76) <= 1)
          draw[args->side](canvas, gMapGridCellBuffer + PANEL_D,
                           gCondemn_PictureX[column * 7 + 3],
                           gCondemn_PictureY[row * 7 + 3] + screenY, 128, 91);
        if ((u32)(frame - 78) <= 1)
          draw[args->side](canvas, gMapGridCellBuffer + PANEL_E,
                           gCondemn_PictureX[column * 7 + 4],
                           gCondemn_PictureY[row * 7 + 4] + screenY, 128, 59);
        if ((u32)(frame - 80) <= 1)
          draw[args->side](canvas, gMapGridCellBuffer + PANEL_F,
                           gCondemn_PictureX[column * 7 + 5],
                           gCondemn_PictureY[row * 7 + 5] + screenY, 122, 29);
        if ((u32)(frame - 82) <= 1)
          draw[args->side](canvas, gMapGridCellBuffer + PANEL_G,
                           gCondemn_PictureX[column * 7 + 6],
                           gCondemn_PictureY[row * 7 + 6] + screenY, 76, 25);
      }
    }

    // The strike: a cue at 68; at 72 the target is thrown, the screen
    // shakes and the results begin.
    if (frame == 68)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == 72) {
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      work->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  // The battle's own background (its id leads unk670) comes back.
  BattleBackground_LoadFar(1, Ram_HeapSlots->battleWork->background, 24);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}
