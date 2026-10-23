/// @file
/// The canvas layer: transfers, opening and closing, tile maps, lines.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

// The canvas layer's functions: opening and closing it, its transfers and
// palette fades, clipping and the target flash timers. README.md describes
// the canvas, its transfer modes and the two BG1 paths.
//
// A reaction may start one of the eight actor timers in the animation's work
// block; ObjectGroup_TickMemberTimers counts them down each frame and gives
// the battler its own palette back when one runs out. The timers are indexed
// like effect->actors, so timer n belongs to actor n.

/// Wipes the whole canvas to one colour with a ragged edge: each row (or
/// column) lags a random 0 to 63 pixels behind the front, which speeds up as
/// it goes. Returns when the wipe is done, a frame at a time: 23 frames
/// across, 15 down.
///
/// Called by the summons and scenes that open on a wiped canvas: Boreas,
/// Thor, Atalanta, Neptune, Kirin, Ramses, the particle streams and
/// Judgment. Writes the canvas and sets work->transferPending each frame;
/// the caller's transfer callback must already be running.
///
/// direction: CANVAS_WIPE_ACROSS sweeps left to right, anything else top to
///            bottom (enum CanvasWipeDirection).
/// fill: what the wipe leaves (enum CanvasWipeFill).
void BattleAnim_WipeCanvas(s32 direction, s32 fill) {
  // The pixel addresses and both bounds tests stay written out: as a shared
  // helper or one merged test they compile differently.
  struct BattleAnimWork *work;
  u8 *canvas;
  u8 delay[128];
  s32 x;
  s32 y;
  s32 wipePosition;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  for (x = 0; x != 128; x++)
    delay[x] = Random16() & 0x3f;

  // The canvas is 16x16 tiles of 8x8 pixels; (x, y) is byte
  // ((y / 8 * 16 + x / 8) * 8 + y % 8) * 8 + x % 8. A line is done once the
  // front passes 190, its last pixel (127) plus the greatest lag (63).
  // Across, the front speeds up by a column a frame and stops only past 256;
  // down, it speeds up by two rows a frame and stops past 191.
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
      ANIM_PRESENT_FRAME(work);
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
      ANIM_PRESENT_FRAME(work);
    } while (front <= 191);
  }
}

/// Copies the canvas to VRAM once the animation has finished drawing a frame
/// (transferPending): the 128x128 canvas into BG2's character block 1, the
/// way transferMode says, leaving the canvas cleared, faded, darkened or
/// brightened for the next frame.
///
/// Runs each frame as a task that nearly every canvas animation
/// adds, most through ANIM_START_TRANSFER, and removes before its blocks
/// are freed (ANIM_FINISH_CANVAS). Clears work->transferPending after a
/// copy, and counts work->framesSinceTransfer: 1 on a frame that copied,
/// one more on each frame that did not. No animation reads that count;
/// overworld/colorfx.c keeps the same count for its own transfer.
void BattleAnim_ProcessPendingGraphicsTransfer(void) {
  void **slots = gHeapSlots + HEAP_SLOT_BATTLE_ANIM;
  struct BattleAnimWork *work = slots[0];
  void *source;
  s32 *counter;
  s32 nextCounter;

  if (work->transferPending == 1) {
    source = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_CANVAS);
    switch (work->transferMode) {
    case CANVAS_COPY:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      break;
    case CANVAS_COPY_AND_CLEAR:
      Iwram_CopyWords((void *)BG_CHAR_BLOCK(1), source, CANVAS_BYTES);
      Iwram_FillWords(source, CANVAS_BYTES, work->transferValue);
      break;
    case CANVAS_COPY_AND_FADE:
      // Any value but 50 keeps three quarters.
      if (work->transferValue == 50) {
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
    counter = &work->framesSinceTransfer;
    nextCounter = 1;
  } else {
    counter = &work->framesSinceTransfer;
    nextCounter = *counter + 1;
  }
  *counter = nextCounter;
}

/// Copies a full-screen picture to VRAM once the animation has finished drawing
/// a frame: the 0x7800 bytes of Ram_MapGridCellBuffer into BG character block 2
/// (0x06008000), the way transferMode says (every mode but
/// CANVAS_COPY_AND_BRIGHTEN).
///
/// Runs each frame as a task added by
/// BattleAnim_RunParticleStreams, which removes it before it returns. Clears
/// work->transferPending after a copy; unlike the canvas transfer it keeps
/// no frame count.
void BattleAnim_FlushPendingGraphicsTransfer(void) {
  // This switch stays apart from BattleAnim_ProcessPendingGraphicsTransfer's:
  // shared, both compile differently.
  void **slots;
  struct BattleAnimWork *work;
  void *source;
  s32 transferMode;

  slots = gHeapSlots + HEAP_SLOT_BATTLE_ANIM;
  work = slots[0];
  source = Ram_MapGridCellBuffer;
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
      PaletteFade_BackupAndHalve(source, (void *)BG_CHAR_BLOCK(2), 0x7800);
    } else {
      PaletteFade_BackupAndScaleThreeQuarters(source, (void *)BG_CHAR_BLOCK(2),
                                              0x7800);
    }
    break;
  case CANVAS_COPY_AND_DARKEN:
    PaletteFade_BackupAndDarken(source, work->transferValue,
                                (void *)BG_CHAR_BLOCK(2), 0x7800);
    break;
  }

  work->transferPending = 0;
}

/// One step of the background palette fade, in 1/65536ths of full
/// brightness: 24 steps take it from full to 60%.
#define CANVAS_FADE_STEP 1092

/// Writes the window, display and blend registers the animation keeps in its
/// work, all at once.
///
/// Nothing in the build calls it or takes its address. Writes WIN0H to
/// BLDALPHA from the work block's copies.
void BattleAnim_ApplyWindowBlendRegisters(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);

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

/// Writes the animation's BG2 reference point (work->bg2x, work->bg2y) to BG2X
/// and BG2Y.
///
/// Nothing in the build calls it or takes its address;
/// BattleAnim_AdvanceBg2OnInterval (display.c) writes the same pair itself.
void Display_ApplyBg2Reference(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);

  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Darkens the battle background while the canvas is open: for fadeFrames
/// frames, writes the battle background's colours one CANVAS_FADE_STEP darker
/// each frame into background palettes 6 to 13, so the animation stands out.
///
/// Runs each frame as a task added by
/// BattleAnim_StartCanvasLayer and BattleAnim_OpenCanvasLayer, and by
/// BattleAnim_RunRingAndSparkSequence; BattleAnim_EndCanvasLayer and
/// BattleAnim_EndElementStreaks remove it. It does nothing until the animation
/// sets work->fadeFrames, which it counts down while counting
/// work->fadeStep up.
void Palette_StepFadeTransfer(void) {
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);

  if (work->fadeFrames > 0) {
    s32 step = ++work->fadeStep;

    Graphics_ScaleRgb555ClampedFar(battle->palette,
                                   (void *)&BG_PLTT_COLOR(6, 0),
                                   0x10000 - step * CANVAS_FADE_STEP, 128);
    work->fadeFrames--;
  }
}

/// Clears every target's flash timer (work->actorTimers).
///
/// Called as the canvas opens, by BattleAnim_StartCanvasLayer,
/// BattleAnim_OpenCanvasLayer and BattleAnim_SetupCanvasTileMap, and by
/// BattleAnim_RunShatteringOpening, which opens its own.
void BattleAnim_ClearActorTimers(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);

  Iwram_ClearWords(work->actorTimers, sizeof work->actorTimers);
}

/// Counts down each target's flash timer, which ObjectGroup_UpdateMembers
/// starts; when one runs out, the target gets its own palette back. Timer n
/// belongs to effect->actors[n].
///
/// Called once a frame by nearly every animation, most through
/// ANIM_END_FRAME. Changes work->actorTimers and, through
/// ObjectGroup_UpdateMembers, the colour of a target whose timer ends.
void ObjectGroup_TickMemberTimers(void) {
  struct BattleAnimWork *work;
  s32 actorIndex;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  actorIndex = 0;
  do {
    if (work->actorTimers[actorIndex] != 0) {
      if ((work->actorTimers[actorIndex] = work->actorTimers[actorIndex] - 1) ==
          0) {
        ObjectGroup_UpdateMembers(work->effect->targets[actorIndex], 0, -1, -1,
                                  0);
      }
    }
    actorIndex += 1;
  } while (actorIndex != 8);
}

// The canvas layer: BG2 shows the animation's 128x128 8bpp canvas (0x4000
// bytes), its tiles from character block 1, stretched to twice the width
// so it spans 256 pixels. Opening and closing it hide BG2 while its
// settings change, through DISPCNT values queued two frames ahead.

/// Opens the canvas layer: starts the background palette fade, makes BG2 the
/// canvas, alpha-blended over the battle, inside the animation windows, and
/// clears the canvas. It also stops any shake and saves BG1's scroll, which
/// BattleAnim_EndCanvasLayer restores.
///
/// Called at the start of nearly every canvas animation, directly or through
/// ANIM_START_NARROW_CANVAS, and by BattleAnim_StartTiledCanvas. Waits five
/// frames of its own. Writes BLDCNT, BLDALPHA, BG2CNT and BG2's affine
/// registers, the windows, WININ and WINOUT, and queues DISPCNT; writes BG2's
/// map in screen block 7, clears the canvas in its slot and in character block
/// 1; adds Palette_StepFadeTransfer with work->fadeFrames 0, so the battle
/// background keeps its colours until the animation sets a count; sets
/// gBgScroll[1].y to 32, clears work->actorTimers and work->shakeFrames, and
/// saves gBgScroll[1] in work->savedBg1X and savedBg1Y.
///
/// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleAnim_StartCanvasLayer(s32 bgControl) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  void *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundView *display =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;
  s32 offset;

  // The background view's secondMode, which also sets BG1's scroll while it
  // is BATTLE_BACKGROUND_FIXED, holds for the frames BG2 is being changed.
  BattleAnim_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, ANIM_DISPCNT, 0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);

  // Start the background palette fade.
  BattlePresentation_ConfigurePaletteFadeFar(BATTLE_BACKGROUND_FIXED,
                                             battle->background, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, ANIM_PLAYBACK_ORDER);

  // Change BG2's settings while it is hidden (each queued DISPCNT lands two
  // frames later).
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  WaitFrames(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  WaitFrames(1);
  // Show it: 14/16 of the canvas over all below, inside both windows.
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  ANIM_SET_CANVAS_AFFINE();
  ANIM_SET_BATTLE_WINDOWS();
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = ANIM_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, ANIM_DISPCNT, 0x20000);

  // The screen map: the 16x16 tiles in order. An affine map has one byte
  // per tile, so they are written two at a time.
  offset = 0;
  for (row = 0; row != 16; row++) {
    ANIM_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  work->shakeFrames = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  WaitFrames(1);
}

/// Opens the canvas layer as BattleAnim_StartCanvasLayer does, but with the
/// windows off until the end, the battle background palette starting half
/// faded, and a 24-frame fade. It neither saves BG1's scroll nor resets the
/// shake.
///
/// Nothing in the build calls it or takes its address. Writes the same
/// registers, map and canvas as BattleAnim_StartCanvasLayer and sets
/// work->fadeFrames to 24.
///
/// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleAnim_OpenCanvasLayer(s32 bgControl) {
  // The tile-map loop is written out here and in BattleAnim_StartCanvasLayer:
  // shared, both compile 4 bytes shorter.
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  void *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleBackgroundView *display =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  s32 row;
  s32 column;
  s32 offset;

  BattleAnim_ClearActorTimers();
  display->secondMode = BATTLE_BACKGROUND_FIXED;
  WaitFrames(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
                      0x20000);
  gBgScroll[1].y = 32;
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(BATTLE_BACKGROUND_FIXED,
                                             battle->background, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, ANIM_PLAYBACK_ORDER);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      0x20000);
  WaitFrames(1);
  REG_BG2CNT =
      bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON,
                      0x20000);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = BATTLE_BACKGROUND_NONE;
  WaitFrames(1);

  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  ANIM_SET_CANVAS_AFFINE();
  ANIM_SET_BATTLE_WINDOWS();
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = ANIM_WINOUT;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, ANIM_DISPCNT, 0x20000);
  WaitFrames(1);

  offset = 0;
  for (row = 0; row != 16; row++) {
    ANIM_WRITE_CANVAS_MAP_ROW(row, column, offset);
  }

  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  WaitFrames(1);
}

/// Opens the canvas layer with BG2 a wrapping 256x256 map: each row of the
/// map is the canvas's 16 tiles followed by 16 of tile 0, so the canvas
/// repeats across the screen with gaps.
///
/// Called by BattleAnim_RunRay, BattleAnim_RunCondemn,
/// BattleAnim_RunCastingImpact and
/// BattleAnim_InitializeTilemapAndFlushQueue. Opens the layer through
/// BattleAnim_StartCanvasLayer, then rewrites BG2CNT and BG2's map in screen
/// block 7.
///
/// bgControl : added to BG2CNT, the layer's priority (0 to 3) and so on.
void BattleAnim_StartTiledCanvas(s32 bgControl) {
  s32 row;
  s32 column;
  s32 offset;

  BattleAnim_StartCanvasLayer(bgControl);
  REG_BG2CNT = bgControl | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP | BGCNT_SIZE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    ANIM_WRITE_CANVAS_MAP_ROW(row, column, offset);
    for (column = 0; column != 8; column++, offset += 2) {
      *(volatile u16 *)((u8 *)BG_CANVAS_MAP + offset) = 0;
    }
  }
}

/// Closes the canvas layer: plays the closing sound, restores BG1's scroll
/// and the projection centre, clears the canvas tiles, stops the palette
/// fade and brings the battle background's palette back from level 21 to 0 over
/// 8 frames, then shows BG2 again and hides BG1.
///
/// Called at the end of nearly every canvas animation, most through
/// ANIM_FINISH_CANVAS, after the transfer callback is removed. Takes 11
/// frames. Writes gBgScroll[1], gProjection's centre, BG2CNT (priority 3)
/// and BLDCNT, clears character block 1, removes Palette_StepFadeTransfer
/// and queues two DISPCNT values.
void BattleAnim_EndCanvasLayer(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  struct BattleWork *battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  s32 i;

  Sound_PlaySongFar(SONG_FADE_OUT_ACTION_SE);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  // The battle's projection centre, which Camera_ApplyShake moves.
  gProjection.centerX = 120;
  gProjection.centerY = 120;
  REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  Task_Remove(Palette_StepFadeTransfer);
  // This overwrites the saved y restored above before any frame passes, so
  // BG1 is always left at y 32.
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  REG_BLDCNT = 0;
  WaitFrames(1);

  BattlePresentation_ConfigurePaletteFadeFar(BATTLE_BACKGROUND_ZOOMED,
                                             battle->background, 7);
  WaitFrames(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(battle->background, 21 - i * 3);
    WaitFrames(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON,
                      0x20000);
  WaitFrames(1);
}

/// Hides BG1 and the windows and starts bringing the battle background's
/// palette back.
///
/// Called by BattleAnim_RunBeamSequence and
/// BattleAnim_RunRingAndSparkSequence (beams.c). Sets the background view's
/// secondMode to BATTLE_BACKGROUND_FIXED, queues DISPCNT for two frames later,
/// starts the battle's palette fade to BATTLE_BACKGROUND_ZOOMED and waits two
/// frames.
void BattleAnim_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)HEAP_SLOT_BLOCK(
               HEAP_SLOT_BATTLE_BACKGROUND))
              ->secondMode;
  battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
  *flag = BATTLE_BACKGROUND_FIXED;
  IoWriteQueue_Store16((u32)&REG_DISPCNT, DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                                              DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                                              DISPCNT_OBJ_ON);
  WaitFrames(1);
  BattlePresentation_ConfigurePaletteFadeFar(BATTLE_BACKGROUND_ZOOMED,
                                             battle->background, 0);
  WaitFrames(1);
}

/// Shows the canvas on BG1 instead of BG2, at its own size: BG1 becomes a
/// 256-colour layer whose map points 16 rows of 16 tiles at the canvas
/// (tiles 0x100 onwards, which at 64 bytes a tile start at character block
/// 1, where the canvas lives), with blank tile 0xff to the right, blended over
/// the rest inside the animation windows. Also clears the canvas. The
/// full-screen Ram_MapGridCellBuffer animations use
/// BattleAnim_FlushPendingGraphicsTransfer instead.
///
/// Called by BattleAnim_RunBeamSequence and
/// BattleAnim_RunRingAndSparkSequence (beams.c). Clears work->actorTimers,
/// starts the battle's palette fade to BATTLE_BACKGROUND_ZOOMED, clears tile
/// 0xff of character block 0 and writes BG1's map in screen block 31; writes
/// DISPCNT, BG1CNT, BLDCNT, BLDALPHA, the windows, WININ and WINOUT directly;
/// sets gBgScroll[1].y to 32 and the background view's secondMode to
/// BATTLE_BACKGROUND_NONE; clears the canvas and character block 1.
void BattleAnim_SetupCanvasTileMap(void) {
  // UB: indexing past gBattleWork reaches the heap slots after it.
#ifdef UBFIX
  void *canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  struct BattleWork *screen = gBattleWork;
  struct BattleBackgroundView *layer =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
#else
  u8 **cells = (u8 **)&gBattleWork;
  void *canvas = cells[31];
  struct BattleWork *screen = (struct BattleWork *)cells[0];
  struct BattleBackgroundView *layer = (struct BattleBackgroundView *)cells[35];
#endif
  s32 row;
  s32 column;
  s32 byteOffset;

  BattleAnim_ClearActorTimers();
  BattlePresentation_ConfigurePaletteFadeFar(BATTLE_BACKGROUND_ZOOMED,
                                             screen->background, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = BATTLE_BACKGROUND_NONE;
  // Blank tile 0xff, then map rows 4 to 11 filled with 0xffff.
  // XXX: what map entry 0xffff shows (tile 0x3ff, flipped, palette 15, in
  // a 256-colour layer); a VRAM view of a beam scene would settle whether
  // those rows are meant to show nothing.
  Iwram_ClearWords(BG_CHAR_BLOCK(0) + 0xff * 64, 64);
  Iwram_FillWords(BG_SCREEN_BLOCK(31) + 4 * 64, 0x200, -1);
  // Rows 12 to 27: the canvas's 16 tiles from tile 0x100, then blank tile
  // 0xff across the rest of the 32-tile row.
  byteOffset = 0;
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 32; column++, byteOffset += 2) {
      if (column > 15)
        *(volatile u16 *)(BG_SCREEN_BLOCK(31) + 12 * 64 + byteOffset) = 0xff;
      else
        *(volatile u16 *)(BG_SCREEN_BLOCK(31) + 12 * 64 + byteOffset) =
            row * 16 + 0x100 + column;
    }
  }
  REG_DISPCNT = ANIM_DISPCNT;
  REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(31);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  ANIM_SET_BATTLE_WINDOWS();
  REG_WININ = ANIM_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
              WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = ANIM_WINOUT;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Iwram_ClearWords(canvas, CANVAS_BYTES);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), CANVAS_BYTES);
  WaitFrames(1);
}

/// Draws a line on the 128x128 8bpp canvas (16 tiles across), keeping the
/// brighter colour where it crosses what is already drawn. The ends' y is
/// kept on the canvas; x is not. The line steps a pixel at a time along its
/// longer axis, following the other in 8-bit fixed point, and leaves out its
/// last pixel on that axis.
///
/// Called by BattleAnim_DrawStreaks (death_plunge.c) and
/// BattleAnim_RunNeptune. Writes only the canvas, and only while the caller
/// keeps x within 0 to 127: any other x wraps into a neighbouring row of tiles,
/// or off the canvas from its first or last row of tiles.
///
/// x0, y0 : the start, in pixels
/// x1, y1 : the end, in pixels
/// color  : the colour index
void BattleAnim_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                      s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80; // 0.5 in 8-bit fixed point
  u8 *canvas = ((u8 **)gBattleAnimWork)[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  CLAMP(y0, 0, 127);
  CLAMP(y1, 0, 127);

  if (ABS(dx) < ABS(dy)) {
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
