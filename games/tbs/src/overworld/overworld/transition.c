/// @file
/// Screen transitions between maps: window transitions and dissolves.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank, so that every line
/// of the screen gets its own window edges.
///
/// A task (priority TASK_PASS_VBLANK) that ScreenTransition_Start,
/// ScreenTransition_Finish and ScreenTransition_InitializeBattleEffectState
/// add for a window transition, and ScreenTransition_UpdateScanlineTable
/// removes when it ends.
void FieldEffect_StartWindowHBlankDma(void) {
  struct ScreenTransitionState *work = gScreenTransitionWork;
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
  Io_Write16(WIN_RANGE(0, DISPLAY_HEIGHT), &REG_WIN0V);
  Io_Write16(WIN_RANGE(0, DISPLAY_HEIGHT), &REG_WIN1V);
  // HBlank repeats one word: both horizontal window bounds for the next line.
  Dma_Set(scanlineData, (void *)&REG_WIN0H, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)dma0);
}

/// A run of screen lines in a transition shape: one window edge pair for every
/// line, or, when that is 0, edges moving from left x0 and right y0 to left
/// x1 and right y1 down the run.
struct TransitionSpan {
  u16 count; ///< lines; 0 for an unused run
  /// The (left << 8 | right) pair every line of the run takes, or 0 for the
  /// sloped edges below.
  u16 value;
  u8 x0; ///< screen pixels: the left edge on the run's first line
  u8 x1; ///< screen pixels: the left edge the run moves toward
  u8 y0; ///< screen pixels: the right edge on the run's first line
  u8 y1; ///< screen pixels: the right edge the run moves toward
};

/// One step of WINDOW_TRANSITION_SHAPE, a row of
/// gScreenTransition_Shapes chosen by the progress: the WININ and WINOUT
/// values, swapped as the transition opens or closes, and three runs of lines
/// written one after another from the top of the screen.
struct TransitionShape {
  u16 first;  ///< WINOUT while opening, WININ while closing
  u16 second; ///< WININ while opening, WINOUT while closing
  struct TransitionSpan spans[3];
};

extern struct TransitionShape gScreenTransition_Shapes[];

/// Each frame of a window transition: advances its value toward its end (and
/// at the end turns the windows off, or moves to the next mode), then builds
/// the next page of window edges, WININ and WINOUT first and then a
/// (left << 8 | right) pair for each of the 160 lines. Bit 5 of the value says
/// whether the transition opens or closes; its low 5 bits are its progress.
/// The mode is an enum WindowTransitionMode.
///
/// A task (priority TASK_PASS_FRAME), added with
/// FieldEffect_StartWindowHBlankDma; it removes both and stops DMA 0 when a
/// transition with no next mode ends. Writes the hidden page of the
/// transition state (HEAP_SLOT_SCREEN_TRANSITION) and then flips state->page.
void ScreenTransition_UpdateScanlineTable(void) {
  struct ScreenTransitionState *state;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  struct MapGridScrollWork *grid;
  u16 *scanlineData;
#ifndef UBFIX
  // UB: viewX is a scalar, so indexing its address cannot reach viewY.
  // Direct named members grow the English function from 0x90c to 0x914
  // bytes. Modern code reads the actual members below.
  s32 *viewInverse;
  s32 *viewRipple;
  s32 *viewLeader;
  s32 *viewLeaderLow;
#endif
  struct Actor *actor;
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

  grid = gMapGridWork[0];
#ifdef UBFIX
  state = gScreenTransitionWork;
#else
  // UB: this indexes past gMapGridWork into the separate transition-work cell.
  // The named cell grows 0x90c to 0x910 native bytes in English.
  state = gMapGridWork[23];
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
        Task_Remove((TaskFunc)FieldEffect_StartWindowHBlankDma);
        Task_Remove((TaskFunc)ScreenTransition_UpdateScanlineTable);
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

  // Build the page not shown. Its line pairs start as the open UI windows'
  // edges (ScanlineWindow_FillSpansFar; with no UI work, 0xe0e0 over the
  // first 80 lines only, as its BUG notes); each mode below then writes window
  // 0's edge of every pair and leaves window 1's.
  scanlineData = state->pages[1 ^ state->page];
  ScanlineWindow_FillSpansFar(scanlineData + 2);

  // Each mode writes WININ and WINOUT first. 0x7f7f shows every layer inside
  // both windows and 1 only background 0 outside them; a mode swaps the two
  // to cover the screen inside the windows instead. Several modes fold the
  // progress when bit 5 is set, so that closing runs their table backwards.
  switch (state->mode) {
  case WINDOW_TRANSITION_BARS:
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
    value = gScreenTransition_NoiseWidths[value];
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      leftEdge = (Random16() * (241 - value)) >> 16;
      scanlineData[0] = ((leftEdge << 8) | (leftEdge + value));
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_SHAPE:
    value = state->value;
    shape = &gScreenTransition_Shapes[value & 31];
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
      lineCount = span->count;
      right = span->value;
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

  case WINDOW_TRANSITION_NOISE:
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
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      horizontalExtent = (value << 4) + ((16 * Random16()) >> 16);
      if (horizontalExtent > 255) {
        horizontalExtent = 255;
      }
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_CIRCLE: {
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
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
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
      if (left > DISPLAY_WIDTH) {
        left = DISPLAY_WIDTH;
      }
      if (right > DISPLAY_WIDTH) {
        right = DISPLAY_WIDTH;
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

  case WINDOW_TRANSITION_INTERLACED_WIPE:
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
    value = (u32)(DISPLAY_WIDTH * (value & 31)) >> 5;
    for (i = 0; i < DISPLAY_HEIGHT; i += 2) {
      oppositeEdges = ((DISPLAY_WIDTH - value) << 8) + DISPLAY_WIDTH;
      scanlineData[0] = value;
      scanlineData[2] = oppositeEdges;
      scanlineData += 4;
    }
    break;

  case WINDOW_TRANSITION_LEADER_INVERSE: {
    s32 ax;

#ifdef UBFIX
    ax = grid->viewX & 0xffff0000;
    cameraY = grid->viewY & 0xffff0000;
#else
    viewInverse = &grid->viewX;
    ax = viewInverse[0] & 0xffff0000;
    cameraY = viewInverse[1] & 0xffff0000;
#endif
    // Centred on the leader's place on screen, 16 pixels above its feet.
    actor = Actor_Lookup(gSaveBlock.leaderActorId);
    centerX = (actor->x.fixed - ax) / Q16_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / Q16_ONE - 16;
    // The frame counter is read here, where WINDOW_TRANSITION_RIPPLE forms
    // its index, and the value is not used.
    gFrameCount;
    // This mode writes the two masks and folds the progress the other way
    // round from WINDOW_TRANSITION_LEADER.
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
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
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
      if (left > DISPLAY_WIDTH) {
        left = DISPLAY_WIDTH;
      }
      if (right > DISPLAY_WIDTH) {
        right = DISPLAY_WIDTH;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  case WINDOW_TRANSITION_RIPPLE:
#ifdef UBFIX
    cameraX = grid->viewX & 0xffff0000;
    cameraY8 = grid->viewY & 0xffff0000;
#else
    viewRipple = &grid->viewX;
    cameraX = viewRipple[0] & 0xffff0000;
    cameraY8 = viewRipple[1] & 0xffff0000;
#endif
    // Centred on state->centreX and centreY; each line's edges move out by
    // gScreenTransition_EdgeOffsets, indexed two entries a line and one a
    // frame, so the outline ripples.
    centerX = (state->centreX - cameraX) / Q16_ONE;
    centerY = (state->centreY - cameraY8) / Q16_ONE - 16;
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
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredQ16 - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        delta = gScreenTransition_EdgeOffsets[k & 31];
        left -= delta;
        right += delta;
        // An offset that closes the line leaves it with no window.
        if (left >= right) {
          left = DISPLAY_WIDTH;
          right = DISPLAY_WIDTH;
        }
      }
      if (left < 0) {
        left = 0;
      }
      if (right < 0) {
        right = 0;
      }
      if (left > DISPLAY_WIDTH) {
        left = DISPLAY_WIDTH;
      }
      if (right > DISPLAY_WIDTH) {
        right = DISPLAY_WIDTH;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
      k += 2;
    }
    break;

  case WINDOW_TRANSITION_LEADER: {
    s32 ax;

#ifdef UBFIX
    ax = grid->viewX & 0xffff0000;
    cameraY = grid->viewY & 0xffff0000;
#else
    viewLeader = &grid->viewX;
    ax = viewLeader[0] & 0xffff0000;
    cameraY = viewLeader[1] & 0xffff0000;
#endif
    actor = Actor_Lookup(gSaveBlock.leaderActorId);
    centerX = (actor->x.fixed - ax) / Q16_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / Q16_ONE - 16;
    // As in WINDOW_TRANSITION_LEADER_INVERSE, the frame counter is read and
    // not used.
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
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
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
      if (left > DISPLAY_WIDTH) {
        left = DISPLAY_WIDTH;
      }
      if (right > DISPLAY_WIDTH) {
        right = DISPLAY_WIDTH;
      }
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  case WINDOW_TRANSITION_LEADER_LOW: {
    s32 ax;

#ifdef UBFIX
    ax = grid->viewX & 0xffff0000;
    cameraY = grid->viewY & 0xffff0000;
#else
    viewLeaderLow = &grid->viewX;
    ax = viewLeaderLow[0] & 0xffff0000;
    cameraY = viewLeaderLow[1] & 0xffff0000;
#endif
    actor = Actor_Lookup(gSaveBlock.leaderActorId);
    centerX = (actor->x.fixed - ax) / Q16_ONE;
    // Centred 8 pixels above the leader's feet, not 16 as in
    // WINDOW_TRANSITION_LEADER.
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / Q16_ONE - 8;
    // As in WINDOW_TRANSITION_LEADER_INVERSE, the frame counter is read and
    // not used.
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
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
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
      if (left > DISPLAY_WIDTH) {
        left = DISPLAY_WIDTH;
      }
      if (right > DISPLAY_WIDTH) {
        right = DISPLAY_WIDTH;
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
/// Called by Reveal's effect (WINDOW_TRANSITION_RIPPLE) and, through
/// ScreenTransition_InitializeBattleEffectStateFar, by three maps (Altmiller
/// Cave, Tret Tree's branch and Babi's palace). Allocates and clears the
/// transition state in HEAP_SLOT_SCREEN_TRANSITION and adds both window
/// callbacks; mask 0x3f3f shows every layer in both windows.
void ScreenTransition_InitializeBattleEffectState(s32 mode) {
  struct ScreenTransitionState *state =
      AllocHeapSlotEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));

  Dma_Fill32(0, state, sizeof(*state));
  state->mode = mode;
  state->value = 0;
  state->mask = 0x3f3f;
  state->active = 1;
  Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
  Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_VBLANK);
}

/// Unused: suspends the window transition's two callbacks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_SuspendAll(&ScreenTransition_UpdateScanlineTable);
  Task_SuspendAll(&FieldEffect_StartWindowHBlankDma);
}

/// Allocates the screen transition state (HEAP_SLOT_SCREEN_TRANSITION) and
/// clears it.
///
/// Called by ScreenTransition_Start and ScreenTransition_Finish for every
/// kind but the two fades.
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void) {
  struct ScreenTransitionState *state;

  state = AllocHeapSlotEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  return state;
}

/// Fades the screen in over frames frames. mode's high byte is its enum
/// ScreenTransitionStyle: from black, from the backdrop colour, a window
/// transition (its low byte an enum WindowTransitionMode), a dissolve (3) or
/// opening from the centre line (4, which a non-zero low byte makes flicker).
/// It returns after one frame while the fade runs on: a caller that must wait
/// for it, as SceneManager_Run does, waits itself.
///
/// Called by SceneManager_Run to fade a map in with work->transition and
/// work->transitionFrames, and by FieldScript_StartScreenTransition for map
/// scripts. Every style but the fades adds a task (and the centre
/// line the HBlank handler, IRQ 1) that runs the transition; every kind
/// ORs the map's display bits into DISPCNT through the I/O write queue.
void ScreenTransition_Start(s32 mode, s32 frames) {
  struct ScreenTransitionWin *display;
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  display = gMapGridWork[0];
  variant = mode & 0xff;
  switch (transitionKind) {
  case TRANSITION_FADE:
    ScreenFade_ToBlack(0);
    ScreenFade_FromBlack(frames);
    WaitFrames(1);
    break;
  case TRANSITION_BACKDROP_FADE:
    PaletteFade_SetSource(PALETTE_TINT_FROM_SOURCE, PLTT_BACKDROP);
    PaletteFade_StartInterpolation(frames);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_16);
    PaletteFade_SetBackdrop(0);
    return;
  // The window, dissolve and centre-line kinds open from value 0 (closed) to
  // 32 (open), or for the centre line from 80 to 0, over frames frames.
  case TRANSITION_WINDOW: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 0;
    state->mask = 63;
    state->active = 1;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_VBLANK);
    WaitFrames(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_16);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case TRANSITION_DISSOLVE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(15);
    WaitFrames(1);
    Task_Add(ScreenTransition_UpdateFrame, TASK_PASS_FRAME);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_16);
    state->start = 0;
    state->end = 32;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case TRANSITION_CENTRE_LINE: {
    struct ScreenTransitionState *state;

    display = gMapGridWork[0];
    state = ScreenTransition_AllocateAndClearState();
    display->firstLine = 80;
    display->secondLine = 80;
    WaitFrames(1);
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 80;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      IO_WRITE_16);
}

/// Fades the screen out over frames frames, as ScreenTransition_Start fades
/// it in.
///
/// Called by SceneManager_Run as a map ends, by Field_StartEncounterTransition
/// before a battle (the flickering centre line, and the
/// WINDOW_TRANSITION_LEADER_INVERSE window), and by
/// FieldScript_FinishScreenTransition for map scripts. The window and
/// dissolve kinds run from value 32 to 64, the centre line from 0 to 80. Like
/// ScreenTransition_Start it does not wait for the fade: SceneManager_Run waits
/// transitionFrames after it.
void ScreenTransition_Finish(s32 mode, s32 frames) {
  s32 variant;
  s32 transitionKind;

  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
  case TRANSITION_FADE:
    ScreenFade_FromBlack(0);
    ScreenFade_ToBlack(frames);
    break;
  case TRANSITION_BACKDROP_FADE:
    PaletteFade_SetTarget(PALETTE_TINT_FROM_SOURCE, 0);
    PaletteFade_StartInterpolation(frames);
    break;
  case TRANSITION_WINDOW: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    state->mask = 63;
    state->active = 1;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_VBLANK);
    WaitFrames(1);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case TRANSITION_DISSOLVE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 32;
    ScreenTransition_FillTilemapAndSolidTile(0);
    WaitFrames(1);
    Task_Add(ScreenTransition_UpdateFrame, TASK_PASS_FRAME);
    state->start = 32;
    state->end = 64;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case TRANSITION_CENTRE_LINE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 0;
      state->end = 80;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
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
/// Reached only through its unnamed far-call veneer. ScreenTransition_Start
/// ORs these bits into DISPCNT.
void ScreenTransition_HideBackgrounds(s32 clearBg3, s32 clearBg2,
                                      s32 clearBg1) {
  struct ScreenTransitionWin *state;

  state = gMapGridWork[0];
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
/// A task added by ScreenTransition_Start and
/// ScreenTransition_Finish for the centre line with variant 0; when its run
/// ends it removes itself and the HBlank handler.
void ScreenTransition_Update(void) {
  struct ScreenTransitionFrame *state = *Ram_DisplayWork;
  struct ScreenTransitionWin *display =
      *(struct ScreenTransitionWin **)Ram_MapGridWork;
  s8 *duration = &state->duration;
  u32 edgeDistance;

  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_Update);
      Irq_SetHandler(IRQ_HBLANK, 0, 0);
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

/// As ScreenTransition_Update, but the lines flicker every other frame.
///
/// The centre line's callback for a non-zero variant.
void ScreenTransition_UpdateFromCentre(void) {
  struct ScreenTransitionFrame *state = *Ram_DisplayWork;
  struct ScreenTransitionWin *window =
      *(struct ScreenTransitionWin **)Ram_MapGridWork;
  s8 *duration = &state->duration;
  u16 edgeDistance;

  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_UpdateFromCentre);
      Irq_SetHandler(IRQ_HBLANK, 0, 0);
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

/// The HBlank interrupt of the centre-line transition: switches the display
/// to DISPCNT_MODE_2 (nothing shown) at the first line and back to
/// DISPCNT_MODE_0 at the second.
///
/// Installed as IRQ handler 1 (IRQ_HBLANK) by ScreenTransition_Start and
/// ScreenTransition_Finish for the centre line, so it runs after every line
/// and reads the line from REG_VCOUNT, waiting through enum
/// ScreenTransitionIrqPhase.
/// The centre line's HBlank handler's phases (irqPhase).
enum ScreenTransitionIrqPhase {
  DISPLAY_TRANSITION_IRQ_WAIT_SCREEN = 0, ///< waits for a line on screen
  /// Waits for whichever edge line comes first.
  DISPLAY_TRANSITION_IRQ_WAIT_EITHER = 1,
  /// The first line has passed: waits for the second.
  DISPLAY_TRANSITION_IRQ_WAIT_SECOND = 2,
  /// The second line has passed: waits for the first.
  DISPLAY_TRANSITION_IRQ_WAIT_FIRST = 3,
  DISPLAY_TRANSITION_IRQ_DONE = 9
};

void ScreenTransition_UpdateScanline(void) {
  u32 scanline;
  struct ScreenTransitionWin *state;

  scanline = REG_VCOUNT;
  state = gMapGridWork[0];

  // Keep the same VCount sample across phase changes. A for/switch retry loop
  // shrinks the English function from 0xd2 to 0xba bytes.
again:
  switch (state->irqPhase) {
  case DISPLAY_TRANSITION_IRQ_WAIT_FIRST:
    if (scanline >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xfff8) | DISPCNT_MODE_2;
      state->irqPhase = DISPLAY_TRANSITION_IRQ_DONE;
    }
    break;
  case DISPLAY_TRANSITION_IRQ_WAIT_SECOND:
    if (scanline >= state->irqSecondLine) {
      REG_DISPCNT &= 0xfff8;
      state->irqPhase = DISPLAY_TRANSITION_IRQ_DONE;
    }
    break;
  case DISPLAY_TRANSITION_IRQ_WAIT_EITHER:
    if (scanline >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & 0xfff8) | DISPCNT_MODE_2;
      state->irqPhase++;
      goto again;
    }
    if (scanline >= state->irqSecondLine) {
      REG_DISPCNT &= 0xfff8;
      state->irqPhase = DISPLAY_TRANSITION_IRQ_WAIT_FIRST;
      goto again;
    }
    break;
  case DISPLAY_TRANSITION_IRQ_WAIT_SCREEN:
    if (scanline <= 158) {
      state->irqPhase = DISPLAY_TRANSITION_IRQ_WAIT_EITHER;
      goto again;
    }
    break;
  }
}

/// Each frame of the dissolve: moves the value toward its end and sets two
/// more of the solid tile's 64 pixels, in the order of
/// gScreenTransition_DitherTable: to colour 0 while the screen opens (values
/// 1 to 32 clear the cover ScreenTransition_Start filled with colour 15), and
/// to colour 15 while it closes (values 33 to 64 fill the clear tile
/// ScreenTransition_Finish starts from). Each frame sets only the current
/// value's two pixels, so over fewer than 32 frames some values, and their
/// pixels, are skipped, and over more some repeat.
///
/// The dissolve's task (from ScreenTransition_Start and
/// ScreenTransition_Finish, and ScreenTransition_InitializeState). Queues the
/// changed tile into character block 0 each frame.
void ScreenTransition_UpdateFrame(void) {
  struct ScreenTransitionFrame *state = *Ram_DisplayWork;
  s32 ditherIndex;
  s32 colorIndex;
  u32 i;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      state->duration = 0;
      Task_Remove(ScreenTransition_UpdateFrame);
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
    u8 pixelIndex = gScreenTransition_DitherTable[ditherIndex & 63];
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

/// Fills background screen block 4 with tile 0 in WINDOW_PALETTE and, unless
/// color is -1, makes tile 0 solid color. The fill is 0x500 bytes: the block's
/// first 20 rows of 32 entries, the screen's height.
///
/// Sets up the dissolve for ScreenTransition_Start (colour 15),
/// ScreenTransition_Finish and ScreenTransition_InitializeState (colour 0).
void ScreenTransition_FillTilemapAndSolidTile(s32 color) {
  struct ScreenTransitionState *work = gScreenTransitionWork;
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
void ScreenTransition_InitializeState(s32 value) {
  struct ScreenTransitionState *state;

  state = AllocHeapSlotEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  ScreenTransition_FillTilemapAndSolidTile(0);
  state->mode = value;
  state->value = 0;
  Task_Add(ScreenTransition_UpdateFrame, TASK_PASS_FRAME);
  WaitFrames(0x78);
}
