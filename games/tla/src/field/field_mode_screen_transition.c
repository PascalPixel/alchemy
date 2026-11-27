// The screen transitions: windows that close over the screen or open it, line by line, and the
// dissolve. Part of field_mode.c.

// A window transition's value: from 0, the screen covered, it opens to TRANSITION_OPEN and closes
// again to TRANSITION_CLOSED; a mode draws the opening half and the closing half mirrored.
#define TRANSITION_OPEN             32
#define TRANSITION_CLOSED           64
// The widest a line's window reaches from the left edge in WINDOW_TRANSITION_NOISE: WIN0H's right
// edge is a byte.
#define TRANSITION_NOISE_EDGE_MAX   255
// WINDOW_TRANSITION_RIPPLE measures a line further than this from its centre as this far.
#define TRANSITION_RIPPLE_LINES_MAX 120
// ScreenTransitionEdgeOffsets' entries, the ripple's period in lines.
#define TRANSITION_EDGE_OFFSETS     32
// Where the centre-line transition puts its two lines once they meet: below the screen, so no
// switch happens.
#define CENTER_LINE_HIDDEN_FIRST    200
#define CENTER_LINE_HIDDEN_SECOND   250
// The colour the dissolve covers the screen in, the window palette's last.
#define DISSOLVE_COVER_COLOR        15
// REG_WINOUT's high byte, what the sprite window shows, which a window transition keeps as it was.
#define WINOUT_WINOBJ_MASK          0xff00

// Keeps a line's window edges on the screen, 0 to DISPLAY_WIDTH.
#define CLAMP_SPAN_TO_SCREEN(left, right)                                                          \
	{                                                                                              \
		if (left < 0)                                                                              \
			left = 0;                                                                              \
		if (right < 0)                                                                             \
			right = 0;                                                                             \
		if (left > DISPLAY_WIDTH)                                                                  \
			left = DISPLAY_WIDTH;                                                                  \
		if (right > DISPLAY_WIDTH)                                                                 \
			right = DISPLAY_WIDTH;                                                                 \
	}
// Writes every line's window edges for an ellipse round (centerX, centerY) whose half-width on a
// line dy from its centre is sqrt(r * r - squash * dy * dy), with r * r in radiusSquaredFixed
// (squash in 16.16).
#define WRITE_ELLIPSE_LINES(squash)                                                                \
	for (i = 0; i < DISPLAY_HEIGHT; i++)                                                           \
	{                                                                                              \
		value            = i - centerY;                                                            \
		horizontalExtent = sySquartIWRAM(radiusSquaredFixed - value * value * (squash)) >> 8;      \
		left             = centerX - horizontalExtent;                                             \
		right            = centerX + horizontalExtent;                                             \
		CLAMP_SPAN_TO_SCREEN(left, right);                                                         \
		scanlineData[0]  = (left << 8) + right;                                                    \
		scanlineData    += 2;                                                                      \
	}

// Each VBlank, before drawing: turns windows 0 and 1 on, writes the first four values of the shown
// page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0 copy the rest into WIN0H and WIN1H one
// word each HBlank, so that every line of the screen gets its own window edges.
void fdEffectStartWindowHBlankDMA(void)
{
	SCREEN_TRANSITION_STATE *state         = RamHeapSlots->screenTransition;
	Uint16                  *scanlineData  = state->pages[state->page];
	volatile Uint16         *dma0          = REG_DMA0;

	// Stop the previous transfer before priming the first line's registers; dma0[5] is the
	// channel's control, REG_DMA0CNT_H.
	dma0[5]                               &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma0[5]                               &= ~DMA_ENABLE;
	(void)dma0[5];
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
	syIOWrite16(*scanlineData++, &REG_WININ);
	syIOWrite16(*scanlineData++, &REG_WINOUT);
	syIOWrite16(*scanlineData++, &REG_WIN0H);
	syIOWrite16(*scanlineData++, &REG_WIN1H);
	syIOWrite16(WIN_RANGE(0, DISPLAY_HEIGHT), &REG_WIN0V);
	syIOWrite16(WIN_RANGE(0, DISPLAY_HEIGHT), &REG_WIN1V);
	// HBlank repeats one word: both horizontal window bounds for the next line.
	syDMAXCopy(scanlineData, (void *)&REG_WIN0H, DMA_HBLANK_WORD_PER_LINE, (volatile Uint32 *)dma0);
}

// Each tick of a window transition: advances its value toward its end (and at the end turns the
// windows off, or moves to the next mode), then builds the next page of window edges, WININ and
// WINOUT first and then a (left << 8 | right) pair for each of the 160 lines.
void fdScreenTransitionUpdateScanlineTable(void)
{
	MAP_STATE               *map   = RamHeapSlots->mapGrid;
	SCREEN_TRANSITION_STATE *state = RamHeapSlots->screenTransition;
	TRANSITION_SPAN         *span;
	TRANSITION_SHAPE        *shape;
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
	Sint32                   cameraX;
	Sint32                   cameraY8;
	Uint32                   leftEdge;
	Sint32                   oppositeEdges;

	if (state->duration != 0)
	{
		if (state->step >= state->duration)
		{
			state->duration = 0;
			if (state->nextMode == 0)
			{
				if (state->end == TRANSITION_CLOSED)
					REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON
					                 | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
				else
					REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
				if (REG_DISPCNT & DISPCNT_OBJWIN_ON)
					REG_WINOUT |= WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2
					            | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ;
				syCloseEvent(fdEffectStartWindowHBlankDMA);
				syCloseEvent(fdScreenTransitionUpdateScanlineTable);
				{
					volatile Uint16 *dma0  = REG_DMA0;

					dma0[5]               &= ~DMA_START_TIMING_DRQ_REPEAT;
					dma0[5]               &= ~DMA_ENABLE;
					dma0[5];
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

	scanlineData = state->pages[1 ^ state->page];
	mnScanlineWindowFillSpansStub(scanlineData + 2);

	switch (state->mode)
	{
		case WINDOW_TRANSITION_BARS:
			*scanlineData++ = TRANSITION_SHOW_ALL;
			*scanlineData++ = WINOUT_WIN01_BG0;
			value           = state->value;
			if (value & TRANSITION_OPEN)
				value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
			else
				value &= TRANSITION_OPEN - 1;
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
						// A slant: the left edge goes from x0 to x1 and the right from y0 to y1
						// over the span's lines, held in radiusSquaredFixed and delta, left and
						// right.
						radiusSquaredFixed = span->x0;
						left               = span->y0;
						delta              = span->x1;
						right              = span->y1;
						for (k = 0; k < lineCount; k++)
						{
							scanlineData[0] =
								((radiusSquaredFixed + (delta - radiusSquaredFixed) * k / lineCount)
							     << 8)
								+ (left + (right - left) * k / lineCount);
							scanlineData += 2;
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
					horizontalExtent = TRANSITION_NOISE_EDGE_MAX;
				scanlineData[0]  = horizontalExtent;
				scanlineData    += 2;
			}
			break;

		case WINDOW_TRANSITION_CIRCLE:
			value           = state->value;
			*scanlineData++ = TRANSITION_SHOW_ALL;
			*scanlineData++ = WINOUT_WIN01_BG0;
			if ((Uint32)value > TRANSITION_OPEN)
			{
				*scanlineData++ = WININ_WIN0_BG0;
				*scanlineData++ = TRANSITION_SHOW_ALL;
				value           = TRANSITION_CLOSED - value;
			}
			// A circle about the screen's centre whose radius grows 5 pixels a step, to 160 at
			// TRANSITION_OPEN; each line's edges are clamped to the screen.
			value              *= 5;
			radiusSquaredFixed  = toFIXED(value * value);
			for (i = 0; i < DISPLAY_HEIGHT; i++)
			{
				value            = i - DISPLAY_HEIGHT / 2;
				horizontalExtent = sySquartIWRAM(radiusSquaredFixed - toFIXED(value * value)) >> 8;
				left             = DISPLAY_WIDTH / 2 - horizontalExtent;
				right            = horizontalExtent + DISPLAY_WIDTH / 2;
				CLAMP_SPAN_TO_SCREEN(left, right);
				scanlineData[0]  = (left << 8) + right;
				scanlineData    += 2;
			}
			break;

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
			value = (Uint32)(DISPLAY_WIDTH * (value & (TRANSITION_OPEN - 1))) >> 5;
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
			Sint32 ax;

			ax              = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
			cameraY         = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
			// Centred on the leader's place on screen, 16 pixels above its feet.
			actor           = fdActorLookup(fdPartyGetLeaderActor());
			centerX         = (actor->x.fixed - ax) / toFIXED(1);
			centerY         = ((actor->z.fixed - actor->y.fixed) - cameraY) / toFIXED(1) - 16;
			// This mode writes the two masks and folds the progress the other way round from
			// WINDOW_TRANSITION_LEADER.
			*scanlineData++ = state->outsideLayers;
			*scanlineData++ = state->insideLayers;
			value           = state->value;
			if ((value & TRANSITION_OPEN) == 0)
				value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
			else
				value &= TRANSITION_OPEN - 1;
			// On odd ticks the ellipse has no size: the flicker.
			if (TickCount & 1)
				value = 0;
			// Squashed by 1.5: an ellipse 1.22 times as wide as it is tall.
			value              *= 5;
			radiusSquaredFixed  = toFIXED(value * value);
			WRITE_ELLIPSE_LINES(3 * (toFIXED(1) / 2));
			break;
		}

		case WINDOW_TRANSITION_RIPPLE:
		{

			cameraX  = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
			cameraY8 = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
		}
			// Centred on state->centerX and centerY; each line's edges move out by
			// ScreenTransitionEdgeOffsets, indexed two entries a line and one a tick, so the
			// outline ripples.
			centerX         = (state->centerX - cameraX) / toFIXED(1);
			centerY         = (state->centerY - cameraY8) / toFIXED(1) - 16;
			k               = -centerY * 2 + TickCount;
			*scanlineData++ = state->insideLayers;
			*scanlineData++ = state->outsideLayers;
			value           = state->value;
			if (value & TRANSITION_OPEN)
				value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
			else
				value &= TRANSITION_OPEN - 1;
			value              *= 5;
			radiusSquaredFixed  = toFIXED(value * value);
			for (i = 0; i < DISPLAY_HEIGHT; i++)
			{
				delta = i - centerY;
				if (delta < -TRANSITION_RIPPLE_LINES_MAX)
					delta = -TRANSITION_RIPPLE_LINES_MAX;
				if (delta > TRANSITION_RIPPLE_LINES_MAX)
					delta = TRANSITION_RIPPLE_LINES_MAX;
				delta *= delta;
				horizontalExtent =
					sySquartIWRAM(radiusSquaredFixed - delta * 3 * (toFIXED(1) / 2)) >> 8;
				left  = centerX - horizontalExtent;
				right = centerX + horizontalExtent;
				if (left < right)
				{
					value  = k & (TRANSITION_EDGE_OFFSETS - 1);
					delta  = ScreenTransitionEdgeOffsets[value];
					left  -= delta;
					right += delta;
					// An offset that closes the line leaves it with no window.
					if (left >= right)
					{
						left  = DISPLAY_WIDTH;
						right = DISPLAY_WIDTH;
					}
				}
				CLAMP_SPAN_TO_SCREEN(left, right);
				scanlineData[0]  = (left << 8) + right;
				scanlineData    += 2;
				k               += 2;
			}
			break;

		case WINDOW_TRANSITION_LEADER:
		{
			Sint32 ax;

			ax              = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
			cameraY         = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
			actor           = fdActorLookup(fdPartyGetLeaderActor());
			centerX         = (actor->x.fixed - ax) / toFIXED(1);
			centerY         = ((actor->z.fixed - actor->y.fixed) - cameraY) / toFIXED(1) - 16;
			*scanlineData++ = state->insideLayers;
			*scanlineData++ = state->outsideLayers;
			value           = state->value;
			if (value & TRANSITION_OPEN)
				value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
			else
				value &= TRANSITION_OPEN - 1;
			value              *= 5;
			radiusSquaredFixed  = toFIXED(value * value);
			WRITE_ELLIPSE_LINES(3 * (toFIXED(1) / 2));
			break;
		}

		case WINDOW_TRANSITION_LEADER_LOW:
		{
			Sint32 ax;

			ax              = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
			cameraY         = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
			actor           = fdActorLookup(fdPartyGetLeaderActor());
			centerX         = (actor->x.fixed - ax) / toFIXED(1);
			// Centred 8 pixels above the leader's feet, not 16 as in WINDOW_TRANSITION_LEADER.
			centerY         = ((actor->z.fixed - actor->y.fixed) - cameraY) / toFIXED(1) - 8;
			*scanlineData++ = state->insideLayers;
			*scanlineData++ = state->outsideLayers;
			value           = state->value;
			if (value & TRANSITION_OPEN)
				value = TRANSITION_OPEN - (value & (TRANSITION_OPEN - 1));
			else
				value &= TRANSITION_OPEN - 1;
			// Squashed by 0.75: 1.15 times as tall as it is wide.
			value              *= 5;
			radiusSquaredFixed  = toFIXED(value * value);
			WRITE_ELLIPSE_LINES(3 * (toFIXED(1) / 4));
			break;
		}
	}

	// Shows the next page only once every line of it is ready.
	state->page ^= 1;
}

// Starts window transition mode at value 0, with no duration: the caller moves it.
void fdScreenTransitionStartWindowHeld(Sint32 mode)
{
	SCREEN_TRANSITION_STATE *state =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));

	DMA_FILL32(0, state, sizeof(*state));
	state->mode          = mode;
	state->value         = 0;
	state->insideLayers  = WININ_WIN0_ALL | WININ_WIN1_ALL;
	state->outsideLayers = (REG_WINOUT & WINOUT_WINOBJ_MASK) | WINOUT_WIN01_BG0;
	sySetEvent(fdScreenTransitionUpdateScanlineTable, EVENT_PASS_TICK);
	sySetEvent(fdEffectStartWindowHBlankDMA, EVENT_PASS_VBLANK - 10);
}

// Unused: suspends the window transition's two events. Nothing calls it, by name, stub call entry
// or table.
UNUSED void fdScreenTransitionSuspendCallbacks(void)
{
	sySuspendEvent(&fdScreenTransitionUpdateScanlineTable);
	sySuspendEvent(&fdEffectStartWindowHBlankDMA);
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
	MAP_VIEW_STATE *display;
	Sint32          variant;
	Sint32          style;

	display = (MAP_VIEW_STATE *)RamHeapSlots->mapGrid;
	style   = SCENE_TRANSITION_STYLE(mode);
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
		case TRANSITION_WINDOW:
		{
			SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();
			state->mode                    = variant;
			state->value                   = 0;
			state->insideLayers            = WININ_WIN0_ALL;
			state->outsideLayers           = (REG_WINOUT & WINOUT_WINOBJ_MASK) | WINOUT_WIN01_BG0;
			sySetEvent(fdScreenTransitionUpdateScanlineTable, EVENT_PASS_TICK);
			sySetEvent(fdEffectStartWindowHBlankDMA, EVENT_PASS_VBLANK - 10);
			sySynch(1);
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_DISPCNT, REG_DISPCNT | display->dispcnt, IO_WRITE_HALFWORD);
			state->start    = 0;
			state->end      = TRANSITION_OPEN;
			state->duration = ticks;
			state->step     = 0;
			return;
		}
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
		case TRANSITION_CENTRE_LINE:
		{
			SCREEN_TRANSITION_STATE *state;

			display               = RamHeapSlots->mapGrid;
			state                 = fdScreenTransitionAllocateAndClearState();
			display->windowTop    = DISPLAY_HEIGHT / 2;
			display->windowBottom = DISPLAY_HEIGHT / 2;
			sySynch(1);
			if (variant == CENTRE_LINE_STEADY)
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
			state->outsideLayers           = (REG_WINOUT & WINOUT_WINOBJ_MASK) | WINOUT_WIN01_BG0;
			sySetEvent(fdScreenTransitionUpdateScanlineTable, EVENT_PASS_TICK);
			sySetEvent(fdEffectStartWindowHBlankDMA, EVENT_PASS_VBLANK - 10);
			sySynch(1);
			state->start    = TRANSITION_OPEN;
			state->end      = TRANSITION_CLOSED;
			state->duration = ticks;
			state->step     = 0;
			break;
		}
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
		case TRANSITION_CENTRE_LINE:
		{
			SCREEN_TRANSITION_STATE *state = fdScreenTransitionAllocateAndClearState();

			state->centreLine              = 1;
			if (variant == CENTRE_LINE_STEADY)
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

// Returns 1 while fdScreenTransitionUpdateScanline is the HBlank handler, that is, while a
// centre-line transition runs.
Bool fdScreenTransitionIsScanlineActive(void)
{
	if (IrqHandlers[IRQ_HBLANK] == fdScreenTransitionUpdateScanline)
		return TRUE;
	return FALSE;
}

// Clears the map's display-control bits for backgrounds 3, 2 and 1 (DISPCNT_BG3_ON, DISPCNT_BG2_ON
// and DISPCNT_BG1_ON) for each argument that is set.
void fdScreenTransitionHideBackgrounds(Sint32 clearBg3, Sint32 clearBg2, Sint32 clearBg1)
{
	MAP_STATE *state;

	state = RamHeapSlots->mapGrid;
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
// where the screen switches between shown and blank; at the end the map draws its usual three extra
// tiles again.
void fdScreenTransitionUpdate(void)
{
	SCREEN_TRANSITION_STATE *state;
	MAP_VIEW_STATE          *window;
	Sint8                   *duration;
	Uint32                   edgeDistance;

	state    = RamHeapSlots->screenTransition;
	duration = &state->duration;
	window   = (MAP_VIEW_STATE *)RamHeapSlots->mapGrid;
	if (*duration != 0)
	{
		Sint8 *step = &state->step;

		if (*step >= *duration)
		{
			*duration = 0;
			syCloseEvent(fdScreenTransitionUpdate);
			syIntFunction(IRQ_HBLANK, 0, 0);
			window->extraTiles = 3;
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
		window->windowTop    = CENTER_LINE_HIDDEN_FIRST;
		window->windowBottom = CENTER_LINE_HIDDEN_SECOND;
	}
	else
	{
		window->windowTop    = edgeDistance;
		window->windowBottom = DISPLAY_HEIGHT - 1 - edgeDistance;
	}
}

// As fdScreenTransitionUpdate, but the lines flicker every other tick.
void fdScreenTransitionUpdateFromCenter(void)
{
	SCREEN_TRANSITION_STATE *state;
	MAP_VIEW_STATE          *window;
	Sint8                   *duration;
	Uint16                   edgeDistance;

	state    = RamHeapSlots->screenTransition;
	duration = &state->duration;
	window   = (MAP_VIEW_STATE *)RamHeapSlots->mapGrid;
	if (*duration != 0)
	{
		Sint8 *step = &state->step;

		if (*step >= *duration)
		{
			*duration = 0;
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
		window->windowTop    = CENTER_LINE_HIDDEN_FIRST;
		window->windowBottom = CENTER_LINE_HIDDEN_SECOND;
	}
	else if (edgeDistance != 0 && (TickCount & 1))
	{
		window->windowTop    = edgeDistance + DISPLAY_HEIGHT / 2;
		window->windowBottom = DISPLAY_HEIGHT / 2 - edgeDistance;
	}
	else
	{
		window->windowTop    = 0;
		window->windowBottom = DISPLAY_HEIGHT - 1;
	}
}

// Where the centre-line transition's HBlank interrupt is.
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
	Uint32          scanline = REG_VCOUNT;
	MAP_VIEW_STATE *state;
	state = RamHeapSlots->mapGrid;

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

// Each tick of the dissolve: moves the value toward its end and sets two more of the solid tile's
// 64 pixels, in the order of ScreenTransitionDitherTable: to colour 0 while the screen opens
// (values up to TRANSITION_OPEN clear the cover fdScreenTransitionStart filled with
// DISSOLVE_COVER_COLOR), and to DISSOLVE_COVER_COLOR while it closes (values after it fill the
// clear tile fdScreenTransitionFinish starts from).
void fdScreenTransitionUpdateDissolve(void)
{
	SCREEN_TRANSITION_DISSOLVE_STATE *state =
		(SCREEN_TRANSITION_DISSOLVE_STATE *)RamHeapSlots->screenTransition;
	Sint32 ditherIndex;
	Sint32 colorIndex;
	Uint32 i;

	if (state->duration != 0)
	{
		if (state->step >= state->duration)
		{
			volatile Uint16 *dma0;

			state->duration = 0;
			syCloseEvent(fdScreenTransitionUpdateDissolve);
			dma0     = REG_DMA0;
			dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
			dma0[5] &= ~DMA_ENABLE;
			(void)dma0[5];
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

// Fills background screen block 4 with tile 0 in WINDOW_PALETTE and, unless color is -1, makes tile
// 0 solid color.
void fdScreenTransitionFillTilemapAndColorTile(Sint32 color)
{
	SCREEN_TRANSITION_STATE *state = RamHeapSlots->screenTransition;
	DMA_FILL32(WINDOW_TILE_PAIR(BG_TILE_PALETTE(WINDOW_PALETTE)),
	           BG_SCREEN_BLOCK(4),
	           DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(Uint16));
	if (color != -1)
	{
		// A row of eight pixels is a word of eight nibbles; a tile is eight rows.
		Uint32  pattern = 0;
		Sint32  i;
		Uint32 *tile;
		for (i = 0; i < 8; i++)
			pattern = (pattern << 4) | color;
		tile = state->solidTile;
		for (i = 0; i < 8; i++)
			*tile++ = pattern;
		syDMAXCopy(
			state->solidTile, BG_CHAR_BLOCK(0), DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
	}
}

// Starts a dissolve of the given mode, its value held at 0, and waits two seconds.
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

#undef TRANSITION_OPEN
#undef TRANSITION_CLOSED
#undef TRANSITION_NOISE_EDGE_MAX
#undef TRANSITION_RIPPLE_LINES_MAX
#undef TRANSITION_EDGE_OFFSETS
#undef CENTER_LINE_HIDDEN_FIRST
#undef CENTER_LINE_HIDDEN_SECOND
#undef DISSOLVE_COVER_COLOR
#undef WINOUT_WINOBJ_MASK
