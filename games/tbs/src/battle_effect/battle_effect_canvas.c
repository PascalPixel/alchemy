/// @file
/// The canvas layer: opening and closing it, its transfers, the battle
/// background's darkening behind it, the targets' flash timers, and lines.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// How far a row of BattleEffect_WipeCanvas's wipe lags behind its front, at
/// most: each row lags a random 0 to WIPE_LAG_MAX pixels.
#define WIPE_LAG_MAX 63
/// Where the across wipe's front stops, three frames past
/// CANVAS_SIZE + WIPE_LAG_MAX, where the wipe is finished.
#define WIPE_ACROSS_END 256

/// Wipes the whole canvas to one colour with a ragged edge, a frame at a time:
/// 23 frames across, 15 down.
void BattleEffect_WipeCanvas(s32 direction, s32 fill) {
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 delay[CANVAS_SIZE];
  s32 x;
  s32 y;
  s32 wipePosition;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  for (x = 0; x != CANVAS_SIZE; x++)
    delay[x] = Random16() & WIPE_LAG_MAX;

  if (direction == CANVAS_WIPE_ACROSS) {
    s32 front;
    s32 speed;

    front = 0;
    speed = 1;
    x = 0;
    do {
      front += speed;
      speed++;
      for (; x != front; x++) {
        for (y = 0; y != CANVAS_SIZE; y++) {
          wipePosition = x - delay[y];
          if (wipePosition >= 0)
            if (wipePosition < CANVAS_SIZE)
              canvas[CANVAS_PIXEL_OFFSET(wipePosition, y)] = 1 - fill;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (front <= WIPE_ACROSS_END);
  } else {
    s32 front;
    s32 speed;

    front = 0;
    speed = 1;
    y = 0;
    do {
      front += speed / 2;
      speed += 4;
      for (; y != front; y++) {
        for (x = 0; x != CANVAS_SIZE; x++) {
          wipePosition = y - delay[x];
          if (wipePosition >= 0)
            if (wipePosition < CANVAS_SIZE)
              canvas[CANVAS_PIXEL_OFFSET(x, wipePosition)] = 1 - fill;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (front <= CANVAS_SIZE + WIPE_LAG_MAX);
  }
}

/// Copies the canvas to VRAM once the effect has finished a frame, then clears,
/// fades, darkens or brightens it as transferMode says.
void BattleEffect_ProcessPendingGraphicsTransfer(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  void *source;
  s32 *counter;
  s32 nextCounter;

  if (work->transferPending == 1) {
    source = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords(BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords(BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      Iwram_FillWords(source, CANVAS_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == CANVAS_FADE_HALF) {
        PaletteFade_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(1),
                                   CANVAS_BYTES);
      } else {
        PaletteFade_BackupAndScaleThreeQuarters(
            source, (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      }
      break;
    case CANVAS_COPY_AND_DARKEN:
      PaletteFade_BackupAndDarken(source, work->transferValue,
                                  (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_BRIGHTEN:
      PaletteFade_BackupAndBrighten(source, work->transferValue,
                                    (void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
      break;
    }
    work->transferPending = 0;
    counter = &work->vblanksSinceTransfer;
    nextCounter = 1;
  } else {
    counter = &work->vblanksSinceTransfer;
    nextCounter = *counter + 1;
  }
  *counter = nextCounter;
}

/// Copies a full-screen image to VRAM once the effect has finished a frame,
/// as transferMode says.
void BattleEffect_FlushPendingGraphicsTransfer(void) {
  struct BattleEffectWork *work;
  void *source;
  s32 transferMode;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  source = Ram_MapGridCellBuffer;
  if (work->transferPending != 1)
    return;

  transferMode = work->transferMode;
  switch (transferMode) {
  case CANVAS_COPY:
    Iwram_CopyWords(BG_CHAR_BLOCK(2), source, BATTLE_BACKGROUND_BYTES);
    break;
  case CANVAS_COPY_AND_CLEAR:
    Iwram_CopyWords(BG_CHAR_BLOCK(2), source, BATTLE_BACKGROUND_BYTES);
    Iwram_FillWords(source, BATTLE_BACKGROUND_BYTES, work->transferValue);
    break;
  case CANVAS_COPY_AND_FADE:
    if (work->transferValue == CANVAS_FADE_HALF) {
      PaletteFade_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(2),
                                 BATTLE_BACKGROUND_BYTES);
    } else {
      PaletteFade_BackupAndScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                              BATTLE_BACKGROUND_BYTES);
    }
    break;
  case CANVAS_COPY_AND_DARKEN:
    PaletteFade_BackupAndDarken(source, work->transferValue,
                                (void *)BG_CHAR_BLOCK(2),
                                BATTLE_BACKGROUND_BYTES);
    break;
  }

  work->transferPending = 0;
}

/// The battle background's first BG palette, of the eight its colours take.
#define BATTLE_BACKGROUND_PALETTE 6

/// Writes the window, display and blend registers the effect keeps in its
/// work, all at once.
void BattleEffect_ApplyWindowBlendRegisters(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  REG_WIN0H = work->win0h;
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes the effect's BG2 reference point to BG2X and BG2Y.
void Display_ApplyBg2Reference(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Darkens the battle background a step each frame while the canvas is open, so
/// the effect stands out.
void Palette_StepFadeTransfer(void) {
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  if (work->fadeFrames > 0) {
    s32 step = ++work->fadeStep;

    Graphics_ScaleRgb555ClampedFar(
        battle->palette, (void *)&BG_PLTT_COLOR(BATTLE_BACKGROUND_PALETTE, 0),
        FIXED_ONE - step * BACKGROUND_LEVEL_STEP, BATTLE_BACKGROUND_COLORS);
    work->fadeFrames--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

  Iwram_ClearWords(work->actorTimers, sizeof work->actorTimers);
}

/// Counts down each target's flash timer and gives it its own palette back when
/// one runs out.
void BattleEffect_CountDownBattlerTints(void) {
  struct BattleEffectWork *work;
  s32 actorIndex;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  for (actorIndex = 0; actorIndex != ARRAY_COUNT(work->actorTimers);
       actorIndex++) {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        BattleEffect_TintBattler(work->args->targets[actorIndex],
                                 SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
      }
    }
  }
}

/// Opens the canvas layer over the battle, darkening the battle background
/// behind it.
void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  void *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundWork *display =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;

  BattleEffect_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_FRAME);

  // Change BG2's settings while it is hidden.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);

  {
    s32 offset = 0;

    for (row = 0; row != CANVAS_TILES; row++) {
      EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    }

    Iwram_ClearWords(canvas, CANVAS_BYTES);
    Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    work->shakeFrames = 0;
    work->savedBg1X = gBgScroll[1].x;
    work->savedBg1Y = gBgScroll[1].y;
    wait(1);
  }
}

/// Opens the canvas layer as BattleEffect_StartCanvasLayer does, with the
/// windows off and a half-faded background.
void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  void *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundWork *display =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;

  BattleEffect_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background,
                              BATTLE_BACKGROUND_FADE_RESTORE);
  work->fadeFrames = BACKGROUND_DARKEN_FRAMES;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_FRAME);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
  wait(1);

  {
    s32 offset = 0;

    for (row = 0; row != CANVAS_TILES; row++) {
      EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    }

    Iwram_ClearWords(canvas, CANVAS_BYTES);
    Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    wait(1);
  }
}

/// Opens the canvas layer on a 256-pixel map that wraps: the canvas fills the
/// left half of the map's top rows, and the right half repeats the canvas's
/// first tile.
void BattleEffect_StartTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP | BGCNT_SIZE(1);
  offset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

/// The levels the battle background brightens by each frame as the canvas
/// closes, back to its own colours.
#define BACKGROUND_RESTORE_STEP 3

/// Closes the canvas layer and brings the battle background's colours back.
void BattleEffect_EndCanvasLayer(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  s32 i;

  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(Palette_StepFadeTransfer);
  // This overwrites the saved y restored above, so BG1 is always left at
  // BATTLE_BACKGROUND_SCROLL_Y.
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  REG_BLDCNT = 0;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 7);
  wait(1);
  // The background brightens from the darkened level back to its own colours
  // over eight frames.
  for (i = 0; i != BACKGROUND_DARKENED_LEVEL / BACKGROUND_RESTORE_STEP; i++) {
    BattleBackground_SetPaletteLevelFar(battle->background,
                                        BACKGROUND_DARKENED_LEVEL -
                                            BACKGROUND_RESTORE_STEP -
                                            i * BACKGROUND_RESTORE_STEP);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_HALFWORD);
  wait(1);
}

/// Hides BG1 and the windows and starts bringing the battle background's
/// colours back.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *secondMode;

  secondMode = &((struct BattleBackgroundWork *)HEAP_SLOT_BLOCK(
                     HEAP_SLOT_BATTLE_BACKGROUND))
                    ->secondMode;
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  *secondMode = BATTLE_BACKGROUND_FIXED;
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                                              DISPCNT_OBJ_ON);
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  wait(1);
}

/// BG1's map while it shows the canvas at its own size: the battle's own BG1
/// map, in screen block 31.
#define CANVAS_BG1_SCREEN 31
/// Row `row` of BG1's map.
#define BG1_MAP_ROW(row)                                                       \
  (BG_SCREEN_BLOCK(CANVAS_BG1_SCREEN) + (row) * MAP_ROW_BYTES)
/// The tile, cleared, that BG1 shows around the canvas.
#define CANVAS_BG1_BLANK_TILE 255
/// The canvas's first tile counted from BG1's character base: the canvas is
/// character block 1.
#define CANVAS_BG1_FIRST_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)
/// The map row where the canvas starts on BG1, 96 pixels down: the beams
/// scroll BG1 to y 80 to show it from the battle view's top line.
#define CANVAS_BG1_TOP_ROW 12

/// Shows the canvas on BG1 instead of BG2, at its own size.
void BattleEffect_SetupCanvasTileMap(void) {
  // UB: indexing past gBattleWork reaches the heap slots after it.
#ifdef UBFIX
  void *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleWork *screen = gBattleWork;
  struct BattleBackgroundWork *layer =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  u8 **cells = (u8 **)&gBattleWork;
  void *canvas = cells[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
  struct BattleWork *screen = (struct BattleWork *)cells[0];
  struct BattleBackgroundWork *layer = (struct BattleBackgroundWork *)
      cells[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
#endif
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleEffect_ClearActorTimers();
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, screen->background, 0);
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  layer->secondMode = BATTLE_BACKGROUND_NONE;
  // A blank tile, then the rows above the canvas, from the first one BG1
  // shows, which show nothing.
  Iwram_ClearWords(BG_CHAR_BLOCK(0) + CANVAS_BG1_BLANK_TILE * TILE_SIZE_8BPP,
                   TILE_SIZE_8BPP);
  Iwram_FillWords(BG1_MAP_ROW(BATTLE_BACKGROUND_SCROLL_Y / 8),
                  (CANVAS_BG1_TOP_ROW - BATTLE_BACKGROUND_SCROLL_Y / 8) *
                      MAP_ROW_BYTES,
                  -1);
  // The canvas's tiles, then the blank tile across the rest of each row.
  byteOffset = 0;
  for (row = 0; row != CANVAS_TILES; row++) {
    for (column = 0; column != BG_SCREEN_WIDTH; column++, byteOffset += 2) {
      if (column >= CANVAS_TILES)
        *(volatile u16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) + byteOffset) =
            CANVAS_BG1_BLANK_TILE;
      else
        *(volatile u16 *)(BG1_MAP_ROW(CANVAS_BG1_TOP_ROW) + byteOffset) =
            row * CANVAS_TILES + CANVAS_BG1_FIRST_TILE + column;
    }
  }
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(CANVAS_BG1_SCREEN);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);
}

/// Draws a line on the canvas, keeping the brighter colour where it crosses
/// what is drawn; the line's ends are clipped to the canvas's rows only.
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  // The slower axis is followed in 8.8 fixed point, a pixel each time the
  // fraction carries into the whole part.
  s32 frac = Q_8_8(0.5);
  u8 *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  CLAMP(y0, 0, CANVAS_SIZE - 1);
  CLAMP(y1, 0, CANVAS_SIZE - 1);

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      /* LEFTOVER(template): dy recomputed after the swap and never read again.
       */
      dy = y1 - y0;
    }
    step = ABS(x1 - x0) * Q_8_8(1) / ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      offset = CANVAS_PIXEL_OFFSET((u32)x, (u32)i);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & Q_8_8(1)) {
        if (dx > 0)
          x++;
        else
          x--;
        frac &= ~Q_8_8(1);
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      /* LEFTOVER(template): dx recomputed after the swap and never read again.
       */
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = ABS(y1 - y0) * Q_8_8(1) / ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      offset = CANVAS_PIXEL_OFFSET((u32)i, (u32)y);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & Q_8_8(1)) {
        if (dy > 0)
          y++;
        else
          y--;
        frac &= ~Q_8_8(1);
      }
    }
  }
}

#undef WIPE_LAG_MAX
#undef WIPE_ACROSS_END
#undef BATTLE_BACKGROUND_PALETTE
#undef BACKGROUND_RESTORE_STEP
#undef CANVAS_BG1_SCREEN
#undef BG1_MAP_ROW
#undef CANVAS_BG1_BLANK_TILE
#undef CANVAS_BG1_FIRST_TILE
#undef CANVAS_BG1_TOP_ROW
