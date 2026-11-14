/// @file
/// The canvas layer: transfers, opening and closing, tile maps, lines.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// The canvas layer: opening and closing it, its transfers and palette fades,
// and the targets' flash timers, which count down each frame and give a unit
// its own palette back.

/// Wipes the whole canvas to one colour with a ragged edge, a frame at a time.
void BattleEffect_WipeCanvas(s32 direction, s32 fill) {
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 delay[128];
  s32 x;
  s32 y;
  s32 wipePosition;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  for (x = 0; x != 128; x++)
    delay[x] = Random16() & 0x3f;

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
        for (y = 0; y != 128; y++) {
          wipePosition = x - delay[y];
          if (wipePosition >= 0)
            if (wipePosition <= 127)
              canvas[(((y / 8) * 16 + wipePosition / 8) * 8 + (y & 7)) * 8 +
                     (wipePosition & 7)] = 1 - fill;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (front <= 256);
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
        for (x = 0; x != 128; x++) {
          wipePosition = y - delay[x];
          if (wipePosition >= 0)
            if (wipePosition <= 127)
              canvas[(((wipePosition / 8) * 16 + x / 8) * 8 +
                      (wipePosition & 7)) *
                         8 +
                     (x & 7)] = 1 - fill;
        }
      }
      EFFECT_PRESENT_CANVAS(work);
    } while (front <= 191);
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
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
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

/// Copies a full-screen picture to VRAM once the effect has finished a frame,
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
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source, BATTLE_BACKGROUND_BYTES);
    break;
  case CANVAS_COPY_AND_CLEAR:
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source, BATTLE_BACKGROUND_BYTES);
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

/// One step of the background palette fade.
#define CANVAS_FADE_STEP 1092

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

    Graphics_ScaleRgb555ClampedFar(battle->palette,
                                   (void *)&BG_PLTT_COLOR(6, 0),
                                   Q16_ONE - step * CANVAS_FADE_STEP, 128);
    work->fadeFrames--;
  }
}

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
  actorIndex = 0;
  do {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        BattleEffect_TintBattler(work->args->targets[actorIndex],
                                 SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
      }
    }
    actorIndex += 1;
  } while (actorIndex != 8);
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
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_16);
  gBgScroll[1].y = 32;
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
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_16);

  {
    s32 offset = 0;

    for (row = 0; row != 16; row++) {
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
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_FIXED, battle->background,
                              BATTLE_BACKGROUND_FADE_RESTORE);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_FRAME);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  wait(1);

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_16);
  wait(1);

  {
    s32 offset = 0;

    for (row = 0; row != 16; row++) {
      EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    }

    Iwram_ClearWords(canvas, CANVAS_BYTES);
    Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
    wait(1);
  }
}

/// Opens the canvas layer with the canvas repeated across a wrapping map.
void BattleEffect_StartTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleEffect_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP | BGCNT_SIZE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

/// Closes the canvas layer and brings the battle background's colours back.
void BattleEffect_EndCanvasLayer(void) {
  struct BattleEffectWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  s32 i;

  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(Palette_StepFadeTransfer);
  // This overwrites the saved y restored above, so BG1 is always left at y 32.
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_16);
  REG_BLDCNT = 0;
  wait(1);

  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(battle->background, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      IO_WRITE_16);
  wait(1);
}

/// Hides BG1 and the windows and starts bringing the battle background's
/// colours back.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundWork *)HEAP_SLOT_BLOCK(
               HEAP_SLOT_BATTLE_BACKGROUND))
              ->secondMode;
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  *flag = BATTLE_BACKGROUND_FIXED;
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                                              DISPCNT_OBJ_ON);
  wait(1);
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, battle->background, 0);
  wait(1);
}

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
  void *canvas = cells[31];
  struct BattleWork *screen = (struct BattleWork *)cells[0];
  struct BattleBackgroundWork *layer = (struct BattleBackgroundWork *)cells[35];
#endif
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleEffect_ClearActorTimers();
  BattleBackground_SetModeFar(BATTLE_BACKGROUND_ZOOMED, screen->background, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = BATTLE_BACKGROUND_NONE;
  // Blank tile 255, then rows that show nothing.
  Iwram_ClearWords(BG_CHAR_BLOCK(0) + 255 * 64, 64);
  Iwram_FillWords(BG_SCREEN_BLOCK(31) + 4 * 64, 8 * 64, -1);
  // The canvas's tiles, then blank tile 255 across the rest of each row.
  byteOffset = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 32; column++, byteOffset += 2) {
      if (column > 15)
        *(volatile u16 *)(BG_SCREEN_BLOCK(31) + 12 * 64 + byteOffset) = 255;
      else
        *(volatile u16 *)(BG_SCREEN_BLOCK(31) + 12 * 64 + byteOffset) =
            row * 16 + 256 + column;
    }
  }
  REG_DISPCNT = EFFECT_DISPCNT;
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  wait(1);
}

/// Draws a line on the canvas, keeping the brighter colour where it crosses
/// what is drawn.
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 128; // 0.5 in 8-bit fixed point
  u8 *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  CLAMP(y0, 0, 127);
  CLAMP(y1, 0, 127);

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      /* LEFTOVER(template): dy recomputed after the swap and never read again;
       * Camelot's template: the swap recomputes both deltas in both halves of
       * this routine and of its sibling LuckyWheels_DrawCanvasLine, and the
       * shallow half reads its dy; without the store, 27 of 202 instructions
       * differ in tbs-en. */
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    x = x0;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & 0x100) {
        if (dx > 0)
          x++;
        else
          x--;
        frac &= ~0x100;
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
      /* LEFTOVER(template): dx recomputed after the swap and never read again;
       * Camelot's template: the swap recomputes both deltas in both halves of
       * this routine and of its sibling LuckyWheels_DrawCanvasLine, and the
       * steep half reads its dx; without the store, 131 of 202 instructions
       * differ in tbs-en. */
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    y = y0;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac & 0x100) {
        if (dy > 0)
          y++;
        else
          y--;
        frac &= ~0x100;
      }
    }
  }
}

#undef CANVAS_FADE_STEP
