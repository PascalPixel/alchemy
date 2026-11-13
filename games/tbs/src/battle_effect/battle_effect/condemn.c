/// @file
/// Condemn.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define CONDEMN_FRAMES 132
#define CONDEMN_SWING_FRAME 88   ///< the reaper swings, in a white flash
#define CONDEMN_BOUNCE_FRAME 120 ///< the falling target bounces

/// Where `picture`, one of the reaper's seven pictures, is drawn
/// (REAPER_PICTURE_X), by the casting side's `column` and `row`.
#define CONDEMN_PICTURE_X(column, picture)                                     \
  gCondemn_PictureX[(column)][(picture)]
#define CONDEMN_PICTURE_Y(row, picture) gCondemn_PictureY[(row)][(picture)]

/// A hooded reaper with a scythe sways across the screen three times, each of
/// the first two passes fading out through a ripple, the last over the first
/// target; on frame 88 the screen flashes white, the reaper swings, and the
/// red crescent its scythe leaves fades away while the target is knocked
/// back, bouncing on frame 120.
///
/// Abilities: Condemn (BATTLE_EFFECT_CONDEMN).
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
   * touches: of the 76 bytes it reserves (sub sp, #76), no instruction
   * addresses the 12 at sp+64 to sp+75, and The Lost Age's
   * BattleEffect_RunCondemn reserves the same second position; as one
   * ScreenPoint, 4 of 580 instructions differ in tbs-en. */
  struct ScreenPoint position[2];
  s32 swayCenter[2];
  s32 drawReaper;
  s32 pictureColumn;
  s32 pictureRow;
  s32 transferValue;
  s32 frame;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  work = *cursor++;
  canvas = *cursor;
  /* FAKEMATCH(temporary): transferValue set here, after the slots; as declared
   * with its 0, 14 of 580 instructions differ in tbs-en. */
  transferValue = 0;
  work->args = args;
  BattleEffect_StartTiledCanvas(0);
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  sheet = Resource_GetTableEntry((s32)&ResourceId_CounterRevealSheet);
  Iwram_CopyWords((void *)BG_PLTT, sheet, 128);
  sheet = sheet + 128;
  Decompress_TaggedLz(sheet, work->sheet);
  cells = Ram_MapGridCellBuffer;
  sheet = Resource_GetTableEntry((s32)&ResourceId_RedCrescentSheetA);
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
  // Redraw from blank each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, transferValue);
  // swayCenter is the point the reaper sways about, in 16.16 pixels: the
  // first pass high up on the near side.
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
    // The second pass, lower and further across; the reaper skips a frame while
    // it moves, and comes back opaque on frame 33.
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
    // The last 16 frames of each of the first two passes ripple harder and
    // harder (up to 30/4 texels) as the reaper fades out.
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

    // Every line's BG2X carries the sway, so the reaper moves by scrolling the
    // canvas rather than by where it is drawn.
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
        /* FAKEMATCH(dead): i is 160 here; as pictureRow = 0, 452 of 580
         * instructions differ in tbs-en. */
        pictureRow = i >> 31;
      }
      // The reaper with its scythe raised until frame 91, and swung from frame
      // 88.
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
        if ((frame - 88) <= 1U) {
          Iwram_FillWords(canvas, CANVAS_BYTES,
                          CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
        }
        if ((frame - 90) <= 1U) {
          draw[work->args->side](canvas, work->sheet + REAPER_SHEET_CRESCENT,
                                 CONDEMN_PICTURE_X(pictureColumn, 2),
                                 CONDEMN_PICTURE_Y(pictureRow, 2) + screenY,
                                 128, 91);
        }
        if ((frame - 92) <= 1U) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_1,
              CONDEMN_PICTURE_X(pictureColumn, 3),
              CONDEMN_PICTURE_Y(pictureRow, 3) + screenY, 128, 91);
        }
        if ((frame - 94) <= 1U) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_2,
              CONDEMN_PICTURE_X(pictureColumn, 4),
              CONDEMN_PICTURE_Y(pictureRow, 4) + screenY, 128, 59);
        }
        if ((frame - 96) <= 1U) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_3,
              CONDEMN_PICTURE_X(pictureColumn, 5),
              CONDEMN_PICTURE_Y(pictureRow, 5) + screenY, 122, 29);
        }
        if ((frame - 98) <= 1U) {
          draw[work->args->side](
              canvas, Ram_MapGridCellBuffer + RED_CRESCENT_FADE_4,
              CONDEMN_PICTURE_X(pictureColumn, 6),
              CONDEMN_PICTURE_Y(pictureRow, 6) + screenY, 76, 25);
        }
      }
    }
    // Knock the target back, away from the middle of the field, to twice its x
    // at up to 2 pixels a frame.
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
  // Finish: the tasks, then the battle background reloaded into
  // character blocks 2 and 3, its palettes darkened to level 24, as
  // BattleEffect_SetupBlendedDisplay does after a summon. It repairs nothing:
  // this effect writes neither those blocks nor the battle's palette, and
  // BattleEffect_EndCanvasLayer rebuilds the background's maps and fades its
  // palette back from level 21 itself.
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
