// How a scene fades in and out: from or to black or the backdrop colour, through windows that open
// or close over the screen line by line, from or to the centre line, or by a dissolve, which no
// scene uses. Part of field_mode.c.

#include "field_mode_internal.h"

// The widest a line's window reaches from the left edge in WINDOW_TRANSITION_NOISE: WIN0H's right
// edge is a byte.
#define TRANSITION_NOISE_EDGE_MAX   255
// How far above its point on the field an ellipse about the leader or WINDOW_TRANSITION_RIPPLE's
// point centres, in pixels, and how far WINDOW_TRANSITION_LEADER_LOW's does.
#define TRANSITION_ELLIPSE_RISE     16
#define TRANSITION_LOW_ELLIPSE_RISE 8
// The pixels the circle's and the ellipses' radius grows a step of the value.
#define TRANSITION_RADIUS_STEP      5
// ScreenTransitionEdgeOffsets' entries, the ripple's period in lines.
#define TRANSITION_EDGE_OFFSETS     32
// Where the centre-line transition puts its two lines once they meet: below the screen, so no
// switch happens.
#define CENTER_LINE_HIDDEN_FIRST    200
#define CENTER_LINE_HIDDEN_SECOND   250
// The colour the dissolve covers the screen in, the window palette's last.
#define DISSOLVE_COVER_COLOR        15
// Keeps a line's window edges on the screen, 0 to DISPLAY_WIDTH.
#define CLAMP_EDGES_TO_SCREEN(left, right)                                                         \
    {                                                                                              \
        if ((left) < 0)                                                                            \
        {                                                                                          \
            (left) = 0;                                                                            \
        }                                                                                          \
        if ((right) < 0)                                                                           \
        {                                                                                          \
            (right) = 0;                                                                           \
        }                                                                                          \
        if ((left) > DISPLAY_WIDTH)                                                                \
        {                                                                                          \
            (left) = DISPLAY_WIDTH;                                                                \
        }                                                                                          \
        if ((right) > DISPLAY_WIDTH)                                                               \
        {                                                                                          \
            (right) = DISPLAY_WIDTH;                                                               \
        }                                                                                          \
    }

// Each VBlank, before drawing: turns windows 0 and 1 on, writes the first four values of the shown
// page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0 copy the rest into WIN0H and WIN1H one
// word each HBlank, so that every line of the screen gets its own window edges.
void fdScreenTransitionStartWindowHBlankDMA(void)
{
    SCREEN_TRANSITION_STATE *state         = ScreenTransitionStateWork;
    Uint16                  *scanlineData  = state->pages[state->page];
    volatile Uint16         *dma0          = REG_DMA0;

    // Stop the previous transfer before priming the first line's registers.
    dma0[DMA_CNT_H_INDEX]                 &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma0[DMA_CNT_H_INDEX]                 &= ~DMA_ENABLE;
    (void)dma0[DMA_CNT_H_INDEX];
    REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
    IO_WRITE16(*scanlineData++, &REG_WININ);
    IO_WRITE16(*scanlineData++, &REG_WINOUT);
    IO_WRITE16(*scanlineData++, &REG_WIN0H);
    IO_WRITE16(*scanlineData++, &REG_WIN1H);
    IO_WRITE16(WIN_RANGE(0, DISPLAY_HEIGHT), &REG_WIN0V);
    IO_WRITE16(WIN_RANGE(0, DISPLAY_HEIGHT), &REG_WIN1V);
    // HBlank repeats one word: both horizontal window bounds for the next line.
    syDMAXCopy(scanlineData, (void *)&REG_WIN0H, DMA_HBLANK_WORD_PER_LINE, (volatile Uint32 *)dma0);
}

// Each tick of a window transition: advances its value toward its end (and at the end turns the
// windows off, or moves to the next mode), then builds the next page of window edges, WININ and
// WINOUT first and then a (left << 8 | right) pair for each of the 160 lines.
void fdScreenTransitionUpdateScanlineTable(void)
{
    SCREEN_TRANSITION_STATE *state;
    TRANSITION_SPAN         *span;
    TRANSITION_SHAPE        *shape;
    MAP_GRID_SCROLL_STATE   *grid;
    Uint16                  *scanlineData;
    ACTOR                   *actor;
    Uint32                   i;
    Sint32                   k;
    Sint32                   lineCount;
    Sint32                   value;
    FIXED                    radiusSquaredFixed;
    Sint32                   delta;
    Uint32                   horizontalExtent;
    Sint32                   left;
    Sint32                   right;
    Sint32                   centerX;
    Sint32                   centerY;

    Sint32                   cameraY;
    Sint32                   rippleCameraX;
    Sint32                   rippleCameraY;
    Uint32                   leftEdge;
    Sint32                   oppositeEdges;

    grid  = MapGridState[0];
    state = ScreenTransitionStateWork;

    // Finish or advance the timed phase before generating the hidden page.
    if (state->duration != 0)
    {
        if (state->step >= state->duration)
        {
            state->duration = 0;
            if (state->nextMode == 0)
            {
                // A transition that closed (TRANSITION_CLOSED) also turns backgrounds 1 to 3 and
                // the sprites off, so the screen stays covered; one that opened just turns the
                // windows off.
                if (state->end == TRANSITION_CLOSED)
                {
                    REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON
                                     | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
                }
                else
                {
                    REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
                }
                syCloseEvent((EVENT_FUNC)fdScreenTransitionStartWindowHBlankDMA);
                syCloseEvent((EVENT_FUNC)fdScreenTransitionUpdateScanlineTable);
                {
                    volatile Uint16 *dma0  = REG_DMA0;

                    dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
                    dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
                    dma0[DMA_CNT_H_INDEX];
                }
                return;
            }
            state->mode = state->nextMode;
        }
        else
        {
            state->value =
                state->start
                + syIntegerDivide((state->end - state->start) * ++state->step, state->duration);
        }
    }

    // Build the page not shown.
    scanlineData = state->pages[1 ^ state->page];
    mnScanlineWindowFillSpansStub(scanlineData + 2);

    // Each mode writes WININ and WINOUT first: every layer inside the windows and only BG0 outside
    // them, or the two swapped to cover the screen inside the windows instead.
    switch (state->mode)
    {
        case WINDOW_TRANSITION_BARS:
            *scanlineData++ = TRANSITION_SHOW_ALL;
            *scanlineData++ = WINOUT_WIN01_BG0;
            value           = state->value;
            if (value & TRANSITION_OPEN)
            {
                value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
            }
            else
            {
                value &= TRANSITION_OPEN - 1;
            }
            // Each line shows one bar of the table's width at a random place on the 240-pixel line.
            value = ScreenTransitionNoiseWidths[value];
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                leftEdge         = (syRandom() * (DISPLAY_WIDTH + 1 - value)) >> 16;
                scanlineData[0]  = ((leftEdge << 8) | (leftEdge + value));
                scanlineData    += 2;
            }
            break;

        case WINDOW_TRANSITION_SHAPE:
            value = state->value;
            shape = &ScreenTransitionShapes[value & (TRANSITION_OPEN - 1)];
            if (value & TRANSITION_OPEN)
            {
                *scanlineData++ = shape->first;
                *scanlineData++ = shape->second;
            }
            else
            {
                *scanlineData++ = shape->second;
                *scanlineData++ = shape->first;
            }
            // Three runs of lines, each one fixed edge pair or a slope between two.
            for (i = 0; i < 3; i++)
            {
                span      = &shape->spans[i];
                lineCount = span->count;
                right     = span->value;
                if (lineCount != 0)
                {
                    if (right != 0)
                    {
                        for (k = 0; k < lineCount; k++)
                        {
                            scanlineData[0]  = right;
                            scanlineData    += 2;
                        }
                    }
                    else
                    {
                        // The run's left edge moves from x0 to x1 and its right edge from y0 to y1,
                        // held here in the other modes' locals.
                        radiusSquaredFixed = span->x0;
                        left               = span->y0;
                        delta              = span->x1;
                        right              = span->y1;
                        for (k = 0; k < lineCount; k++)
                        {
                            scanlineData[0]  = (((radiusSquaredFixed
                                                 + (delta - radiusSquaredFixed) * k / lineCount)
                                                << 8)
                                               + (left + (right - left) * k / lineCount));
                            scanlineData    += 2;
                        }
                    }
                }
            }
            break;

        case WINDOW_TRANSITION_NOISE:
            value = state->value - 1;
            if (value & TRANSITION_OPEN)
            {
                *scanlineData++ = WININ_WIN0_BG0;
                *scanlineData++ = TRANSITION_SHOW_ALL;
            }
            else
            {
                *scanlineData++ = TRANSITION_SHOW_ALL;
                *scanlineData++ = WINOUT_WIN01_BG0;
            }
            // Each line's window runs from the left edge to 16 pixels per step of progress plus up
            // to 15 at random.
            value &= TRANSITION_OPEN - 1;
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                horizontalExtent = (value << 4) + ((16 * syRandom()) >> 16);
                if (horizontalExtent > TRANSITION_NOISE_EDGE_MAX)
                {
                    horizontalExtent = TRANSITION_NOISE_EDGE_MAX;
                }
                scanlineData[0]  = horizontalExtent;
                scanlineData    += 2;
            }
            break;

        case WINDOW_TRANSITION_CIRCLE:
        {
#if FIX_POINTER_OUTSIDE_OBJECT
            // Closing writes four header halfwords, then 160 pairs: its last write crosses the
            // 322-halfword page.
            Uint8       *pageBytes = (Uint8 *)state + (1 ^ state->page) * sizeof(state->pages[0]);
            const Uint16 header[4] = {
                TRANSITION_SHOW_ALL, WINOUT_WIN01_BG0, WININ_WIN0_BG0, TRANSITION_SHOW_ALL};
            Uint32 headerSize = 2 * sizeof(Uint16);
            Uint16 edges;
#endif
            value = state->value;
#if FIX_POINTER_OUTSIDE_OBJECT
            if ((Uint32)value > TRANSITION_OPEN)
            {
                headerSize = sizeof(header);
                value      = TRANSITION_CLOSED - value;
            }
            memcpy(pageBytes, header, headerSize);
            pageBytes += headerSize;
#else
            *scanlineData++ = TRANSITION_SHOW_ALL;
            *scanlineData++ = WINOUT_WIN01_BG0;
            if ((Uint32)value > TRANSITION_OPEN)
            {
                *scanlineData++ = WININ_WIN0_BG0;
                *scanlineData++ = TRANSITION_SHOW_ALL;
                value           = TRANSITION_CLOSED - value;
            }
#endif
            // A circle about the screen's centre whose radius grows TRANSITION_RADIUS_STEP pixels a
            // step; each line's edges are clamped to the screen.
            value              *= TRANSITION_RADIUS_STEP;
            radiusSquaredFixed  = toFIXED(value * value);
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                value            = i - DISPLAY_HEIGHT / 2;
                horizontalExtent = sySquartIWRAM(radiusSquaredFixed - toFIXED(value * value)) >> 8;
                left             = DISPLAY_WIDTH / 2 - horizontalExtent;
                right            = horizontalExtent + DISPLAY_WIDTH / 2;
                CLAMP_EDGES_TO_SCREEN(left, right);
#if FIX_POINTER_OUTSIDE_OBJECT
                edges = (left << 8) + right;
                memcpy(pageBytes, &edges, sizeof(edges));
                pageBytes += 2 * sizeof(Uint16);
#else
                scanlineData[0]  = ((left << 8) + right);
                scanlineData    += 2;
#endif
            }
            break;
        }

        case WINDOW_TRANSITION_INTERLACED_WIPE:
            value = state->value;
            if (value & TRANSITION_OPEN)
            {
                *scanlineData++ = WININ_WIN0_BG0;
                *scanlineData++ = TRANSITION_SHOW_ALL;
            }
            else
            {
                *scanlineData++ = TRANSITION_SHOW_ALL;
                *scanlineData++ = WINOUT_WIN01_BG0;
            }
            // Even lines' windows grow from the left edge and odd lines' from the right, across the
            // screen over TRANSITION_OPEN steps.
            value = (Uint32)(DISPLAY_WIDTH * (value & (TRANSITION_OPEN - 1))) / TRANSITION_OPEN;
            for (i = 0; i < DISPLAY_HEIGHT; i += 2)
            {
                oppositeEdges    = ((DISPLAY_WIDTH - value) << 8) + DISPLAY_WIDTH;
                scanlineData[0]  = value;
                scanlineData[2]  = oppositeEdges;
                scanlineData    += 4;
            }
            break;

        case WINDOW_TRANSITION_LEADER_INVERSE:
        {
            Sint32 cameraX;

            cameraX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
            cameraY = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
            // Centred on the leader's place on screen, above its feet.
            actor   = fdActorLookup(MainRoutineStateWork.leaderActorId);
            centerX = (actor->x.fixed - cameraX) / toFIXED(1);
            centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / toFIXED(1)
                    - TRANSITION_ELLIPSE_RISE;
            // DEAD: the tick counter is read here, where WINDOW_TRANSITION_RIPPLE forms its index,
            // and the value is not used; the ROM keeps the load.
            TickCount;
            // This mode writes the two masks and folds the progress the other way round from
            // WINDOW_TRANSITION_LEADER.
            *scanlineData++ = state->outsideLayers;
            *scanlineData++ = state->insideLayers;
            value           = state->value;
            if ((value & TRANSITION_OPEN) == 0)
            {
                value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
            }
            else
            {
                value &= TRANSITION_OPEN - 1;
            }
            // On odd ticks the ellipse has no size: the flicker.
            if (TickCount & 1)
            {
                value = 0;
            }
            // Half-widths of sqrt(r * r - 1.5 * dy * dy) with r TRANSITION_RADIUS_STEP pixels a
            // step: an ellipse 1.22 times as wide as it is tall.
            value              *= TRANSITION_RADIUS_STEP;
            radiusSquaredFixed  = toFIXED(value * value);
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                value = i - centerY;
                horizontalExtent =
                    sySquartIWRAM(radiusSquaredFixed - value * value * 3 * (toFIXED(1) / 2)) >> 8;
                left  = centerX - horizontalExtent;
                right = centerX + horizontalExtent;
                CLAMP_EDGES_TO_SCREEN(left, right);
                scanlineData[0]  = ((left << 8) + right);
                scanlineData    += 2;
            }
            break;
        }
        case WINDOW_TRANSITION_RIPPLE:
            rippleCameraX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
            rippleCameraY = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
            // Centred on state->centerX and centerY; each line's edges move out by
            // ScreenTransitionEdgeOffsets, indexed two entries a line and one a tick, so the
            // outline ripples.
            centerX       = (state->centerX - rippleCameraX) / toFIXED(1);
            centerY       = (state->centerY - rippleCameraY) / toFIXED(1) - TRANSITION_ELLIPSE_RISE;
            k             = -centerY * 2 + TickCount;
            *scanlineData++ = state->insideLayers;
            *scanlineData++ = state->outsideLayers;
            value           = state->value;
            if (value & TRANSITION_OPEN)
            {
                value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
            }
            else
            {
                value &= TRANSITION_OPEN - 1;
            }
            value              *= TRANSITION_RADIUS_STEP;
            radiusSquaredFixed  = toFIXED(value * value);
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                value = i - centerY;
                horizontalExtent =
                    sySquartIWRAM(radiusSquaredFixed - value * value * 3 * (toFIXED(1) / 2)) >> 8;
                left  = centerX - horizontalExtent;
                right = centerX + horizontalExtent;
                if (left < right)
                {
                    delta  = ScreenTransitionEdgeOffsets[k & (TRANSITION_EDGE_OFFSETS - 1)];
                    left  -= delta;
                    right += delta;
                    // An offset that closes the line leaves it with no window.
                    if (left >= right)
                    {
                        left  = DISPLAY_WIDTH;
                        right = DISPLAY_WIDTH;
                    }
                }
                CLAMP_EDGES_TO_SCREEN(left, right);
                scanlineData[0]  = ((left << 8) + right);
                scanlineData    += 2;
                k               += 2;
            }
            break;

        case WINDOW_TRANSITION_LEADER:
        {
            Sint32 cameraX;

            cameraX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
            cameraY = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
            actor   = fdActorLookup(MainRoutineStateWork.leaderActorId);
            centerX = (actor->x.fixed - cameraX) / toFIXED(1);
            centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / toFIXED(1)
                    - TRANSITION_ELLIPSE_RISE;
            // DEAD: as in WINDOW_TRANSITION_LEADER_INVERSE, the tick counter is read and not used;
            // the ROM keeps the load.
            TickCount;
            *scanlineData++ = state->insideLayers;
            *scanlineData++ = state->outsideLayers;
            value           = state->value;
            if (value & TRANSITION_OPEN)
            {
                value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
            }
            else
            {
                value &= TRANSITION_OPEN - 1;
            }
            value              *= TRANSITION_RADIUS_STEP;
            radiusSquaredFixed  = toFIXED(value * value);
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                value = i - centerY;
                horizontalExtent =
                    sySquartIWRAM(radiusSquaredFixed - value * value * 3 * (toFIXED(1) / 2)) >> 8;
                left  = centerX - horizontalExtent;
                right = centerX + horizontalExtent;
                CLAMP_EDGES_TO_SCREEN(left, right);
                scanlineData[0]  = ((left << 8) + right);
                scanlineData    += 2;
            }
            break;
        }
        case WINDOW_TRANSITION_LEADER_LOW:
        {
            Sint32 cameraX;

            cameraX = syMapGridScrollStateView(grid)[0] & FIELD_PIXEL_MASK;
            cameraY = syMapGridScrollStateView(grid)[1] & FIELD_PIXEL_MASK;
            actor   = fdActorLookup(MainRoutineStateWork.leaderActorId);
            centerX = (actor->x.fixed - cameraX) / toFIXED(1);
            // Centred lower above the leader's feet than WINDOW_TRANSITION_LEADER's.
            centerY = ((actor->z.fixed - actor->y.fixed) - cameraY) / toFIXED(1)
                    - TRANSITION_LOW_ELLIPSE_RISE;
            // DEAD: as in WINDOW_TRANSITION_LEADER_INVERSE, the tick counter is read and not used;
            // the ROM keeps the load.
            TickCount;
            *scanlineData++ = state->insideLayers;
            *scanlineData++ = state->outsideLayers;
            value           = state->value;
            if (value & TRANSITION_OPEN)
            {
                value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
            }
            else
            {
                value &= TRANSITION_OPEN - 1;
            }
            // sqrt(r * r - 0.75 * dy * dy): 1.15 times as tall as it is wide.
            value              *= TRANSITION_RADIUS_STEP;
            radiusSquaredFixed  = toFIXED(value * value);
            for (i = 0; i < DISPLAY_HEIGHT; i++)
            {
                value = i - centerY;
                horizontalExtent =
                    sySquartIWRAM(radiusSquaredFixed - value * value * 3 * (toFIXED(1) / 4)) >> 8;
                left  = centerX - horizontalExtent;
                right = centerX + horizontalExtent;
                CLAMP_EDGES_TO_SCREEN(left, right);
                scanlineData[0]  = ((left << 8) + right);
                scanlineData    += 2;
            }
            break;
        }
    }

    // Publish only after every span of the next page is ready.
    state->page ^= 1;
}

// Starts window transition mode at value 0, with no duration: the caller moves it.
void fdScreenTransitionStartHeldWindow(Sint32 mode)
{
    SCREEN_TRANSITION_STATE *state =
        syHeapSlotAllocateEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));

    DMA_FILL32(0, state, sizeof(*state));
    state->mode          = mode;
    state->value         = 0;
    state->insideLayers  = WININ_WIN0_ALL | WININ_WIN1_ALL;
    state->outsideLayers = WINOUT_WIN01_BG0;
    sySetEvent(fdScreenTransitionUpdateScanlineTable, EVENT_PASS_TICK);
    sySetEvent(fdScreenTransitionStartWindowHBlankDMA, EVENT_PASS_VBLANK);
}

// Unused: suspends the window transition's two events. Nothing calls it, by name, stub call entry
// or table.
UNUSED void fdScreenTransitionSuspendCallbacks(void)
{
    sySuspendEvent(&fdScreenTransitionUpdateScanlineTable);
    sySuspendEvent(&fdScreenTransitionStartWindowHBlankDMA);
}

// Allocates the screen transition state (HEAP_SLOT_SCREEN_TRANSITION) and clears it.
SCREEN_TRANSITION_STATE *fdScreenTransitionAllocateAndClearState(void)
{
    SCREEN_TRANSITION_STATE *state;

    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
    DMA_FILL32(0, state, sizeof(*state));
    return state;
}

// Fades the screen in over ticks ticks. mode is a SCENE_TRANSITION: its high byte an
// SCREEN_TRANSITION_STYLE (from black, from the backdrop colour, a window transition whose low byte
// is an WINDOW_TRANSITION_MODE, a dissolve, or opening from the centre line, which a non-zero low
// byte makes flicker).
void fdScreenTransitionStart(Sint32 mode, Sint32 ticks)
{
    SCREEN_TRANSITION_DISPLAY *display;
    Sint32                     variant;
    Sint32                     style;

    style   = SCENE_TRANSITION_STYLE(mode);
    display = MapGridState[0];
    variant = SCENE_TRANSITION_VARIANT(mode);
    switch (style)
    {
        case TRANSITION_FADE:
            syScreenFadeToBlack(0);
            syScreenFadeFromBlack(ticks);
            sySynch(1);
            break;
        case TRANSITION_BACKDROP_FADE:
            fdPaletteFadeSetSource(PALETTE_TINT_FROM_SOURCE, PLTT_BACKDROP);
            fdPaletteFadeStartInterpolation(ticks);
            sySynch(1);
            IO_WRITE_QUEUE_PUSH(
                (Uint32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, IO_WRITE_HALFWORD);
            fdPaletteFadeSetBackdrop(0);
            return;
        // A window or dissolve opens from value 0 (closed) to TRANSITION_OPEN, and the centre line
        // from half the screen's height to 0, over ticks ticks.
        case TRANSITION_WINDOW:
        {
            SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();
            state->mode                    = variant;
            state->value                   = 0;
            state->insideLayers            = WININ_WIN0_ALL;
            state->outsideLayers           = WINOUT_WIN01_BG0;
            sySetEvent(fdScreenTransitionUpdateScanlineTable, EVENT_PASS_TICK);
            sySetEvent(fdScreenTransitionStartWindowHBlankDMA, EVENT_PASS_VBLANK);
            sySynch(1);
            IO_WRITE_QUEUE_PUSH(
                (Uint32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, IO_WRITE_HALFWORD);
            state->start    = 0;
            state->end      = TRANSITION_OPEN;
            state->duration = ticks;
            state->step     = 0;
            return;
        }
        // Unused: no scene's transition is a TRANSITION_DISSOLVE.
        case TRANSITION_DISSOLVE:
        {
            SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();
            state->mode                    = variant;
            state->value                   = TRANSITION_OPEN;
            fdScreenTransitionFillTilemapAndColorTile(DISSOLVE_COVER_COLOR);
            sySynch(1);
            sySetEvent(fdScreenTransitionUpdateDissolve, EVENT_PASS_TICK);
            IO_WRITE_QUEUE_PUSH(
                (Uint32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, IO_WRITE_HALFWORD);
            state->start    = 0;
            state->end      = TRANSITION_OPEN;
            state->duration = ticks;
            state->step     = 0;
            return;
        }
        case TRANSITION_CENTER_LINE:
        {
            SCREEN_TRANSITION_STATE *state;

            display             = MapGridState[0];
            state               = fdScreenTransitionAllocateAndClearState();
            display->firstLine  = DISPLAY_HEIGHT / 2;
            display->secondLine = DISPLAY_HEIGHT / 2;
            sySynch(1);
            if (variant == CENTER_LINE_STEADY)
            {
                sySetEvent(fdScreenTransitionUpdate, EVENT_PASS_TICK);
                syIntFunction(IRQ_HBLANK, 0, fdScreenTransitionUpdateScanline);
                state->start    = DISPLAY_HEIGHT / 2;
                state->end      = 0;
                state->duration = ticks;
                state->step     = 0;
            }
            else
            {
                sySetEvent(fdScreenTransitionUpdateFromCenter, EVENT_PASS_TICK);
                syIntFunction(IRQ_HBLANK, 0, fdScreenTransitionUpdateScanline);
                state->start    = DISPLAY_HEIGHT / 2;
                state->end      = 0;
                state->duration = ticks;
                state->step     = 0;
            }
            break;
        }
    }
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, IO_WRITE_HALFWORD);
}

// Fades the screen out over ticks ticks, as fdScreenTransitionStart fades it in.
void fdScreenTransitionFinish(Sint32 mode, Sint32 ticks)
{
    Sint32 variant;
    Sint32 style;

    style   = SCENE_TRANSITION_STYLE(mode);
    variant = SCENE_TRANSITION_VARIANT(mode);
    switch (style)
    {
        case TRANSITION_FADE:
            syScreenFadeFromBlack(0);
            syScreenFadeToBlack(ticks);
            break;
        case TRANSITION_BACKDROP_FADE:
            fdPaletteFadeSetTarget(PALETTE_TINT_FROM_SOURCE, 0);
            fdPaletteFadeStartInterpolation(ticks);
            break;
        case TRANSITION_WINDOW:
        {
            SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();
            state->mode                    = variant;
            state->value                   = TRANSITION_OPEN;
            state->insideLayers            = WININ_WIN0_ALL;
            state->outsideLayers           = WINOUT_WIN01_BG0;
            sySetEvent(fdScreenTransitionUpdateScanlineTable, EVENT_PASS_TICK);
            sySetEvent(fdScreenTransitionStartWindowHBlankDMA, EVENT_PASS_VBLANK);
            sySynch(1);
            state->start    = TRANSITION_OPEN;
            state->end      = TRANSITION_CLOSED;
            state->duration = ticks;
            state->step     = 0;
            break;
        }
        // Unused, as in fdScreenTransitionStart.
        case TRANSITION_DISSOLVE:
        {
            SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();
            state->mode                    = variant;
            state->value                   = TRANSITION_OPEN;
            fdScreenTransitionFillTilemapAndColorTile(0);
            sySynch(1);
            sySetEvent(fdScreenTransitionUpdateDissolve, EVENT_PASS_TICK);
            state->start    = TRANSITION_OPEN;
            state->end      = TRANSITION_CLOSED;
            state->duration = ticks;
            state->step     = 0;
            break;
        }
        case TRANSITION_CENTER_LINE:
        {
            SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();
            if (variant == CENTER_LINE_STEADY)
            {
                sySetEvent(fdScreenTransitionUpdate, EVENT_PASS_TICK);
                syIntFunction(IRQ_HBLANK, 0, fdScreenTransitionUpdateScanline);
                state->start    = 0;
                state->end      = DISPLAY_HEIGHT / 2;
                state->duration = ticks;
                state->step     = 0;
            }
            else
            {
                sySetEvent(fdScreenTransitionUpdateFromCenter, EVENT_PASS_TICK);
                syIntFunction(IRQ_HBLANK, 0, fdScreenTransitionUpdateScanline);
                state->start    = 0;
                state->end      = DISPLAY_HEIGHT / 2;
                state->duration = ticks;
                state->step     = 0;
            }
            break;
        }
    }
}

// Clears the map's display-control bits for backgrounds 3, 2 and 1 (DISPCNT_BG3_ON, DISPCNT_BG2_ON
// and DISPCNT_BG1_ON) for each argument that is set.
void fdScreenTransitionHideBackgrounds(Sint32 clearBg3, Sint32 clearBg2, Sint32 clearBg1)
{
    SCREEN_TRANSITION_DISPLAY *state;

    state = MapGridState[0];
    if (state != NULL)
    {
        if (clearBg1 != 0)
        {
            state->dispcnt &= ~DISPCNT_BG1_ON;
        }
        if (clearBg2 != 0)
        {
            state->dispcnt &= ~DISPCNT_BG2_ON;
        }
        if (clearBg3 != 0)
        {
            state->dispcnt &= ~DISPCNT_BG3_ON;
        }
    }
}

// Each tick of the centre-line transition: moves the value toward its end and sets the two lines
// where the screen switches between shown and blank.
void fdScreenTransitionUpdate(void)
{
    SCREEN_TRANSITION_STATE   *state   = *syRamDisplayState;
    SCREEN_TRANSITION_DISPLAY *display = *(SCREEN_TRANSITION_DISPLAY **)syRamMapGridState;
    Uint32                     edgeDistance;

    if (state->duration != 0)
    {
        if (state->step >= state->duration)
        {
            state->duration = 0;
            syCloseEvent(fdScreenTransitionUpdate);
            syIntFunction(IRQ_HBLANK, 0, 0);
            return;
        }
        else
        {
            Sint32 delta = state->end - state->start;
            Sint32 value;

            state->step++;
            value        = syIntegerDivide(delta * state->step, state->duration);
            state->value = state->start + value;
        }
    }

    // Once the lines meet at the centre they are put below the screen.
    edgeDistance = state->value;
    if (edgeDistance >= DISPLAY_HEIGHT / 2)
    {
        display->firstLine  = CENTER_LINE_HIDDEN_FIRST;
        display->secondLine = CENTER_LINE_HIDDEN_SECOND;
    }
    else
    {
        display->firstLine  = edgeDistance;
        display->secondLine = DISPLAY_HEIGHT - 1 - edgeDistance;
    }
}

// As fdScreenTransitionUpdate, but the lines flicker every other tick.
void fdScreenTransitionUpdateFromCenter(void)
{
    SCREEN_TRANSITION_STATE   *state  = *syRamDisplayState;
    SCREEN_TRANSITION_DISPLAY *window = *(SCREEN_TRANSITION_DISPLAY **)syRamMapGridState;
    Uint16                     edgeDistance;

    if (state->duration != 0)
    {
        if (state->step >= state->duration)
        {
            state->duration = 0;
            syCloseEvent(fdScreenTransitionUpdateFromCenter);
            syIntFunction(IRQ_HBLANK, 0, 0);
            return;
        }
        else
        {
            Sint32 delta = state->end - state->start;
            Sint32 edgeOffset;

            state->step++;
            edgeOffset   = syIntegerDivide(delta * state->step, state->duration);
            state->value = state->start + edgeOffset;
        }
    }

    edgeDistance = state->value;
    if (edgeDistance >= DISPLAY_HEIGHT / 2)
    {
        window->firstLine  = CENTER_LINE_HIDDEN_FIRST;
        window->secondLine = CENTER_LINE_HIDDEN_SECOND;
    }
    else if (edgeDistance != 0 && (TickCount & 1))
    {
        window->firstLine  = edgeDistance + DISPLAY_HEIGHT / 2;
        window->secondLine = DISPLAY_HEIGHT / 2 - edgeDistance;
    }
    else
    {
        window->firstLine  = 0;
        window->secondLine = DISPLAY_HEIGHT - 1;
    }
}

// Where the centre-line transition's HBlank interrupt is in the tick.
typedef enum SCREEN_TRANSITION_IRQ_PHASE
{
    DISPLAY_TRANSITION_IRQ_WAIT_SCREEN = 0, // waits for a line on screen
    // Waits for whichever edge line comes first.
    DISPLAY_TRANSITION_IRQ_WAIT_EITHER = 1,
    // The first line has passed: waits for the second.
    DISPLAY_TRANSITION_IRQ_WAIT_SECOND = 2,
    // The second line has passed: waits for the first.
    DISPLAY_TRANSITION_IRQ_WAIT_FIRST  = 3,
    DISPLAY_TRANSITION_IRQ_DONE        = 9
} SCREEN_TRANSITION_IRQ_PHASE;

// The HBlank interrupt of the centre-line transition: switches the display to DISPCNT_MODE_2
// (nothing shown) at the first line and back to DISPCNT_MODE_0 at the second.
void fdScreenTransitionUpdateScanline(void)
{
    Uint32                     scanline = REG_VCOUNT;
    SCREEN_TRANSITION_DISPLAY *state;
    state = MapGridState[0];

    // Keep the same VCount sample across phase changes.
again:
    switch (state->irqPhase)
    {
        case DISPLAY_TRANSITION_IRQ_WAIT_FIRST:
            if (scanline >= state->irqFirstLine)
            {
                REG_DISPCNT     = (REG_DISPCNT & (Uint16)~DISPCNT_MODE_MASK) | DISPCNT_MODE_2;
                state->irqPhase = DISPLAY_TRANSITION_IRQ_DONE;
            }
            break;
        case DISPLAY_TRANSITION_IRQ_WAIT_SECOND:
            if (scanline >= state->irqSecondLine)
            {
                REG_DISPCNT     &= (Uint16)~DISPCNT_MODE_MASK;
                state->irqPhase  = DISPLAY_TRANSITION_IRQ_DONE;
            }
            break;
        case DISPLAY_TRANSITION_IRQ_WAIT_EITHER:
            if (scanline >= state->irqFirstLine)
            {
                REG_DISPCNT = (REG_DISPCNT & (Uint16)~DISPCNT_MODE_MASK) | DISPCNT_MODE_2;
                state->irqPhase++;
                goto again;
            }
            if (scanline >= state->irqSecondLine)
            {
                REG_DISPCNT     &= (Uint16)~DISPCNT_MODE_MASK;
                state->irqPhase  = DISPLAY_TRANSITION_IRQ_WAIT_FIRST;
                goto again;
            }
            break;
        case DISPLAY_TRANSITION_IRQ_WAIT_SCREEN:
            if (scanline <= DISPLAY_HEIGHT - 2)
            {
                state->irqPhase = DISPLAY_TRANSITION_IRQ_WAIT_EITHER;
                goto again;
            }
            break;
    }
}

// Unused, as no scene dissolves. Each tick of the dissolve: moves the value toward its end and sets
// two more of the solid tile's 64 pixels, in the order of ScreenTransitionDitherTable: to colour 0
// while the screen opens (values up to TRANSITION_OPEN clear the cover fdScreenTransitionStart
// filled with DISSOLVE_COVER_COLOR), and to DISSOLVE_COVER_COLOR while it closes (values after it
// fill the clear tile fdScreenTransitionFinish starts from).
void fdScreenTransitionUpdateDissolve(void)
{
    SCREEN_TRANSITION_STATE *state = *syRamDisplayState;
    Sint32                   ditherIndex;
    Sint32                   colorIndex;
    Uint32                   i;

    if (state->duration != 0)
    {
        if (state->step >= state->duration)
        {
            volatile Uint16 *dma0;
            state->duration = 0;
            syCloseEvent(fdScreenTransitionUpdateDissolve);
            dma0                   = REG_DMA0;
            dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
            dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
            (void)dma0[DMA_CNT_H_INDEX];
            return;
        }
        else
        {
            Sint32 delta = state->end - state->start;
            state->step++;
            state->value = state->start + syIntegerDivide(delta * state->step, state->duration);
        }
    }

    ditherIndex  = state->value - 1;
    state->page ^= 1;
    colorIndex   = 0;
    if (ditherIndex & TRANSITION_OPEN)
        colorIndex = DISSOLVE_COVER_COLOR;
    ditherIndex = (ditherIndex & (TRANSITION_OPEN - 1)) * 2;
    for (i = 0; i <= 1; i++)
    {
        Uint8  pixelIndex = ScreenTransitionDitherTable[ditherIndex & (8 * 8 - 1)];
        Uint8 *tileByte   = &state->solidTilePixels[pixelIndex >> 1];
        if (pixelIndex & 1)
            *tileByte = (*tileByte & 0x0f) | (colorIndex << 4);
        else
            *tileByte = (*tileByte & 0xf0) | colorIndex;
        ditherIndex++;
    }

    IO_WRITE_QUEUE_PUSH((Uint32)BG_CHAR_BLOCK(0),
                        (Uint32)state->solidTilePixels,
                        DMA_COPY_WORDS(TILE_SIZE_4BPP / 4));
}

// Unused, as the dissolve is: fills background screen block 4 with tile 0 in WINDOW_PALETTE and,
// unless color is -1, makes tile 0 solid color.
void fdScreenTransitionFillTilemapAndColorTile(Sint32 color)
{
    SCREEN_TRANSITION_STATE *state = ScreenTransitionStateWork;
    DMA_FILL32(WINDOW_TILE_PAIR(BG_TILE_PALETTE(WINDOW_PALETTE)),
               BG_SCREEN_BLOCK(4),
               DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(Uint16));
    if (color != -1)
    {
        Uint32  pattern = 0;
        Sint32  remainingWords;
        Uint32 *tile;
        for (remainingWords = 0; remainingWords < 8; remainingWords++)
            pattern = (pattern << 4) | color;
        tile = state->solidTile;
        for (remainingWords = 0; remainingWords < 8; remainingWords++)
            *tile++ = pattern;
        syDMAXCopy(
            state->solidTile, BG_CHAR_BLOCK(0), DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
    }
}

// Unused: nothing calls it, by name or through its stub. Starts a dissolve of the given mode, its
// value held at 0, and waits two seconds.
void fdScreenTransitionStartDissolveHeld(Sint32 mode)
{
    SCREEN_TRANSITION_STATE *state;

    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
    DMA_FILL32(0, state, sizeof(*state));
    fdScreenTransitionFillTilemapAndColorTile(0);
    state->mode  = mode;
    state->value = 0;
    sySetEvent(fdScreenTransitionUpdateDissolve, EVENT_PASS_TICK);
    sySynch(120);
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
