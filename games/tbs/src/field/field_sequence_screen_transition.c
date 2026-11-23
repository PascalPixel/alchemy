/// @file
/// How a scene fades in and out: from or to black or the backdrop colour,
/// through windows that open or close over the screen line by line, from or to
/// the centre line, or by a dissolve, which no scene uses.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// The widest a line's window reaches from the left edge in
/// WINDOW_TRANSITION_NOISE: WIN0H's right edge is a byte.
#define TRANSITION_NOISE_EDGE_MAX 255
/// How far above its point on the field an ellipse about the leader or
/// WINDOW_TRANSITION_RIPPLE's point centres, in pixels, and how far
/// WINDOW_TRANSITION_LEADER_LOW's does.
#define TRANSITION_ELLIPSE_RISE 16
#define TRANSITION_LOW_ELLIPSE_RISE 8
/// The pixels the circle's and the ellipses' radius grows a step of the value.
#define TRANSITION_RADIUS_STEP 5
/// gScreenTransition_EdgeOffsets' entries, the ripple's period in lines.
#define TRANSITION_EDGE_OFFSETS 32
/// Where the centre-line transition puts its two lines once they meet: below
/// the screen, so no switch happens.
#define CENTER_LINE_HIDDEN_FIRST 200
#define CENTER_LINE_HIDDEN_SECOND 250
/// The colour the dissolve covers the screen in, the window palette's last.
#define DISSOLVE_COVER_COLOR 15
/// Keeps a line's window edges on the screen, 0 to DISPLAY_WIDTH.
#define CLAMP_EDGES_TO_SCREEN(left, right)                                     \
  {                                                                            \
    if ((left) < 0) {                                                          \
      (left) = 0;                                                              \
    }                                                                          \
    if ((right) < 0) {                                                         \
      (right) = 0;                                                             \
    }                                                                          \
    if ((left) > DISPLAY_WIDTH) {                                              \
      (left) = DISPLAY_WIDTH;                                                  \
    }                                                                          \
    if ((right) > DISPLAY_WIDTH) {                                             \
      (right) = DISPLAY_WIDTH;                                                 \
    }                                                                          \
  }

/// Each VBlank, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank, so that every line
/// of the screen gets its own window edges.
void ScreenTransition_StartWindowHBlankDma(void) {
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

/// Each frame of a window transition: advances its value toward its end (and at
/// the end turns the windows off, or moves to the next mode), then builds the
/// next page of window edges, WININ and WINOUT first and then a (left << 8 |
/// right) pair for each of the 160 lines.
void ScreenTransition_UpdateScanlineTable(void) {
  struct ScreenTransitionState *state;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
  struct MapGridScrollWork *grid;
  u16 *scanlineData;
  struct Actor *actor;
  u32 i;
  s32 k;
  s32 lineCount;
  s32 value;
  FIXED radiusSquaredFixed;
  s32 delta;
  u32 horizontalExtent;
  s32 left;
  s32 right;
  s32 centerX;
  s32 centerY;

  s32 cameraY;
  s32 rippleCameraX;
  s32 rippleCameraY;
  u32 leftEdge;
  s32 oppositeEdges;

  grid = gMapGridWork[0];
  state = gScreenTransitionWork;

  // Finish or advance the timed phase before generating the hidden page.
  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      if (state->nextMode == 0) {
        // A transition that closed (TRANSITION_CLOSED) also turns backgrounds
        // 1 to 3 and the sprites off, so the screen stays covered; one that
        // opened just turns the windows off.
        if (state->end == TRANSITION_CLOSED) {
          REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                           DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        } else {
          REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        }
        Task_Remove((TaskFunc)ScreenTransition_StartWindowHBlankDma);
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

  // Build the page not shown.
  scanlineData = state->pages[1 ^ state->page];
  ScanlineWindow_FillSpansFar(scanlineData + 2);

  // Each mode writes WININ and WINOUT first: every layer inside the windows
  // and only BG0 outside them, or the two swapped to cover the screen inside
  // the windows instead.
  switch (state->mode) {
  case WINDOW_TRANSITION_BARS:
    *scanlineData++ = TRANSITION_SHOW_ALL;
    *scanlineData++ = WINOUT_WIN01_BG0;
    value = state->value;
    if (value & TRANSITION_OPEN) {
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    } else {
      value &= TRANSITION_OPEN - 1;
    }
    // Each line shows one bar of the table's width at a random place on the
    // 240-pixel line.
    value = gScreenTransition_NoiseWidths[value];
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      leftEdge = (Random16() * (DISPLAY_WIDTH + 1 - value)) >> 16;
      scanlineData[0] = ((leftEdge << 8) | (leftEdge + value));
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_SHAPE:
    value = state->value;
    shape = &gScreenTransition_Shapes[value & (TRANSITION_OPEN - 1)];
    if (value & TRANSITION_OPEN) {
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
          // The run's left edge moves from x0 to x1 and its right edge from
          // y0 to y1, held here in the other modes' locals.
          radiusSquaredFixed = span->x0;
          left = span->y0;
          delta = span->x1;
          right = span->y1;
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] = (((radiusSquaredFixed +
                                 (delta - radiusSquaredFixed) * k / lineCount)
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
    if (value & TRANSITION_OPEN) {
      *scanlineData++ = WININ_WIN0_BG0;
      *scanlineData++ = TRANSITION_SHOW_ALL;
    } else {
      *scanlineData++ = TRANSITION_SHOW_ALL;
      *scanlineData++ = WINOUT_WIN01_BG0;
    }
    // Each line's window runs from the left edge to 16 pixels per step of
    // progress plus up to 15 at random.
    value &= TRANSITION_OPEN - 1;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      horizontalExtent = (value << 4) + ((16 * Random16()) >> 16);
      if (horizontalExtent > TRANSITION_NOISE_EDGE_MAX) {
        horizontalExtent = TRANSITION_NOISE_EDGE_MAX;
      }
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_CIRCLE: {
#ifdef UBFIX
    // Closing writes four header halfwords, then 160 pairs: its last write
    // crosses the 322-halfword page.
    u8 *pageBytes = (u8 *)state + (1 ^ state->page) * sizeof(state->pages[0]);
    const u16 header[4] = {TRANSITION_SHOW_ALL, WINOUT_WIN01_BG0,
                           WININ_WIN0_BG0, TRANSITION_SHOW_ALL};
    u32 headerSize = 2 * sizeof(u16);
    u16 edges;
#endif
    value = state->value;
#ifdef UBFIX
    if ((u32)value > TRANSITION_OPEN) {
      headerSize = sizeof(header);
      value = TRANSITION_CLOSED - value;
    }
    memcpy(pageBytes, header, headerSize);
    pageBytes += headerSize;
#else
    *scanlineData++ = TRANSITION_SHOW_ALL;
    *scanlineData++ = WINOUT_WIN01_BG0;
    if ((u32)value > TRANSITION_OPEN) {
      *scanlineData++ = WININ_WIN0_BG0;
      *scanlineData++ = TRANSITION_SHOW_ALL;
      value = TRANSITION_CLOSED - value;
    }
#endif
    // A circle about the screen's centre whose radius grows
    // TRANSITION_RADIUS_STEP pixels a step; each line's edges are clamped to
    // the screen.
    value *= TRANSITION_RADIUS_STEP;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - DISPLAY_HEIGHT / 2;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - INTtoFIXED(value * value)) >> 8;
      left = DISPLAY_WIDTH / 2 - horizontalExtent;
      right = horizontalExtent + DISPLAY_WIDTH / 2;
      CLAMP_EDGES_TO_SCREEN(left, right);
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
    if (value & TRANSITION_OPEN) {
      *scanlineData++ = WININ_WIN0_BG0;
      *scanlineData++ = TRANSITION_SHOW_ALL;
    } else {
      *scanlineData++ = TRANSITION_SHOW_ALL;
      *scanlineData++ = WINOUT_WIN01_BG0;
    }
    // Even lines' windows grow from the left edge and odd lines' from the
    // right, across the screen over TRANSITION_OPEN steps.
    value = (u32)(DISPLAY_WIDTH * (value & (TRANSITION_OPEN - 1))) /
            TRANSITION_OPEN;
    for (i = 0; i < DISPLAY_HEIGHT; i += 2) {
      oppositeEdges = ((DISPLAY_WIDTH - value) << 8) + DISPLAY_WIDTH;
      scanlineData[0] = value;
      scanlineData[2] = oppositeEdges;
      scanlineData += 4;
    }
    break;

  case WINDOW_TRANSITION_LEADER_INVERSE: {
    s32 cameraX;

    {
      s32 *view = grid->view;

      cameraX = view[0] & FIELD_PIXEL_MASK;
      cameraY = view[1] & FIELD_PIXEL_MASK;
    }
    // Centred on the leader's place on screen, above its feet.
    actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
    centerX = (actor->x.fixed - cameraX) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE -
              TRANSITION_ELLIPSE_RISE;
    // DEAD: the frame counter is read here, where WINDOW_TRANSITION_RIPPLE
    // forms its index, and the value is not used; the ROM keeps the load.
    gFrameCount;
    // This mode writes the two masks and folds the progress the other way
    // round from WINDOW_TRANSITION_LEADER.
    *scanlineData++ = state->active;
    *scanlineData++ = state->mask;
    value = state->value;
    if ((value & TRANSITION_OPEN) == 0) {
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    } else {
      value &= TRANSITION_OPEN - 1;
    }
    // On odd frames the ellipse has no size: the flicker.
    if (gFrameCount & 1) {
      value = 0;
    }
    // Half-widths of sqrt(r * r - 1.5 * dy * dy) with r TRANSITION_RADIUS_STEP
    // pixels a step: an ellipse 1.22 times as wide as it is tall.
    value *= TRANSITION_RADIUS_STEP;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent = Iwram_Sqrt(radiusSquaredFixed -
                                    value * value * 3 * (FIXED_ONE / 2)) >>
                         8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      CLAMP_EDGES_TO_SCREEN(left, right);
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  case WINDOW_TRANSITION_RIPPLE: {
    s32 *view = grid->view;

    rippleCameraX = view[0] & FIELD_PIXEL_MASK;
    rippleCameraY = view[1] & FIELD_PIXEL_MASK;
  }
    // Centred on state->centreX and centreY; each line's edges move out by
    // gScreenTransition_EdgeOffsets, indexed two entries a line and one a
    // frame, so the outline ripples.
    centerX = (state->centreX - rippleCameraX) / FIXED_ONE;
    centerY =
        (state->centreY - rippleCameraY) / FIXED_ONE - TRANSITION_ELLIPSE_RISE;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & TRANSITION_OPEN) {
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    } else {
      value &= TRANSITION_OPEN - 1;
    }
    value *= TRANSITION_RADIUS_STEP;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent = Iwram_Sqrt(radiusSquaredFixed -
                                    value * value * 3 * (FIXED_ONE / 2)) >>
                         8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        delta =
            gScreenTransition_EdgeOffsets[k & (TRANSITION_EDGE_OFFSETS - 1)];
        left -= delta;
        right += delta;
        // An offset that closes the line leaves it with no window.
        if (left >= right) {
          left = DISPLAY_WIDTH;
          right = DISPLAY_WIDTH;
        }
      }
      CLAMP_EDGES_TO_SCREEN(left, right);
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
      k += 2;
    }
    break;

  case WINDOW_TRANSITION_LEADER: {
    s32 cameraX;

    {
      s32 *view = grid->view;

      cameraX = view[0] & FIELD_PIXEL_MASK;
      cameraY = view[1] & FIELD_PIXEL_MASK;
    }
    actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
    centerX = (actor->x.fixed - cameraX) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE -
              TRANSITION_ELLIPSE_RISE;
    // DEAD: as in WINDOW_TRANSITION_LEADER_INVERSE, the frame counter is read
    // and not used; the ROM keeps the load.
    gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & TRANSITION_OPEN) {
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    } else {
      value &= TRANSITION_OPEN - 1;
    }
    value *= TRANSITION_RADIUS_STEP;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent = Iwram_Sqrt(radiusSquaredFixed -
                                    value * value * 3 * (FIXED_ONE / 2)) >>
                         8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      CLAMP_EDGES_TO_SCREEN(left, right);
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  case WINDOW_TRANSITION_LEADER_LOW: {
    s32 cameraX;

    {
      s32 *view = grid->view;

      cameraX = view[0] & FIELD_PIXEL_MASK;
      cameraY = view[1] & FIELD_PIXEL_MASK;
    }
    actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
    centerX = (actor->x.fixed - cameraX) / FIXED_ONE;
    // Centred lower above the leader's feet than WINDOW_TRANSITION_LEADER's.
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE -
              TRANSITION_LOW_ELLIPSE_RISE;
    // DEAD: as in WINDOW_TRANSITION_LEADER_INVERSE, the frame counter is read
    // and not used; the ROM keeps the load.
    gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & TRANSITION_OPEN) {
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    } else {
      value &= TRANSITION_OPEN - 1;
    }
    // sqrt(r * r - 0.75 * dy * dy): 1.15 times as tall as it is wide.
    value *= TRANSITION_RADIUS_STEP;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent = Iwram_Sqrt(radiusSquaredFixed -
                                    value * value * 3 * (FIXED_ONE / 4)) >>
                         8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      CLAMP_EDGES_TO_SCREEN(left, right);
      scanlineData[0] = ((left << 8) + right);
      scanlineData += 2;
    }
    break;
  }
  }

  // Publish only after every span of the next page is ready.
  state->page ^= 1;
}

/// Starts window transition mode at value 0, with no duration: the caller moves
/// it.
void ScreenTransition_StartHeldWindow(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));

  Dma_Fill32(0, state, sizeof(*state));
  state->mode = mode;
  state->value = 0;
  state->mask = WININ_WIN0_ALL | WININ_WIN1_ALL;
  state->active = WINOUT_WIN01_BG0;
  Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
  Task_Add(ScreenTransition_StartWindowHBlankDma, TASK_PASS_VBLANK);
}

/// Unused: suspends the window transition's two tasks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_SuspendAll(&ScreenTransition_UpdateScanlineTable);
  Task_SuspendAll(&ScreenTransition_StartWindowHBlankDma);
}

/// Allocates the screen transition state (HEAP_SLOT_SCREEN_TRANSITION) and
/// clears it.
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  return state;
}

/// Fades the screen in over frames frames. mode is a SCENE_TRANSITION: its
/// high byte an enum ScreenTransitionStyle (from black, from the backdrop
/// colour, a window transition whose low byte is an enum WindowTransitionMode,
/// a dissolve, or opening from the centre line, which a non-zero low byte makes
/// flicker).
void ScreenTransition_Start(s32 mode, s32 frames) {
  struct ScreenTransitionWin *display;
  s32 variant;
  s32 style;

  style = SCENE_TRANSITION_STYLE(mode);
  display = gMapGridWork[0];
  variant = SCENE_TRANSITION_VARIANT(mode);
  switch (style) {
  case TRANSITION_FADE:
    ScreenFade_ToBlack(0);
    ScreenFade_FromBlack(frames);
    wait(1);
    break;
  case TRANSITION_BACKDROP_FADE:
    PaletteFade_SetSource(PALETTE_TINT_FROM_SOURCE, PLTT_BACKDROP);
    PaletteFade_StartInterpolation(frames);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_HALFWORD);
    PaletteFade_SetBackdrop(0);
    return;
  // A window or dissolve opens from value 0 (closed) to TRANSITION_OPEN, and
  // the centre line from half the screen's height to 0, over frames frames.
  case TRANSITION_WINDOW: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 0;
    state->mask = WININ_WIN0_ALL;
    state->active = WINOUT_WIN01_BG0;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
    Task_Add(ScreenTransition_StartWindowHBlankDma, TASK_PASS_VBLANK);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_HALFWORD);
    state->start = 0;
    state->end = TRANSITION_OPEN;
    state->duration = frames;
    state->step = 0;
    return;
  }
  // Unused: no scene's transition is a TRANSITION_DISSOLVE.
  case TRANSITION_DISSOLVE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = TRANSITION_OPEN;
    ScreenTransition_FillTilemapAndColorTile(DISSOLVE_COVER_COLOR);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_FRAME);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_HALFWORD);
    state->start = 0;
    state->end = TRANSITION_OPEN;
    state->duration = frames;
    state->step = 0;
    return;
  }
  case TRANSITION_CENTER_LINE: {
    struct ScreenTransitionState *state;

    display = gMapGridWork[0];
    state = ScreenTransition_AllocateAndClearState();
    display->firstLine = DISPLAY_HEIGHT / 2;
    display->secondLine = DISPLAY_HEIGHT / 2;
    wait(1);
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = DISPLAY_HEIGHT / 2;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCenter, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = DISPLAY_HEIGHT / 2;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                      IO_WRITE_HALFWORD);
}

/// Fades the screen out over frames frames, as ScreenTransition_Start fades it
/// in.
void ScreenTransition_Finish(s32 mode, s32 frames) {
  s32 variant;
  s32 style;

  style = SCENE_TRANSITION_STYLE(mode);
  variant = SCENE_TRANSITION_VARIANT(mode);
  switch (style) {
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
    state->value = TRANSITION_OPEN;
    state->mask = WININ_WIN0_ALL;
    state->active = WINOUT_WIN01_BG0;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
    Task_Add(ScreenTransition_StartWindowHBlankDma, TASK_PASS_VBLANK);
    wait(1);
    state->start = TRANSITION_OPEN;
    state->end = TRANSITION_CLOSED;
    state->duration = frames;
    state->step = 0;
    break;
  }
  // Unused, as in ScreenTransition_Start.
  case TRANSITION_DISSOLVE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = TRANSITION_OPEN;
    ScreenTransition_FillTilemapAndColorTile(0);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_FRAME);
    state->start = TRANSITION_OPEN;
    state->end = TRANSITION_CLOSED;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case TRANSITION_CENTER_LINE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 0;
      state->end = DISPLAY_HEIGHT / 2;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCenter, TASK_PASS_FRAME);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 0;
      state->end = DISPLAY_HEIGHT / 2;
      state->duration = frames;
      state->step = 0;
    }
    break;
  }
  }
}

/// Clears the map's display-control bits for backgrounds 3, 2 and 1
/// (DISPCNT_BG3_ON, DISPCNT_BG2_ON and DISPCNT_BG1_ON) for each argument that
/// is set.
void ScreenTransition_HideBackgrounds(s32 clearBg3, s32 clearBg2,
                                      s32 clearBg1) {
  struct ScreenTransitionWin *state;

  state = gMapGridWork[0];
  if (state != NULL) {
    if (clearBg1 != 0) {
      state->dispcnt &= ~DISPCNT_BG1_ON;
    }
    if (clearBg2 != 0) {
      state->dispcnt &= ~DISPCNT_BG2_ON;
    }
    if (clearBg3 != 0) {
      state->dispcnt &= ~DISPCNT_BG3_ON;
    }
  }
}

/// Each frame of the centre-line transition: moves the value toward its end and
/// sets the two lines where the screen switches between shown and blank.
void ScreenTransition_Update(void) {
  struct ScreenTransitionDissolveState *state = *Ram_DisplayWork;
  struct ScreenTransitionWin *display =
      *(struct ScreenTransitionWin **)Ram_MapGridWork;
  u32 edgeDistance;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      Task_Remove(ScreenTransition_Update);
      Irq_SetHandler(IRQ_HBLANK, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 value;

      state->step++;
      value = Iwram_SignedDivide(delta * state->step, state->duration);
      state->value = state->start + value;
    }
  }

  // Once the lines meet at the centre they are put below the screen.
  edgeDistance = state->value;
  if (edgeDistance >= DISPLAY_HEIGHT / 2) {
    display->firstLine = CENTER_LINE_HIDDEN_FIRST;
    display->secondLine = CENTER_LINE_HIDDEN_SECOND;
  } else {
    display->firstLine = edgeDistance;
    display->secondLine = DISPLAY_HEIGHT - 1 - edgeDistance;
  }
}

/// As ScreenTransition_Update, but the lines flicker every other frame.
void ScreenTransition_UpdateFromCenter(void) {
  struct ScreenTransitionDissolveState *state = *Ram_DisplayWork;
  struct ScreenTransitionWin *window =
      *(struct ScreenTransitionWin **)Ram_MapGridWork;
  u16 edgeDistance;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      Task_Remove(ScreenTransition_UpdateFromCenter);
      Irq_SetHandler(IRQ_HBLANK, 0, 0);
      return;
    } else {
      s32 delta = state->end - state->start;
      s32 edgeOffset;

      state->step++;
      edgeOffset = Iwram_SignedDivide(delta * state->step, state->duration);
      state->value = state->start + edgeOffset;
    }
  }

  edgeDistance = state->value;
  if (edgeDistance >= DISPLAY_HEIGHT / 2) {
    window->firstLine = CENTER_LINE_HIDDEN_FIRST;
    window->secondLine = CENTER_LINE_HIDDEN_SECOND;
  } else if (edgeDistance != 0 && (gFrameCount & 1)) {
    window->firstLine = edgeDistance + DISPLAY_HEIGHT / 2;
    window->secondLine = DISPLAY_HEIGHT / 2 - edgeDistance;
  } else {
    window->firstLine = 0;
    window->secondLine = DISPLAY_HEIGHT - 1;
  }
}

/// Where the centre-line transition's HBlank interrupt is in the frame.
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

/// The HBlank interrupt of the centre-line transition: switches the display to
/// DISPCNT_MODE_2 (nothing shown) at the first line and back to DISPCNT_MODE_0
/// at the second.
void ScreenTransition_UpdateScanline(void) {
  u32 scanline = REG_VCOUNT;
  struct ScreenTransitionWin *state;
  state = gMapGridWork[0];

  // Keep the same VCount sample across phase changes.
again:
  switch (state->irqPhase) {
  case DISPLAY_TRANSITION_IRQ_WAIT_FIRST:
    if (scanline >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & (u16)~DISPCNT_MODE_MASK) | DISPCNT_MODE_2;
      state->irqPhase = DISPLAY_TRANSITION_IRQ_DONE;
    }
    break;
  case DISPLAY_TRANSITION_IRQ_WAIT_SECOND:
    if (scanline >= state->irqSecondLine) {
      REG_DISPCNT &= (u16)~DISPCNT_MODE_MASK;
      state->irqPhase = DISPLAY_TRANSITION_IRQ_DONE;
    }
    break;
  case DISPLAY_TRANSITION_IRQ_WAIT_EITHER:
    if (scanline >= state->irqFirstLine) {
      REG_DISPCNT = (REG_DISPCNT & (u16)~DISPCNT_MODE_MASK) | DISPCNT_MODE_2;
      state->irqPhase++;
      goto again;
    }
    if (scanline >= state->irqSecondLine) {
      REG_DISPCNT &= (u16)~DISPCNT_MODE_MASK;
      state->irqPhase = DISPLAY_TRANSITION_IRQ_WAIT_FIRST;
      goto again;
    }
    break;
  case DISPLAY_TRANSITION_IRQ_WAIT_SCREEN:
    if (scanline <= DISPLAY_HEIGHT - 2) {
      state->irqPhase = DISPLAY_TRANSITION_IRQ_WAIT_EITHER;
      goto again;
    }
    break;
  }
}

/// Unused, as no scene dissolves. Each frame of the dissolve: moves the value
/// toward its end and sets two more of the solid tile's 64 pixels, in the
/// order of gScreenTransition_DitherTable: to colour 0 while the screen opens
/// (values up to TRANSITION_OPEN clear the cover ScreenTransition_Start filled
/// with DISSOLVE_COVER_COLOR), and to DISSOLVE_COVER_COLOR while it closes
/// (values after it fill the clear tile ScreenTransition_Finish starts from).
void ScreenTransition_UpdateDissolve(void) {
  struct ScreenTransitionDissolveState *state = *Ram_DisplayWork;
  s32 ditherIndex;
  s32 colorIndex;
  u32 i;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      volatile u16 *dma0;
      state->duration = 0;
      Task_Remove(ScreenTransition_UpdateDissolve);
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
  if (ditherIndex & TRANSITION_OPEN)
    colorIndex = DISSOLVE_COVER_COLOR;
  ditherIndex = (ditherIndex & (TRANSITION_OPEN - 1)) * 2;
  for (i = 0; i <= 1; i++) {
    u8 pixelIndex = gScreenTransition_DitherTable[ditherIndex & (8 * 8 - 1)];
    u8 *tileByte = &state->paletteNibbles[pixelIndex >> 1];
    if (pixelIndex & 1)
      *tileByte = (*tileByte & 0x0f) | (colorIndex << 4);
    else
      *tileByte = (*tileByte & 0xf0) | colorIndex;
    ditherIndex++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->paletteNibbles,
                      DMA_COPY_WORDS(TILE_SIZE_4BPP / 4));
}

/// Unused, as the dissolve is: fills background screen block 4 with tile 0 in
/// WINDOW_PALETTE and, unless color is -1, makes tile 0 solid color.
void ScreenTransition_FillTilemapAndColorTile(s32 color) {
  struct ScreenTransitionState *work = gScreenTransitionWork;
  Dma_Fill32(WINDOW_TILE_PAIR(BG_TILE_PALETTE(WINDOW_PALETTE)),
             BG_SCREEN_BLOCK(4),
             DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(u16));
  if (color != -1) {
    u32 pattern = 0;
    s32 remainingWords;
    u32 *tile;
    for (remainingWords = 0; remainingWords < 8; remainingWords++)
      pattern = (pattern << 4) | color;
    tile = work->solidTile;
    for (remainingWords = 0; remainingWords < 8; remainingWords++)
      *tile++ = pattern;
    Dma_Set(work->solidTile, BG_CHAR_BLOCK(0),
            DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
  }
}

/// Unused: nothing calls it, by name or through its far-call veneer. Starts a
/// dissolve of the given mode, its value held at 0, and waits two seconds.
void ScreenTransition_InitializeState(s32 mode) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  ScreenTransition_FillTilemapAndColorTile(0);
  state->mode = mode;
  state->value = 0;
  Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_FRAME);
  wait(120);
}

#undef TRANSITION_NOISE_EDGE_MAX
#undef TRANSITION_ELLIPSE_RISE
#undef TRANSITION_EDGE_OFFSETS
#undef TRANSITION_LOW_ELLIPSE_RISE
#undef TRANSITION_RADIUS_STEP
#undef CENTER_LINE_HIDDEN_FIRST
#undef CENTER_LINE_HIDDEN_SECOND
#undef DISSOLVE_COVER_COLOR
#undef CLAMP_EDGES_TO_SCREEN
