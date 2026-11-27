/* The name entry screen. Part of screens.c. */

#if LANGUAGE != LANGUAGE_JAPANESE
/* The international name entry screen has no voicing marks: where the Japanese
   edition checks a mark against the letter before it, this edition keeps a
   function that accepts every character. Nothing calls it. */
UNUSED Sint32 mnNameEntryAcceptsMark(Sint32 mark, Sint32 letter)
{
	return 1;
}
#else

/* The keyboard's keys for the kana voicing marks (TEXT_MARK_VOICED and
   TEXT_MARK_SEMIVOICED), as its tilemap's low bytes hold them. */
#define NAME_VOICED_KEY		254
#define NAME_SEMIVOICED_KEY 255

/* Returns whether `mark` may follow `letter` in a Japanese name: any character
   but a voicing mark may, and a voicing mark only after a letter in its table
   of ranges (pairs of first and last letter, ended by 0). */
Bool mnNameEntryAcceptsMark(Sint32 mark, Sint32 letter)
{
	const Uint8 *range;

	if(mark < TEXT_MARK_VOICED || mark > TEXT_MARK_SEMIVOICED)
		return TRUE;
	if(mark == TEXT_MARK_VOICED)
		range = NameEntryVoicedMarkRanges;
	else
		range = NameEntrySemivoicedMarkRanges;
	for(; *range != 0; range += 2) {
		if(letter >= range[0] && letter <= range[1])
			return TRUE;
	}
	return FALSE;
}

/* Returns how many letters the Japanese name `name` has, not counting the
   voicing marks. */
Sint32 mnNameEntryCountLetters(const Uint8 *name)
{
	Sint32 cnt;

	cnt = 0;
	for(; *name != 0; name++) {
		/* Neither TEXT_MARK_VOICED nor TEXT_MARK_SEMIVOICED. */
		if((Uint8)(*name - TEXT_MARK_VOICED) > 1)
			cnt++;
	}
	return cnt;
}
#endif

/* Decompresses the type-01 tilemap `src` and copies it over `win`'s
   rectangle, both into WINDOW_TILEMAP_VRAM and into the UI tilemap that mirrors
   it. */
void mnWindowCopyTilemapRegion(const WINDOW *win, const void *src)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint16	  *mirror = (Sint16 *)state->tilemap;
	Sint16	  *buf = syHeapAllocEWRAM(768);
	Sint16	  *input = buf;
	Uint32	   cell;
	Sint16	  *vram;
	Sint32	   row;

	syDecompressTaggedLZ(src, buf);
	cell = win->y * BG_SCREEN_WIDTH + win->x;
	vram = (Sint16 *)WINDOW_TILEMAP_VRAM + cell;
	mirror += cell;

	for(row = 0; row < win->height; row++) {
		Sint32 column;

		for(column = 0; column < win->width; column++) {
			Sint16 value = *input++;

			*vram++ = value;
			*mirror++ = value;
		}
		vram += BG_SCREEN_WIDTH - win->width;
		mirror += BG_SCREEN_WIDTH - win->width;
	}
	syHeapRelease(buf);
}

/* Puts the tilemap entries of the `width` x `height` tile rectangle at (x, y)
   inside `window` in `palette`, clipped to the screen: the key or choice under
   the cursor flashes so between WINDOW_PALETTE and HIGHLIGHT_PALETTE. Only the
   palette number's lowest bit is cleared before `palette` is ORed in, as those
   two differ in it alone. */
void mnWindowSetInteriorPalette(const WINDOW *window, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Uint32 palette)
{
	Uint8 *tilemap = WindowStateWork[0];

	x += window->x + 1;
	y += window->y + 1;
	palette = BG_TILE_PALETTE(palette);
	if(x < 0) {
		width += x;
		x = 0;
	}
	if(x + width > DISPLAY_TILE_WIDTH - 1) {
		width = DISPLAY_TILE_WIDTH - x;
	}
	if(y < 0) {
		height += y;
		y = 0;
	}
	/* BUG: tests the bottom against the screen's width, so a window reaching
	   below the screen draws into the map's unseen rows. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}
	if(width > 0 && height > 0) {
		y *= MAP_ROW_BYTES;
		x = y + x * 2;
		do {
			Uint16 *cell = (Uint16 *)(x + (Uint32)tilemap);
			Sint32	remaining = width;
			while(remaining != 0) {
				Uint32 entry = *cell;
				entry &= ~BG_TILE_PALETTE(1);
				entry |= palette;
				remaining--;
				*cell = entry;
				cell++;
			}
			height--;
			x += MAP_ROW_BYTES;
		} while(height != 0);
		((WIN_STATE *)tilemap)->dirty = 1;
	}
}

/* Loads the pointing hand cursor's tiles into VRAM slot `slot`. */
void mnNameEntryLoadPointingHandTiles(Sint32 slot)
{
	syVRAMCacheLoad(slot, 2 * 2 * TILE_SIZE_4BPP, PointingHandTiles);
}

/* Loads the 16x16 sprite NameEntry_CaretTiles into VRAM slot `slot`. */
void mnNameEntryLoadCaretTiles(Sint32 slot)
{
	syVRAMCacheLoad(slot, 2 * 2 * TILE_SIZE_4BPP, NameEntry_CaretTiles);
}

/* The name entry screen: a keyboard window of letter keys, with a column of
   command keys on its right (Delete and End, and in Japanese the keyboard
   switch), the name above it and the character's portrait. */
#define NAME_COMMAND_COLUMN		18 /* the keyboard column of the command keys */
#define NAME_LAST_LETTER_COLUMN 16
/* The keyboard's last row, where the command column has End, and the row of its
   Delete; the Japanese keyboard's switch is on its first row, and its two
   blocks of letters have an empty row between them. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define NAME_LAST_ROW	10
#define NAME_DELETE_ROW 8
#define NAME_SWITCH_ROW 0
#define NAME_GAP_ROW	5
#else
#define NAME_LAST_ROW	5
#define NAME_DELETE_ROW 4
#endif
/* The empty columns between the keyboard's groups of letters. */
#define NAME_IS_GAP_COLUMN(column) \
	((column) == 5 || (column) == 11 || (column) == 17)
#define NAME_MAX_LETTERS 5
#define NAME_MAX_BYTES	 14 /* stored name bytes, excluding its terminator */

#if LANGUAGE != LANGUAGE_JAPANESE
/* Copies the name `str` into the window state's text buffer and returns its
   width in pixels, where the caret stands after it. */
Sint32 mnTextSetString(const Uint8 *str)
{
	WIN_STATE *state;
	Uint16	  *dst;
	Sint32	   cnt;
	Uint32	   width;
	Uint32	   height;

	state = WINDOW_STATE;
	cnt = 0;
	if(*str != 0) {
		dst = state->textBuf;
		do {
			*dst = *str;
			str++;
			dst++;
			cnt++;
		} while(*str != 0);
	}
	state->textBuf[cnt] = 0;
	mnTextMeasureDecoded(0, &width, &height, 0);
	return width;
}

/* The text control code that sets the colour of the text after it, from the
   byte that follows ({text_color:N} in the message catalogs). */
#define TEXT_COLOR_COMMAND 8

/* Draws the name being entered, `name`, at the top left of `win`, followed
   in TEXT_COLOR_RED by an underscore for each of the NAME_MAX_LETTERS places
   still free: the loop fills the text to 7 bytes, the name and the two-byte
   colour code before the underscores. */
void mnNameEntryDrawName(WINDOW *win, Uint8 *name)
{
	Uint8  text[20];
	Sint32 length;

	for(length = 0; name[length] != 0; length++)
		text[length] = name[length];

	text[length++] = TEXT_COLOR_COMMAND;
	text[length++] = TEXT_COLOR_RED;

	while(length < NAME_MAX_LETTERS + 2) {
		text[length++] = '_';
	}

	text[length++] = TEXT_COLOR_COMMAND;
	text[length++] = TEXT_COLOR_WHITE;
	text[length] = 0;
	mnTextDrawString(text, win, 0, -2);
}

/* The width of the window showing the name, in tiles. */
#define NAME_LABEL_WIDTH BY_LANGUAGE(8, 8, 8, 9, 8, 9)

/* Whether the edition draws the Delete and End key labels as text over the
   keyboard, and how wide its End key is, in tiles. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#define NAME_END_KEY_WIDTH GERMAN_OR(4, 3)

/* The name entry screen for party member `member`: the player types a name of
   up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or Start,
   which jumps there). */
Sint32 mnNameEntryEditMemberName(Sint32 member)
{
	WINDOW		   *nameWin;
	Sint32			result = 0;
	Sint32			originalLength = 0;
	Sint32			letterCnt = 0;
	Uint8			text[NAME_MAX_BYTES + 2];
	Uint8		   *editedName = text + 1;
	Uint8		   *originalName = gmUnitGetStub(member)->name;
	WIN_STATE	   *state = WINDOW_STATE;
	Bool			textDirty = TRUE;
	Bool			cursorDirty = TRUE;
	WINDOW		   *keyboardWin;
	MENU_CURSOR_REF keyCursor;
	MENU_CURSOR_REF nameCaret;
	Sint32			keyColumn, keyRow, vramSlot;
	WINDOW_SPR	   *spr;
	Uint8		   *end;
	Uint16		   *palette;

	/* The keyboard, the name and the portrait. */
	syWindowLoadGraphics();
	keyboardWin = (WINDOW *)mnWindowCreate(3, 6, 24, 9, WINDOW_FLAG_INSTANT);
	nameWin = (WINDOW *)mnWindowCreate(
		8, 3, NAME_LABEL_WIDTH, 3, WINDOW_FLAG_INSTANT);
	mnPortraitOpenWindow(mnPortraitGetForSpeaker(member), 0, 3, 1);
	mnWindowCopyTilemapRegion(keyboardWin, NameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
	mnTextDrawStringInWindow(NameEntryDeleteKeyLabel, keyboardWin, 144, 32);
	mnTextDrawStringInWindow(NameEntryEndKeyLabel, keyboardWin, 144, 40);
#endif
	mnWindowDrawDividerLine(keyboardWin, 18, 0, 18, 7);
	state->dirty = 1;
	/* A zero byte before editedName, as in the Japanese entry, where it is the
	   letter before the first; nothing here reads it. */
	text[0] = 0;
	{
		Sint32 i;

		for(i = 0; i < NAME_MAX_BYTES; i++) {
			Uint32 c = originalName[i];

			editedName[i] = c;
			if(c != 0) {
				originalLength++;
				letterCnt++;
			}
		}
	}
	editedName[NAME_MAX_BYTES] = 0;
	mnNameEntryDrawName(nameWin, originalName);
	vramSlot = syVRAMCacheFindFreeEntry();
	/* The cursor starts on End. Without a free VRAM cache entry a sprite is not
	   made, though the loop below still uses it. */
	keyColumn = NAME_COMMAND_COLUMN;
	keyRow = NAME_LAST_ROW;
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(vramSlot, 2 * 2 * TILE_SIZE_4BPP, PointingHandTiles);
		spr = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, keyboardWin, 0, 0);
		keyCursor.spr = spr;
		mnCursorSetPosImmediateStub(&keyCursor.cursor, keyboardWin->x * 8 + 140,
			keyboardWin->y * 8 + 52);
	}
	vramSlot = syVRAMCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(vramSlot, 2 * 2 * TILE_SIZE_4BPP, NameEntry_CaretTiles);
		spr = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, keyboardWin, 0, 0);
		nameCaret.spr = spr;
		spr->depth = OAM_DEPTH_FRONT;
		spr->spr.oam.pri = 0;
		/* The caret stands after the name, at its width in pixels. */
		mnCursorSetPosImmediateStub(
			&nameCaret.cursor, mnTextSetString(editedName) + 70, 22);
	}
	/* The key under the cursor flashes between WINDOW_PALETTE and
	   HIGHLIGHT_PALETTE, a copy of it with WINDOW_BACKGROUND_COLOR grey. */
	palette = (Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	end = (Uint8 *)(originalLength + (Sint32)editedName);
	while(1) {
		Sint32 width = 1;

		/* Delete and End are wider than a letter key. */
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_DELETE_ROW)
				width = 3;
			if(keyRow == NAME_LAST_ROW)
				width = NAME_END_KEY_WIDTH;
		}
		/* Flash the key under the cursor, then move the cursor and the
		   caret. */
		mnWindowSetInteriorPalette(
			keyboardWin, keyColumn, keyRow, width, 1, HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetInteriorPalette(
			keyboardWin, keyColumn, keyRow, width, 1, WINDOW_PALETTE);
		if(cursorDirty) {
			cursorDirty = FALSE;
			mnCursorSetStub(&keyCursor.cursor,
				(keyboardWin->x + keyColumn) * 8 - 7,
				(keyboardWin->y + keyRow) * 8 + 15, 3);
		}
		if(textDirty) {
			textDirty = FALSE;
			mnCursorSetStub(
				&nameCaret.cursor, mnTextSetString(editedName) + 70, 22, 3);
		}
		mnCursorAdvanceStub(&keyCursor.cursor);
		mnCursorMoveTowardTargetStub(&nameCaret.cursor);
		{
			Uint32 frm = (FrameCount >> 1) & 7;

			spr = nameCaret.spr;
			spr->spr.oam.x = spr->x + NameEntryCaretWave[frm];
			spr->spr.oam.y = spr->y + NameEntryCaretWave[(frm + 5) & 7];
		}
		/* The D-pad moves over the keys, skipping the gaps between groups and
		   wrapping around; Start jumps to End. */
		if(PerRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow--;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == -1)
					keyRow = NAME_LAST_ROW;
			} else {
				/* In the command column up and down swap Delete and End. */
				keyRow = (keyRow != NAME_DELETE_ROW - 1);
				keyRow = NAME_LAST_ROW - keyRow;
			}
		}
		if(PerRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow++;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_LAST_ROW + 1)
					keyRow = 0;
			} else {
				keyRow = (keyRow != NAME_LAST_ROW + 1);
				keyRow += NAME_DELETE_ROW;
			}
		}
		if(PerRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn--;
			/* Left of column 0 wraps to the command column on the rows it has
			   keys, else to the last letter column. */
			if(keyColumn == -1) {
				keyColumn = NAME_COMMAND_COLUMN;
				if(keyRow < NAME_DELETE_ROW || keyRow > NAME_LAST_ROW)
					keyColumn = NAME_LAST_LETTER_COLUMN;
			} else if(NAME_IS_GAP_COLUMN(keyColumn)) {
				keyColumn--;
			}
		}
		if(PerRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN + 1)
				keyColumn = 0;
			else if(NAME_IS_GAP_COLUMN(keyColumn))
				keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN &&
				(keyRow < NAME_DELETE_ROW || keyRow > NAME_LAST_ROW))
				keyColumn = 0;
		}
		if(PerPush & KEY_START) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn = NAME_COMMAND_COLUMN;
			keyRow = NAME_LAST_ROW;
		}
		if(PerRepeat & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
		remove_character:
			if(letterCnt != 0) {
				letterCnt--;
				*--end = 0;
				mnWindowSprPrepareForRedraw(nameWin);
				mnNameEntryDrawName(nameWin, editedName);
				textDirty = TRUE;
				continue;
			}
			result = -1;
			break;
		}
		/* A: a command key, or a letter. */
		if(!(PerRepeat & KEY_A))
			continue;
		sySndPlayStub(SE_MENU_CONFIRM);
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_LAST_ROW) {
				/* End: an empty name shows the old one again for 10 ticks, the
				   caret hidden (WINDOW_SPRITE_MOTION_HIDDEN), and keeps it;
				   otherwise the name's bytes and their terminator are copied
				   over the unit's name. */
				if(letterCnt == 0) {
					nameCaret.spr->motion = WINDOW_SPRITE_MOTION_HIDDEN;
					mnWindowSprPrepareForRedraw(nameWin);
					mnNameEntryDrawName(nameWin, originalName);
					sySynch(10);
				} else {
					Uint8 *dst = originalName;
					Uint8 *src = editedName;
					Sint32 i = 0;
					Uint8 *dstSave = dst;
					Uint8 *srcSave = src;

					/* FAKEMATCH(dead): no-op saves. */
					if(originalName)
						src = srcSave;
					if(originalName)
						dst = NULL;
					if(originalName)
						dst = dstSave;
					while(i < NAME_MAX_BYTES + 1) {
						*dst++ = *src++;
						i++;
					}
				}
				break;
			}
			if(keyRow == NAME_DELETE_ROW)
				goto remove_character;
			continue;
		}
		{
			Uint32 tx = keyboardWin->x + keyColumn + 1;
			Uint32 cell;
			Uint32 letter;

			cell = ((keyboardWin->y + keyRow + 1) << 5) + tx;
			/* Key glyphs use the low byte of the mirrored tilemap entries. */
			letter = ((Uint8 *)&state->tilemap)[cell * 2];
			if(letterCnt == NAME_MAX_LETTERS)
				continue;
			*end++ = letter;
			*end = 0;
			letterCnt++;
			/* A full name sends the cursor to End. */
			if(letterCnt == NAME_MAX_LETTERS) {
				keyColumn = NAME_COMMAND_COLUMN;
				keyRow = NAME_LAST_ROW;
				cursorDirty = TRUE;
			}
			mnWindowSprPrepareForRedraw(nameWin);
			mnNameEntryDrawName(nameWin, editedName);
			textDirty = TRUE;
		}
	}
	mnWindowClose(keyboardWin, 2);
	mnWindowClose(nameWin, 2);
	mnPortraitCloseWindow(member);
	sySynch(1);
	return result;
}
#else

/* The name entry screen for party member `member`: the player types a name of
   up to NAME_MAX_LETTERS letters on the keyboard and ends with End (or Start,
   which jumps there). */
Sint32 mnNameEntryEditMemberName(Sint32 member)
{
	WINDOW		   *nameWin;
	Sint32			result = 0;
	Sint32			keyboard = 0;
	Sint32			bytes = 0;
	Sint32			letterCnt = 0;
	Uint8			text[NAME_MAX_BYTES + 2];
	Uint8		   *editedName = text + 1;
	Uint8		   *originalName = gmUnitGetStub(member)->name;
	WIN_STATE	   *state = WINDOW_STATE;
	Bool			textDirty = TRUE;
	Bool			cursorDirty = TRUE;
	WINDOW		   *keyboardWin;
	MENU_CURSOR_REF keyCursor;
	MENU_CURSOR_REF nameCaret;
	Sint32			keyColumn, keyRow, vramSlot, x;
	WINDOW_SPR	   *spr;
	Uint32			letter;
	Uint16		   *palette;

	/* The keyboard, the name and the portrait. */
	syWindowLoadGraphics();
	keyboardWin = (WINDOW *)mnWindowCreate(3, 6, 24, 13, WINDOW_FLAG_INSTANT);
	nameWin = (WINDOW *)mnWindowCreate(8, 3, 8, 3, WINDOW_FLAG_INSTANT);
	mnPortraitOpenWindow(mnPortraitGetForSpeaker(member), 0, 3, 1);
	mnWindowCopyTilemapRegion(keyboardWin, NameEntry_KeyboardTilemap);
	mnWindowDrawDividerLine(keyboardWin, 18, 0, 18, 12);
	state->dirty = 1;
	/* A zero byte before editedName: the letter before the first, which
	   mnNameEntryAcceptsMark reads when a voicing mark is entered first. */
	text[0] = 0;
	{
		Sint32 i;

		for(i = 0; i < NAME_MAX_BYTES; i++) {
			letter = originalName[i];

			editedName[i] = letter;
			if(letter != 0) {
				bytes++;
				letterCnt++;
				if((Uint8)(letter - TEXT_MARK_VOICED) <= 1)
					letterCnt--;
			}
		}
	}
	editedName[NAME_MAX_BYTES] = 0;
	mnTextDrawString(originalName, nameWin, 0, 0);
	keyColumn = NAME_COMMAND_COLUMN;
	vramSlot = syVRAMCacheFindFreeEntry();
	keyRow = NAME_LAST_ROW;
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(vramSlot, 2 * 2 * TILE_SIZE_4BPP, PointingHandTiles);
		spr = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, keyboardWin, 0, 0);
		keyCursor.spr = spr;
		mnCursorSetPosImmediateStub(&keyCursor.cursor, keyboardWin->x * 8 + 140,
			keyboardWin->y * 8 + 92);
	}
	vramSlot = syVRAMCacheFindFreeEntry();
	if(vramSlot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(vramSlot, 2 * 2 * TILE_SIZE_4BPP, NameEntry_CaretTiles);
		spr = mnWindowSprCreate(
			vramSlot, WINDOW_SPRITE_SIZE_16X16, keyboardWin, 0, 0);
		nameCaret.spr = spr;
		spr->depth = OAM_DEPTH_FRONT;
		spr->spr.oam.pri = 0;
		/* The caret stands after the name: its letters are 10 pixels apart. */
		x = 72 + mnNameEntryCountLetters(editedName) * 10;
		mnCursorSetPosImmediateStub(&nameCaret.cursor, x, 22);
	}
	/* The key under the cursor flashes between WINDOW_PALETTE and
	   HIGHLIGHT_PALETTE, a copy of it with WINDOW_BACKGROUND_COLOR grey. */
	palette = (Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	while(1) {
		Sint32 width = 1;

		/* The command keys are wider than a letter key. */
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_SWITCH_ROW)
				width = 4;
			if(keyRow == NAME_DELETE_ROW)
				width = 3;
			if(keyRow == NAME_LAST_ROW)
				width = 3;
		}
		/* Flash the key under the cursor, then move the cursor and the
		   caret. */
		mnWindowSetInteriorPalette(
			keyboardWin, keyColumn, keyRow, width, 1, HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetInteriorPalette(
			keyboardWin, keyColumn, keyRow, width, 1, WINDOW_PALETTE);
		if(cursorDirty) {
			cursorDirty = FALSE;
			mnCursorSetStub(&keyCursor.cursor,
				(keyboardWin->x + keyColumn) * 8 - 7,
				(keyboardWin->y + keyRow) * 8 + 15, 3);
		}
		if(textDirty) {
			textDirty = FALSE;
			x = 72 + mnNameEntryCountLetters(editedName) * 10;
			mnCursorSetStub(&nameCaret.cursor, x, 22, 3);
		}
		mnCursorAdvanceStub(&keyCursor.cursor);
		mnCursorMoveTowardTargetStub(&nameCaret.cursor);
		{
			Uint32 frm = (FrameCount >> 1) & 7;

			spr = nameCaret.spr;
			spr->spr.oam.x = spr->x + NameEntryCaretWave[frm];
			spr->spr.oam.y = spr->y + NameEntryCaretWave[(frm + 5) & 7];
		}
		/* The D-pad moves over the keys, skipping the gaps between groups and
		   wrapping around; Start jumps to End. */
		if(PerRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow--;
			/* Letters skip the gap row; the command column cycles the switch,
			   Delete and End. */
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == -1)
					keyRow = NAME_LAST_ROW;
				else if(keyRow == NAME_GAP_ROW)
					keyRow = NAME_GAP_ROW - 1;
			} else {
				if(keyRow == -1) {
					keyRow = NAME_LAST_ROW;
				} else {
					keyRow = (keyRow != NAME_DELETE_ROW - 1);
					keyRow *= NAME_DELETE_ROW;
				}
			}
		}
		if(PerRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyRow++;
			if(keyColumn != NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_LAST_ROW + 1)
					keyRow = 0;
				if(keyRow == NAME_GAP_ROW)
					keyRow = NAME_GAP_ROW + 1;
			} else {
				if(keyRow == NAME_SWITCH_ROW + 1)
					keyRow = NAME_DELETE_ROW;
				else if(keyRow == NAME_DELETE_ROW + 1)
					keyRow = NAME_LAST_ROW;
				else
					keyRow = NAME_SWITCH_ROW;
			}
		}
		if(PerRepeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn--;
			if(keyColumn == -1) {
				if(keyRow == NAME_SWITCH_ROW || keyRow == NAME_DELETE_ROW ||
					keyRow == NAME_LAST_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = NAME_LAST_LETTER_COLUMN;
			} else if(NAME_IS_GAP_COLUMN(keyColumn)) {
				keyColumn--;
			}
		}
		if(PerRepeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN + 1)
				keyColumn = 0;
			else if(NAME_IS_GAP_COLUMN(keyColumn))
				keyColumn++;
			if(keyColumn == NAME_COMMAND_COLUMN) {
				if(keyRow == NAME_SWITCH_ROW || keyRow == NAME_DELETE_ROW ||
					keyRow == NAME_LAST_ROW)
					keyColumn = NAME_COMMAND_COLUMN;
				else
					keyColumn = 0;
			}
		}
		if(PerPush & KEY_START) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			cursorDirty = TRUE;
			keyColumn = NAME_COMMAND_COLUMN;
			keyRow = NAME_LAST_ROW;
		}
		if(PerPush & KEY_SELECT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
		change_keyboard:
			keyboard ^= 1;
			if(keyboard)
				mnWindowCopyTilemapRegion(
					keyboardWin, NameEntry_AlternateKeyboardTilemap);
			else
				mnWindowCopyTilemapRegion(
					keyboardWin, NameEntry_KeyboardTilemap);
			mnWindowDrawDividerLine(keyboardWin, 18, 0, 18, 12);
			state->dirty = 1;
			continue;
		}
		if(PerRepeat & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
		remove_character:
			/* A letter with a voicing mark goes with its mark. */
			if(letterCnt != 0) {
				letterCnt--;
				bytes--;
				letter = editedName[bytes];
				editedName[bytes] = 0;
				if((Uint8)(letter - TEXT_MARK_VOICED) <= 1) {
					bytes--;
					editedName[bytes] = 0;
				}
			} else {
				result = -1;
				break;
			}
			mnWindowSprPrepareForRedraw(nameWin);
			mnTextDrawString(editedName, nameWin, 0, 0);
			textDirty = TRUE;
			continue;
		}
		/* A: a command key, or a letter. */
		if(!(PerRepeat & KEY_A))
			continue;
		sySndPlayStub(SE_MENU_CONFIRM);
		if(keyColumn == NAME_COMMAND_COLUMN) {
			if(keyRow == NAME_LAST_ROW) {
				if(letterCnt == 0) {
					nameCaret.spr->motion = WINDOW_SPRITE_MOTION_HIDDEN;
					mnWindowSprPrepareForRedraw(nameWin);
					mnTextDrawString(originalName, nameWin, 0, 0);
					sySynch(10);
				} else {
					Uint8 *dst;
					Uint8 *src;
					Sint32 i;
					Uint8 *dstSave;
					Uint8 *srcSave;

					dst = originalName;
					src = editedName;
					i = 0;
					/* FAKEMATCH(dead): no-op saves. */
					dstSave = dst;
					srcSave = src;
					if(keyboardWin->height)
						dst = NULL;
					if(keyboardWin->height)
						dst = dstSave;
					if(keyboardWin->height)
						src = NULL;
					if(keyboardWin->height)
						src = srcSave;
					while(i < NAME_MAX_BYTES + 1) {
						*dst++ = *src++;
						i++;
					}
				}
				break;
			}
			if(keyRow == NAME_DELETE_ROW)
				goto remove_character;
			if(keyRow == NAME_SWITCH_ROW)
				goto change_keyboard;
			continue;
		}
		{
			Uint32 tx = keyboardWin->x + keyColumn + 1;
			Uint32 cell;
			Uint8 *dst;

			cell = ((keyboardWin->y + keyRow + 1) << 5) + tx;
			/* Key glyphs use the low byte of the mirrored tilemap entries. */
			letter = ((Uint8 *)&state->tilemap)[cell * 2];
			/* The keyboard's dakuten key. */
			if(letter == NAME_VOICED_KEY)
				letter = TEXT_MARK_VOICED;
			/* The keyboard's handakuten key. */
			if(letter == NAME_SEMIVOICED_KEY)
				letter = TEXT_MARK_SEMIVOICED;
			/* A full name still takes a voicing mark on its last letter. */
			if(letterCnt == NAME_MAX_LETTERS &&
				(Uint8)(letter - TEXT_MARK_VOICED) > 1)
				continue;
			dst = editedName + bytes;
			if(!mnNameEntryAcceptsMark(letter, editedName[bytes - 1])) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}
			*dst = letter;
			bytes++;
			editedName[bytes] = 0;
			letterCnt++;
			if((Uint8)(letter - TEXT_MARK_VOICED) <= 1)
				letterCnt--;
			if(letterCnt == NAME_MAX_LETTERS) {
				keyColumn = NAME_COMMAND_COLUMN;
				keyRow = NAME_LAST_ROW;
				cursorDirty = TRUE;
			}
			mnWindowSprPrepareForRedraw(nameWin);
			mnTextDrawString(editedName, nameWin, 0, 0);
			textDirty = TRUE;
		}
	}
	mnWindowClose(keyboardWin, 2);
	mnWindowClose(nameWin, 2);
	mnPortraitCloseWindow(member);
	sySynch(1);
	return result;
}
#endif
