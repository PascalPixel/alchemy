/* Window zoom and borders, and opening and waiting on message windows. Part of
   window.c. */

/* The pixels between sprite glyphs at a message's start. */
#define TEXT_DEFAULT_LETTER_SPACING 9

/* The glyph tiles' size in sprite VRAM: the four tiles of a 16x16 glyph for
   each window sprite. */
#define GLYPH_TILES_SIZE (WINDOW_SPRITE_COUNT * 4 * TILE_SIZE_4BPP)

/* The tile rows a field message is centred in, from the top of the screen. */
#define FIELD_MESSAGE_TOP_ROWS 12

/* A tilemap entry's tile number and flip bits, below its palette. */
#define TILE_NUMBER_AND_FLIPS (TILE_NUMBER_MASK | TILE_HFLIP | TILE_VFLIP)

/* Scratch values for a stage of a window's zoom: the scaled part of a side, the
   scaled duration and their ratio. A local of mnWindowDrawZoomBorder. */
typedef struct WINDOW_ZOOM_SCRATCH {
	FIXED scaledPart; /* zoom stage (or stages left) times a side in tiles */
	FIXED scaledDuration; /* twice the zoom's duration */
	FIXED result;		  /* scaledPart / scaledDuration, in tiles */
} WINDOW_ZOOM_SCRATCH;

/* The per-frame window update: advances each window's zoom while it opens or
   closes, draws it in full once open, and frees its slot once closed. */
void mnWindowUpdateZooms(void)
{
	WIN_STATE *windowState = WINDOW_STATE;
	WINDOW	  *window = windowState->wins;
	Sint32	   index = 0;

loop:
	/* Flags 0 with a duration left means closing (mnWindowClose clears the
	   flags first). */
	if(window->flg != 0) {
		/* Opening: zoom out one stage a frame, then draw the window in full,
		   which clears the duration. */
		if(window->zoomStage != 0) {
			mnWindowDrawZoomBorder(window, 0);
			window->zoomStage--;
		} else if(window->duration != 0) {
			mnWindowDrawOpened(window);
		}
	} else if(window->duration != 0) {
		/* Closing: zoom in one stage a frame, then erase it and free its
		   slot. */
		if(window->zoomStage != window->duration) {
			mnWindowEraseBorderRect(window->prevX, window->prevY,
				window->prevWidth, window->prevHeight);
			mnWindowDrawZoomBorder(window, 1);
			window->zoomStage++;
			/* FAKEMATCH(dead): a once-only block that places allBands, the
			 * dirty bit that uploads every band, at its store; as
			 * windowState->dirty = 1. */
			{
				Uint8 allBands = 1;

				windowState->dirty = allBands;
			}
		} else {
			mnWindowEraseBorderRect(window->prevX, window->prevY,
				window->prevWidth, window->prevHeight);
			window->sprs.head = NULL;
			window->sprs.tailLink = NULL;
			window->width = 0;
			window->height = 0;
			window->x = 0;
			window->y = 0;
			window->opened = 0;
			window->anm = 0;
			window->state = 0;
			window->flg = 0;
			window->zoomStage = 0;
			window->duration = 0;
			window->prevX = 0;
			window->prevY = 0;
			window->prevWidth = 0;
			window->prevHeight = 0;
			windowState->dirty = 1;
		}
	}
	index++;
	window++;
	if(index != WINDOW_COUNT)
		goto loop;
}

/* Draws a zooming window at its size for this frame: the window grows from its
   centre as zoomStage counts down to 0. */
void mnWindowDrawZoomBorder(void *windowArg, Sint32 savePosition)
{
	WINDOW			   *window = windowArg;
	WINDOW_ZOOM_SCRATCH scratch;
	Sint32				stage;
	Sint32				duration;
	Sint32				remaining;
	Sint32				x;
	Sint32				y;
	Sint32				width;
	Sint32				height;

	stage = window->zoomStage;
	duration = window->duration;
	remaining = duration - stage;
	/* syDivFX(d, v) is v / d in 16.16 fixed point: the left edge moves in by
	   stage / duration of half the width, and the width is (duration - stage) /
	   duration of the whole, twice that half; the same for the top and the
	   height. */
	scratch.scaledPart = toFIXED(stage * window->width);
	scratch.scaledDuration = toFIXED(2 * duration);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	x = FIXEDtoINT(scratch.result) + window->x;

	scratch.scaledPart = toFIXED(remaining * window->width);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	width = FIXEDtoINT_TWICE(scratch.result);

	scratch.scaledPart = toFIXED(stage * window->height);
	scratch.scaledDuration = toFIXED(2 * window->duration);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	y = FIXEDtoINT(scratch.result) + window->y;

	scratch.scaledPart = toFIXED(remaining * window->height);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	height = FIXEDtoINT_TWICE(scratch.result);

	mnWindowDrawBorder(x, y, width, height);
	if(savePosition != 0) {
		window->prevX = x;
		window->prevY = y;
		window->prevWidth = width;
		window->prevHeight = height;
	}
}

/* Fills count halfwords at dst with value by DMA, and returns the position
   after them. value holds the halfword in both its halves. */
Uint16 *mnMemoryFillHalfwordsDMA(Uint16 *dst, Sint32 value, Sint32 count)
{
	if(count > 0) {
		DMA_FILL16(value, dst, count * 2);
		dst += count;
	}
	return dst;
}

/* Draws an empty window: a border around a filled interior, at tile (x, y) of
   width by height tiles, and frees the text tiles the rectangle showed. */
void mnWindowDrawBorder(Sint32 x, Sint32 y, Uint32 width, Uint32 height)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint16	  *cursor =
		(Uint16 *)((y * BG_SCREEN_WIDTH + x) * 2 + (Uint32)state->tilemap);
	Uint32 row;

	if(width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
		height > DISPLAY_TILE_WIDTH)
		return;
	mnWindowClearTextTilesInRect(x, y, width, height);
	/* The top edge. */
	if(state->mindRead != 0)
		*cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER;
	else
		*cursor++ = WINDOW_TILE_BORDER_TOP_LEFT;
	cursor = mnMemoryFillHalfwordsDMA(
		cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_TOP), width - 2);
	if(state->mindRead != 0)
		*cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP;
	else
		*cursor++ = WINDOW_TILE_BORDER_TOP_RIGHT;
	cursor += BG_SCREEN_WIDTH - width;
	/* The sides and the fill between them. */
	for(row = 1; row < height - 1; row++) {
		*cursor++ = WINDOW_TILE_BORDER_LEFT;
		if(width != 2)
			cursor = mnMemoryFillHalfwordsDMA(
				cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
		*cursor++ = WINDOW_TILE_BORDER_RIGHT;
		cursor += BG_SCREEN_WIDTH - width;
	}
	/* The bottom edge. */
	if(state->mindRead != 0)
		*cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_VFLIP;
	else
		*cursor++ = WINDOW_TILE_BORDER_BOTTOM_LEFT;
	cursor = mnMemoryFillHalfwordsDMA(
		cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_BOTTOM), width - 2);
	if(state->mindRead != 0)
		*cursor = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP | TILE_VFLIP;
	else
		*cursor = WINDOW_TILE_BORDER_BOTTOM_RIGHT;
	state->dirty = 1;
}

/* Maps the text canvas tiles into the interior of a window at tile (x, y). With
   fullWidth set the canvas covers the side border columns too. */
void mnWindowMapTextCanvasTiles(
	Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 fullWidth)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint16	  *cursor =
		(Uint16 *)((y * BG_SCREEN_WIDTH + x) * 2 + (Uint32)state->tilemap);
	Uint32 row;
	Uint32 col;

	if(width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
		height > DISPLAY_TILE_WIDTH)
		return;
	/* Canvas tiles run down each column of the interior: the tile at interior
	   row r (from 1) of column c (from 0) is TEXT_CANVAS_FIRST_TILE - 1 + r
	   plus c columns of height - 2 tiles. */
	cursor += BG_SCREEN_WIDTH;
	if(fullWidth == 0) {
		for(row = 1; row < height - 1; row++) {
			cursor++;
			for(col = 1; col < width - 1; col++)
				*cursor++ = WINDOW_TILE((TEXT_CANVAS_FIRST_TILE - 1 + row +
											(col - 1) * (height - 2)) &
					TILE_NUMBER_AND_FLIPS);
			cursor++;
			cursor += BG_SCREEN_WIDTH - width;
		}
	} else {
		for(row = 1; row < height - 1; row++) {
			for(col = 0; col < width; col++)
				*cursor++ = WINDOW_TILE(
					(TEXT_CANVAS_FIRST_TILE - 1 + row + col * (height - 2)) &
					TILE_NUMBER_AND_FLIPS);
			cursor += BG_SCREEN_WIDTH - width;
		}
	}
	state->dirty = 1;
}

/* Returns 1 when every message has finished its text (or is idle), and 0 while
   one is still typing. */
Bool mnMessageAllTyped(void)
{
	MESSAGE *message;
	Sint32	 i;

	message = WINDOW_STATE->msgs;
	for(i = 0; i != MESSAGE_COUNT; i++, message++) {
		if(message->win != NULL && message->win->state == WINDOW_TYPING)
			return FALSE;
	}
	return TRUE;
}

/* Returns 1 when a window is fully closed (its slot free), and 0 while it is
   open or closing. */
Bool mnWindowIsClosed(WINDOW *window)
{
	/* Japanese only: a NULL window counts as closed; the other editions read
	   through it. */
#if !WINDOW_IS_CLOSED_NO_NULL_CHECK
	if(window == NULL)
		return TRUE;
#endif
	if(window->flg == 0 && window->duration == 0)
		return TRUE;
	return FALSE;
}

/* Resets the style glyphs are drawn in: the default colour, plain, and the
   default line and letter spacing. */
void mnTextResetStyle(void)
{
	WIN_STATE *state = WINDOW_STATE;

	state->colour = TEXT_DEFAULT_COLOR;
	state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	state->letterSpacing = TEXT_DEFAULT_LETTER_SPACING;
	state->outline = TEXT_STYLE_PLAIN;
	state->unusedAfterLineSpacing = 1;
}

/* Reserves the glyph sprites' tiles, resets the text style, starts the ring of
   decoded text from its beginning and starts the per-frame window update
   (mnWindowSystemUpdate). */
void mnWindowSystemStartFieldUpdate(void)
{
	WIN_STATE *state = WINDOW_STATE;

	/* The glyph tiles, reserved with nothing to load. */
	state->glyphTiles =
		syVRAMCacheLoad(VRAM_CACHE_ENTRY_GLYPH_TILES, GLYPH_TILES_SIZE, 0);
	state->letterSpacing = TEXT_DEFAULT_LETTER_SPACING;
	state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	state->outline = TEXT_STYLE_PLAIN;
	state->colour = TEXT_DEFAULT_COLOR;
	state->textBufEnd = 0;
	sySetEvent(mnWindowSystemUpdate, EVENT_PASS_FRAME);
}

/* Resets the text style and starts the per-frame window update
   (mnWindowSystemUpdate), reserving the glyph sprites' tiles first when
   loadGlyphTiles is set. */
void mnWindowSystemStartBattleUpdate(Sint32 loadGlyphTiles)
{
	WIN_STATE *state = WINDOW_STATE;

	if(loadGlyphTiles != 0)
		state->glyphTiles =
			syVRAMCacheLoad(VRAM_CACHE_ENTRY_GLYPH_TILES, GLYPH_TILES_SIZE, 0);
	state->letterSpacing = TEXT_DEFAULT_LETTER_SPACING;
	state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	state->outline = TEXT_STYLE_PLAIN;
	state->colour = TEXT_DEFAULT_COLOR;
	state->textBufEnd = 0;
	sySetEvent(mnWindowSystemUpdate, EVENT_PASS_FRAME);
}

/* Closes the battle message window at once, if one is open, and forgets it, so
   the next battle message (mnBattleMessageOpen) opens a new one. */
void mnBattleMessageClose(void)
{
	BATTLE_MESSAGE_STATE *display = BattleMessageStateWork;
	WINDOW				 *window = display->win;

	if(window != NULL) {
		mnWindowClose(window, 1);
		display->win = NULL;
	}
}

/* Shows a message in the battle message window, opening the window (a
   full-width text canvas along the bottom of the screen) if it is not open, and
   returns without waiting. */
void mnBattleMessageOpen(Sint32 message)
{
	Sint32				  entryIndex;
	MESSAGE				 *continued;
	WINDOW				 *previousWindow;
	WINDOW				 *messageWindow;
	WIN_STATE			 *state;
	BATTLE_MESSAGE_STATE *control;

	state = WINDOW_STATE;
	control = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_MESSAGE);
	continued = NULL;
	state->battleMode = WINDOW_BATTLE_MESSAGE;
	entryIndex = mnMessageDecode(message, 1);
	state->battleMode = WINDOW_BATTLE;

	if(state->textBuf[entryIndex] != 0) {
		previousWindow = control->win;
		/* A borderless canvas whose interior is rows 16 to 19, the bottom four,
		   and the full 30 columns: all 120 canvas tiles. */
		if(previousWindow == NULL) {
			messageWindow = mnWindowCreate(
				0, 15, 30, 6, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
			control->win = messageWindow;
			mnWindowMapTextCanvasTiles(0, 15, 30, 6, 1);
			control->marked = 0;
		} else {
			messageWindow = previousWindow;
		}
		if(messageWindow != NULL) {
			continued =
				mnBattleMessageCont(messageWindow, entryIndex, control->marked);
			control->ofs = (BATTLE_MESSAGE_OFFSET *)continued;
			control->marked = 0;
			/* With no message free the window is closed, though control->win
			   still names its slot. */
			if(continued == NULL) {
				mnWindowClose(messageWindow, 1);
			}
		}
	}
}

/* Shows a message in the battle message window and waits until it has been
   typed out. */
void mnBattleMessageShowAndWait(Sint32 message)
{
	mnBattleMessageOpen(message);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	sySynch(1);
}

/* Types a message into an open window from its top left, keeping the window's
   size from page to page, with no voice blips. */
Sint32 mnMessageOpenDecoded(WINDOW *window, Sint32 message)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   entry;
	Sint32	   typing = 0;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Uint16 frameReserve[4];

	state->voiceSnd = 0;
	state->voiceCountdown = 0;
	entry = mnMessageDecode(message, 1);
	if(state->textBuf[entry] == 0)
		return 0;
	if(window == NULL)
		return 0;
	typing = (Sint32)mnMessageStart(window, entry, 0, 0, 0, 1);
	if(typing == 0)
		return 0;
	return typing;
}

/* Sets the decode flags the next message is decoded with (mnMessageDecode):
   MESSAGE_DECODE_HALTING_SPEECH makes it halting speech, a short pause before
   each character, broken off with an ellipsis after the first word;
   MESSAGE_DECODE_SPACING_AFTER puts a wide gap (ENTRY_WIDE_GAP) after certain
   characters in English and Japanese. */
void mnMessageSetDecodeFlags(Sint32 flags)
{
	WIN_STATE *state;

	state = WINDOW_STATE;
	if(state != NULL) {
		if(flags & MESSAGE_DECODE_HALTING_SPEECH) {
			state->haltingSpeech = 1;
		}
		if(MESSAGE_DECODE_SPACING_AFTER & flags) {
			state->spacingAfter = 1;
		}
	}
}

/* Opens a window sized to a message near tile (x, y) and starts typing the
   message into it. packed holds the window options (MESSAGE_WINDOW_ZOOM and the
   rest) in its low bits and the speaker's voice sound in bits 16 to 27
   (MESSAGE_WINDOW_VOICE). */
WINDOW *mnMessageOpenWindow(Sint32 message, Sint32 x, Sint32 y, Uint32 packed)
{
	Sint32	   entry;
	Uint32	   height;
	Uint32	   width;
	Uint16	   bounds[4];
	Sint32	   flags;
	Uint16	  *out;
	WINDOW	  *window;
	WIN_STATE *state;

	state = WINDOW_STATE;
	state->voiceSnd = MESSAGE_WINDOW_VOICE_OF(packed);
	/* Default: no window. */
	window = NULL;
	state->voiceCountdown = 0;
	packed &= MESSAGE_WINDOW_OPTIONS_MASK;
	flags = 0;
	entry = mnMessageDecode(message, 1);
	if(state->textBuf[entry] == 0)
		return NULL;
	/* bounds receives each line's extra gap between characters, which the
	   message spreads the line with. */
	mnWindowFitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
	if(width == 0 && height == 0)
		return NULL;
	if(!(packed & MESSAGE_WINDOW_ZOOM))
		flags |= WINDOW_FLAG_INSTANT;
	if(packed & MESSAGE_WINDOW_TEXT_CANVAS)
		flags |= WINDOW_FLAG_TEXT_CANVAS;
	if(packed & MESSAGE_WINDOW_ANCHOR_BOTTOM)
		flags |= WINDOW_FLAG_ANCHOR_BOTTOM;
	if(packed & MESSAGE_WINDOW_FIXED_X)
		flags |= WINDOW_FLAG_FIXED_X;
	window = mnWindowCreate(x, y, width, height, flags);
	if(window == NULL)
		return NULL;
	if(mnMessageStart(window, entry, 0, 0, out, 0) == 0) {
		mnWindowClose(window, 1);
		return NULL;
	}
	state->haltingSpeech = 0;
	state->spacingAfter = 0;
	return window;
}

/* Shows a message in a window centred on the screen, waits until it is typed
   out and closes it, as its FIELD_MESSAGE_FLAG flags say. */
void mnFieldMessageShow(Sint32 message, Sint32 flags)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   zoom;
	Sint32	   actorPosition[2] = {0, 0};
	Sint32	   height;
	Sint32	   width;
	Sint32	   y;
	Sint32	   x;
	WINDOW	  *window = NULL;

	y = x = 0;
	zoom = flags & FIELD_MESSAGE_ZOOM;

	if(flags & FIELD_MESSAGE_WAITING_FOR_SOUND) {
		state->waitingForSnd = 1;
	}

	mnTextGetMessageSize(message, &x, &y, (Uint32 *)&width, (Uint32 *)&height);

	/* Centre the window across the screen and in its top rows, then lower it by
	   4 or 12 rows when asked. */
	x = (DISPLAY_TILE_WIDTH - width) >> 1;
	y = (FIELD_MESSAGE_TOP_ROWS - height) >> 1;

	if(flags & FIELD_MESSAGE_LOWER) {
		y += 4;
	} else if(flags & FIELD_MESSAGE_LOWEST) {
		y += 12;
	} else {
		Sint32 actorTileY;

		/* Otherwise the window keeps clear of the leader: above them when they
		   stand in the screen's lower half, below them when in its upper
		   half. */
		fdActorGetScreenPosStub(
			MainRoutineStateWork.leaderActorId, actorPosition);
		actorTileY = actorPosition[1] >> 3;
		if(actorTileY > DISPLAY_TILE_HEIGHT / 2 - 1) {
			y = actorTileY - 5;
		} else {
			y = actorTileY + 4;
		}
	}

	window = mnMessageOpenWindow(message, x, y, zoom);

	if(window != NULL) {
		while(mnMessageAllTyped() == 0) {
			sySynch(1);
		}

		if(flags & FIELD_MESSAGE_HOLD_UPLOAD) {
			WINDOW_STATE->holdTilemapUpload = 1;
		}

		if(!(flags & FIELD_MESSAGE_KEEP_OPEN)) {
			mnWindowClose(window, zoom);
			while(mnWindowIsClosed(window) == 0) {
				sySynch(1);
			}
		}
	}

	state->waitingForSnd = 0;
	state->voiceSnd = 0;
	state->voiceCountdown = 0;
	sySynch(3);
}

/* The window system's per-frame event: windows, then messages, then sprite
   animation. */
void mnWindowSystemUpdate(void)
{
	mnWindowUpdateZooms();
	mnMessageUpdateAll();
	mnWindowSprUpdateAll();
}
