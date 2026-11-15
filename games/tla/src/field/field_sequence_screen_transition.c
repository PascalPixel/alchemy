/// @file
/// The screen transitions: windows that close over the screen or open it,
/// line by line, and the dissolve.
// Part of field_sequence.c.

/// A window transition's value: from 0, the screen covered, it opens to
/// TRANSITION_OPEN and closes again to TRANSITION_CLOSED; a mode draws the
/// opening half and the closing half mirrored.
#define TRANSITION_OPEN 32
#define TRANSITION_CLOSED 64

/// Each VBlank, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank, so that every line
/// of the screen gets its own window edges.
void FieldEffect_StartWindowHBlankDma(void) {
  struct ScreenTransitionState *work = Ram_HeapSlots->screenTransition;
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
  struct MapWork *map = Ram_HeapSlots->mapGrid;
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
    *scanlineData++ = 1;
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
      *scanlineData++ = 1;
      *scanlineData++ = TRANSITION_SHOW_ALL;
    } else {
      *scanlineData++ = TRANSITION_SHOW_ALL;
      *scanlineData++ = 1;
    }
    value &= TRANSITION_OPEN - 1;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      horizontalExtent = (value << 4) + ((16 * Random16()) >> 16);
      if (horizontalExtent > 255)
        horizontalExtent = 255;
      scanlineData[0] = horizontalExtent;
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_CIRCLE:
    value = state->value;
    *scanlineData++ = TRANSITION_SHOW_ALL;
    *scanlineData++ = 1;
    if ((u32)value > TRANSITION_OPEN) {
      *scanlineData++ = 1;
      *scanlineData++ = TRANSITION_SHOW_ALL;
      value = TRANSITION_CLOSED - value;
    }
    value *= 5;
    radiusSquaredFixed = (value * value) << 16;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - DISPLAY_HEIGHT / 2;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - ((value * value) << 16)) >> 8;
      left = DISPLAY_WIDTH / 2 - horizontalExtent;
      right = horizontalExtent + DISPLAY_WIDTH / 2;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > DISPLAY_WIDTH)
        left = DISPLAY_WIDTH;
      if (right > DISPLAY_WIDTH)
        right = DISPLAY_WIDTH;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;

  case WINDOW_TRANSITION_INTERLACED_WIPE:
    value = state->value;
    if (value & TRANSITION_OPEN) {
      *scanlineData++ = 1;
      *scanlineData++ = TRANSITION_SHOW_ALL;
    } else {
      *scanlineData++ = TRANSITION_SHOW_ALL;
      *scanlineData++ = 1;
    }
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

    s32 *view = &map->viewX;

    ax = view[0] & FIELD_PIXEL_MASK;
    cameraY = view[1] & FIELD_PIXEL_MASK;
    actor = Actor_Lookup(Party_GetLeaderActor());
    centerX = (actor->x.fixed - ax) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE - 16;
    *scanlineData++ = state->active;
    *scanlineData++ = state->mask;
    value = state->value;
    if ((value & TRANSITION_OPEN) == 0)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    if (gFrameCount & 1)
      value = 0;
    value *= 5;
    radiusSquaredFixed = (value * value) << 16;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > DISPLAY_WIDTH)
        left = DISPLAY_WIDTH;
      if (right > DISPLAY_WIDTH)
        right = DISPLAY_WIDTH;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }

  case WINDOW_TRANSITION_RIPPLE: {
    s32 *view = &map->viewX;

    cameraX = view[0] & FIELD_PIXEL_MASK;
    cameraY8 = view[1] & FIELD_PIXEL_MASK;
  }
    centerX = (state->centreX - cameraX) / FIXED_ONE;
    centerY = (state->centreY - cameraY8) / FIXED_ONE - 16;
    k = -centerY * 2 + gFrameCount;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    value *= 5;
    radiusSquaredFixed = (value * value) << 16;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      delta = i - centerY;
      if (delta < -120)
        delta = -120;
      if (delta > 120)
        delta = 120;
      delta *= delta;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - ((delta * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < right) {
        value = k & 31;
        delta = gScreenTransition_EdgeOffsets[value];
        left -= delta;
        right += delta;
        if (left >= right) {
          left = DISPLAY_WIDTH;
          right = DISPLAY_WIDTH;
        }
      }
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > DISPLAY_WIDTH)
        left = DISPLAY_WIDTH;
      if (right > DISPLAY_WIDTH)
        right = DISPLAY_WIDTH;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
      k += 2;
    }
    break;

  case WINDOW_TRANSITION_LEADER: {
    s32 ax;

    s32 *view = &map->viewX;

    ax = view[0] & FIELD_PIXEL_MASK;
    cameraY = view[1] & FIELD_PIXEL_MASK;
    actor = Actor_Lookup(Party_GetLeaderActor());
    centerX = (actor->x.fixed - ax) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE - 16;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    value *= 5;
    radiusSquaredFixed = (value * value) << 16;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - ((value * value * 3) << 15)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > DISPLAY_WIDTH)
        left = DISPLAY_WIDTH;
      if (right > DISPLAY_WIDTH)
        right = DISPLAY_WIDTH;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }

  case WINDOW_TRANSITION_LEADER_LOW: {
    s32 ax;

    s32 *view = &map->viewX;

    ax = view[0] & FIELD_PIXEL_MASK;
    cameraY = view[1] & FIELD_PIXEL_MASK;
    actor = Actor_Lookup(Party_GetLeaderActor());
    centerX = (actor->x.fixed - ax) / FIXED_ONE;
    centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / FIXED_ONE - 8;
    *scanlineData++ = state->mask;
    *scanlineData++ = state->active;
    value = state->value;
    if (value & TRANSITION_OPEN)
      value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
    else
      value &= TRANSITION_OPEN - 1;
    value *= 5;
    radiusSquaredFixed = (value * value) << 16;
    for (i = 0; i < DISPLAY_HEIGHT; i++) {
      value = i - centerY;
      horizontalExtent =
          Iwram_Sqrt(radiusSquaredFixed - ((value * value * 3) << 14)) >> 8;
      left = centerX - horizontalExtent;
      right = centerX + horizontalExtent;
      if (left < 0)
        left = 0;
      if (right < 0)
        right = 0;
      if (left > DISPLAY_WIDTH)
        left = DISPLAY_WIDTH;
      if (right > DISPLAY_WIDTH)
        right = DISPLAY_WIDTH;
      scanlineData[0] = (left << 8) + right;
      scanlineData += 2;
    }
    break;
  }
  }

  state->page ^= 1;
}

/// Starts window transition mode at value 0, with no duration: the caller moves
/// it.
void ScreenTransition_InitializeBattleEffectState(s32 mode) {
  struct ScreenTransitionState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));

  Dma_Fill32(0, state, sizeof(*state));
  state->mode = mode;
  state->value = 0;
  state->mask = WININ_WIN0_ALL | WININ_WIN1_ALL;
  state->active = (REG_WINOUT & 0xff00) | WINOUT_WIN01_BG0;
  Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_VBLANK);
  Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
}

/// Unused: suspends the window transition's two tasks. Nothing calls it,
/// by name, far-call entry or table.
UNUSED void ScreenTransition_SuspendCallbacks(void) {
  Task_SuspendAll(&ScreenTransition_UpdateScanlineTable);
  Task_SuspendAll(&FieldEffect_StartWindowHBlankDma);
}

/// Allocates the screen transition state (HEAP_SLOT_SCREEN_TRANSITION) and
/// clears it.
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  return state;
}

/// Fades the screen in over frames frames. mode's high byte is its enum
/// ScreenTransitionStyle: from black, from the backdrop colour, a window
/// transition (its low byte an enum WindowTransitionMode), a dissolve or
/// opening from the centre line (which a non-zero low byte makes flicker).
void ScreenTransition_Start(s32 mode, s32 frames) {
  struct MapViewWork *display;
  s32 variant;
  s32 transitionKind;

  display = (struct MapViewWork *)Ram_HeapSlots->mapGrid;
  transitionKind = (mode >> 8) & 0xff;
  variant = mode & 0xff;
  switch (transitionKind) {
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
    state->mask = WININ_WIN0_ALL;
    state->active = (REG_WINOUT & 0xff00) | WINOUT_WIN01_BG0;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_VBLANK);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
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
    ScreenTransition_FillTilemapAndSolidTile(15);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
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
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = DISPLAY_HEIGHT / 2;
      state->end = 0;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
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
    state->value = TRANSITION_OPEN;
    state->mask = WININ_WIN0_ALL;
    state->active = (REG_WINOUT & 0xff00) | WINOUT_WIN01_BG0;
    Task_Add(ScreenTransition_UpdateScanlineTable, TASK_PASS_VBLANK);
    Task_Add(FieldEffect_StartWindowHBlankDma, TASK_PASS_FRAME - 10);
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
    ScreenTransition_FillTilemapAndSolidTile(0);
    wait(1);
    Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
    state->start = TRANSITION_OPEN;
    state->end = TRANSITION_CLOSED;
    state->duration = frames;
    state->step = 0;
    break;
  }
  case TRANSITION_CENTRE_LINE: {
    struct ScreenTransitionState *state =
        ScreenTransition_AllocateAndClearState();
    // One zero for both branches' stores; a literal 0 at each store differs.
    s32 zero = 0;

    ((u8 *)state)[0x53f] = 1;
    if (variant == 0) {
      Task_Add(ScreenTransition_Update, TASK_PASS_VBLANK);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 0;
      state->end = DISPLAY_HEIGHT / 2;
      state->duration = frames;
      state->step = 0;
    } else {
      Task_Add(ScreenTransition_UpdateFromCentre, TASK_PASS_VBLANK);
      Irq_SetHandler(IRQ_HBLANK, 0, ScreenTransition_UpdateScanline);
      state->start = 0;
      state->end = DISPLAY_HEIGHT / 2;
      state->duration = frames;
      state->step = zero;
    }
    break;
  }
  }
}

/// Returns 1 while ScreenTransition_UpdateScanline is the VCount handler.
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
  struct MapWork *state;

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
  struct MapViewWork *window;
  s8 *duration;
  u32 edgeDistance;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapGrid;
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

  // At 80 or more the lines are put below the screen's 160, so no switch
  // happens.
  edgeDistance = state->value;
  if (edgeDistance >= DISPLAY_HEIGHT / 2) {
    window->windowTop = 200;
    window->windowBottom = 250;
  } else {
    window->windowTop = edgeDistance;
    window->windowBottom = DISPLAY_HEIGHT - 1 - edgeDistance;
  }
}

/// As ScreenTransition_Update, but the lines flicker every other frame.
void ScreenTransition_UpdateFromCentre(void) {
  struct ScreenTransitionState *state;
  struct MapViewWork *window;
  s8 *duration;
  u16 edgeDistance;

  state = Ram_HeapSlots->screenTransition;
  duration = &state->duration;
  window = (struct MapViewWork *)Ram_HeapSlots->mapGrid;
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

      state->step++;
      edgeOffset = Iwram_SignedDivide(delta * state->step, state->duration);
      state->value = state->start + edgeOffset;
    }
  }

  edgeDistance = state->value;
  if (edgeDistance >= DISPLAY_HEIGHT / 2) {
    window->windowTop = 200;
    window->windowBottom = 250;
  } else if (edgeDistance != 0 && (gFrameCount & 1)) {
    window->windowTop = edgeDistance + DISPLAY_HEIGHT / 2;
    window->windowBottom = DISPLAY_HEIGHT / 2 - edgeDistance;
  } else {
    window->windowTop = 0;
    window->windowBottom = DISPLAY_HEIGHT - 1;
  }
}

/// The HBlank interrupt of the centre-line transition: switches the display to
/// DISPCNT_MODE_2 (nothing shown) at the first line and back to DISPCNT_MODE_0
/// at the second.
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
  u32 scanline = REG_VCOUNT;
  struct MapViewWork *state;
  state = Ram_HeapSlots->mapGrid;

  // Keep the same VCount sample across phase changes.
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

/// Each frame of the dissolve: moves the value toward its end and sets two more
/// of the solid tile's 64 pixels, in the order of
/// gScreenTransition_DitherTable: to colour 0 while the screen opens (values 1
/// to 32 clear the cover ScreenTransition_Start filled with colour 15), and to
/// colour 15 while it closes (values 33 to 64 fill the clear tile
/// ScreenTransition_Finish starts from).
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
  state->ditherToggle ^= 1;
  colorIndex = 0;
  if (ditherIndex & TRANSITION_OPEN)
    colorIndex = 15;
  ditherIndex = (ditherIndex & (TRANSITION_OPEN - 1)) * 2;
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
                      DMA_COPY_WORDS(TILE_SIZE_4BPP / 4));
}

/// Fills background screen block 4 with tile 0 in WINDOW_PALETTE and, unless
/// color is -1, makes tile 0 solid color.
void ScreenTransition_FillTilemapAndSolidTile(s32 color) {
  struct ScreenTransitionState *work = Ram_HeapSlots->screenTransition;
  Dma_Fill32(0xf000f000, BG_SCREEN_BLOCK(4), 1280);
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
            DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
  }
}

/// Starts a dissolve of the given mode and waits 120 frames.
void ScreenTransition_InitializeState(s32 value) {
  struct ScreenTransitionState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
  Dma_Fill32(0, state, sizeof(*state));
  ScreenTransition_FillTilemapAndSolidTile(0);
  state->mode = value;
  state->value = 0;
  Task_Add(ScreenTransition_UpdateDissolve, TASK_PASS_VBLANK);
  wait(120);
}

#undef TRANSITION_OPEN
#undef TRANSITION_CLOSED
#undef TRANSITION_SHOW_ALL
