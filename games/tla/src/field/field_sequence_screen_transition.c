/// @file
/// The screen transitions: windows that close over the screen or open it,
/// line by line, and the dissolve.
// Part of field_sequence.c.

/// A window transition's value: from 0, the screen covered, it opens to
/// TRANSITION_OPEN and closes again to TRANSITION_CLOSED; a mode draws the
/// opening half and the closing half mirrored.
#define TRANSITION_OPEN 32
#define TRANSITION_CLOSED 64
/// The widest a line's window reaches from the left edge in
/// WINDOW_TRANSITION_NOISE: WIN0H's right edge is a byte.
#define TRANSITION_NOISE_EDGE_MAX 255
/// WINDOW_TRANSITION_RIPPLE measures a line further than this from its centre
/// as this far.
#define TRANSITION_RIPPLE_LINES_MAX 120
/// gScreenTransition_EdgeOffsets' entries, the ripple's period in lines.
#define TRANSITION_EDGE_OFFSETS 32
/// Where the centre-line transition puts its two lines once they meet: below
/// the screen, so no switch happens.
#define CENTER_LINE_HIDDEN_FIRST 200
#define CENTER_LINE_HIDDEN_SECOND 250
/// The colour the dissolve covers the screen in, the window palette's last.
#define DISSOLVE_COVER_COLOR 15
/// REG_WINOUT's high byte, what the sprite window shows, which a window
/// transition keeps as it was.
#define WINOUT_WINOBJ_MASK 0xff00

/// Keeps a line's window edges on the screen, 0 to DISPLAY_WIDTH.
#define CLAMP_SPAN_TO_SCREEN(left, right)                                      \
  {                                                                            \
    if (left < 0)                                                              \
      left = 0;                                                                \
    if (right < 0)                                                             \
      right = 0;                                                               \
    if (left > DISPLAY_WIDTH)                                                  \
      left = DISPLAY_WIDTH;                                                    \
    if (right > DISPLAY_WIDTH)                                                 \
      right = DISPLAY_WIDTH;                                                   \
  }
/// Writes every line's window edges for an ellipse round (centerX, centerY)
/// whose half-width on a line dy from its centre is sqrt(r * r - squash * dy *
/// dy), with r * r in radiusSquaredFixed (squash in 16.16).
#define WRITE_ELLIPSE_LINES(squash)                                            \
  for (i = 0; i < DISPLAY_HEIGHT; i++) {                                       \
    value = i - centerY;                                                       \
    horizontalExtent =                                                         \
        Iwram_Sqrt(radiusSquaredFixed - value * value * (squash)) >> 8;        \
    left = centerX - horizontalExtent;                                         \
    right = centerX + horizontalExtent;                                        \
    CLAMP_SPAN_TO_SCREEN(left, right);                                         \
    scanlineData[0] = (left << 8) + right;                                     \
    scanlineData += 2;                                                         \
  }

/// Each VBlank, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank, so that every line
/// of the screen gets its own window edges.
void FieldEffect_StartWindowHBlankDma(void) {
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  u16 *scanlineData = state->pages[state->page];
  volatile u16 *dma0 = REG_DMA0;

  // Stop the previous transfer before priming the first line's registers;
  // dma0[5] is the channel's control, REG_DMA0CNT_H.
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
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  struct TransitionSpan *span;
  struct TransitionShape *shape;
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
  s32 cameraX;
  s32 cameraY8;
  u32 leftEdge;
  s32 oppositeEdges;

  if (state->duration != 0) {
    if (state->step >= state->duration) {
      state->duration = 0;
      if (state->nextMode == 0) {
        if (state->end == TRANSITION_CLOSED)
          REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON |
                           DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        else
          REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
        if (REG_DISPCNT & DISPCNT_OBJWIN_ON)
          REG_WINOUT |= WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
                        WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ;
        Task_Remove(FieldEffect_StartWindowHBlankDma);
        Task_Remove(ScreenTransition_UpdateScanlineTable);
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
  ScanlineWindow_FillSpansFar(scanlineData + 2);

  switch (state->mode) {
  case WINDOW_TRANSITION_BARS:
    *scanlineData++ = TRANSITION_SHOW_ALL;
    *scanlineData++ = WINOUT_WIN01_BG0;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
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
          // A slant: the left edge goes from x0 to x1 and the right from y0
          // to y1 over the span's lines, held in radiusSquaredFixed and delta,
          // left and right.
          radiusSquaredFixed = span->x0;
          left = span->y0;
          delta = span->x1;
          right = span->y1;
          for (k = 0; k < lineCount; k++) {
            scanlineData[0] = ((radiusSquaredFixed +
                                (delta - radiusSquaredFixed) * k / lineCount)
                               << 8) +
                              (left + (right - left) * k / lineCount);
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
      if (horizontalExtent > TRANSITION_NOISE_EDGE_MAX)
        horizontalExtent = TRANSITION_NOISE_EDGE_MAX;
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_CIRCLE:
    value = state->value;
    *scanlineData++ = TRANSITION_SHOW_ALL;
    *scanlineData++ = WINOUT_WIN01_BG0;
    if ((u32)value > TRANSITION_OPEN) {
      *scanlineData++ = WININ_WIN0_BG0;
      *scanlineData++ = TRANSITION_SHOW_ALL;
      value = TRANSITION_CLOSED - value;
    }
    // A circle about the screen's centre whose radius grows 5 pixels a step,
    // to 160 at TRANSITION_OPEN; each line's edges are clamped to the screen.
    value *= 5;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - DISPLAY_HEIGHT / 2;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - INTtoFIXED(value * value)) >> 8;
      left = DISPLAY_WIDTH / 2 - horizontalExtent;
      right = horizontalExtent + DISPLAY_WIDTH / 2;
      CLAMP_SPAN_TO_SCREEN(left, right);
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;

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
    value = (u32)(DISPLAY_WIDTH * (value & (TRANSITION_OPEN - 1))) >> 5;
    for (i = 0; i < DISPLAY_HEIGHT; i += 2) {
      oppositeEdges = ((DISPLAY_WIDTH - value) << 8) + DISPLAY_WIDTH;
      scanlineData[0] = value;
      scanlineData[2] = oppositeEdges;
      scanlineData += 4;
    }
    break;

  case WINDOW_TRANSITION_LEADER_INVERSE: {
    s32 ax;

    ax = MapState_View(map)[0] & FIELD_PIXEL_MASK;
    cameraY = MapState_View(map)[1] & FIELD_PIXEL_MASK;
    // Centred on the leader's place on screen, 16 pixels above its feet.
    actor = Actor_Lookup(Party_GetLeaderActor());
    centerX = (actor->x.fixed - ax) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE - 16;
    // This mode writes the two masks and folds the progress the other way
    // round from WINDOW_TRANSITION_LEADER.
    *scanlineData++ = state->outsideLayers;
    *scanlineData++ = state->insideLayers;
    value = state->value;
    if ((value & TRANSITION_OPEN) == 0)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    // On odd frames the ellipse has no size: the flicker.
    if (gFrameCount & 1)
      value = 0;
    // Squashed by 1.5: an ellipse 1.22 times as wide as it is tall.
    value *= 5;
    radiusSquaredFixed = INTtoFIXED(value * value);
    WRITE_ELLIPSE_LINES(3 * (FIXED_ONE / 2));
    break;
  }

  case WINDOW_TRANSITION_RIPPLE: {

    cameraX = MapState_View(map)[0] & FIELD_PIXEL_MASK;
    cameraY8 = MapState_View(map)[1] & FIELD_PIXEL_MASK;
  }
    // Centred on state->centerX and centerY; each line's edges move out by
    // gScreenTransition_EdgeOffsets, indexed two entries a line and one a
    // frame, so the outline ripples.
    centerX = (state->centerX - cameraX) / FIXED_ONE;
    centerY = (state->centerY - cameraY8) / FIXED_ONE - 16;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->insideLayers;
    *scanlineData++ = state->outsideLayers;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    value *= 5;
    radiusSquaredFixed = INTtoFIXED(value * value);
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      delta = i - centerY;
      if (delta < -TRANSITION_RIPPLE_LINES_MAX)
        delta = -TRANSITION_RIPPLE_LINES_MAX;
      if (delta > TRANSITION_RIPPLE_LINES_MAX)
        delta = TRANSITION_RIPPLE_LINES_MAX;
      delta *= delta;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - delta * 3 * (FIXED_ONE / 2)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        value = k & (TRANSITION_EDGE_OFFSETS - 1);
        delta = gScreenTransition_EdgeOffsets[value];
        left -= delta;
        right += delta;
        // An offset that closes the line leaves it with no window.
        if (left >= right) {
          left = DISPLAY_WIDTH;
          right = DISPLAY_WIDTH;
        }
      }
      CLAMP_SPAN_TO_SCREEN(left, right);
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
      k += 2;
    }
    break;

  case WINDOW_TRANSITION_LEADER: {
    s32 ax;

    ax = MapState_View(map)[0] & FIELD_PIXEL_MASK;
    cameraY = MapState_View(map)[1] & FIELD_PIXEL_MASK;
    actor = Actor_Lookup(Party_GetLeaderActor());
    centerX = (actor->x.fixed - ax) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE - 16;
    *scanlineData++ = state->insideLayers;
    *scanlineData++ = state->outsideLayers;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    value *= 5;
    radiusSquaredFixed = INTtoFIXED(value * value);
    WRITE_ELLIPSE_LINES(3 * (FIXED_ONE / 2));
    break;
  }

  case WINDOW_TRANSITION_LEADER_LOW: {
    s32 ax;

    ax = MapState_View(map)[0] & FIELD_PIXEL_MASK;
    cameraY = MapState_View(map)[1] & FIELD_PIXEL_MASK;
    actor = Actor_Lookup(Party_GetLeaderActor());
    centerX = (actor->x.fixed - ax) / FIXED_ONE;
    // Centred 8 pixels above the leader's feet, not 16 as in
    // WINDOW_TRANSITION_LEADER.
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE - 8;
    *scanlineData++ = state->insideLayers;
    *scanlineData++ = state->outsideLayers;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    // Squashed by 0.75: 1.15 times as tall as it is wide.
    value *= 5;
    radiusSquaredFixed = INTtoFIXED(value * value);
    WRITE_ELLIPSE_LINES(3 * (FIXED_ONE / 4));
    break;
  }
  }

  // Shows the next page only once every line of it is ready.
  state->page ^= 1;
}

/// Starts window transition mode at value 0, with no duration: the caller moves
/// it.
void ScreenTransition_StartWindowHeld(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));

  Dma_Fill32(0, state, sizeof(*state));
  state->mode = mode;
  state->value = 0;
  state->insideLayers = WININ_WIN0_ALL | WININ_WIN1_ALL;
  state->outsideLayers = (REG_WINOUT & WINOUT_WINOBJ_MASK) | WINOUT_WIN01_BG0;
  Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
  Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_VBLANK - 10);
}

/// Unused: suspends the window transition's two tasks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_Suspend(&ScreenTransition_UpdateScanlineTable);
  Task_Suspend(&FieldEffect_StartWindowHBlankDma);
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
  struct MapViewState *display;
  s32 variant;
  s32 style;

  display = (struct MapViewState *)Ram_HeapSlots->mapGrid;
  style = SCENE_TRANSITION_STYLE(mode);
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
  case TRANSITION_WINDOW: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    state->mode = variant;
    state->value = 0;
    state->insideLayers = WININ_WIN0_ALL;
    state->outsideLayers = (REG_WINOUT & WINOUT_WINOBJ_MASK) | WINOUT_WIN01_BG0;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_VBLANK - 10);
    wait(1);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt,
                        IO_WRITE_HALFWORD);
    state->start = 0;
    state->end = TRANSITION_OPEN;
    state->duration = frames;
    state->step = 0;
    return;
  }
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
  case TRANSITION_CENTRE_LINE: {
    struct ScreenTransitionState *state;

    display = Ram_HeapSlots->mapGrid;
    state = ScreenTransition_AllocateAndClearState();
    display->windowTop = DISPLAY_HEIGHT / 2;
    display->windowBottom = DISPLAY_HEIGHT / 2;
    wait(1);
    if (variant == CENTRE_LINE_STEADY) {
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
    state->insideLayers = WININ_WIN0_ALL;
    state->outsideLayers = (REG_WINOUT & WINOUT_WINOBJ_MASK) | WINOUT_WIN01_BG0;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_FRAME);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_VBLANK - 10);
    wait(1);
    state->start = TRANSITION_OPEN;
    state->end = TRANSITION_CLOSED;
    state->duration = frames;
    state->step = 0;
    break;
  }
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
  case TRANSITION_CENTRE_LINE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();

    state->centreLine = 1;
    if (variant == CENTRE_LINE_STEADY) {
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

/// Returns 1 while ScreenTransition_UpdateScanline is the HBlank handler, that
/// is, while a centre-line transition runs.
s32 ScreenTransition_IsScanlineActive(void) {
  s32 other =
      (s32)gIrqHandlers[IRQ_HBLANK] ^ (s32)ScreenTransition_UpdateScanline;

  return 1 - NONZERO(other);
}

/// Clears the map's display-control bits for backgrounds 3, 2 and 1
/// (DISPCNT_BG3_ON, DISPCNT_BG2_ON and DISPCNT_BG1_ON) for each argument that
/// is set.
void ScreenTransition_HideBackgrounds(s32 clearBg3, s32 clearBg2,
                                      s32 clearBg1) {
  struct MapState *state;

  state = Ram_HeapSlots->mapGrid;
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
/// sets the two lines where the screen switches between shown and blank; at
/// the end the map draws its usual three extra tiles again.
void ScreenTransition_Update(void) {
  struct ScreenTransitionState *state;
  struct MapViewState *window;
  s8 *duration;
  u32 edgeDistance;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewState *)Ram_HeapSlots->mapGrid;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
      Task_Remove(ScreenTransition_Update);
      Irq_SetHandler(IRQ_HBLANK, 0, 0);
      window->extraTiles = 3;
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
    window->windowTop = CENTER_LINE_HIDDEN_FIRST;
    window->windowBottom = CENTER_LINE_HIDDEN_SECOND;
  } else {
    window->windowTop = edgeDistance;
    window->windowBottom = DISPLAY_HEIGHT - 1 - edgeDistance;
  }
}

/// As ScreenTransition_Update, but the lines flicker every other frame.
void ScreenTransition_UpdateFromCenter(void) {
  struct ScreenTransitionState *state;
  struct MapViewState *window;
  s8 *duration;
  u16 edgeDistance;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewState *)Ram_HeapSlots->mapGrid;
  if (*duration != 0) {
    s8 *step = &state->step;

    if (*step >= *duration) {
      *duration = 0;
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
    window->windowTop = CENTER_LINE_HIDDEN_FIRST;
    window->windowBottom = CENTER_LINE_HIDDEN_SECOND;
  } else if (edgeDistance != 0 && (gFrameCount & 1)) {
    window->windowTop = edgeDistance + DISPLAY_HEIGHT / 2;
    window->windowBottom = DISPLAY_HEIGHT / 2 - edgeDistance;
  } else {
    window->windowTop = 0;
    window->windowBottom = DISPLAY_HEIGHT - 1;
  }
}

/// Where the centre-line transition's HBlank interrupt is.
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
  struct MapViewState *state;
  state = Ram_HeapSlots->mapGrid;

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

/// Each frame of the dissolve: moves the value toward its end and sets two more
/// of the solid tile's 64 pixels, in the order of
/// gScreenTransition_DitherTable: to colour 0 while the screen opens (values up
/// to TRANSITION_OPEN clear the cover ScreenTransition_Start filled with
/// DISSOLVE_COVER_COLOR), and to DISSOLVE_COVER_COLOR while it closes (values
/// after it fill the clear tile ScreenTransition_Finish starts from).
void ScreenTransition_UpdateDissolve(void) {
  struct ScreenTransitionDissolveState *state =
      (struct ScreenTransitionDissolveState *)Ram_HeapSlots->screenTransition;
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
  state->page ^= 1;
  colorIndex = 0;
  if (ditherIndex & TRANSITION_OPEN)
    colorIndex = DISSOLVE_COVER_COLOR;
  ditherIndex = (ditherIndex & (TRANSITION_OPEN - 1)) * 2;
  for (i = 0; i <= 1; i++) {
    u8 pixelIndex = gScreenTransition_DitherTable[ditherIndex & (8 * 8 - 1)];
    u8 *tileByte = &state->solidTilePixels[pixelIndex >> 1];

    if (pixelIndex & 1)
      *tileByte = (*tileByte & 0x0f) | (colorIndex << 4);
    else
      *tileByte = (*tileByte & 0xf0) | colorIndex;
    ditherIndex++;
  }

  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0), (u32)state->solidTilePixels,
                      DMA_COPY_WORDS(TILE_SIZE_4BPP / 4));
}

/// Fills background screen block 4 with tile 0 in WINDOW_PALETTE and, unless
/// color is -1, makes tile 0 solid color.
void ScreenTransition_FillTilemapAndColorTile(s32 color) {
  struct ScreenTransitionState *state = Ram_HeapSlots->screenTransition;
  Dma_Fill32(WINDOW_TILE_PAIR(BG_TILE_PALETTE(WINDOW_PALETTE)),
             BG_SCREEN_BLOCK(4),
             DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(u16));
  if (color != -1) {
    // A row of eight pixels is a word of eight nibbles; a tile is eight rows.
    u32 pattern = 0;
    s32 i;
    u32 *tile;
    for (i = 0; i < 8; i++)
      pattern = (pattern << 4) | color;
    tile = state->solidTile;
    for (i = 0; i < 8; i++)
      *tile++ = pattern;
    Dma_Set(state->solidTile, BG_CHAR_BLOCK(0),
            DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
  }
}

/// Starts a dissolve of the given mode, its value held at 0, and waits two
/// seconds.
void ScreenTransition_StartDissolveHeld(s32 mode) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  ScreenTransition_FillTilemapAndColorTile(0);
  state->mode = mode;
  state->value = 0;
  Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_FRAME);
  wait(120);
}

#undef TRANSITION_OPEN
#undef TRANSITION_CLOSED
#undef TRANSITION_NOISE_EDGE_MAX
#undef TRANSITION_RIPPLE_LINES_MAX
#undef TRANSITION_EDGE_OFFSETS
#undef CENTER_LINE_HIDDEN_FIRST
#undef CENTER_LINE_HIDDEN_SECOND
#undef DISSOLVE_COVER_COLOR
#undef WINOUT_WINOBJ_MASK
