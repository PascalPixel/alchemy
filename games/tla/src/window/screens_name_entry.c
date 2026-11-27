/* The name entry screen. Part of screens.c. */

/* The text control code that sets the colour of the text after it, from the
   byte that follows ({text_color:N} in the message catalogs). */
#define TEXT_COLOR_COMMAND 8

/* The Japanese name entry screen enters kana, whose voicing marks (dakuten and
   handakuten) are characters of their own that join the kana before them. The
   other editions have no marks: in the first function's place they keep one
   that accepts every letter, and they drop the second. */
#if LANGUAGE == LANGUAGE_JAPANESE
/* The keyboard's keys for the kana voicing marks (TEXT_MARK_VOICED and
   TEXT_MARK_SEMIVOICED), as its tilemap's low bytes hold them. */
#define NAME_VOICED_KEY		254
#define NAME_SEMIVOICED_KEY 255

extern const Uint8 NameEntry_VoicedMarkRanges[];
extern const Uint8 NameEntry_SemivoicedMarkRanges[];

/* Returns whether `letter` may follow `previous` in a name: any character but a
   voicing mark may, and a voicing mark only after a kana in its table of ranges
   (pairs of first and last kana, ended by 0). */
Bool mnNameEntryAcceptsMark(Sint32 letter, Sint32 previous)
{
	const Uint8 *range;

	if(letter < TEXT_MARK_VOICED || letter > TEXT_MARK_SEMIVOICED)
		return TRUE;
	if(letter == TEXT_MARK_VOICED)
		range = NameEntry_VoicedMarkRanges;
	else
		range = NameEntry_SemivoicedMarkRanges;
	for(; *range != 0; range += 2) {
		if(previous >= range[0] && previous <= range[1])
			return TRUE;
	}
	return FALSE;
}

/* Returns how many letters `name` has, not counting its voicing marks. */
Sint32 mnNameEntryCountLetters(const Uint8 *name)
{
	Sint32 count;

	count = 0;
	for(; *name != 0; name++) {
		if((Uint8)(*name - TEXT_MARK_VOICED) >
			TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)
			count++;
	}
	return count;
}
#else
/* Unused: mnNameEntryAcceptsMark's place in the editions whose letters take no
   marks, which accepts every letter. */
UNUSED Sint32 mnNameEntryAcceptsAnyLetter(void)
{
	return 1;
}
#endif

/* Decompresses a tilemap, such as the name entry's keyboard, and copies it over
   the window's rectangle, into the BG map in VRAM and the window state's copy
   of it. */
void mnWindowCopyTilemapRegion(WINDOW *window, const void *resource)
{
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	Uint16	  *buffer = syHeapAllocEWRAM(768);
	Sint32	   offset;
	Uint16	  *map;
	Uint16	  *vram;
	Uint16	  *source;
	Sint32	   row;
	Sint32	   col;

	source = buffer;
	syDecompressTaggedLZ(resource, buffer);
	offset = ((window->y << 5) + window->x) * 2;
	map = (Uint16 *)((Uint8 *)state->tilemap + offset);
	vram = (Uint16 *)(offset + WINDOW_TILEMAP_VRAM);
	for(row = 0; row < window->height; row++) {
		for(col = 0; col < window->width;) {
			Sint16 tile = *source;

			col++;
			*vram = tile;
			*map = tile;
			source++;
			vram++;
			map++;
		}
		vram += BG_SCREEN_WIDTH - window->width;
		map += BG_SCREEN_WIDTH - window->width;
	}
	syHeapRelease(buffer);
}

#include "screens_set_text_tile_rect.c"

/* Loads the pointing hand cursor's tiles into a free VRAM slot and returns the
   slot, which the caller frees with syVRAMCacheFreeEntry. */
Sint32 mnPointingHandLoadIntoFreeSlot(void)
{
	Sint32 slot;

	slot = syVRAMCacheFindFreeEntry();
	mnVRAMBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
	return slot;
}

/* Loads the pointing hand's tiles into VRAM cache entry slot. */
void mnPointingHandLoadIntoSlot(Sint32 slot)
{
	mnVRAMBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
}

/* Loads the name entry caret's tiles into VRAM cache entry slot. */
void mnNameCaretLoadIntoSlot(Sint32 slot)
{
	mnVRAMBlockLoadResourceStub(
		slot, ICON_TILE_BYTES, RESOURCE_NAME_CARET_TILES);
}

#if (LANGUAGE != LANGUAGE_JAPANESE)
/* Returns the width of the string `str` in pixels, measured as decoded text (it
   is copied into the window state's text buffer). */
Sint32 mnTextMeasureString(const Uint8 *str)
{
	WIN_STATE *state;
	Uint16	  *dst;
	Sint32	   count;
	Uint32	   width;
	Uint32	   height;

	state = (WIN_STATE *)Ram_HeapSlots->winState;
	count = 0;
	if(*str != 0) {
		dst = state->textBuf;
		do {
			*dst = *str;
			str++;
			dst++;
			count++;
		} while(*str != 0);
	}
	state->textBuf[count] = 0;
	mnTextMeasureDecoded(0, &width, &height, 0);
	return width;
}

/* Draws the name being entered, `name`, at the top left of `window`, followed
   in TEXT_COLOR_RED by an underscore for each of the NAME_MAX_LETTERS places
   still free: the loop fills the text to 7 bytes, the name and the two-byte
   colour code before the underscores. */
void mnNameEntryDrawName(Sint32 window, Uint8 *name)
{
	Uint8  text[20];
	Sint32 length = 0;

	if(*name != 0) {
		do {
			text[length] = *name;
			name++;
			length++;
		} while(*name != 0);
	}

	text[length++] = TEXT_COLOR_COMMAND;
	text[length++] = TEXT_COLOR_RED;

	while(length <= 6) {
		text[length++] = '_';
	}

	text[length++] = TEXT_COLOR_COMMAND;
	text[length++] = TEXT_COLOR_WHITE;
	text[length] = 0;
	mnTextDrawString(text, window, 0, -2);
}
#endif

/* A name's room in bytes and in letters, and the keyboard column of its
   commands. */
#define NAME_MAX_BYTES			14
#define NAME_MAX_LETTERS		5
#define NAME_COMMAND_COLUMN		18
/* The last column of letters, before the gap and the commands. */
#define NAME_LAST_LETTER_COLUMN (NAME_COMMAND_COLUMN - 2)
/* The empty columns between the keyboard's groups of letters. */
#define NAME_IS_GAP_COLUMN(column) \
	((column) == 5 || (column) == 11 || (column) == 17)

/* The Japanese keyboard is a kana grid of eleven rows with a blank row in its
   middle, and its command column has a third key, at the top, that turns to the
   other kana page. */
#define NAME_KEYBOARD_HEIGHT JAPANESE_OR(13, 9)
#define NAME_DIVIDER_BOTTOM	 JAPANESE_OR(12, 7)
#define NAME_KEY_CURSOR_Y	 JAPANESE_OR(92, 52)
#define NAME_REMOVE_ROW		 JAPANESE_OR(8, 4)
#define NAME_FINISH_ROW		 JAPANESE_OR(10, 5)
#define NAME_PAGE_ROW		 0
#define NAME_BLANK_ROW		 5
#define NAME_LAST_ROW		 NAME_FINISH_ROW

/* The width of the window showing the name, in tiles. */
#define NAME_LABEL_WIDTH SPANISH_OR(9, 8)

/* Whether the edition draws the remove and finish keys' labels as text over the
   keyboard, and how wide its finish key is, in tiles. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#define NAME_FINISH_WIDTH BY_LANGUAGE(3, 3, 4, 3, 3, 4)

/* A dakuten or handakuten, which joins the kana before it and is no letter of
   its own. */
#define IS_KANA_MARK(c) \
	((Uint8)((c) - TEXT_MARK_VOICED) <= TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)

#if LANGUAGE == LANGUAGE_JAPANESE
/* The Japanese keyboard's second page of letters, which Select turns to. */
extern const Uint8 NameEntry_AlternateKeyboardTilemap[];

#endif
#if NAME_KEY_LABELS
extern const Uint8 NameEntry_DeleteKeyLabel[];
extern const Uint8 NameEntry_EndKeyLabel[];

#endif

/* Places a menu cursor and its sprite at (x, y) at once. */
void mnCursorSetPosImmediateStub(void *cursor, Sint32 x, Sint32 y);

/* The Japanese form enters kana: a dakuten or handakuten joins the kana before
   it (mnNameEntryAcceptsMark refuses a mark that cannot), takes a byte but is
   no letter, and goes with it when that kana is removed; Select or the top
   command key turns to the other kana page. It also returns without waiting for
   every button to be released, which the later editions added. */
#if LANGUAGE == LANGUAGE_JAPANESE
/* The name entry screen for party member `member`: the player types a name of
   up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or Start,
   which jumps there). */
Sint32 mnNameEntryEditMemberName(Sint32 member)
{
	WINDOW		*nameWindow;
	Sint32		 result = 0;
	Sint32		 page = 0;
	Sint32		 byteCount = 0;
	Sint32		 letterCount = 0;
	Uint8		 text[NAME_MAX_BYTES + 2];
	Uint8		*editedName = text + 1;
	Uint8		*originalName = syUnitGet(member);
	WIN_STATE	*render = (WIN_STATE *)Ram_HeapSlots->winState;
	Bool		 textDirty = TRUE;
	Bool		 cursorDirty = TRUE;
	WINDOW		*keyboardWindow;
	NAME_CURSOR	 keyCursor;
	NAME_CURSOR	 nameCaret;
	Sint32		 keyColumn, keyRow, vramSlot, x;
	WINDOW_SPR	*output;
	MENU_CURSOR *keyMotion;
	Uint32		 letter;
	Uint8		*end;
	Uint16		*palette;

	syWindowLoadGraphics();
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	keyboardWindow =
		mnWindowCreate(3, 6, 24, NAME_KEYBOARD_HEIGHT, WINDOW_FLAG_INSTANT);
	nameWindow = mnWindowCreate(8, 3, 8, 3, WINDOW_FLAG_INSTANT);
	mnPortraitOpenWindow(member, 0, 3, 1);
	mnWindowCopyTilemapRegion(keyboardWindow, NameEntry_KeyboardTilemap);
	mnWindowDrawDividerLine(keyboardWindow, NAME_COMMAND_COLUMN, 0,
		NAME_COMMAND_COLUMN, NAME_DIVIDER_BOTTOM);
	render->dirty = textDirty;
	text[0] = result;
	{
		Sint32 i;

		for(i = 0; i < NAME_MAX_BYTES; i++) {
			letter = originalName[i];

			editedName[i] = letter;
			if(letter != 0) {
				byteCount++;
				letterCount++;
				if(IS_KANA_MARK(letter))
					letterCount--;
			}
		}
	}
	editedName[NAME_MAX_BYTES] = 0;
	mnTextDrawString(originalName, (Sint32)nameWindow, 0, 0);
	keyColumn = NAME_COMMAND_COLUMN;
	vramSlot = syVRAMCacheFindFreeEntry();
	keyRow = NAME_FINISH_ROW;
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVRAMBlockLoadResourceStub(
			vramSlot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
		output = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, (WINDOW *)keyboardWindow, 0, 0);
		keyMotion = &keyCursor.cursor;
		keyCursor.spr = output;
		mnCursorSetPosImmediateStub(keyMotion, keyboardWindow->x * 8 + 140,
			keyboardWindow->y * 8 + NAME_KEY_CURSOR_Y);
	} else {
		keyMotion = &keyCursor.cursor;
	}
	vramSlot = syVRAMCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVRAMBlockLoadResourceStub(
			vramSlot, ICON_TILE_BYTES, RESOURCE_NAME_CARET_TILES);
		output = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, (WINDOW *)keyboardWindow, 0, 0);
		nameCaret.spr = output;
		output->depth = OAM_DEPTH_FRONT;
		output->spr.oam.pri = 0;
		x = 72 + mnNameEntryCountLetters(editedName) * 10;
		mnCursorSetPosImmediateStub(&nameCaret.cursor, x, 22);
	}
	palette = (Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	end = (Uint8 *)(byteCount + (Sint32)editedName) - 1;
	while(1) {
		Sint32 width = 1;

		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_PAGE_ROW)
				width = 4;
			if(keyRow == NAME_REMOVE_ROW)
				width = 3;
			if(keyRow == NAME_FINISH_ROW)
				width = 3;
		}
		mnWindowSetTextTileRect(
			keyboardWindow, keyColumn, keyRow, width, 1, HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetTextTileRect(
			keyboardWindow, keyColumn, keyRow, width, 1, WINDOW_PALETTE);
		if(cursorDirty) {
			cursorDirty = FALSE;
			mnCursorSetStub(keyMotion, (keyboardWindow->x + keyColumn) * 8 - 7,
				(keyboardWindow->y + keyRow) * 8 + 15, 3);
		}
		if(textDirty) {
			textDirty = FALSE;
			x = 72 + mnNameEntryCountLetters(editedName) * 10;
			mnCursorSetStub(&nameCaret.cursor, x, 22, 3);
		}
		mnCursorAdvanceStub(keyMotion);
		mnCursorMoveTowardTargetStub(&nameCaret.cursor);
		{
			Uint32 frame = (VBlankCount >> 1) & 7;

			output = nameCaret.spr;
			output->spr.oam.x = output->x + NameEntry_CaretWave[frame];
			output->spr.oam.y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if(PerWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow--;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == -1)
					keyRow = NAME_LAST_ROW;
				else if(keyRow == NAME_BLANK_ROW)
					keyRow = NAME_BLANK_ROW - 1;
			} else if(keyRow == -1) {
				keyRow = NAME_FINISH_ROW;
			} else {
				keyRow = (keyRow != NAME_REMOVE_ROW - 1);
				keyRow *= NAME_REMOVE_ROW;
			}
		}
		if(PerWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow++;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_LAST_ROW + 1)
					keyRow = 0;
				if(keyRow == NAME_BLANK_ROW)
					keyRow = NAME_BLANK_ROW + 1;
			} else if(keyRow == NAME_PAGE_ROW + 1) {
				keyRow = NAME_REMOVE_ROW;
			} else if(keyRow == NAME_REMOVE_ROW + 1) {
				keyRow = NAME_FINISH_ROW;
			} else {
				keyRow = NAME_PAGE_ROW;
			}
		}
		if(PerWork.repeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn--;
			if(keyColumn == -1) {
				if(keyRow == NAME_PAGE_ROW || keyRow == NAME_REMOVE_ROW ||
					keyRow == NAME_FINISH_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = NAME_LAST_LETTER_COLUMN;
			} else if(NAME_IS_GAP_COLUMN(keyColumn)) {
				keyColumn--;
			}
		}
		if(PerWork.repeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN + 1)
				keyColumn = 0;
			else if(NAME_IS_GAP_COLUMN(keyColumn))
				keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_PAGE_ROW || keyRow == NAME_REMOVE_ROW ||
					keyRow == NAME_FINISH_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = 0;
			}
		}
		if(PerWork.push & KEY_START) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn = NAME_COMMAND_COLUMN;
			keyRow = NAME_FINISH_ROW;
		}
		if(PerWork.push & KEY_SELECT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		turn_page:
			page ^= 1;
			if(page)
				mnWindowCopyTilemapRegion(
					keyboardWindow, NameEntry_AlternateKeyboardTilemap);
			else
				mnWindowCopyTilemapRegion(
					keyboardWindow, NameEntry_KeyboardTilemap);
			mnWindowDrawDividerLine(keyboardWindow, NAME_COMMAND_COLUMN, 0,
				NAME_COMMAND_COLUMN, NAME_DIVIDER_BOTTOM);
			render->dirty = 1;
			continue;
		}
		if(PerWork.repeat & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
		remove_character:
			if(letterCount != 0) {
				letterCount--;
				end--;
				byteCount--;
				letter = end[1];
				end[1] = 0;
				if(IS_KANA_MARK(letter)) {
					end--;
					byteCount--;
					end[1] = 0;
				}
			} else {
				result = -1;
				break;
			}
			mnWindowSprPrepareForRedraw(nameWindow);
			mnTextDrawString(editedName, (Sint32)nameWindow, 0, 0);
			textDirty = TRUE;
			continue;
		}
		if(!(PerWork.repeat & KEY_A))
			continue;
		sySndPlayStub(SE_MENU_CONFIRM);
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_FINISH_ROW) {
				if(letterCount == 0) {
					nameCaret.spr->motion = WINDOW_SPRITE_MOTION_HIDDEN;
					mnWindowSprPrepareForRedraw(nameWindow);
					mnTextDrawString(originalName, (Sint32)nameWindow, 0, 0);
					sySynch(10);
				} else {
					Uint8 *dst = originalName;
					Uint8 *src = editedName;
					Sint32 i;

					for(i = 0; i < UNIT_NAME_SIZE; i++)
						*dst++ = *src++;
				}
				break;
			}
			if(keyRow == NAME_REMOVE_ROW)
				goto remove_character;
			if(keyRow == NAME_PAGE_ROW)
				goto turn_page;
			continue;
		}
		{
			Uint32 tx = keyboardWindow->x + keyColumn + 1;
			Uint8 *slot;

			letter = ((Uint8 *)render->tilemap +
				((keyboardWindow->y + keyRow + 1) << 6))[tx + tx];
			if(letter == NAME_VOICED_KEY)
				letter = TEXT_MARK_VOICED;
			if(letter == NAME_SEMIVOICED_KEY)
				letter = TEXT_MARK_SEMIVOICED;
			if(letterCount == NAME_MAX_LETTERS && !IS_KANA_MARK(letter))
				continue;
			slot = editedName + byteCount;
			if(!mnNameEntryAcceptsMark(letter, *end)) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}
			*slot = letter;
			end++;
			byteCount++;
			end[1] = 0;
			letterCount++;
			if(IS_KANA_MARK(letter))
				letterCount--;
			if(letterCount == NAME_MAX_LETTERS) {
				keyColumn = NAME_COMMAND_COLUMN;
				keyRow = NAME_FINISH_ROW;
				cursorDirty = TRUE;
			}
			mnWindowSprPrepareForRedraw(nameWindow);
			mnTextDrawString(editedName, (Sint32)nameWindow, 0, 0);
			textDirty = TRUE;
		}
	}
	mnWindowClose(keyboardWindow, 2);
	mnWindowClose(nameWindow, 2);
	mnPortraitCloseWindow(member);
	sySynch(1);
	return result;
}
#else
/* The name entry screen for party member `member`: the player types a name of
   up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or Start,
   which jumps there). B with no letters left cancels and returns -1. */
Sint32 mnNameEntryEditMemberName(Sint32 member)
{
	WINDOW		*nameWindow;
	Sint32		 result = 0;
	Sint32		 originalLength = 0;
	Sint32		 letterCount = 0;
	Uint8		 text[NAME_MAX_BYTES + 2];
	Uint8		*editedName = text + 1;
	Uint8		*originalName = syUnitGet(member);
	WIN_STATE	*render = (WIN_STATE *)Ram_HeapSlots->winState;
	Bool		 textDirty = TRUE;
	Bool		 cursorDirty = TRUE;
	WINDOW		*keyboardWindow;
	NAME_CURSOR	 keyCursor;
	NAME_CURSOR	 nameCaret;
	Sint32		 keyColumn, keyRow, vramSlot;
	WINDOW_SPR	*output;
	MENU_CURSOR *keyMotion;
	Uint8		*end;
	Uint16		*palette;

	syWindowLoadGraphics();
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	keyboardWindow =
		mnWindowCreate(3, 6, 24, NAME_KEYBOARD_HEIGHT, WINDOW_FLAG_INSTANT);
	nameWindow = mnWindowCreate(8, 3, NAME_LABEL_WIDTH, 3, WINDOW_FLAG_INSTANT);
	mnPortraitOpenWindow(member, 0, 3, 1);
	mnWindowCopyTilemapRegion(keyboardWindow, NameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
	mnTextDrawStringInWindow(NameEntry_DeleteKeyLabel, keyboardWindow, 144, 32);
	mnTextDrawStringInWindow(NameEntry_EndKeyLabel, keyboardWindow, 144, 40);
#endif
	mnWindowDrawDividerLine(keyboardWindow, NAME_COMMAND_COLUMN, 0,
		NAME_COMMAND_COLUMN, NAME_DIVIDER_BOTTOM);
	render->dirty = textDirty;
	text[0] = result;
	{
		Sint32 i;

		for(i = 0; i < NAME_MAX_BYTES; i++) {
			Uint32 c = originalName[i];

			editedName[i] = c;
			if(c != 0) {
				originalLength++;
				letterCount++;
			}
		}
	}
	editedName[NAME_MAX_BYTES] = 0;
	mnNameEntryDrawName((Sint32)nameWindow, originalName);
	keyRow = NAME_FINISH_ROW;
	vramSlot = syVRAMCacheFindFreeEntry();
	keyColumn = NAME_COMMAND_COLUMN;
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVRAMBlockLoadResourceStub(
			vramSlot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
		output = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, (WINDOW *)keyboardWindow, 0, 0);
		keyMotion = &keyCursor.cursor;
		keyCursor.spr = output;
		mnCursorSetPosImmediateStub(keyMotion, keyboardWindow->x * 8 + 140,
			keyboardWindow->y * 8 + NAME_KEY_CURSOR_Y);
	} else {
		keyMotion = &keyCursor.cursor;
	}
	vramSlot = syVRAMCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVRAMBlockLoadResourceStub(
			vramSlot, ICON_TILE_BYTES, RESOURCE_NAME_CARET_TILES);
		output = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, (WINDOW *)keyboardWindow, 0, 0);
		nameCaret.spr = output;
		output->depth = OAM_DEPTH_FRONT;
		output->spr.oam.pri = 0;
		mnCursorSetPosImmediateStub(
			&nameCaret.cursor, mnTextMeasureString(editedName) + 70, 22);
	}
	palette = (Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	end = (Uint8 *)(originalLength + (Sint32)editedName);
	while(1) {
		Sint32 width = 1;

		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_REMOVE_ROW)
				width = 3;
			if(keyRow == NAME_FINISH_ROW)
				width = NAME_FINISH_WIDTH;
		}
		mnWindowSetTextTileRect(
			keyboardWindow, keyColumn, keyRow, width, 1, HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetTextTileRect(
			keyboardWindow, keyColumn, keyRow, width, 1, WINDOW_PALETTE);
		if(cursorDirty) {
			cursorDirty = FALSE;
			mnCursorSetStub(keyMotion, (keyboardWindow->x + keyColumn) * 8 - 7,
				(keyboardWindow->y + keyRow) * 8 + 15, 3);
		}
		if(textDirty) {
			textDirty = FALSE;
			mnCursorSetStub(
				&nameCaret.cursor, mnTextMeasureString(editedName) + 70, 22, 3);
		}
		mnCursorAdvanceStub(keyMotion);
		mnCursorMoveTowardTargetStub(&nameCaret.cursor);
		{
			Uint32 frame = (VBlankCount >> 1) & 7;

			output = nameCaret.spr;
			output->spr.oam.x = output->x + NameEntry_CaretWave[frame];
			output->spr.oam.y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if(PerWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow--;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == -1)
					keyRow = NAME_LAST_ROW;
			} else {
				keyRow = (keyRow != NAME_REMOVE_ROW - 1);
				keyRow = NAME_FINISH_ROW - keyRow;
			}
		}
		if(PerWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow++;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_LAST_ROW + 1)
					keyRow = 0;
			} else {
				keyRow = (keyRow != NAME_FINISH_ROW + 1);
				keyRow += NAME_REMOVE_ROW;
			}
		}
		if(PerWork.repeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn--;
			if(keyColumn == -1) {
				if(keyRow >= NAME_REMOVE_ROW && keyRow <= NAME_FINISH_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = NAME_LAST_LETTER_COLUMN;
			} else if(NAME_IS_GAP_COLUMN(keyColumn)) {
				keyColumn--;
			}
		}
		if(PerWork.repeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN + 1)
				keyColumn = 0;
			else if(NAME_IS_GAP_COLUMN(keyColumn))
				keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN &&
				(keyRow < NAME_REMOVE_ROW || keyRow > NAME_FINISH_ROW))
				keyColumn = 0;
		}
		if(PerWork.push & KEY_START) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn = NAME_COMMAND_COLUMN;
			keyRow = NAME_FINISH_ROW;
		}
		if(PerWork.repeat & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
		remove_character:
			if(letterCount != 0) {
				letterCount--;
				*--end = 0;
				mnWindowSprPrepareForRedraw(nameWindow);
				mnNameEntryDrawName((Sint32)nameWindow, editedName);
				textDirty = TRUE;
				continue;
			}
			result = -1;
			break;
		}
		if(!(PerWork.repeat & KEY_A))
			continue;
		sySndPlayStub(SE_MENU_CONFIRM);
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_FINISH_ROW) {
				if(letterCount == 0) {
					nameCaret.spr->motion = WINDOW_SPRITE_MOTION_HIDDEN;
					mnWindowSprPrepareForRedraw(nameWindow);
					mnNameEntryDrawName((Sint32)nameWindow, originalName);
					sySynch(10);
				} else {
					Uint8 *dst = originalName;
					Uint8 *src = editedName;
					Sint32 i;

					for(i = 0; i < UNIT_NAME_SIZE; i++)
						*dst++ = *src++;
				}
				break;
			}
			if(keyRow == NAME_REMOVE_ROW)
				goto remove_character;
			continue;
		}
		{
			Uint32 tx = keyboardWindow->x + keyColumn + 1;
			Uint32 letter = ((Uint8 *)render->tilemap +
				((keyboardWindow->y + keyRow + 1) << 6))[tx << 1];

			if(letterCount == NAME_MAX_LETTERS)
				continue;
			*end++ = letter;
			*end = 0;
			letterCount++;
			if(letterCount == NAME_MAX_LETTERS) {
				keyColumn = NAME_COMMAND_COLUMN;
				keyRow = NAME_FINISH_ROW;
				cursorDirty = TRUE;
			}
			mnWindowSprPrepareForRedraw(nameWindow);
			mnNameEntryDrawName((Sint32)nameWindow, editedName);
			textDirty = TRUE;
		}
	}
	mnWindowClose(keyboardWindow, 2);
	mnWindowClose(nameWindow, 2);
	mnPortraitCloseWindow(member);
	sySynch(1);
	while(PerWork.data != 0)
		sySynch(1);
	return result;
}
#endif

#undef NAME_MAX_BYTES
#undef NAME_MAX_LETTERS
#undef NAME_COMMAND_COLUMN
#undef NAME_LAST_LETTER_COLUMN
#undef NAME_IS_GAP_COLUMN
#undef NAME_KEYBOARD_HEIGHT
#undef NAME_DIVIDER_BOTTOM
#undef NAME_KEY_CURSOR_Y
#undef NAME_REMOVE_ROW
#undef NAME_FINISH_ROW
#undef NAME_PAGE_ROW
#undef NAME_BLANK_ROW
#undef NAME_LAST_ROW
#undef NAME_LABEL_WIDTH
#undef NAME_KEY_LABELS
#undef NAME_FINISH_WIDTH
#undef IS_KANA_MARK
