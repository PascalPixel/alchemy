/* @file */
/* The name entry screen. */
/* Part of screens.c. */

/* The text control code that sets the colour of the text after it, from the */
/* byte that follows ({text_color:N} in the message catalogs). */
#define TEXT_COLOR_COMMAND 8

/* The Japanese name entry screen enters kana, whose voicing marks (dakuten */
/* and handakuten) are characters of their own that join the kana before them.
 */
/* The other editions have no marks: in the first function's place they keep */
/* one that accepts every letter, and they drop the second. */
#if LANGUAGE == LANGUAGE_JAPANESE
/* The keyboard's keys for the kana voicing marks (TEXT_MARK_VOICED and */
/* TEXT_MARK_SEMIVOICED), as its tilemap's low bytes hold them. */
#define NAME_VOICED_KEY		254
#define NAME_SEMIVOICED_KEY 255

extern const u8 NameEntry_VoicedMarkRanges[];
extern const u8 NameEntry_SemivoicedMarkRanges[];

/* Returns whether `letter` may follow `previous` in a name: any character but
 */
/* a voicing mark may, and a voicing mark only after a kana in its table of */
/* ranges (pairs of first and last kana, ended by 0). */
s32 mnNameEntryAcceptsMark(s32 letter, s32 previous) {
	const u8 *range;

	if(letter < TEXT_MARK_VOICED || letter > TEXT_MARK_SEMIVOICED)
		return 1;
	if(letter == TEXT_MARK_VOICED)
		range = NameEntry_VoicedMarkRanges;
	else
		range = NameEntry_SemivoicedMarkRanges;
	for(; *range != 0; range += 2) {
		if(previous >= range[0] && previous <= range[1])
			return 1;
	}
	return 0;
}

/* Returns how many letters `name` has, not counting its voicing marks. */
s32 mnNameEntryCountLetters(const u8 *name) {
	s32 count;

	count = 0;
	for(; *name != 0; name++) {
		if((u8)(*name - TEXT_MARK_VOICED) >
		   TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)
			count++;
	}
	return count;
}
#else
/* Unused: mnNameEntryAcceptsMark's place in the editions whose letters take no
 */
/* marks, which accepts every letter. */
UNUSED s32 mnNameEntryAcceptsAnyLetter(void) { return 1; }
#endif

/* Decompresses a tilemap, such as the name entry's keyboard, and copies it */
/* over the window's rectangle, into the BG map in VRAM and the window state's
 */
/* copy of it. */
void mnWindowCopyTilemapRegion(WINDOW *window, const void *resource) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	u16 *buffer = syHeapAllocEwram(768);
	s32 offset;
	u16 *map;
	u16 *vram;
	u16 *source;
	s32 row;
	s32 col;

	source = buffer;
	syDecompressTaggedLz(resource, buffer);
	offset = ((window->y << 5) + window->x) * 2;
	map = (u16 *)((u8 *)state->tilemap + offset);
	vram = (u16 *)(offset + WINDOW_TILEMAP_VRAM);
	for(row = 0; row < window->height; row++) {
		for(col = 0; col < window->width;) {
			s16 tile = *source;

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
 */
/* slot, which the caller frees with syVramCacheFreeEntry. */
s32 mnPointingHandLoadIntoFreeSlot(void) {
	s32 slot;

	slot = syVramCacheFindFreeEntry();
	mnVramBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
	return slot;
}

/* Loads the pointing hand's tiles into VRAM cache entry slot. */
void mnPointingHandLoadIntoSlot(s32 slot) {
	mnVramBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
}

/* Loads the name entry caret's tiles into VRAM cache entry slot. */
void mnNameCaretLoadIntoSlot(s32 slot) {
	mnVramBlockLoadResourceStub(slot, ICON_TILE_BYTES,
								RESOURCE_NAME_CARET_TILES);
}

#if (LANGUAGE != LANGUAGE_JAPANESE)
/* Returns the width of the string `str` in pixels, measured as decoded text */
/* (it is copied into the window state's text buffer). */
s32 mnTextMeasureString(const u8 *str) {
	WIN_STATE *state;
	u16 *dst;
	s32 count;
	u32 width;
	u32 height;

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

/* Draws the name being entered, `name`, at the top left of `window`, */
/* followed in TEXT_COLOR_RED by an underscore for each of the */
/* NAME_MAX_LETTERS places still free: the loop fills the text to 7 bytes, */
/* the name and the two-byte colour code before the underscores. */
void mnNameEntryDrawName(s32 window, u8 *name) {
	u8 text[20];
	s32 length = 0;

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

/* A name's room in bytes and in letters, and the keyboard column of its */
/* commands. */
#define NAME_MAX_BYTES		14
#define NAME_MAX_LETTERS	5
#define NAME_COMMAND_COLUMN 18
/* The last column of letters, before the gap and the commands. */
#define NAME_LAST_LETTER_COLUMN (NAME_COMMAND_COLUMN - 2)
/* The empty columns between the keyboard's groups of letters. */
#define NAME_IS_GAP_COLUMN(column)                                             \
	((column) == 5 || (column) == 11 || (column) == 17)

/* The Japanese keyboard is a kana grid of eleven rows with a blank row in its
 */
/* middle, and its command column has a third key, at the top, that turns to */
/* the other kana page. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define NAME_KEYBOARD_HEIGHT 13
#define NAME_DIVIDER_BOTTOM	 12
#define NAME_KEY_CURSOR_Y	 92
#define NAME_REMOVE_ROW		 8
#define NAME_FINISH_ROW		 10
#else
#define NAME_KEYBOARD_HEIGHT 9
#define NAME_DIVIDER_BOTTOM	 7
#define NAME_KEY_CURSOR_Y	 52
#define NAME_REMOVE_ROW		 4
#define NAME_FINISH_ROW		 5
#endif
#define NAME_PAGE_ROW  0
#define NAME_BLANK_ROW 5
#define NAME_LAST_ROW  NAME_FINISH_ROW

/* The width of the window showing the name, in tiles. */
#if LANGUAGE == LANGUAGE_SPANISH
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

/* Whether the edition draws the remove and finish keys' labels as text over */
/* the keyboard, and how wide its finish key is, in tiles. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_FRENCH
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_ITALIAN
#define NAME_FINISH_WIDTH 4
#else
#define NAME_FINISH_WIDTH 3
#endif

/* A dakuten or handakuten, which joins the kana before it and is no letter of
 */
/* its own. */
#define IS_KANA_MARK(c)                                                        \
	((u8)((c) - TEXT_MARK_VOICED) <= TEXT_MARK_SEMIVOICED - TEXT_MARK_VOICED)

#if LANGUAGE == LANGUAGE_JAPANESE
/* The Japanese keyboard's second page of letters, which Select turns to. */
extern const u8 NameEntry_AlternateKeyboardTilemap[];

#endif
#if NAME_KEY_LABELS
extern const u8 NameEntry_DeleteKeyLabel[];
extern const u8 NameEntry_EndKeyLabel[];

#endif

/* Places a menu cursor and its sprite at (x, y) at once. */
void mnCursorSetPosImmediateStub(void *cursor, s32 x, s32 y);

/* The Japanese form enters kana: a dakuten or handakuten joins the kana before
 */
/* it (mnNameEntryAcceptsMark refuses a mark that cannot), takes a byte but is
 */
/* no letter, and goes with it when that kana is removed; Select or the top */
/* command key turns to the other kana page. It also returns without waiting */
/* for every button to be released, which the later editions added. */
#if LANGUAGE == LANGUAGE_JAPANESE
/* The name entry screen for party member `member`: the player types a name of
 */
/* up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or Start,
 */
/* which jumps there). */
s32 mnNameEntryEditMemberName(s32 member) {
	WINDOW *nameWindow;
	s32 result = 0;
	s32 page = 0;
	s32 byteCount = 0;
	s32 letterCount = 0;
	u8 text[NAME_MAX_BYTES + 2];
	u8 *editedName = text + 1;
	u8 *originalName = syUnitGet(member);
	WIN_STATE *render = (WIN_STATE *)Ram_HeapSlots->winState;
	s32 textDirty = 1;
	s32 cursorDirty = 1;
	WINDOW *keyboardWindow;
	NAME_CURSOR keyCursor;
	NAME_CURSOR nameCaret;
	s32 keyColumn, keyRow, vramSlot, x;
	WINDOW_SPR *output;
	MENU_CURSOR *keyMotion;
	u32 letter;
	u8 *end;
	u16 *palette;

	syWindowLoadGraphics();
	mnWindowColApply(MainRoutineStateWork.winHue,
					 MainRoutineStateWork.winBrightness);
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
		s32 i;

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
	mnTextDrawString(originalName, (s32)nameWindow, 0, 0);
	keyColumn = NAME_COMMAND_COLUMN;
	vramSlot = syVramCacheFindFreeEntry();
	keyRow = NAME_FINISH_ROW;
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVramBlockLoadResourceStub(vramSlot, ICON_TILE_BYTES,
									RESOURCE_UI_ICON_TILES);
		output = mnWindowSprCreate(vramSlot, WINDOW_SPRITE_SIZE_16X16,
								   (WINDOW *)keyboardWindow, 0, 0);
		keyMotion = &keyCursor.cursor;
		keyCursor.spr = output;
		mnCursorSetPosImmediateStub(keyMotion, keyboardWindow->x * 8 + 140,
									keyboardWindow->y * 8 + NAME_KEY_CURSOR_Y);
	}
	else {
		keyMotion = &keyCursor.cursor;
	}
	vramSlot = syVramCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVramBlockLoadResourceStub(vramSlot, ICON_TILE_BYTES,
									RESOURCE_NAME_CARET_TILES);
		output = mnWindowSprCreate(vramSlot, WINDOW_SPRITE_SIZE_16X16,
								   (WINDOW *)keyboardWindow, 0, 0);
		nameCaret.spr = output;
		output->depth = OAM_DEPTH_FRONT;
		output->spr.oam.pri = 0;
		x = 72 + mnNameEntryCountLetters(editedName) * 10;
		mnCursorSetPosImmediateStub(&nameCaret.cursor, x, 22);
	}
	palette = (u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
			   DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	end = (u8 *)(byteCount + (s32)editedName) - 1;
	while(1) {
		s32 width = 1;

		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_PAGE_ROW)
				width = 4;
			if(keyRow == NAME_REMOVE_ROW)
				width = 3;
			if(keyRow == NAME_FINISH_ROW)
				width = 3;
		}
		mnWindowSetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
								HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
								WINDOW_PALETTE);
		if(cursorDirty) {
			cursorDirty = 0;
			mnCursorSetStub(keyMotion, (keyboardWindow->x + keyColumn) * 8 - 7,
							(keyboardWindow->y + keyRow) * 8 + 15, 3);
		}
		if(textDirty) {
			textDirty = 0;
			x = 72 + mnNameEntryCountLetters(editedName) * 10;
			mnCursorSetStub(&nameCaret.cursor, x, 22, 3);
		}
		mnCursorAdvanceStub(keyMotion);
		mnCursorMoveTowardTargetStub(&nameCaret.cursor);
		{
			u32 frame = (VBlankCount >> 1) & 7;

			output = nameCaret.spr;
			output->spr.oam.x = output->x + NameEntry_CaretWave[frame];
			output->spr.oam.y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if(InputWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyRow--;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == -1)
					keyRow = NAME_LAST_ROW;
				else if(keyRow == NAME_BLANK_ROW)
					keyRow = NAME_BLANK_ROW - 1;
			}
			else if(keyRow == -1) {
				keyRow = NAME_FINISH_ROW;
			}
			else {
				keyRow = (keyRow != NAME_REMOVE_ROW - 1);
				keyRow *= NAME_REMOVE_ROW;
			}
		}
		if(InputWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyRow++;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_LAST_ROW + 1)
					keyRow = 0;
				if(keyRow == NAME_BLANK_ROW)
					keyRow = NAME_BLANK_ROW + 1;
			}
			else if(keyRow == NAME_PAGE_ROW + 1) {
				keyRow = NAME_REMOVE_ROW;
			}
			else if(keyRow == NAME_REMOVE_ROW + 1) {
				keyRow = NAME_FINISH_ROW;
			}
			else {
				keyRow = NAME_PAGE_ROW;
			}
		}
		if(InputWork.repeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyColumn--;
			if(keyColumn == -1) {
				if(keyRow == NAME_PAGE_ROW || keyRow == NAME_REMOVE_ROW ||
				   keyRow == NAME_FINISH_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = NAME_LAST_LETTER_COLUMN;
			}
			else if(NAME_IS_GAP_COLUMN(keyColumn)) {
				keyColumn--;
			}
		}
		if(InputWork.repeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
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
		if(InputWork.pressed & KEY_START) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyColumn = NAME_COMMAND_COLUMN;
			keyRow = NAME_FINISH_ROW;
		}
		if(InputWork.pressed & KEY_SELECT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		turn_page:
			page ^= 1;
			if(page)
				mnWindowCopyTilemapRegion(keyboardWindow,
										  NameEntry_AlternateKeyboardTilemap);
			else
				mnWindowCopyTilemapRegion(keyboardWindow,
										  NameEntry_KeyboardTilemap);
			mnWindowDrawDividerLine(keyboardWindow, NAME_COMMAND_COLUMN, 0,
									NAME_COMMAND_COLUMN, NAME_DIVIDER_BOTTOM);
			render->dirty = 1;
			continue;
		}
		if(InputWork.repeat & KEY_B) {
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
			}
			else {
				result = -1;
				break;
			}
			mnWindowSprPrepareForRedraw(nameWindow);
			mnTextDrawString(editedName, (s32)nameWindow, 0, 0);
			textDirty = 1;
			continue;
		}
		if(!(InputWork.repeat & KEY_A))
			continue;
		sySndPlayStub(SE_MENU_CONFIRM);
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_FINISH_ROW) {
				if(letterCount == 0) {
					nameCaret.spr->motion = WINDOW_SPRITE_MOTION_HIDDEN;
					mnWindowSprPrepareForRedraw(nameWindow);
					mnTextDrawString(originalName, (s32)nameWindow, 0, 0);
					sySynch(10);
				}
				else {
					u8 *dst = originalName;
					u8 *src = editedName;
					s32 i;

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
			u32 tx = keyboardWindow->x + keyColumn + 1;
			u8 *slot;

			letter = ((u8 *)render->tilemap +
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
				cursorDirty = 1;
			}
			mnWindowSprPrepareForRedraw(nameWindow);
			mnTextDrawString(editedName, (s32)nameWindow, 0, 0);
			textDirty = 1;
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
 */
/* up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or Start,
 */
/* which jumps there). B with no letters left cancels and returns -1. */
s32 mnNameEntryEditMemberName(s32 member) {
	WINDOW *nameWindow;
	s32 result = 0;
	s32 originalLength = 0;
	s32 letterCount = 0;
	u8 text[NAME_MAX_BYTES + 2];
	u8 *editedName = text + 1;
	u8 *originalName = syUnitGet(member);
	WIN_STATE *render = (WIN_STATE *)Ram_HeapSlots->winState;
	s32 textDirty = 1;
	s32 cursorDirty = 1;
	WINDOW *keyboardWindow;
	NAME_CURSOR keyCursor;
	NAME_CURSOR nameCaret;
	s32 keyColumn, keyRow, vramSlot;
	WINDOW_SPR *output;
	MENU_CURSOR *keyMotion;
	u8 *end;
	u16 *palette;

	syWindowLoadGraphics();
	mnWindowColApply(MainRoutineStateWork.winHue,
					 MainRoutineStateWork.winBrightness);
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
		s32 i;

		for(i = 0; i < NAME_MAX_BYTES; i++) {
			u32 c = originalName[i];

			editedName[i] = c;
			if(c != 0) {
				originalLength++;
				letterCount++;
			}
		}
	}
	editedName[NAME_MAX_BYTES] = 0;
	mnNameEntryDrawName((s32)nameWindow, originalName);
	keyRow = NAME_FINISH_ROW;
	vramSlot = syVramCacheFindFreeEntry();
	keyColumn = NAME_COMMAND_COLUMN;
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVramBlockLoadResourceStub(vramSlot, ICON_TILE_BYTES,
									RESOURCE_UI_ICON_TILES);
		output = mnWindowSprCreate(vramSlot, WINDOW_SPRITE_SIZE_16X16,
								   (WINDOW *)keyboardWindow, 0, 0);
		keyMotion = &keyCursor.cursor;
		keyCursor.spr = output;
		mnCursorSetPosImmediateStub(keyMotion, keyboardWindow->x * 8 + 140,
									keyboardWindow->y * 8 + NAME_KEY_CURSOR_Y);
	}
	else {
		keyMotion = &keyCursor.cursor;
	}
	vramSlot = syVramCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		mnVramBlockLoadResourceStub(vramSlot, ICON_TILE_BYTES,
									RESOURCE_NAME_CARET_TILES);
		output = mnWindowSprCreate(vramSlot, WINDOW_SPRITE_SIZE_16X16,
								   (WINDOW *)keyboardWindow, 0, 0);
		nameCaret.spr = output;
		output->depth = OAM_DEPTH_FRONT;
		output->spr.oam.pri = 0;
		mnCursorSetPosImmediateStub(&nameCaret.cursor,
									mnTextMeasureString(editedName) + 70, 22);
	}
	palette = (u16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
			   DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	end = (u8 *)(originalLength + (s32)editedName);
	while(1) {
		s32 width = 1;

		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_REMOVE_ROW)
				width = 3;
			if(keyRow == NAME_FINISH_ROW)
				width = NAME_FINISH_WIDTH;
		}
		mnWindowSetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
								HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetTextTileRect(keyboardWindow, keyColumn, keyRow, width, 1,
								WINDOW_PALETTE);
		if(cursorDirty) {
			cursorDirty = 0;
			mnCursorSetStub(keyMotion, (keyboardWindow->x + keyColumn) * 8 - 7,
							(keyboardWindow->y + keyRow) * 8 + 15, 3);
		}
		if(textDirty) {
			textDirty = 0;
			mnCursorSetStub(&nameCaret.cursor,
							mnTextMeasureString(editedName) + 70, 22, 3);
		}
		mnCursorAdvanceStub(keyMotion);
		mnCursorMoveTowardTargetStub(&nameCaret.cursor);
		{
			u32 frame = (VBlankCount >> 1) & 7;

			output = nameCaret.spr;
			output->spr.oam.x = output->x + NameEntry_CaretWave[frame];
			output->spr.oam.y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if(InputWork.repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyRow--;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == -1)
					keyRow = NAME_LAST_ROW;
			}
			else {
				keyRow = (keyRow != NAME_REMOVE_ROW - 1);
				keyRow = NAME_FINISH_ROW - keyRow;
			}
		}
		if(InputWork.repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyRow++;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_LAST_ROW + 1)
					keyRow = 0;
			}
			else {
				keyRow = (keyRow != NAME_FINISH_ROW + 1);
				keyRow += NAME_REMOVE_ROW;
			}
		}
		if(InputWork.repeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyColumn--;
			if(keyColumn == -1) {
				if(keyRow >= NAME_REMOVE_ROW && keyRow <= NAME_FINISH_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = NAME_LAST_LETTER_COLUMN;
			}
			else if(NAME_IS_GAP_COLUMN(keyColumn)) {
				keyColumn--;
			}
		}
		if(InputWork.repeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN + 1)
				keyColumn = 0;
			else if(NAME_IS_GAP_COLUMN(keyColumn))
				keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN &&
			   (keyRow < NAME_REMOVE_ROW || keyRow > NAME_FINISH_ROW))
				keyColumn = 0;
		}
		if(InputWork.pressed & KEY_START) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = 1;
			keyColumn = NAME_COMMAND_COLUMN;
			keyRow = NAME_FINISH_ROW;
		}
		if(InputWork.repeat & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
		remove_character:
			if(letterCount != 0) {
				letterCount--;
				*--end = 0;
				mnWindowSprPrepareForRedraw(nameWindow);
				mnNameEntryDrawName((s32)nameWindow, editedName);
				textDirty = 1;
				continue;
			}
			result = -1;
			break;
		}
		if(!(InputWork.repeat & KEY_A))
			continue;
		sySndPlayStub(SE_MENU_CONFIRM);
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_FINISH_ROW) {
				if(letterCount == 0) {
					nameCaret.spr->motion = WINDOW_SPRITE_MOTION_HIDDEN;
					mnWindowSprPrepareForRedraw(nameWindow);
					mnNameEntryDrawName((s32)nameWindow, originalName);
					sySynch(10);
				}
				else {
					u8 *dst = originalName;
					u8 *src = editedName;
					s32 i;

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
			u32 tx = keyboardWindow->x + keyColumn + 1;
			u32 letter = ((u8 *)render->tilemap +
						  ((keyboardWindow->y + keyRow + 1) << 6))[tx << 1];

			if(letterCount == NAME_MAX_LETTERS)
				continue;
			*end++ = letter;
			*end = 0;
			letterCount++;
			if(letterCount == NAME_MAX_LETTERS) {
				keyColumn = NAME_COMMAND_COLUMN;
				keyRow = NAME_FINISH_ROW;
				cursorDirty = 1;
			}
			mnWindowSprPrepareForRedraw(nameWindow);
			mnNameEntryDrawName((s32)nameWindow, editedName);
			textDirty = 1;
		}
	}
	mnWindowClose(keyboardWindow, 2);
	mnWindowClose(nameWindow, 2);
	mnPortraitCloseWindow(member);
	sySynch(1);
	while(InputWork.held != 0)
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
