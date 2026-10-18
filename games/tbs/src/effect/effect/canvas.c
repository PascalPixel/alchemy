// Part of effect.c, the battle effect object, compiled only through it.
// The canvas layer: transfers, opening and closing, tile maps, lines.

// The canvas layer's functions: opening and closing it, its transfers and
// palette fades, clipping and the target flash timers. README.md describes
// the canvas, its transfer modes and the two BG1 paths.
//
// A reaction may start one of the eight actor timers in the effect's work
// block; ObjectGroup_TickMemberTimers counts them down each frame and gives
// the battler its own palette back when one runs out. The timers are indexed
// like effect->actors, so timer n belongs to actor n.

// Wipes the whole canvas to one colour with a ragged edge: each row (or
// column) lags a random 0 to 63 pixels behind the front, which speeds up as
// it goes. Returns when the wipe is done, a frame at a time.
// mode  : 1 sweeps left to right; anything else sweeps top to bottom.
// value : 0 wipes to colour 1, 1 wipes to colour 0 (clears the canvas).
void BattleFx_WipeCanvas(s32 mode, s32 value) {
  // The pixel addresses and both bounds tests stay written out: as a shared
  // helper or one merged test they compile differently.
  struct BattleEffectWork *work;
  u8 *canvas;
  u8 delay[128];
  s32 x;
  s32 y;
  s32 wipePosition;

  canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  for (x = 0; x != 128; x++)
    delay[x] = Random16() & 0x3f;

  // The canvas is 16x16 tiles of 8x8 pixels; (x, y) is byte
  // ((y / 8 * 16 + x / 8) * 8 + y % 8) * 8 + x % 8.
  if (mode == 1) {
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
                     (wipePosition & 7)] = 1 - value;
        }
      }
      EFFECT_PRESENT_FRAME(work);
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
                     (x & 7)] = 1 - value;
        }
      }
      EFFECT_PRESENT_FRAME(work);
    } while (front <= 191);
  }
}

// Scheduler callback of nearly every effect: when the effect has finished
// drawing a frame (transferPending), copies the 128x128 canvas into BG2's
// character block 1 the way transferMode says, and counts the frames since
// the last copy.
void BattleFx_ProcessPendingGraphicsTransfer(void) {
  void **slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  struct BattleEffectWork *work = slots[0];
  void *source;
  s32 *counter;
  s32 nextCounter;

  if (work->transferPending == 1) {
    source = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_CANVAS);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, 0x4000);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, 0x4000);
      Iwram_FillWords(source, 0x4000, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      if (work->transferValue == 50) {
        ColorBuffer_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(1), 0x4000);
      } else {
        ColorBuffer_BackupAndScaleThreeQuarters(
            source, (void *)BG_CHAR_BLOCK(1), 0x4000);
      }
      break;
    case CANVAS_COPY_AND_DARKEN:
      ColorBuffer_BackupAndDarken(source, work->transferValue,
                                  (void *)BG_CHAR_BLOCK(1), 0x4000);
      break;
    case CANVAS_COPY_AND_BRIGHTEN:
      ColorBuffer_BackupAndBrighten(source, work->transferValue,
                                    (void *)BG_CHAR_BLOCK(1), 0x4000);
      break;
    }
    work->transferPending = 0;
    counter = &work->framesSinceTransfer;
    nextCounter = 1;
  } else {
    counter = &work->framesSinceTransfer;
    nextCounter = *counter + 1;
  }
  *counter = nextCounter;
}

// The same for the effects that draw a full-screen picture in
// gMapCellBuffer instead of the canvas: copies its 0x7800 bytes to VRAM
// 0x06008000 the way transferMode says (modes 0 to 3).
void BattleFx_FlushPendingGraphicsTransfer(void) {
  // This switch stays apart from BattleFx_ProcessPendingGraphicsTransfer's:
  // shared, both compile differently.
  void **slots;
  struct BattleEffectWork *work;
  void *source;
  s32 transferMode;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  work = slots[0];
  source = Ram_MapCellBuffer;
  if (work->transferPending != 1)
    return;

  transferMode = work->transferMode;
  switch (transferMode) {
  case CANVAS_COPY:
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source, 0x7800);
    break;
  case CANVAS_COPY_AND_CLEAR:
    Iwram_CopyWords((void *)BG_CHAR_BLOCK(2), source, 0x7800);
    Iwram_FillWords(source, 0x7800, work->transferValue);
    break;
  case CANVAS_COPY_AND_FADE:
    if (work->transferValue == 50) {
      ColorBuffer_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(2), 0x7800);
    } else {
      ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                              0x7800);
    }
    break;
  case CANVAS_COPY_AND_DARKEN:
    ColorBuffer_BackupAndDarken(source, work->transferValue,
                                (void *)BG_CHAR_BLOCK(2), 0x7800);
    break;
  }

  work->transferPending = 0;
}

// One step of the background palette fade, in 1/65536ths of full
// brightness: 24 steps take it from full to 60%.
#define CANVAS_FADE_STEP 1092

// Writes the window, display and blend registers the effect keeps in its
// work, all at once.
void Graphics_ApplyWindowBlendRegisters(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

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

// Writes the effect's background 2 reference point to the hardware.
void Display_ApplyBg2Reference(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

// Scheduler callback while the canvas is open: for fadeFrames frames,
// darkens the battle backdrop's colours one CANVAS_FADE_STEP more each frame
// into background palettes 6 to 13, so the effect stands out.
void Palette_StepFadeTransfer(void) {
  struct BattleWork *battle = WORK_SLOT(HEAP_SLOT_BATTLE);
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  if (work->fadeFrames > 0) {
    s32 step = ++work->fadeStep;

    Graphics_ScaleRgb555ClampedFar(battle->palette,
                                   (void *)&BG_PLTT_COLOR(6, 0),
                                   0x10000 - step * CANVAS_FADE_STEP, 128);
    work->fadeFrames--;
  }
}

// Clears every target's flash timer.
void BattleFx_ClearActorTimers(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);

  Iwram_ClearWords(work->actorTimers, sizeof work->actorTimers);
}

// Counts down each target's flash timer, which ObjectGroup_UpdateMembers
// starts; when one runs out, the target gets its own palette back. Timer n
// belongs to effect->actors[n]. EFFECT_END_FRAME calls it every frame.
void ObjectGroup_TickMemberTimers(void) {
  struct BattleEffectWork *work;
  s32 actorIndex;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  actorIndex = 0;
  do {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        ObjectGroup_UpdateMembers(work->effect->actors[actorIndex], 0, -1, -1,
                                  0);
      }
    }
    actorIndex += 1;
  } while (actorIndex != 8);
}

// The canvas layer: BG2 shows the effect's 128x128 8bpp canvas (0x4000
// bytes), its tiles from character block 1, stretched to twice the width
// so it spans 256 pixels.
//
// REG_DISPCNT values used here (mode 1, 1D sprites):
//   0x7741 : BG0 to BG2, sprites, windows 0 and 1
//   0x7341 : BG0, BG1, sprites, windows 0 and 1 (BG2 hidden)
//   0x7541 : BG0, BG2, sprites, windows 0 and 1 (BG1 hidden)
//   0x1741 : BG0 to BG2, sprites
//   0x1341 : BG0, BG1, sprites
//   0x1541 : BG0, BG2, sprites

// Opens the canvas layer: starts the background palette fade, makes BG2 the
// canvas, alpha-blended over the battle, inside the effect windows, and
// clears the canvas. It also stops any shake and saves BG1's scroll, which
// BattleFx_EndCanvasLayer restores.
// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleFx_BeginCanvasLayer(s32 bgControl) {
  struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
  void *canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundView *display =
      *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;
  s32 offset;

  BattleFx_ClearActorTimers();
  display->secondMode = 1;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);

  // Start the background palette fade.
  BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);

  // Change BG2's settings while it is hidden.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  WaitFrames(1);
  REG_BG2CNT = bgControl | 0x784;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  display->secondMode = 0;
  WaitFrames(1);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);

  // The screen map: the 16x16 tiles in order. An affine map has one byte
  // per tile, so they are written two at a time.
  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  WaitFrames(1);
}

// Opens the canvas layer as BattleFx_BeginCanvasLayer does, but with the
// windows off until the end, the backdrop palette starting half faded, and
// a 24-frame fade. It neither saves BG1's scroll nor resets the shake.
// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleFx_OpenCanvasLayer(s32 bgControl) {
  // The tile-map loop is written out here and in BattleFx_BeginCanvasLayer:
  // shared, both compile 4 bytes shorter.
  struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
  void *canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundView *display =
      *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;
  s32 offset;

  BattleFx_ClearActorTimers();
  display->secondMode = 1;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Scheduler_AddOrUpdateCallback(Palette_StepFadeTransfer, 0xc80);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      0x20000);
  WaitFrames(1);
  REG_BG2CNT = bgControl | 0x784;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      0x20000);
  BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
  display->secondMode = 0;
  WaitFrames(1);

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  EFFECT_SET_CANVAS_AFFINE();
  EFFECT_SET_BATTLE_WINDOWS();
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = EFFECT_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, EFFECT_DISPCNT, 0x20000);
  WaitFrames(1);

  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  WaitFrames(1);
}

// Opens the canvas layer with BG2 a wrapping 256x256 map: each row of the
// map is the canvas's 16 tiles followed by 16 of tile 0, so the canvas
// repeats across the screen with gaps.
// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleFx_BeginTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleFx_BeginCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | 0x6784;
  offset = 0;
  for (row = 0; row != 16; row++) {
    EFFECT_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

// Closes the canvas layer: plays the closing sound, restores BG1's scroll
// and the projection centre, clears the canvas tiles, stops the palette
// fade and brings the backdrop's palette back from level 21 to 0 over 8
// frames, then shows BG2 again and hides BG1.
void BattleFx_EndCanvasLayer(void) {
  struct BattleEffectWork *work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
  struct BattleWork *battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
  s32 i;

  AudioCommand_PlayFar(289);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Scheduler_RemoveCallback(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  REG_BLDCNT = 0;
  WaitFrames(1);

  BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 7);
  WaitFrames(1);
  for (i = 0; i != 8; i++) {
    BattlePresentation_SetPaletteLevelFar(battle->background, 21 - i * 3);
    WaitFrames(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  WaitFrames(1);
}

// Hides BG1 and the windows and starts bringing the backdrop's palette back.
void BattleFx_SetTransitionFlagAndDisplay(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)
               Ram_WorkSlot[HEAP_SLOT_BATTLE_BACKGROUND])
              ->secondMode;
  battle = Ram_WorkSlot[HEAP_SLOT_BATTLE];
  *flag = 1;
  QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 0);
  WaitFrames(1);
}

// Shows the canvas on BG1 instead of BG2, at its own size: BG1 becomes a
// 256-colour layer whose map points 16 rows of 16 tiles at the canvas
// (tiles 0x100 onwards, which at 64 bytes a tile start at character block
// 1, where the canvas lives), with blank tile 0xff to the right, blended over
// the rest inside the effect windows. Also clears the canvas. The beam
// scenes use it; the full-screen gMapCellBuffer effects use
// BattleFx_FlushPendingGraphicsTransfer instead.
void BattleFx_SetupCanvasTileMap(void) {
  // UB: indexing past gBattleWork reaches the work slots after it.
#ifdef UBFIX
  void *canvas = WORK_SLOT(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleWork *screen = gBattleWork;
  struct BattleBackgroundView *layer = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  u8 **cells = (u8 **)&gBattleWork;
  void *canvas = cells[31];
  struct BattleWork *screen = (struct BattleWork *)cells[0];
  struct BattleBackgroundView *layer = (struct BattleBackgroundView *)cells[35];
#endif
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleFx_ClearActorTimers();
  BattlePresentation_ConfigurePaletteFadeFar(2, screen->background, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  Iwram_ClearWords((void *)0x06003fc0, 64);
  Iwram_FillWords((void *)0x0600f900, 0x200, -1);
  byteOffset = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 32; column++, byteOffset += 2) {
      if (column > 15)
        *(volatile u16 *)(0x0600fb00 + byteOffset) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + byteOffset) = row * 16 + 0x100 + column;
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
  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  WaitFrames(1);
}

// Draws a line on the 128x128 8bpp canvas (16 tiles across), keeping the
// brighter colour where it crosses what is already drawn. The ends' y is
// kept on the canvas; x is not. The line steps a pixel at a time along its
// longer axis, following the other in 8-bit fixed point, and leaves out its
// last pixel on that axis.
// x0, y0 : the start, in pixels
// x1, y1 : the end, in pixels
// color  : the colour index
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80; // 0.5 in 8-bit fixed point
  u8 *canvas = ((u8 **)gBattleFxWork)[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  CLAMP(y0, 0, 127);
  CLAMP(y1, 0, 127);

  if (EFFECT_ABS(dx) < EFFECT_ABS(dy)) {
    // Steep: step y a pixel at a time and follow x in fixed point.
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (EFFECT_ABS(x1 - x0) << 8) / EFFECT_ABS(y1 - y0);
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
    // Shallow: step x a pixel at a time and follow y in fixed point.
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (EFFECT_ABS(y1 - y0) << 8) / EFFECT_ABS(x1 - x0);
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
