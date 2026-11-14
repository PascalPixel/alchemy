/// @file
/// Condemn.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define CONDEMN_FRAMES 132
#define CONDEMN_SWING_FRAME 88   ///< the reaper swings, in a white flash
#define CONDEMN_BOUNCE_FRAME 120 ///< the falling target bounces

/// Where `picture`, one of the reaper's seven pictures, is drawn
/// (REAPER_PICTURE_X), by the casting side's `column` and `row`.
#define CONDEMN_PICTURE_X(column, picture)                                     \
  gCondemn_PictureX[(column)][(picture)]
#define CONDEMN_PICTURE_Y(row, picture) gCondemn_PictureY[(row)][(picture)]

/// A hooded reaper with a scythe sways across the screen three times, fading
/// out through a ripple, the last pass over the first target; the screen
/// flashes white, the reaper swings, and the red crescent its scythe leaves
/// fades while the target is knocked back.
///
/// Abilities: Condemn.
void BattleEffect_RunCondemn(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  void *cells;
  BlitterPair draw;
  struct Actor *target;
  /* LEFTOVER(frame): position[1], which the ROM's frame reserves and never
   * touches. */
  struct ScreenPoint position[2];
  s32 swayCenter[2];
  s32 drawReaper;
  s32 pictureColumn;
  s32 pictureRow;
  s32 frame;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = *cursor++;
  canvas = *cursor;
  {
    // The canvas is cleared to 0 as the transfer starts.
    s32 transferValue = 0;

    work->args = args;
    BattleEffect_StartTiledCanvas(0);
    REG_BG2PA = Q_8_8(1);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);

    sheet = Resource_GetTableEntry(RESOURCE_COUNTER_REVEAL_SHEET);
    Iwram_CopyWords((void *)BG_PLTT, sheet, 128);
    sheet = sheet + 128;
    Decompress_TaggedLz(sheet, work->sheet);
    cells = Ram_MapGridCellBuffer;
    sheet = Resource_GetTableEntry(RESOURCE_RED_CRESCENT_SHEET_A);
    sheet = sheet + 128;
    Decompress_TaggedLz(sheet, cells);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
    EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, transferValue);
  }
  drawReaper = 1;
  if (work->args->side == EFFECT_SIDE_PARTY) {
    swayCenter[0] = -px(80);
  } else {
    swayCenter[0] = px(112);
  }
  swayCenter[1] = -px(32);
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
    screenX = (swayCenter[0] >> 16) + (((Trig_Sin(spin) * 16)) >> 16) + 48;
    screenY = (swayCenter[1] >> 16) + (((Trig_Cos(spin) * 4)) >> 16) + 16;
    if (frame == CONDEMN_SWING_FRAME) {
      Sound_PlayFar(SE_HEAVY_IMPACT);
    }
    // The second pass, lower and further across.
    if (frame == 32) {
      if (work->args->side == EFFECT_SIDE_PARTY) {
        swayCenter[0] = -px(32);
      } else {
        swayCenter[0] = px(72);
      }
      swayCenter[1] = px(24);
      drawReaper = 0;
    }
    if (frame == 33) {
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      drawReaper = 1;
    }
    // The last pass, on the first target.
    if (frame == 64) {
      Projection_ScaledUnit(work->args->targets[0], position);
      if (work->args->side == EFFECT_SIDE_PARTY) {
        swayCenter[0] = px(position[0].x - 128);
      } else {
        swayCenter[0] = px(position[0].x - 64);
      }
      swayCenter[1] = 0;
      drawReaper = 0;
    }
    if (frame == 65) {
      REG_BLDALPHA = BLEND_ALPHA(16, 16);
      drawReaper = 1;
    }
    // Each of the first two passes ripples harder as the reaper fades out.
    scanline = work->bg2xLines;
    ripple = 0;
    if (frame <= 31) {
      if (frame > 15) {
        ripple = (frame * 2) - 32;
        REG_BLDALPHA = BLEND_ALPHA(31 - frame, 16);
      }
    } else if (frame <= 63) {
      if (frame > 47) {
        ripple = (frame * 2) - 96;
        REG_BLDALPHA = BLEND_ALPHA(63 - frame, 16);
      }
    }
    if (ripple < 0) {
      ripple = 0;
    }

    // The reaper moves by scrolling the canvas line by line, not by where it is
    // drawn.
    i = 0;
    lineOffset = (6 - screenX) << 8;
    angle = frame * (TRIG_TURN / 32);
    for (; i != DISPLAY_HEIGHT; i++) {
      *scanline++ = lineOffset - ((Trig_Sin(angle) * ripple) >> 10);
      angle += TRIG_TURN / 32;
    }
    if (drawReaper != 0) {
      if (work->args->side == EFFECT_SIDE_ENEMIES) {
        pictureColumn = 0;
        pictureRow = 0;
      } else {
        pictureColumn = 1;
        /* FAKEMATCH(dead): i is 160 here. */
        pictureRow = i >> 31;
      }
      if (frame <= 87) {
        draw[work->args->side](
            canvas, work->sheet, CONDEMN_PICTURE_X(pictureColumn, 0),
            CONDEMN_PICTURE_Y(pictureRow, 0) + screenY, 57, 98);
      } else {
        if (frame <= 91) {
          draw[work->args->side](
              canvas, work->sheet, CONDEMN_PICTURE_X(pictureColumn, 0),
              CONDEMN_PICTURE_Y(pictureRow, 0) + screenY, 57, 98);
        }
        draw[work->args->side](canvas, work->sheet + REAPER_SHEET_SWUNG,
                               CONDEMN_PICTURE_X(pictureColumn, 1),
                               CONDEMN_PICTURE_Y(pictureRow, 1) + screenY, 99,
                               69);
        if (frame >= 88 && frame <= 89) {
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        }
        if (frame >= 90 && frame <= 91) {
          draw[work->args->side](canvas, work->sheet + REAPER_SHEET_CRESCENT,
                                 CONDEMN_PICTURE_X(pictureColumn, 2),
                                 CONDEMN_PICTURE_Y(pictureRow, 2) + screenY,
                                 128, 91);
        }
        if (frame >= 92 && frame <= 93) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_1,
              CONDEMN_PICTURE_X(pictureColumn, 3),
              CONDEMN_PICTURE_Y(pictureRow, 3) + screenY, 128, 91);
        }
        if (frame >= 94 && frame <= 95) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_2,
              CONDEMN_PICTURE_X(pictureColumn, 4),
              CONDEMN_PICTURE_Y(pictureRow, 4) + screenY, 128, 59);
        }
        if (frame >= 96 && frame <= 97) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_3,
              CONDEMN_PICTURE_X(pictureColumn, 5),
              CONDEMN_PICTURE_Y(pictureRow, 5) + screenY, 122, 29);
        }
        if (frame >= 98 && frame <= 99) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_4,
              CONDEMN_PICTURE_X(pictureColumn, 6),
              CONDEMN_PICTURE_Y(pictureRow, 6) + screenY, 76, 25);
        }
      }
    }
    // Knock the target back.
    if (frame == CONDEMN_SWING_FRAME) {
      target = GetBattleObjectSlotFar(work->args->targets[0])->actor;
      target->velocityY = px(1);
      target->acceleration = px(2);
      target->speedLimit = px(2);
      target->gravity = 0;
      target->steeringFlags = 0;
      target->snapToTarget = 0;
      Actor_SetMoveTargetFar(target, target->x.fixed << 1, 0, target->z.fixed);
      BattleEffect_TintBattler(work->args->targets[0], TINT_KEEP, POSE_DAMAGE,
                               TINT_KEEP, 0);
    }
    if (frame == CONDEMN_BOUNCE_FRAME) {
      GetBattleObjectSlotFar(work->args->targets[0])->actor->gravity =
          BATTLE_UNIT_GRAVITY;
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  // Finish: the tasks, then the battle background reloaded, which repairs
  // nothing this effect changed.
  // XXX (no evidence): why Condemn reloads the background; the reload puts
  // back the picture already there, so nothing the game shows says why.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, gBattleWork->background,
                           24);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_EndCanvasLayer();
}

#undef CONDEMN_FRAMES
#undef CONDEMN_SWING_FRAME
#undef CONDEMN_BOUNCE_FRAME
#undef CONDEMN_PICTURE_X
#undef CONDEMN_PICTURE_Y
