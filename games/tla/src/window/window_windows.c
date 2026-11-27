/* The window system's setup and tilemap upload, the pool of window sprites,
   opening and closing windows, and starting a message typing into one. Part of
   window.c. */

void mnWindowDrawOpened(WINDOW *window);
void mnWindowWaitZoom(void *window);

/* A text tile battle keeps for the bottom border's lower rows: neither taken
   nor shown, it is never free (WIN_STATE.textTileState). */
#define TEXT_TILE_RESERVED 4

/* The tilemap uploads in five bands of four tile rows, each a bit of
   WIN_STATE.dirty above bit 0, which asks for them all. */
#define TILEMAP_BANDS	  5
#define TILEMAP_BAND_SIZE (4 * BG_SCREEN_WIDTH * sizeof(Uint16))
#define TILEMAP_DIRTY_ALL ((1 << (TILEMAP_BANDS + 1)) - 1)

/* Takes the first window sprite from the free list, or returns NULL when every
   sprite is in use. */
WINDOW_SPR *mnWindowSpriteAcquireFree(void)
{
	WIN_STATE  *state = WINDOW_STATE;
	WINDOW_SPR *out = state->freeSprites.head;

	if(out != NULL) {
		if(out->next == NULL)
			state->freeSprites.tailLink = &state->freeSprites.head;
		state->freeSprites.head = out->next;
		out->next = NULL;
	}
	return out;
}

/* Returns a window sprite to the end of the free list. A sprite from outside
   the window system's pool is ignored. */
void mnWindowSpriteReleaseFree(WINDOW_SPR *out)
{
	WIN_STATE *state = WINDOW_STATE;

	if((Uint32)out >= (Uint32)state->sprites &&
		(Uint32)out < (Uint32)&state->freeSprites.head) {
		WINDOW_SPR **tail = state->freeSprites.tailLink;

		state->freeSprites.tailLink = &out->next;
		*tail = out;
		out->next = NULL;
	}
}

/* Links all 64 window sprites of the pool into the free list. */
void mnWindowSpriteInitFreeList(void)
{
	Sint32		n;
	WIN_STATE  *state;
	WINDOW_SPR *out;
	WINDOW_SPR *next;

	state = WINDOW_STATE;
	out = state->sprites;
	state->freeSprites.head = out;
	for(n = 0; n < WINDOW_SPRITE_COUNT - 1; n++) {
		next = out + 1;
		out->next = next;
		out = next;
	}
	next->next = NULL;
	state->freeSprites.tailLink = &next->next;
}

/* Sets up the window system for the field (and the other screens outside
   battle): allocates and clears its state, blanks the tilemap, frees every
   window sprite and starts the two per-tick events, the tilemap upload and the
   window update. */
void mnWindowSystemInitField(void)
{
	WIN_STATE *state;

	state = (WIN_STATE *)syHeapSlotAllocateEWRAM(
		HEAP_SLOT_WINDOW, sizeof(WIN_STATE));
	syWordsClearEntry(state, sizeof(WIN_STATE));
	state->dirty = 1;
	state->glyphResource = NO_GLYPH_RESOURCE;
	state->textPalette = WINDOW_PALETTE;
	syWordsFill(state->tilemap, sizeof(state->tilemap),
		WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
	mnWindowSpriteInitFreeList();
	mnPortraitClearSlots();
	sySetEvent((Sint32)mnWindowSystemUploadTilemap, EVENT_PASS_VBLANK);
	mnWindowSystemStartFieldUpdate();
}

/* Sets up the window system for battle, as mnWindowSystemInitField does for the
   field, starting the field's window update when loadGlyphTiles is set
   (mnWindowSystemStartBattleUpdate). */
void mnWindowSystemInitBattle(Sint32 loadGlyphTiles)
{
	WIN_STATE *state;
	Sint32	   i;
	/* Copies pixel rows 4 to 6 of tile sourceTile to rows 0 to 2 of
	   destinationTile and clears the rest of it. */
	Sint32 mnWindowSystemCopyTileLowerRows(
		Uint32 sourceTile, Uint32 destinationTile)
	{
		sourceTile &= TILE_NUMBER_MASK;
		destinationTile &= TILE_NUMBER_MASK;
		syDMAXCopy(BG_CHAR_BLOCK(0) + sourceTile * TILE_SIZE_4BPP +
				4 * TILE_ROW_SIZE_4BPP,
			BG_CHAR_BLOCK(0) + destinationTile * TILE_SIZE_4BPP,
			DMA_COPY_HALFWORDS(4 * TILE_ROW_SIZE_4BPP / 2), REG_DMA3);
		return syWordsClearEntry(BG_CHAR_BLOCK(0) +
				destinationTile * TILE_SIZE_4BPP + 3 * TILE_ROW_SIZE_4BPP,
			5 * TILE_ROW_SIZE_4BPP);
	}

	state = (WIN_STATE *)syHeapSlotAllocateEWRAM(
		HEAP_SLOT_WINDOW, sizeof(WIN_STATE));
	syWordsClearEntry(state, sizeof(WIN_STATE));
	state->dirty = 1;
	state->glyphResource = NO_GLYPH_RESOURCE;
	state->battleMode = WINDOW_BATTLE;
	state->textPalette = WINDOW_PALETTE;
	syWordsFillEntry(state->tilemap, sizeof(state->tilemap),
		WINDOW_TILE_PAIR(WINDOW_TILE_BLANK));
	mnWindowSpriteInitFreeList();
	sySetEvent((Sint32)mnWindowSystemUploadTilemap, EVENT_PASS_VBLANK);
	mnWindowSystemStartBattleUpdate(loadGlyphTiles);
	/* The first three text tiles take the lower rows of the bottom border
	   tiles, and are kept from text. */
	mnWindowSystemCopyTileLowerRows(
		WINDOW_TILE_BORDER_BOTTOM_LEFT, TEXT_TILE_FIRST);
	mnWindowSystemCopyTileLowerRows(
		WINDOW_TILE_BORDER_BOTTOM, TEXT_TILE_FIRST + 1);
	mnWindowSystemCopyTileLowerRows(
		WINDOW_TILE_BORDER_BOTTOM_RIGHT, TEXT_TILE_FIRST + 2);
	{
		Uint8 mode = TEXT_TILE_RESERVED;

		for(i = 0; i < 3; i++)
			state->textTileState[i] = mode;
	}
}

/* The tilemap upload, a VBlank-pass event: copies each band of the tilemap
   marked in the dirty bits to VRAM. */
void mnWindowSystemUploadTilemap(void)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   bands;
	Uint8	  *src;
	Uint8	  *dest;

	bands = state->dirty;
	if(bands != 0) {
		if(!state->holdTilemapUpload) {
			src = (Uint8 *)state->tilemap;
			dest = (Uint8 *)BG_SCREEN_BLOCK(4);
			/* Bit 0 asks for all five bands; after the shift bit n is band
			   n. */
			if(bands & 1)
				bands = TILEMAP_DIRTY_ALL;
			bands &= TILEMAP_DIRTY_ALL;
			bands >>= 1;
			do {
				if(bands & 1)
					syDMAXCopy(src, dest, DMA_COPY_WORDS(TILEMAP_BAND_SIZE / 4),
						REG_DMA3);
				bands >>= 1;
				src += TILEMAP_BAND_SIZE;
				dest += TILEMAP_BAND_SIZE;
			} while(bands);
			state->dirty = bands;
		}
	}
}

/* Erases a window's rectangle of the tilemap (x, y, width and height in tiles)
   and frees the text tiles it showed; in battle the rows of the command window
   go back to its panel. */
void mnWindowEraseBorderRect(Sint32 x, Sint32 y, Uint32 width, Uint32 height)
{
	WIN_STATE *canvas = WINDOW_STATE;
	Uint16	  *cursor = &canvas->tilemap[y][x];
	Sint32	   tile;
	Uint32	   bottom;
	Uint32	   row;

	tile = WINDOW_TILE_BLANK;
	bottom = y + height;
	if(bottom > DISPLAY_TILE_HEIGHT)
		height = DISPLAY_TILE_HEIGHT - y;
	if(width <= 1)
		width = 2;
	if(width > DISPLAY_TILE_WIDTH)
		width = DISPLAY_TILE_WIDTH;
	if(height <= 1)
		height = 2;
	if(height > 30)
		height = 30;

	mnWindowClearTextTilesInRect(x, y, width, height);

	row = 0;
	if(row < height) {
		do {
			Uint32 column;

			if(canvas->battleMode != 0) {
				if((Uint32)(y + row) >= COMMAND_ROW_TOP)
					tile = WINDOW_TILE_COMMAND_PANEL;
				else
					tile = WINDOW_TILE_BLANK;
			}
			column = 0;
			if(column < width) {
				for(; column < width; column++) {
					*cursor++ = tile;
				}
			}
			row++;
			cursor += BG_SCREEN_WIDTH - width;
		} while(row < height);
	}
	canvas->dirty = 1;
}

/* Draws a window at its full size: its border, and for a text canvas window the
   canvas, filled with colour 4 under a border when the window is bordered and
   transparent otherwise. */
void mnWindowDrawOpened(WINDOW *window)
{
	Uint32	   flags;
	Sint32	   x;
	Sint32	   y;
	Uint32	   width;
	Uint32	   height;
	WIN_STATE *state;

	state = WINDOW_STATE;
	x = window->x;
	y = window->y;
	width = window->width;
	height = window->height;
	flags = window->flags;
	window->duration = 0;
	if(WINDOW_FLAG_TEXT_CANVAS & flags) {
		if(WINDOW_FLAG_BORDERED_CANVAS & flags) {
			mnWindowDrawBorder(x, y, width, height);
			syWordsFill(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE,
				TEXT_CANVAS_BACKGROUND_WORD);
		} else {
			syWordsFill(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
		}
		mnWindowMapTextCanvasTiles(x, y, width, height, 0);
	} else {
		mnWindowDrawBorder(x, y, width, height);
	}
	state->dirty = 1;
}

/* Opens a window at tile (x, y) of width by height tiles in the first free
   window slot, and returns it, or NULL when all slots are in use. attributes
   takes the WINDOW_FLAG_ flags. */
WINDOW *mnWindowCreate(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 attributes)
{
	WINDOW *slot;
	WINDOW *found;
	Sint32	i;

	slot = WINDOW_STATE->windows;
	found = 0;
	for(i = 0; i != WINDOW_COUNT; i++, slot++) {
		if((slot->flags & WINDOW_FLAG_OPEN) == 0 && slot->duration == 0) {
			found = slot;
			break;
		}
	}
	if(found != 0) {
		found->x = x;
		found->y = y;
		found->width = width;
		found->height = height;
		found->sprites.head = NULL;
		found->state = 0;
		found->sprites.tailLink = &slot->sprites.head;
		found->opened = 1;
		found->flags = WINDOW_FLAG_OPEN;
		mnTextResetStyle();
		if(attributes & WINDOW_FLAG_TEXT_CANVAS) {
			found->flags |= WINDOW_FLAG_TEXT_CANVAS;
		}
		if(attributes & WINDOW_FLAG_BORDERED_CANVAS) {
			found->flags |= WINDOW_FLAG_BORDERED_CANVAS;
		}
		if(attributes & WINDOW_FLAG_OUTLINED_TEXT) {
			found->flags |= WINDOW_FLAG_OUTLINED_TEXT;
		}
		if(attributes & WINDOW_FLAG_ANCHOR_BOTTOM) {
			found->flags |= WINDOW_FLAG_ANCHOR_BOTTOM;
		}
		if(attributes & WINDOW_FLAG_FIXED_X) {
			found->flags |= WINDOW_FLAG_FIXED_X;
		}
		if(attributes & WINDOW_FLAG_INSTANT) {
			found->flags |= WINDOW_FLAG_INSTANT;
			found->zoomStage = 0;
			found->duration = 1;
			mnWindowDrawOpened(found);
		} else {
			found->duration = WINDOW_OPEN_TICKS;
			found->zoomStage = WINDOW_OPEN_TICKS - 1;
			mnWindowWaitZoom(found);
			sySynch(1);
		}
	}
	return found;
}

/* Waits until a zooming window has finished opening or closing. */
void mnWindowWaitZoom(void *window)
{
	if(!(WINDOW_FLAG_INSTANT & ((WINDOW *)window)->flags) &&
		(((WINDOW *)window)->duration != 0)) {
		do {
			sySynch(1);
		} while(((WINDOW *)window)->duration != 0);
	}
}

/* Closes a window, its sprites and contents going at once: with instant set
   (callers pass WINDOW_FLAG_INSTANT or 1) its rectangle is erased and its slot
   freed now, else its tick shrinks away over WINDOW_CLOSE_TICKS. */
void mnWindowClose(WINDOW *window, Sint32 instant)
{
	if(window == 0)
		return;

	mnWindowSpritePrepareForRedraw(window);
	window->flags = 0;
	window->prevX = window->x;
	window->prevY = window->y;
	window->prevWidth = window->width;
	window->prevHeight = window->height;

	if(instant != 0) {
		mnWindowEraseBorderRect(
			window->x, window->y, window->width, window->height);
		window->sprites.head = NULL;
		window->sprites.tailLink = NULL;
		window->width = 0;
		window->height = 0;
		window->x = 0;
		window->y = 0;
		window->opened = 0;
		window->animation = 0;
		window->state = 0;
		window->flags = 0;
		window->zoomStage = 0;
		window->duration = 0;
		window->prevX = 0;
		window->prevY = 0;
		window->prevWidth = 0;
		window->prevHeight = 0;
	} else {
		window->zoomStage = instant;
		window->duration = WINDOW_CLOSE_TICKS;
	}
}

/* Clears a window's contents: redraws it empty and releases the window sprites
   it carries. A text canvas window is left alone. */
void mnWindowSpritePrepareForRedraw(WINDOW *window)
{
	if(!(WINDOW_FLAG_TEXT_CANVAS & window->flags)) {
		mnWindowSpriteRedrawSavedRect(window);
		mnWindowSpriteClearList(window);
	}
}

/* Redraws a window empty, its border and fill, at its current rectangle. */
void mnWindowSpriteRedrawSavedRect(WINDOW *window)
{
	mnWindowDrawBorder(window->x, window->y, window->width, window->height);
}

/* Releases every window sprite in a list and empties it. sprites is the list,
   or a window (whose sprite list comes first); NULL is ignored. */
void mnWindowSpriteClearList(void *sprites)
{
	WINDOW_SPR_LIST *list = sprites;
	WINDOW_SPR		*node;
	WINDOW_SPR		*next;

	next = NULL;
	if(list != NULL) {
		node = list->head;
		/* Detach the chain before returning its nodes to the shared free
		   pool. */
		list->tailLink = &list->head;
		list->head = next;
		while(node != NULL) {
			next = node->next;
			mnWindowSpriteRelease(node);
			node = next;
		}
	}
}

/* Clears an area of a window's interior to the window fill. left, top, right
   and bottom are pixels inside the window, and the area covers every tile they
   touch. */
void mnWindowClearInteriorTiles(
	const WINDOW *window, Uint32 left, Uint32 top, Uint32 right, Uint32 bottom)
{
	WIN_STATE *state;
	Uint32	   x;
	Uint32	   y;
	Uint32	   width;
	Uint32	   height;
	Uint16	  *tiles;
	Uint32	   row;
	Sint32	   windowX;
	Sint32	   windowY;

	state = WINDOW_STATE;
	x = left >> 3;
	windowX = window->x;
	y = top >> 3;
	windowY = window->y;
	right += 7;
	bottom += 7;
	right >>= 3;
	bottom >>= 3;
	y += windowY;
	x += windowX;
	right += windowX;
	bottom += windowY;
	left = x + 1;
	top = y + 1;
	width = right - x;
	height = bottom - y;
	/* UB: mnWindowClearTextTilesInRect returns nothing; this call goes through
	   a function type that returns Sint32. */
#if FIX_FUNCTION_SIGNATURES
	mnWindowClearTextTilesInRect(left, top, width, height);
#else
	((Sint32 (*)(Sint32, Sint32, Uint32, Uint32))mnWindowClearTextTilesInRect)(
		left, top, width, height);
#endif
	tiles = state->tilemap[0] + top * BG_SCREEN_WIDTH + left;
	for(row = 0; row < height; row++) {
		for(left = 0; left < width; left++)
			*tiles++ = WINDOW_TILE_WINDOW_FILL;
		tiles += BG_SCREEN_WIDTH - width;
	}
	state->dirty = 1;
}

/* Points a list's tail link back at its last sprite, after the list has been
   edited by hand. */
void mnWindowSpriteUpdateListTail(WINDOW_SPR_LIST *list)
{
	WINDOW_SPR	*node = list->head;
	WINDOW_SPR **tail = &list->head;

	while(node != NULL) {
		tail = &node->next;
		node = node->next;
	}
	list->tailLink = tail;
}

/* Adds a window sprite to the end of a list; NULL lists are ignored. */
void mnWindowSpriteAppendToList(WINDOW_SPR_LIST *list, WINDOW_SPR *node)
{
	if(list != NULL) {
		*list->tailLink = node;
		/* UB: the new tail is stored as node (next is its first member) through
		   a WINDOW_SPR * view of tailLink. */
#if FIX_TYPE_PUNNING
		list->tailLink = &node->next;
#else
		*(WINDOW_SPR **)&list->tailLink = node;
#endif
	}
}

/* Frees a window sprite: returns it to the free list, releases its VRAM cache
   entry unless it is a glyph, and for a portrait frees its palette slot too. */
void mnWindowSpriteRelease(WINDOW_SPR *entry)
{
	mnWindowSpriteReleaseFree(entry);
	if((Uint8)entry->kind != WINDOW_SPRITE_PLAIN) {
		syVRAMCacheFreeEntry((Uint8)entry->index);
		if((Uint8)entry->kind == WINDOW_SPRITE_PORTRAIT) {
			WINDOW_STATE->portraitPictures[entry->sprite.oam.palette -
				PORTRAIT_PALETTE] = PORTRAIT_SLOT_EMPTY;
		}
	}
	entry->motion = 0;
}

/* Starts a free message typing the decoded text that begins at entry into
   window, with the pen at pixel (x, y). spacing gives each of four lines an
   extra gap between characters, in 1/256 pixels (or none for NULL), and is
   copied, so the caller's array need not outlive the call; flags is kept with
   the message (nonzero keeps the window's size from page to page). */
MESSAGE *mnMessageStart(WINDOW *window, Sint32 entry, Sint32 x, Sint32 y,
	Uint16 *spacing, Sint32 flags)
{
	WIN_STATE *state = WINDOW_STATE;
	MESSAGE	  *render = state->msgs;
	MESSAGE	  *found = NULL;
	Uint32	   i;

	for(i = 0; i != MESSAGE_COUNT; i++, render++) {
		if(render->window == NULL) {
			found = render;
			break;
		}
	}
	if(found != NULL) {
		found->window = window;
		found->startX = x << 8;
		found->x = x << 8;
		found->y = y << 8;
		found->textIndex = entry;
		found->colour = TEXT_COLOR_WHITE;
		found->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
		found->countdown = 0;
		found->outline = TEXT_STYLE_PLAIN;
		found->ended = 0;
		found->compressLines = 0;
		found->flags = flags;
		if(state->textMode != 0 && spacing != NULL)
			found->compressLines = spacing[3];
		if(spacing != NULL) {
			for(i = 0; i < MESSAGE_LINE_COUNT; i++)
				found->spacing[i] = *spacing++;
		} else {
			for(i = 0; i < MESSAGE_LINE_COUNT; i++)
				found->spacing[i] = 0;
		}
		found->line = 0;
	}
	return found;
}

void mnTextStartCanvasScroll(MESSAGE *msg);

/* Starts a message typing the decoded text at entry into the battle message
   window. A message that already serves a typed-out window continues below its
   last line (scrolling up once both lines are used) unless keepPos is set.
   Returns the message, or NULL when all three are busy. */
MESSAGE *mnBattleMessageContinue(WINDOW *window, Sint32 entry, Sint32 keepPos)
{
	MESSAGE *slot;
	MESSAGE *selected;
	Uint16	*spacing;
	Uint16	 lineSpacing;
	Uint32	 index;

	slot = WINDOW_STATE->msgs;
	selected = 0;
	for(index = 0; index != MESSAGE_COUNT; slot++, index++) {
		if(slot->window == 0 || slot->window->state != 0) {
			selected = slot;
			break;
		}
	}

	if(selected != 0) {
		/* Positions are in 1/256 pixel: the first line is 10 pixels down, the
		   second 13 pixels below it, and after the second the text scrolls
		   up. */
		if(selected->window == 0) {
			selected->y = 10 << 8;
			selected->window = window;
			selected->x = 3 << 8;
		} else if(keepPos == 0) {
			if(selected->y == 0) {
				selected->y = 10 << 8;
			} else if(selected->y < 13 << 8) {
				selected->y += 13 << 8;
			} else {
				mnTextStartCanvasScroll(selected);
			}
			selected->x = 3 << 8;
		}

		selected->startX = 3 << 8;
		selected->window->state = 0;
		/* Start the next text run without retaining the previous run's
		   style. */
		selected->colour = TEXT_COLOR_WHITE;
		selected->textIndex = entry;
		lineSpacing = TEXT_DEFAULT_LINE_SPACING;
		selected->countdown = 0;
		selected->outline = 0;
		selected->line = 0;
		selected->ended = 0;
		selected->lineSpacing = lineSpacing;
		index = 0;
		spacing = selected->spacing;
		for(; index < MESSAGE_LINE_COUNT; index++) {
			*spacing++ = 0;
		}
	}
	return selected;
}

/* Clears the text canvas to transparent. */
Sint32 mnTextClearCanvas(void)
{
	return syWordsFill(TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, 0);
}

/* Fills the text canvas with the window background colour (4). */
Sint32 mnTextFillCanvasBackground(void)
{
	return syWordsFill(
		TEXT_CANVAS_VRAM, TEXT_CANVAS_SIZE, TEXT_CANVAS_BACKGROUND_WORD);
}

/* Resets the pen and text style of the first idle message, clearing the canvas
   when the message still has a window. */
void mnMessageResetIdle(void)
{
	MESSAGE *slot = WINDOW_STATE->msgs;
	MESSAGE *selection = 0;
	Sint32	 i;

	for(i = 0; i != MESSAGE_COUNT; slot++, i++) {
		if(slot->window == 0 || slot->window->state != 0) {
			selection = slot;
			break;
		}
	}
	if(selection != 0) {
		if(selection->window != 0) {
			mnTextClearCanvas();
			selection->y = 0;
		}
		selection->x = 0;
		selection->countdown = 0;
		selection->colour = TEXT_COLOR_WHITE;
		selection->outline = TEXT_STYLE_PLAIN;
		selection->lineSpacing = TEXT_DEFAULT_LINE_SPACING;
	}
}

/* Makes the message's text colour, style and line spacing the ones glyphs are
   drawn with, before it draws its next character. */
void mnMessageApplyStyle(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;

	state->colour = msg->colour;
	state->outline = msg->outline;
	state->lineSpacing = msg->lineSpacing;
}

/* Starts a message scrolling its canvas up a line: 12 pixel rows, 6 a tick
   over two ticks (mnMessageType, mnTextScrollCanvasUp), during which the
   message types nothing. */
void mnTextStartCanvasScroll(MESSAGE *msg)
{
	msg->scrollFrame = 2;
}
