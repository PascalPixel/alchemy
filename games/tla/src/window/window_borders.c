/* Window zoom and borders, and opening and waiting on message windows. Part of
   window.c. */

void mnWindowDrawZoomBorder(WINDOW *win, Sint32 savePos);
void mnWindowDrawOpened(WINDOW *win);

/* A tilemap entry's tile number and flip bits, below its palette. */
#define TILE_NUMBER_AND_FLIPS (TILE_NUMBER_MASK | TILE_HFLIP | TILE_VFLIP)

/* Steps each window's open or close animation: an open window moves toward its
   geometry and is drawn when it arrives; a closing one is erased tick by tick
   and cleared when its duration is reached. */
void mnWindowUpdateZooms(void)
{
	WIN_STATE *state = WINDOW_STATE;
	WINDOW	  *win = state->wins;
	Sint32	   idx = 0;

	do {
		if(win->flg != 0) {
			if(win->zoomStage != 0) {
				mnWindowDrawZoomBorder(win, 0);
				win->zoomStage--;
			} else if(win->duration != 0) {
				mnWindowDrawOpened(win);
			}
		} else if(win->duration != 0) {
			if(win->zoomStage != win->duration) {
				mnWindowEraseBorderRect(
					win->prevX, win->prevY, win->prevWidth, win->prevHeight);
				mnWindowDrawZoomBorder(win, 1);
				win->zoomStage++;
				state->dirty = 1;
			} else {
				mnWindowEraseBorderRect(
					win->prevX, win->prevY, win->prevWidth, win->prevHeight);
				win->sprs.head = NULL;
				win->sprs.tailLink = NULL;
				win->width = 0;
				win->height = 0;
				win->x = 0;
				win->y = 0;
				win->opened = 0;
				win->anm = 0;
				win->state = 0;
				win->flg = 0;
				win->zoomStage = 0;
				win->duration = 0;
				win->prevX = 0;
				win->prevY = 0;
				win->prevWidth = 0;
				win->prevHeight = 0;
				state->dirty = 1;
			}
		}
		idx++;
		win++;
	} while(idx != WINDOW_COUNT);
}

/* Scratch values for a stage of a window's zoom, as in The Broken Seal: the
   scaled part of a side, the scaled duration and their ratio. */
typedef struct WINDOW_ZOOM_SCRATCH {
	FIXED scaledPart; /* zoom stage (or stages left) times a side in tiles */
	FIXED scaledDuration; /* twice the zoom's duration */
	FIXED result;		  /* scaledPart / scaledDuration, in tiles */
} WINDOW_ZOOM_SCRATCH;

/* Draws a zooming window at its size for this tick: the window grows from its
   centre as zoomStage counts down to 0. With savePos set (when closing)
   the drawn rectangle is remembered, for the next tick to erase. */
void mnWindowDrawZoomBorder(WINDOW *win, Sint32 savePos)
{
	WINDOW_ZOOM_SCRATCH scratch;
	Sint32				stage;
	Sint32				duration;
	Sint32				remaining;
	Sint32				x;
	Sint32				y;
	Sint32				width;
	Sint32				height;

	stage = win->zoomStage;
	duration = win->duration;
	remaining = duration - stage;
	/* syDivFX(d, v) is v / d in 16.16 fixed point, as in The Broken Seal: the
	   left edge moves in by stage / duration of half the width, and the width
	   is (duration - stage) / duration of the whole, twice that half; the same
	   for the top and the height. */
	scratch.scaledPart = toFIXED(stage * win->width);
	scratch.scaledDuration = toFIXED(2 * duration);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	x = FIXEDtoINT(scratch.result) + win->x;

	scratch.scaledPart = toFIXED(remaining * win->width);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	{
		Sint32 part = stage * win->height;
		width = FIXEDtoINT_TWICE(scratch.result);
		scratch.scaledPart = toFIXED(part);
	}
	scratch.scaledDuration = toFIXED(2 * win->duration);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	y = FIXEDtoINT(scratch.result) + win->y;

	scratch.scaledPart = toFIXED(remaining * win->height);
	scratch.result = syDivFX(scratch.scaledDuration, scratch.scaledPart);
	height = FIXEDtoINT_TWICE(scratch.result);

	mnWindowDrawBorder(x, y, width, height);
	if(savePos != 0) {
		win->prevX = x;
		win->prevY = y;
		win->prevWidth = width;
		win->prevHeight = height;
	}
}

/* Fills count halfwords at dst with value and returns the position after the
   fill. */
Uint16 *mnMemoryFillHalfwordsDMA(Uint16 *dst, Sint32 value, Sint32 cnt)
{
	if(cnt > 0) {
		DMA_FILL16(value, dst, cnt * 2);
		dst += cnt;
	}
	return dst;
}

/* Draws an empty window: a border around a filled interior, at tile (x, y) of
   width by height tiles, and frees the text tiles the rectangle showed. */
void mnWindowDrawBorder(Sint32 x, Sint32 y, Uint32 width, Uint32 height)
{
	WIN_STATE *canvas = WINDOW_STATE;
	Uint16	  *cursor = &canvas->tilemap[y][x];
	Uint32	   row;

	if(width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
		height > DISPLAY_TILE_WIDTH)
		return;
	mnWindowClearTextTilesInRect(x, y, width, height);
	if(canvas->mindRead != 0)
		*cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER;
	else
		*cursor++ = WINDOW_TILE_BORDER_TOP_LEFT;
	cursor = mnMemoryFillHalfwordsDMA(
		cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_TOP), width - 2);
	if(canvas->mindRead != 0)
		*cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP;
	else
		*cursor++ = WINDOW_TILE_BORDER_TOP_RIGHT;
	cursor += BG_SCREEN_WIDTH - width;
	for(row = 1; row < height - 1; row++) {
		*cursor++ = WINDOW_TILE_BORDER_LEFT;
		if(width != 2)
			cursor = mnMemoryFillHalfwordsDMA(
				cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
		*cursor++ = WINDOW_TILE_BORDER_RIGHT;
		cursor += BG_SCREEN_WIDTH - width;
	}
	if(canvas->mindRead != 0)
		*cursor++ = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_VFLIP;
	else
		*cursor++ = WINDOW_TILE_BORDER_BOTTOM_LEFT;
	cursor = mnMemoryFillHalfwordsDMA(
		cursor, WINDOW_TILE_PAIR(WINDOW_TILE_BORDER_BOTTOM), width - 2);
	if(canvas->mindRead != 0)
		*cursor = WINDOW_TILE_BORDER_ROUND_CORNER | TILE_HFLIP | TILE_VFLIP;
	else
		*cursor = WINDOW_TILE_BORDER_BOTTOM_RIGHT;
	canvas->dirty = 1;
}

/* mnWindowDrawBorder in the flash border's tiles (8 to 15), which
   mnWindowShowFlashFrame loads: the tick a window flashes with as it
   swells. */
void mnWindowDrawFlashBorder(Sint32 x, Sint32 y, Uint32 width, Uint32 height)
{
	WIN_STATE *canvas = WINDOW_STATE;
	Uint16	  *cursor = &canvas->tilemap[y][x];
	Uint32	   row;

	if(width <= 1 || height <= 1 || width > DISPLAY_TILE_WIDTH ||
		height > DISPLAY_TILE_WIDTH)
		return;
	mnWindowClearTextTilesInRect(x, y, width, height);
	*cursor++ = WINDOW_TILE_FLASH_TOP_LEFT;
	cursor = mnMemoryFillHalfwordsDMA(
		cursor, WINDOW_TILE_PAIR(WINDOW_TILE_FLASH_TOP), width - 2);
	*cursor++ = WINDOW_TILE_FLASH_TOP_RIGHT;
	cursor += BG_SCREEN_WIDTH - width;
	for(row = 1; row < height - 1; row++) {
		*cursor++ = WINDOW_TILE_FLASH_LEFT;
		if(width != 2)
			cursor = mnMemoryFillHalfwordsDMA(
				cursor, WINDOW_TILE_PAIR(WINDOW_TILE_WINDOW_FILL), width - 2);
		*cursor++ = WINDOW_TILE_FLASH_RIGHT;
		cursor += BG_SCREEN_WIDTH - width;
	}
	*cursor++ = WINDOW_TILE_FLASH_BOTTOM_LEFT;
	cursor = mnMemoryFillHalfwordsDMA(
		cursor, WINDOW_TILE_PAIR(WINDOW_TILE_FLASH_BOTTOM), width - 2);
	*cursor = WINDOW_TILE_FLASH_BOTTOM_RIGHT;
	canvas->dirty = 1;
}

/* Maps the text canvas tiles into the interior of a window at tile (x, y). With
   fullWidth set the canvas covers the side border columns too. */
void mnWindowMapTextCanvasTiles(
	Sint32 x, Sint32 y, Uint32 width, Uint32 height, Sint32 fullWidth)
{
	WIN_STATE *canvas = WINDOW_STATE;
	Uint16	  *cursor = &canvas->tilemap[y][x];
	Uint32	   row;
	Uint32	   column;

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
			for(column = 1; column < width - 1; column++)
				*cursor++ = WINDOW_TILE((TEXT_CANVAS_FIRST_TILE - 1 + row +
											(column - 1) * (height - 2)) &
					TILE_NUMBER_AND_FLIPS);
			cursor++;
			cursor += BG_SCREEN_WIDTH - width;
		}
	} else {
		for(row = 1; row < height - 1; row++) {
			for(column = 0; column < width; column++)
				*cursor++ = WINDOW_TILE(
					(TEXT_CANVAS_FIRST_TILE - 1 + row + column * (height - 2)) &
					TILE_NUMBER_AND_FLIPS);
			cursor += BG_SCREEN_WIDTH - width;
		}
	}
	canvas->dirty = 1;
}

/* Returns 1 once every message's window has its text typed out, or 0 while one
   is still typing (its state 0). */
Bool mnMessageAllTyped(void)
{
	MESSAGE *msg;
	Sint32	 i;

	msg = WINDOW_STATE->msgs;
	for(i = 0; i != MESSAGE_COUNT; i++, msg++) {
		if(msg->win != NULL && msg->win->state == 0)
			return FALSE;
	}
	return TRUE;
}

/* Returns 1 when a window is fully closed (its slot free), and 0 while it is
   open or closing. */
Bool mnWindowIsClosed(WINDOW *win)
{
	if(win == NULL)
		return TRUE;
	if(win->flg == 0 && win->duration == 0)
		return TRUE;
	return FALSE;
}

/* Resets the text style glyphs are drawn with: white, plain, and the default
   line and glyph spacing. */
void mnTextResetStyle(void)
{
	WIN_STATE *state = WINDOW_STATE;

	state->colour = TEXT_COLOR_WHITE;
	state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	state->letterSpacing = TEXT_DEFAULT_GLYPH_SPACING;
	state->outline = TEXT_STYLE_PLAIN;
	state->unusedAfterLineSpacing = 1;
}

void mnWindowSystemUpdate(void);
void mnWindowSystemUpdateBattle(void);

/* Reserves the glyph sprites' tiles, resets the text style, starts the ring of
   decoded text from its beginning and starts the per-tick window update
   (mnWindowSystemUpdate). */
void mnWindowSystemStartFieldUpdate(void)
{
	WIN_STATE *state = WINDOW_STATE;

	state->glyphTiles =
		syVRAMCacheLoad(VRAM_CACHE_ENTRY_GLYPH_TILES, GLYPH_TILES_SIZE, 0);
	state->letterSpacing = TEXT_DEFAULT_GLYPH_SPACING;
	state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	state->outline = TEXT_STYLE_PLAIN;
	state->colour = TEXT_COLOR_WHITE;
	state->textBufEnd = 0;
	sySetEvent(mnWindowSystemUpdate, EVENT_PASS_TICK);
}

/* Resets the text style and starts the per-tick window update: the field's
   (mnWindowSystemUpdate) when loadGlyphTiles is set, which first reserves the
   glyph sprites' tiles, else the battle's (mnWindowSystemUpdateBattle). */
void mnWindowSystemStartBattleUpdate(Sint32 loadGlyphTiles)
{
	WIN_STATE *state = WINDOW_STATE;

	if(loadGlyphTiles != 0)
		state->glyphTiles =
			syVRAMCacheLoad(VRAM_CACHE_ENTRY_GLYPH_TILES, GLYPH_TILES_SIZE, 0);
	state->letterSpacing = TEXT_DEFAULT_GLYPH_SPACING;
	state->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	state->outline = TEXT_STYLE_PLAIN;
	state->colour = TEXT_COLOR_WHITE;
	state->textBufEnd = 0;
	if(loadGlyphTiles != 0)
		sySetEvent(mnWindowSystemUpdate, EVENT_PASS_TICK);
	else
		sySetEvent(mnWindowSystemUpdateBattle, EVENT_PASS_TICK);
}

/* Closes the battle message window at once, if one is open, and forgets it, so
   the next battle message (mnBattleMessageOpen) opens a new one. */
void mnBattleMessageClose(void)
{
	BATTLE_MESSAGE_STATE *slot = Ram_HeapSlots->battleMsg;

	if(slot->win != NULL) {
		mnWindowClose(slot->win, 1);
		slot->win = NULL;
	}
}

Sint32 mnMessageDecode(Sint32 msg, Sint32 clear);

/* Decodes message and starts it in the message window along the bottom of the
   screen (tiles 0, 15, 30 by 6), creating the window first if it is not open; a
   message that decodes to nothing shows nothing. */
void mnBattleMessageOpen(Sint32 msg)
{
	Sint32				  entry;
	BATTLE_MESSAGE_STATE *slot;
	WINDOW				 *prev;
	WINDOW				 *win;
	WIN_STATE			 *state;
	MESSAGE				 *typing;

	state = WINDOW_STATE;
	slot = Ram_HeapSlots->battleMsg;
	typing = NULL;
	/* Decoded as a battle message: in Japanese its names are set off and its
	   spaces doubled. */
	state->battleMode = WINDOW_BATTLE_MESSAGE;
	entry = mnMessageDecode(msg, 1);
	state->battleMode = WINDOW_BATTLE;
	if(state->textBuf[entry] != 0) {
		prev = slot->win;
		if(prev == NULL) {
			win = mnWindowCreate(
				0, 15, 30, 6, WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS);
			slot->win = win;
			mnWindowMapTextCanvasTiles(0, 15, 30, 6, 1);
			slot->marked = 0;
		} else {
			win = prev;
		}
		if(win != NULL) {
			typing = mnBattleMessageCont(win, entry, slot->marked);
			slot->msg = typing;
			slot->marked = 0;
			if(typing == NULL)
				mnWindowClose(win, 1);
		}
	}
}

/* Shows a message in the battle message window and waits until it has been
   typed out. */
void mnBattleMessageShowAndWait(Sint32 msg)
{
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	mnBattleMessageOpen(msg);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
}

/* Decodes message and starts it typing in window, which is already open;
   returns 0 when the message is empty or there is no window. */
Sint32 mnMessageOpenDecoded(Sint32 win, Sint32 msg)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   entry;
	Sint32	   result = 0;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Uint16 frmReserve[4];

	state->speakerVoiceSnd = 0;
	state->speakerVoiceCountdown = 0;
	entry = mnMessageDecode(msg, 1);
	if(state->textBuf[entry] == 0)
		return 0;
	if(win == 0)
		return 0;
	result = (Sint32)mnMessageStart((WINDOW *)win, entry, 0, 0, 0, 1);
	if(result == 0)
		return 0;
	return result;
}

/* Sets the decode flags the next message is decoded with (mnMessageDecode):
   MESSAGE_DECODE_HALTING_SPEECH makes it halting speech, a short pause before
   each character, broken off with an ellipsis after the first word;
   MESSAGE_DECODE_SPACING_AFTER puts a wide gap (ENTRY_WIDE_GAP) after certain
   characters in English and Japanese. Flags already set stay set: only
   mnMessageOpenWindow clears both, once it has started a message typing, so
   they also apply to every message decoded before that. */
void mnMessageSetDecodeFlags(Uint32 flg)
{
	WIN_STATE *state;

	state = WINDOW_STATE;
	if(state == NULL)
		return;
	if(flg & MESSAGE_DECODE_HALTING_SPEECH)
		state->haltingSpeech = 1;
	if(flg & MESSAGE_DECODE_SPACING_AFTER)
		state->spacingAfter = 1;
}

/* Opens a window sized to a message near tile (x, y) and starts typing the
   message into it. packed holds the window options in its low bits
   (MESSAGE_WINDOW_OPTION) and the speaker's voice sound in bits 16 to 27
   (MESSAGE_WINDOW_VOICE). */
WINDOW *mnMessageOpenWindow(Sint32 msg, Sint32 x, Sint32 y, Uint32 packed)
{
	Sint32	   entry;
	Uint32	   height;
	Uint32	   width;
	Uint16	   bounds[4];
	Sint32	   flg;
	Uint16	  *out;
	WINDOW	  *win;
	WIN_STATE *state;

	state = WINDOW_STATE;
	state->speakerVoiceSnd = MESSAGE_WINDOW_VOICE_OF(packed);
	win = NULL;
	state->speakerVoiceCountdown = 0;
	packed &= MESSAGE_WINDOW_OPTIONS_MASK;
	flg = 0;
	entry = mnMessageDecode(msg, 1);
	if(state->textBuf[entry] == 0)
		return NULL;
	mnWindowFitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
	if(width == 0 && height == 0)
		return NULL;
	if(!(packed & MESSAGE_WINDOW_ZOOM))
		flg |= WINDOW_FLAG_INSTANT;
	if(packed & MESSAGE_WINDOW_TEXT_CANVAS)
		flg |= WINDOW_FLAG_TEXT_CANVAS;
	if(packed & MESSAGE_WINDOW_ANCHOR_BOTTOM)
		flg |= WINDOW_FLAG_ANCHOR_BOTTOM;
	if(packed & MESSAGE_WINDOW_FIXED_X)
		flg |= WINDOW_FLAG_FIXED_X;
	win = mnWindowCreate(x, y, width, height, flg);
	if(win == NULL)
		return NULL;
	if(mnMessageStart(win, entry, 0, 0, out, 0) == 0) {
		mnWindowClose(win, 1);
		return NULL;
	}
	state->haltingSpeech = 0;
	state->spacingAfter = 0;
	return win;
}

/* The tile rows a field message is centred in, from the top of the screen. */
#define FIELD_MESSAGE_TOP_ROWS 12

/* Shows a message in a window centred on the screen, waits until it is typed
   out and closes it, as its flags say. */
void mnFieldMessageShow(Sint32 msg, Sint32 flg)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   zoom;
	Sint32	   actorPos[2] = {0, 0};
	Sint32	   height;
	Sint32	   width;
	Sint32	   y;
	Sint32	   x;
	WINDOW	  *window = NULL;
	Sint32	   margin;
	Sint32	   rows;

	y = x = 0;
	zoom = flg & FIELD_MESSAGE_ZOOM;

	if(flg & FIELD_MESSAGE_WAITING_FOR_SOUND) {
		state->waitingForSnd = 1;
	}

	mnTextGetMessageSize(msg, &x, &y, (Uint32 *)&width, (Uint32 *)&height);

	/* Centre the window across the screen, and in the top rows. */
	margin = DISPLAY_TILE_WIDTH - width;
	rows = height;
	x = margin >> 1;
	y = (FIELD_MESSAGE_TOP_ROWS - rows) >> 1;

	if(flg & FIELD_MESSAGE_LOWER) {
		y = y + 4;
	} else if(flg & FIELD_MESSAGE_LOWEST) {
		y = y + 12;
	} else {
		Sint32 actorTileY;

		/* The leader's screen y in pixels, as a tile row. */
		fdActorGetScreenPosStub(fdPartyGetLeaderActorStub(), actorPos);
		actorTileY = actorPos[1] >> 3;
		if(actorTileY > 9) {
			y = actorTileY - 5;
		} else {
			y = actorTileY + 4;
		}
	}

	window = mnMessageOpenWindow(msg, x, y, zoom);

	if(window != NULL) {
		while(mnMessageAllTyped() == 0) {
			sySynch(1);
		}

		if(flg & FIELD_MESSAGE_HOLD_UPLOAD) {
			WINDOW_STATE->holdTilemapUpload = 1;
		}

		if(!(flg & FIELD_MESSAGE_KEEP_OPEN)) {
			mnWindowClose(window, zoom);
			while(mnWindowIsClosed(window) == 0) {
				sySynch(1);
			}
		}
	}

	state->waitingForSnd = 0;
	state->speakerVoiceSnd = 0;
	state->speakerVoiceCountdown = 0;
	sySynch(3);
}

/* The window system's per-tick event: windows, then messages, then sprite
   animation. */
void mnWindowSystemUpdate(void)
{
	mnWindowUpdateZooms();
	mnMessageUpdateAll();
	mnWindowSprUpdateAll();
}

/* The window system's per-tick event in battle: windows, messages and sprite
   animation while the battle menu's state exists, otherwise messages alone. */
void mnWindowSystemUpdateBattle(void)
{
	if(Ram_HeapSlots->battleMenuState != 0) {
		mnWindowUpdateZooms();
		mnMessageUpdateAll();
		mnWindowSprUpdateAll();
	} else {
		mnMessageUpdateAll();
	}
}
