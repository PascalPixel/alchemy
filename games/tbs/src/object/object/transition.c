/// @file
/// Window transitions and dissolves between scenes.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

/// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank, so that every line
/// of the screen gets its own window edges.
///
/// A scheduler callback (priority 0x480) that DisplayTransition_Start,
/// DisplayTransition_Finish and DisplayTransition_InitializeBattleEffectState
/// add for a window transition, and DisplayTransition_UpdateScanlineTable
/// removes when it ends.
void BattleFx_StartWindowHBlankDma(void) {
  struct DisplayTransitionState *work = gDisplayTransition;
  u16 *scanlineData = work->pages[work->page];
  volatile u16 *dma0 = REG_DMA0;

  // Stop the previous transfer before priming the first line's registers.
  dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma0[5] &= ~DMA_ENABLE;
  (void)dma0[5];
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  Io_Write16(*scanlineData++, &REG_WININ);
  Io_Write16(*scanlineData++, &REG_WINOUT);
  Io_Write16(*scanlineData++, &REG_WIN0H);
  Io_Write16(*scanlineData++, &REG_WIN1H);
  Io_Write16(WIN_RANGE(0, 160), &REG_WIN0V);
  Io_Write16(WIN_RANGE(0, 160), &REG_WIN1V);
  // HBlank repeats one word: both horizontal window bounds for the next line.
  Dma_Set(scanlineData, (void *)&REG_WIN0H, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)dma0);
}

/// A run of screen lines in a transition shape: one window edge pair for every
/// line, or, when that is 0, edges moving from left x0 and right y0 to left
/// x1 and right y1 down the run.
struct TransitionSpan {
  u16 cnt; ///< lines
  u16 val;
  u8 x0; ///< screen pixels
  u8 x1; ///< screen pixels
  u8 y0; ///< screen pixels
  u8 y1; ///< screen pixels
};

/// One step of the shape window transition: the WININ and WINOUT values,
/// swapped as the transition opens or closes, and three runs of lines.
struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

extern struct TransitionShape gDisplayTransition_Shapes[];

/// Each frame of a window transition: advances its value toward its end (and
/// at the end turns the windows off, or moves to the next mode), then builds
/// the next page of window edges, WININ and WINOUT first and then a
/// (left << 8 | right) pair for each of the 160 lines. Bit 5 of the value says
/// whether the transition opens or closes; its low 5 bits are its progress.
/// Modes: 0 random bars, 1 a shape from gDisplayTransition_Shapes, 2 random
/// horizontal noise, 3 a circle at the screen's centre, 4 alternate lines
/// wiping in from the left and the right, 7 and 9 an ellipse wider than tall
/// around the party leader (7 flickers), 8 a wobbling ellipse around a point,
/// 77 an ellipse a little taller than wide, centred 8 pixels lower on the
/// leader.
///
/// A scheduler callback (priority 0xc80), added with
/// BattleFx_StartWindowHBlankDma; it removes both and stops DMA 0 when a
/// transition with no next mode ends. Writes the hidden page of the
/// transition state (heap slot 31) and then flips state->page.
void DisplayTransition_UpdateScanlineTable(void) {
  struct DisplayTransitionState *state;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  struct MapScrollWork *map;
  u16 *scanlineData;
#ifndef UBFIX
  // UB: viewX is a scalar, so indexing its address cannot reach viewY.
  // Direct named members grow the English function from 0x90c to 0x914
  // bytes. Modern code reads the actual members below.
  s32 *view7;
  s32 *view8;
  s32 *view9;
  s32 *view77;
#endif
  struct ObjectRuntime *obj;
  u32 i;
  s32 k;
  s32 lineCount;
  s32 value;
  s32 radiusSquaredQ16;
  s32 delta;
  u32 horizontalExtent;
  s32 left;
  s32 right;
  s32 centerX;
  s32 centerY;

  s32 cameraY;
  s32 cameraX;
  s32 cameraY8;
  u32 leftEdge;
  s32 oppositeEdges;

  map = gMapWork[0];
#ifdef UBFIX
  state = gDisplayTransition;
#else
  // UB: this indexes past gMapWork into the separate transition-work cell.
  // The named cell grows 0x90c to 0x910 native bytes in English.
  state = gMapWork[23];
#endif

  // Finish or advance the timed phase before generating the hidden page.
  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      if (state->nextMode == 0) {
        // A transition that closed (value 64) also turns backgrounds 1 to 3
        // and the sprites off, so the screen stays covered; one that opened
        // just turns the windows off.
        if (state->end == 64) {
          REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                           DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        } else {
          REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        }
        Scheduler_RemoveCallback(
            (SchedulerCallback)BattleFx_StartWindowHBlankDma);
        Scheduler_RemoveCallback(
            (SchedulerCallback)DisplayTransition_UpdateScanlineTable);
        {
          volatile u16 *dma0 = REG_DMA0;

          dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
          dma0[5] &= ~DMA_ENABLE;
          dma0[5];
        }
        return;
      }
      state->mode = state->nextMode;
    } else {
      state->value =
          state->start +
          Iwram_SignedDivide((state->end - state->start) * ++state->step,
                             state->duration);
    }
  }

  scanlineData = state->pages[1 ^ state->page];
  UiWindow_FillFromSceneFar(scanlineData + 2);

  // Each mode writes WININ and WINOUT first. 0x7f7f shows every layer inside
  // both windows and 1 only background 0 outside them; a mode swaps the two
  // to cover the screen inside the windows instead. Several modes fold the
  // progress when bit 5 is set, so that closing runs their table backwards.
  switch (state->mode) {
  case 0:
    *scanlineData++ = 0x7f7f;
    *scanlineData++ = 1;
    value = state->value;
    if (value & 32) {
      value = 32 - (value & 31);
    } else {
      value &= 31;
    }
    // Each line shows one bar of the table's width at a random place on the
    // 240-pixel line.
    value = gDisplayTransition_NoiseWidths[value];
    for (i = 0; i < 160; i++) {
      leftEdge = (Random16() * (241 - value)) >> 16;
      scanlineData[0] = ((leftEdge << 8) | (leftEdge + value));
      scanlineData += 2;
    }
    break;

  case 1:
    value = state->value;
    shape = &gDisplayTransition_Shapes[value & 31];
    if (value & 32) {
      *scanlineData++ = shape->first;
      *scanlineData++ = shape->second;
    } else {
      *scanlineData++ = shape->second;
      *scanlineData++ = shape->first;
    }
    // Three runs of lines, each one fixed edge pair or a slope between two.
    for (i = 0; i < 3; i++) {
      span = &shape->spans[i];
      lineCount = span->cnt;
      right = span->val;
      if (lineCount != 0) {
        if (right != 0) {
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] = right;
            scanlineData += 2;
          }
        } else {
          // These shared temporaries hold the left and right endpoints here.
          // Four span-local endpoints shrink 0x90c to 0x8bc bytes in English;
          // keep this reuse until a plain form matches.
          radiusSquaredQ16 = span->x0;
          left = span->y0;
          delta = span->x1;
          right = span->y1;
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] = (((radiusSquaredQ16 +
                                 (delta - radiusSquaredQ16) * k / lineCount)
                                << 8) +
                               (left + (right - left) * k / lineCount));
            scanlineData += 2;
          }
        }
      }
    }
    break;

  case 2:
    value = state->value - 1;
    if (value & 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
    } else {
      *scanlineData++ = 0x7f7f;
      *scanlineData++ = 1;
    }
    // Each line's window runs from the left edge to 16 pixels per step of
    // progress plus up to 15 at random, at most 255.
    value &= 31;
    for (i = 0; i < 160; i++) {
      horizontalExtent = (value << 4) + ((16 * Random16()) >> 16);
      if (horizontalExtent > 255) {
        horizontalExtent = 255;
      }
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case 3: {
#ifdef UBFIX
    // Closing writes four header halfwords, then 160 pairs: its last write
    // crosses the 322-halfword page. Keep that write into the following page
    // or solid tile, using bytes of the whole allocated state rather than an
    // out-of-bounds page pointer or a u16 alias of the u32 solid tile.
    u8 *pageBytes = (u8 *)state + (1 ^ state->page) * sizeof(state->pages[0]);
    const u16 header[4] = {0x7f7f, 1, 1, 0x7f7f};
    u32 headerSize = 2 * sizeof(u16);
    u16 edges;
#endif
    value = state->value;
#ifdef UBFIX
    if ((u32)value > 32) {
      headerSize = sizeof(header);
      value = 64 - value;
    }
    memcpy(pageBytes, header, headerSize);
    pageBytes += headerSize;
#else
    *scanlineData++ = 0x7f7f;
    *scanlineData++ = 1;
    if ((u32)value > 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
      value = 64 - value;
    }
#endif
    // A circle about the screen's centre (120, 80) whose radius grows 5
    // pixels a step, to 160 at 32; each line's edges are clamped to the
    // screen.
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - 80;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value) << 16)) >> 8;
      left = 120 - horizontalExtent;
      right = horizontalExtent + 120;
      if (left < 0) {
        left = 0;
      }
      if (right < 0) {
        right = 0;
      }
      if (left > 240) {
        left = 240;
      }
      if (right > 240) {
        right = 240;
      }
#ifdef UBFIX
      edges = (left << 8) + right;
      memcpy(pageBytes, &edges, sizeof(edges));
      pageBytes += 2 * sizeof(u16);
#else
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
#endif
    }
    break;
  }

  case 4:
    value = state->value;
    if (value & 32) {
      *scanlineData++ = 1;
      *scanlineData++ = 0x7f7f;
    } else {
      *scanlineData++ = 0x7f7f;
      *scanlineData++ = 1;
    }
    // Even lines' windows grow from the left edge and odd lines' from the
    // right, by up to 240 pixels over 32 steps.
    value = (u32)(240 * (value & 31)) >> 5;
    for (i = 0; i < 160; i += 2) {
      oppositeEdges = ((240 - value) << 8) + 240;
      scanlineData[0] = value;
      scanlineData[2] = oppositeEdges;
      scanlineData += 4;
    }
    break;

  case 7: {
    s32 ax;

#ifdef UBFIX
    ax = map->viewX & 0xffff0000;
    cameraY = map->viewY & 0xffff0000;
#else
    view7 = &map->viewX;
    ax = view7[0] & 0xffff0000;
    cameraY = view7[1] & 0xffff0000;
#endif
    // Centred on the leader's place on screen, 16 pixels above its feet.
    obj = ObjectTable_Get(gGameState.selectedActor);
    centerX = (obj->x - ax) / 0x10000;
    centerY = ((obj->z - obj->y) - cameraY) / 0x10000 - 16;
    // The frame counter is read here, where mode 8 forms its index, and the
    // value is not used.
    gFrameCount;
    // Mode 7 writes the two masks and folds the progress the other way round
    // from mode 9.
    *scanlineData++ = state->active;
    *scanlineData++ = state->mask;
    value = state->value;
    if ((value & 32) == 0) {
      value = 32 - (value & 31);
    } else {
      value &= 31;
    }
    // On odd frames the ellipse has no size: the flicker.
    if (gFrameCount & 1) {
      value = 0;
    }
    // Half-widths of sqrt(r * r - 1.5 * dy * dy) with r 5 pixels a step: an
    // ellipse 1.22 times as wide as it is tall.
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0) {
        left = 0;
      }
      if (right < 0) {
        right = 0;
      }
      if (left > 240) {
        left = 240;
      }
      if (right > 240) {
        right = 240;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  case 8:
#ifdef UBFIX
    cameraX = map->viewX & 0xffff0000;
    cameraY8 = map->viewY & 0xffff0000;
#else
    view8 = &map->viewX;
    cameraX = view8[0] & 0xffff0000;
    cameraY8 = view8[1] & 0xffff0000;
#endif
    // Centred on state->centreX and centreY; each line's edges move out by
    // gDisplayTransition_EdgeOffsets, indexed two entries a line and one a
    // frame, so the outline ripples.
    centerX = (state->centreX - cameraX) / 0x10000;
    centerY = (state->centreY - cameraY8) / 0x10000 - 16;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32) {
      value = 32 - (value & 31);
    } else {
      value &= 31;
    }
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        delta = gDisplayTransition_EdgeOffsets[k & 31];
        left -= delta;
        right += delta;
        // An offset that closes the line leaves it with no window.
        if (left >= right) {
          left = 240;
          right = 240;
        }
      }
      if (left < 0) {
        left = 0;
      }
      if (right < 0) {
        right = 0;
      }
      if (left > 240) {
        left = 240;
      }
      if (right > 240) {
        right = 240;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
      k += 2;
    }
    break;

  case 9: {
    s32 ax;

#ifdef UBFIX
    ax = map->viewX & 0xffff0000;
    cameraY = map->viewY & 0xffff0000;
#else
    view9 = &map->viewX;
    ax = view9[0] & 0xffff0000;
    cameraY = view9[1] & 0xffff0000;
#endif
    obj = ObjectTable_Get(gGameState.selectedActor);
    centerX = (obj->x - ax) / 0x10000;
    centerY = ((obj->z - obj->y) - cameraY) / 0x10000 - 16;
    // As in mode 7, the frame counter is read and not used.
    gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32) {
      value = 32 - (value & 31);
    } else {
      value &= 31;
    }
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0) {
        left = 0;
      }
      if (right < 0) {
        right = 0;
      }
      if (left > 240) {
        left = 240;
      }
      if (right > 240) {
        right = 240;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  case 77: {
    s32 ax;

#ifdef UBFIX
    ax = map->viewX & 0xffff0000;
    cameraY = map->viewY & 0xffff0000;
#else
    view77 = &map->viewX;
    ax = view77[0] & 0xffff0000;
    cameraY = view77[1] & 0xffff0000;
#endif
    obj = ObjectTable_Get(gGameState.selectedActor);
    centerX = (obj->x - ax) / 0x10000;
    // Centred 8 pixels above the leader's feet, not 16 as in modes 7 and 9.
    centerY = ((obj->z - obj->y) - cameraY) / 0x10000 - 8;
    // As in mode 7, the frame counter is read and not used.
    gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & 32) {
      value = 32 - (value & 31);
    } else {
      value &= 31;
    }
    // sqrt(r * r - 0.75 * dy * dy): 1.15 times as tall as it is wide.
    value *= 5;
    radiusSquaredQ16 = (value * value) << 16;
    for (i = 0; i < 160; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 14)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0) {
        left = 0;
      }
      if (right < 0) {
        right = 0;
      }
      if (left > 240) {
        left = 240;
      }
      if (right > 240) {
        right = 240;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  }

  // Publish only after every span of the next page is ready.
  state->page ^= 1;
}

/// Starts window transition mode at value 0, with no duration: the caller
/// moves it.
///
/// Called by Reveal's effect (mode 8) and, through
/// DisplayTransition_InitializeBattleEffectStateFar, by three scenes (Altmiller
/// Cave, Tret Tree's branch and Babi's palace). Allocates and clears the
/// transition state in heap slot 31 and adds both window callbacks; mask
/// 0x3f3f shows every layer in both windows.
void DisplayTransition_InitializeBattleEffectState(s32 mode) {
  struct DisplayTransitionState *state =
      Runtime_AllocateBlock(31, sizeof(*state));

  Dma_Fill32(0, state, sizeof(*state));
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = 1;
  Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
  Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
}

/// Suspends the window transition's two callbacks.
///
/// XXX: nothing in ☀️ names it and it has no far-call entry, yet it is not
/// marked UNUSED; a caller through a table or ⚓️'s twin would settle it.
void DisplayTransition_SuspendCallbacks(void) {
  Scheduler_SuspendTasks(&DisplayTransition_UpdateScanlineTable);
  Scheduler_SuspendTasks(&BattleFx_StartWindowHBlankDma);
}

/// Allocates the display transition state (heap block 31) and clears it.
///
/// Called by DisplayTransition_Start and DisplayTransition_Finish for every
/// kind but the two fades.
struct DisplayTransitionState *DisplayTransition_AllocateAndClearState(void) {
  struct DisplayTransitionState *state;

  state = Runtime_AllocateBlock(31, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  return state;
}

/// Fades the screen in over frames frames. mode's high byte chooses how: 0
/// from black, 1 from the backdrop colour, 2 a window transition (its low
/// byte the mode), 3 a dissolve, 4 opening from the centre line (low byte 1
/// flickers).
///
/// Called by Field_RunScene to fade a scene in with work->transition and
/// work->transitionFrames, and by Event_StartScreenTransition for scene
/// scripts. Kinds 2 to 4 add a scheduler callback (and kind 4 the VCount
/// handler, IRQ 1) that runs the transition; every kind ORs the map's display
/// bits into DISPCNT through the I/O write queue.
void DisplayTransition_Start(s32 mode, s32 frames) {
  struct DisplayTransitionWindow *display;
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  display = gMapWork[0];
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    Fade_ToBlack(0);
    Fade_FromBlack(frames);
    WaitFrames(1);
    break;
  case 1:
    BattleFx_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
    BattleFx_StartBufferInterpolation(frames);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    BattleFx_SetPrimaryBufferValue(0);
    return;
  // The window, dissolve and centre-line kinds open from value 0 (closed) to
  // 32 (open), or for the centre line from 80 to 0, over frames frames.
  case 2: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 0;
    state->mask = 63;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 3: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(15);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        0x20000);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case 4: {
    struct DisplayTransitionState *state;

    display = gMapWork[0];
    state = DisplayTransition_AllocateAndClearState();
    display->firstLine = 80;
    display->secondLine = 80;
    WaitFrames(1);
    if (variant == 0) {
      Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      0x20000);
}

/// Fades the screen out over frames frames, as DisplayTransition_Start fades
/// it in.
///
/// Called by Field_RunScene as a scene ends, by Field_StartEncounterTransition
/// before a battle (kinds 0x401 and 0x207), and by Event_FinishScreenTransition
/// for scene scripts. The window and dissolve kinds run from value 32 to 64,
/// the centre line from 0 to 80.
void DisplayTransition_Finish(s32 mode, s32 frames) {
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case 0:
    Fade_FromBlack(0);
    Fade_ToBlack(frames);
    break;
  case 1:
    BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
    BattleFx_StartBufferInterpolation(frames);
    break;
  case 2: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    state->mask = 63;
    state->active = 1;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
    Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
    WaitFrames(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 3: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    DisplayTransition_FillTilemapAndSolidTile(0);
    WaitFrames(1);
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case 4: {
    struct DisplayTransitionState *state =
        DisplayTransition_AllocateAndClearState();
    if (variant == 0) {
      Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    } else {
      Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
      Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
}

/// Clears the map's display-control bits for backgrounds 3, 2 and 1
/// (0x0800, 0x0400 and 0x0200) for each argument that is set.
///
/// Reached only through its unnamed far-call veneer. DisplayTransition_Start
/// ORs these bits into DISPCNT.
void DisplayState_ClearFlags(s32 clearBg3, s32 clearBg2, s32 clearBg1) {
  struct DisplayTransitionWindow *state;

  state = gMapWork[0];
  if (state != NULL) {
    if (clearBg1 != 0) {
      state->dispcnt &= 0xFDFF;
    }
    if (clearBg2 != 0) {
      state->dispcnt &= 0xFBFF;
    }
    if (clearBg3 != 0) {
      state->dispcnt &= 0xF7FF;
    }
  }
}

/// Each frame of the centre-line transition: moves the value toward its end
/// and sets the two lines where the screen switches between shown and blank.
///
/// A scheduler callback added by DisplayTransition_Start and
/// DisplayTransition_Finish for kind 4, variant 0; when its run ends it removes
/// itself and the VCount handler.
void DisplayTransition_Update(void) {
  struct DisplayTransitionFrame *state = *Ram_DisplayWork;
  struct DisplayTransitionWindow *display =
      *(struct DisplayTransitionWindow **)Ram_MapWork;
  s8 *duration = &state->duration;
  u32 edgeDistance;

  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_Update);
      Runtime_SetIrqHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 value;

      (*step)++;
      value = Iwram_SignedDivide(delta * *step, *duration);
      state->value = state->start + value;
    }
  }

  // At 80 or more the lines are put below the screen's 160, so no switch
  // happens.
  edgeDistance = state->value;
  if (edgeDistance > 79) {
    display->firstLine = 200;
    display->secondLine = 250;
  } else {
    display->firstLine = edgeDistance;
    display->secondLine = 159 - edgeDistance;
  }
}

/// As DisplayTransition_Update, but the lines flicker every other frame.
///
/// The kind 4 callback for a non-zero variant.
void DisplayTransition_UpdateFromCentre(void) {
  struct DisplayTransitionFrame *state = *Ram_DisplayWork;
  struct DisplayTransitionWindow *window =
      *(struct DisplayTransitionWindow **)Ram_MapWork;
  s8 *duration = &state->duration;
  u16 edgeDistance;

  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
      Runtime_SetIrqHandler(1, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 edgeOffset;

      (*step)++;
      edgeOffset = Iwram_SignedDivide(delta * *step, *duration);
      state->value = state->start + edgeOffset;
    }
  }

  edgeDistance = state->value;
  if (edgeDistance > 79) {
    window->firstLine = 200;
    window->secondLine = 250;
  } else if (edgeDistance != 0 && (gFrameCount & 1)) {
    window->firstLine = edgeDistance + 80;
    window->secondLine = 80 - edgeDistance;
  } else {
    window->firstLine = 0;
    window->secondLine = 159;
  }
}

/// The VCount interrupt of the centre-line transition: switches the display
/// to mode 2 (nothing shown) at the first line and back to mode 0 at the
/// second.
///
/// Installed as IRQ handler 1 by DisplayTransition_Start and
/// DisplayTransition_Finish for kind 4. Phases: 0 waits for a line on screen,
/// 1 for whichever edge line comes first, 2 and 3 for the other one; 9 is done.
void DisplayTransition_UpdateScanline(void) {
  u32 scanline;
  struct DisplayTransitionWindow *state;

  scanline = REG_VCOUNT;
  state = gMapWork[0];

  // Keep the same VCount sample across phase changes. A for/switch retry loop
  // shrinks the English function from 0xd2 to 0xba bytes.
again:
  switch (state->irqPhase) {
  case 3:
    if (scanline >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xfff8) | DISPCNT_MODE_2;
      state->irqPhase = 9;
    }
    break;
  case 2:
    if (scanline >= state->irqSecondLine) {
      REG_DISPCNT &= 0xfff8;
      state->irqPhase = 9;
    }
    break;
  case 1:
    if (scanline >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xfff8) | DISPCNT_MODE_2;
      state->irqPhase++;
      goto again;
    }
    if (scanline >= state->irqSecondLine) {
      REG_DISPCNT &= 0xfff8;
      state->irqPhase = 3;
      goto again;
    }
    break;
  case 0:
    if (scanline <= 158) {
      state->irqPhase = 1;
      goto again;
    }
    break;
  }
}

/// Each frame of the dissolve: moves the value toward its end and sets two
/// more of the solid tile's 64 pixels, in the order of
/// gDisplayTransition_DitherTable, to colour 15 (or 0 when closing).
///
/// The dissolve's scheduler callback (kind 3 of DisplayTransition_Start and
/// DisplayTransition_Finish, and DisplayTransition_InitializeState). Queues the
/// changed tile into character block 0 each frame.
void DisplayTransition_UpdateFrame(void) {
  struct DisplayTransitionFrame *state = *Ram_DisplayWork;
  s32 ditherIndex;
  s32 colorIndex;
  u32 i;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      state->duration = 0;
      Scheduler_RemoveCallback(DisplayTransition_UpdateFrame);
      dma0 = REG_DMA0;
      dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
      dma0[5] &= ~DMA_ENABLE;
      (void)dma0[5];
      return;
    } else {
      s32 delta = state->end - state->start;
      state->step++;
      state->value = state->start +
                     Iwram_SignedDivide(delta * state->step, state->duration);
    }
  }

  ditherIndex = state->value - 1;
  state->ditherToggle ^= 1;
  colorIndex = 0;
  if (ditherIndex & 32)
    colorIndex = 15;
  ditherIndex = (ditherIndex & 31) * 2;
  for (i = 0; i <= 1; i++) {
    u8 pixelIndex = gDisplayTransition_DitherTable[ditherIndex & 63];
    u8 *tileByte = &state->paletteNibbles[pixelIndex >> 1];
    if (pixelIndex & 1)
      *tileByte = (*tileByte & 0x0f) | (colorIndex << 4);
    else
      *tileByte = (*tileByte & 0xf0) | colorIndex;
    ditherIndex++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->paletteNibbles,
                      (DMA_ENABLE | DMA_32BIT) << 16 | 0x8);
}

/// Fills background screen block 4 with tile 0 in palette 15 and, unless color
/// is -1, makes tile 0 solid color.
///
/// Sets up the dissolve for DisplayTransition_Start (colour 15),
/// DisplayTransition_Finish and DisplayTransition_InitializeState (colour 0).
void DisplayTransition_FillTilemapAndSolidTile(s32 color) {
  struct DisplayTransitionState *work = gDisplayTransition;
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 0x500);
  if (color != -1) {
    u32 pattern = 0;
    s32 remainingWords;
    u32 *tile;
    for (remainingWords = 7; remainingWords >= 0; --remainingWords)
      pattern = (pattern << 4) | color;
    tile = work->solidTile;
    for (remainingWords = 7; remainingWords >= 0; --remainingWords)
      *tile++ = pattern;
    Dma_Set(work->solidTile, BG_CHAR_BLOCK(0),
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x8, REG_DMA3);
  }
}

/// Starts a dissolve of the given mode and waits 120 frames.
///
/// It sets no duration, so the dissolve stays at value 0 until a caller moves
/// it. Reached only through its unnamed far-call veneer.
void DisplayTransition_InitializeState(s32 value) {
  struct DisplayTransitionState *state;

  state = Runtime_AllocateBlock(0x1f, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  DisplayTransition_FillTilemapAndSolidTile(0);
  state->mode = value;
  state->value = 0;
  Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
  WaitFrames(0x78);
}
