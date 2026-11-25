/// @file
/// Condemn.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "resource_ids.h"

#define CONDEMN_FRAMES 104
#define CONDEMN_SWING_FRAME 72 ///< the reaper swings, in a white flash
/// How far the reaper sways across and up and down as it circles, in pixels.
#define CONDEMN_SWAY_X 16
#define CONDEMN_SWAY_Y 4

/// The pictures Condemn draws: the reaper raised and swinging, the crescent
/// its scythe leaves and the crescent's four fades.
#define CONDEMN_PICTURES 7
/// Where `picture`, one of Condemn's pictures, is drawn, by the casting side's
/// `column` and `row`.
#define CONDEMN_PICTURE_X(column, picture)                                     \
  gCondemn_PictureX[(column) * CONDEMN_PICTURES + (picture)]
#define CONDEMN_PICTURE_Y(row, picture)                                        \
  gCondemn_PictureY[(row) * CONDEMN_PICTURES + (picture)]

/// A hooded reaper with a scythe circles in, fading out through a ripple as it
/// jumps closer twice, the second time onto the first target; the screen
/// flashes white, the reaper swings, and the red crescent its scythe leaves
/// fades while the target is knocked back and the screen shakes.
///
/// Abilities: Condemn.
void BattleEffect_RunCondemn(struct BattleEffectParams *args) {
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  s32 transferValue = 0; // what the canvas is cleared to after each copy
  BlitterPair draw;
  /* LEFTOVER(frame): position[1], which the ROM's frame reserves and never
   * touches; as one struct ScreenPoint. */
  struct ScreenPoint position[2];
  FIXED swayCenter[2];
  s32 drawReaper;
  s32 pictureColumn;
  s32 pictureRow;
  u8 *cells; // the cell buffer, held across the reveal sheet's load
  s32 frame;

  BattleEffect_StartTiledCanvas(BGCNT_WRAP);
  REG_BG2PA = Q_8_8(1);
  cells = gMapGridCellBuffer;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_COUNTER_REVEAL_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_RED_CRESCENT_SHEET_A, cells, 1, 0);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      BLITTER_TRANSPARENT);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, transferValue);

  // The reaper starts off to the side, above the field.
  drawReaper = 1;
  if (args->side == EFFECT_SIDE_PARTY)
    swayCenter[0] = -toFIXED(80);
  else
    swayCenter[0] = toFIXED(112);
  swayCenter[1] = -toFIXED(32);

  for (frame = 0; frame != CONDEMN_FRAMES; frame++) {
    s32 spin;
    s32 screenX;
    s32 screenY;
    s32 ripple;
    s32 lineOffset;
    s32 angle;
    s32 i;
    s32 *scanline;

    spin = frame * (TRIG_TURN / 128);
    screenX = FIXEDtoINT(swayCenter[0]) +
              FIXEDtoINT(Trig_Sin(spin) * CONDEMN_SWAY_X) + 48;
    screenY = FIXEDtoINT(swayCenter[1]) +
              FIXEDtoINT(Trig_Cos(spin) * CONDEMN_SWAY_Y) + 16;

    // The second pass, lower and further across, hidden for the frame of the
    // jump.
    if (frame == 24) {
      if (args->side == EFFECT_SIDE_PARTY)
        swayCenter[0] = -toFIXED(32);
      else
        swayCenter[0] = toFIXED(72);
      swayCenter[1] = toFIXED(24);
      drawReaper = 0;
    }
    if (frame == 25) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      drawReaper = 1;
    }
    // The last pass, on the first target.
    if (frame == 48) {
      EffectPosition_ProjectScaledBattler(args->targets[0], position);
      if (args->side == EFFECT_SIDE_PARTY)
        swayCenter[0] = INTtoFIXED(position[0].x - 128);
      else
        swayCenter[0] = INTtoFIXED(position[0].x - 64);
      swayCenter[1] = 0;
      drawReaper = 0;
    }
    if (frame == 49) {
      REG_BLDALPHA = EFFECT_BLEND_ADD;
      drawReaper = 1;
    }

    // Each of the first two passes ripples harder as the reaper fades out
    // over its last eight frames.
    scanline = state->bg2xLines;
    ripple = 0;
    if (frame <= 23) {
      if (frame > 15) {
        ripple = (frame * 2) - 32;
        REG_BLDALPHA = EFFECT_BLEND_CANVAS(31 - frame);
      }
    } else if (frame <= 47) {
      if (frame > 31) {
        ripple = (frame * 2) - 64;
        REG_BLDALPHA = EFFECT_BLEND_CANVAS(47 - frame);
      }
    }
    if (ripple < 0)
      ripple = 0;

    // The reaper moves by scrolling the canvas line by line, not by where it
    // is drawn.
    i = 0;
    lineOffset = (6 - screenX) << 8;
    angle = frame * (TRIG_TURN / 32);
    for (; i != DISPLAY_HEIGHT; i++) {
      *scanline++ = lineOffset - CANVAS_WAVE_BG2X(Trig_Sin(angle) * ripple);
      angle += TRIG_TURN / 32;
    }

    // The reaper, then from the swing its swung pose and the crescent fading,
    // each picture shown for two frames.
    if (drawReaper != 0) {
      /* FAKEMATCH(temporary): side, the side copied into a local for the test.
       */
      s32 side = args->side;

      if (side == EFFECT_SIDE_ENEMIES) {
        pictureColumn = 0;
        pictureRow = 0;
      } else {
        pictureColumn = 1;
        /* FAKEMATCH(dead): i is 160 here. */
        pictureRow = i >> 31;
        /* FAKEMATCH(dead): side set again, never read. */
        side = EFFECT_SIDE_ENEMIES;
      }
      if (frame < CONDEMN_SWING_FRAME) {
        draw[args->side](canvas, state->sheet,
                         CONDEMN_PICTURE_X(pictureColumn, 0),
                         CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
                         REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
      } else {
        if (frame <= CONDEMN_SWING_FRAME + 3)
          draw[args->side](
              canvas, state->sheet, CONDEMN_PICTURE_X(pictureColumn, 0),
              CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
              REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
        EFFECT_DRAW_PICTURE(draw[args->side], canvas, state->sheet,
                            REAPER_SHEET_SWUNG,
                            CONDEMN_PICTURE_X(pictureColumn, 1),
                            CONDEMN_PICTURE_Y(pictureRow, 1) + screenY);
        if (frame >= CONDEMN_SWING_FRAME && frame <= CONDEMN_SWING_FRAME + 1)
          Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                               CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        if (frame >= CONDEMN_SWING_FRAME + 2 &&
            frame <= CONDEMN_SWING_FRAME + 3)
          EFFECT_DRAW_PICTURE(draw[args->side], canvas, state->sheet,
                              REAPER_SHEET_CRESCENT,
                              CONDEMN_PICTURE_X(pictureColumn, 2),
                              CONDEMN_PICTURE_Y(pictureRow, 2) + screenY);
        if (frame >= CONDEMN_SWING_FRAME + 4 &&
            frame <= CONDEMN_SWING_FRAME + 5)
          EFFECT_DRAW_PICTURE(draw[args->side], canvas, gMapGridCellBuffer,
                              RED_CRESCENT_FADE_A,
                              CONDEMN_PICTURE_X(pictureColumn, 3),
                              CONDEMN_PICTURE_Y(pictureRow, 3) + screenY);
        if (frame >= CONDEMN_SWING_FRAME + 6 &&
            frame <= CONDEMN_SWING_FRAME + 7)
          EFFECT_DRAW_PICTURE(draw[args->side], canvas, gMapGridCellBuffer,
                              RED_CRESCENT_FADE_B,
                              CONDEMN_PICTURE_X(pictureColumn, 4),
                              CONDEMN_PICTURE_Y(pictureRow, 4) + screenY);
        if (frame >= CONDEMN_SWING_FRAME + 8 &&
            frame <= CONDEMN_SWING_FRAME + 9)
          EFFECT_DRAW_PICTURE(draw[args->side], canvas, gMapGridCellBuffer,
                              RED_CRESCENT_FADE_C,
                              CONDEMN_PICTURE_X(pictureColumn, 5),
                              CONDEMN_PICTURE_Y(pictureRow, 5) + screenY);
        if (frame >= CONDEMN_SWING_FRAME + 10 &&
            frame <= CONDEMN_SWING_FRAME + 11)
          EFFECT_DRAW_PICTURE(draw[args->side], canvas, gMapGridCellBuffer,
                              RED_CRESCENT_FADE_D,
                              CONDEMN_PICTURE_X(pictureColumn, 6),
                              CONDEMN_PICTURE_Y(pictureRow, 6) + screenY);
      }
    }

    // A cue as the scythe rises; on the swing the target is knocked back, the
    // screen shakes and the results begin.
    if (frame == CONDEMN_SWING_FRAME - 4)
      Sound_PlayFar(SE_POWER_UP);
    if (frame == CONDEMN_SWING_FRAME) {
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      state->shakeFrames = 8;
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_TransferCanvas);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED,
                           Ram_HeapSlots->battleState->background,
                           BACKGROUND_DARKENED_LEVEL);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#undef CONDEMN_FRAMES
#undef CONDEMN_SWING_FRAME
#undef CONDEMN_SWAY_X
#undef CONDEMN_SWAY_Y
#undef CONDEMN_PICTURES
#undef CONDEMN_PICTURE_X
#undef CONDEMN_PICTURE_Y
