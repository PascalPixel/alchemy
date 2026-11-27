/* @file */
/* The debug menus' keys, the flag browser, and hexadecimal numbers written as
 */
/* text. */
/* Part of screens.c. */

/* The debug destination menu's range: its scene runs 0 to DEBUG_SCENE_MAX, */
/* its entrance 0 to DEBUG_ENTRANCE_MAX. */
#define DEBUG_SCENE_MAX	   324
#define DEBUG_ENTRANCE_MAX 99

/* The flag browser's pages of DEBUG_FLAG_ROWS rows of DEBUG_FLAGS_PER_ROW */
/* flags, 256 a page: every game flag. */
#define DEBUG_FLAG_PAGES	16
#define DEBUG_FLAG_ROWS		16
#define DEBUG_FLAGS_PER_ROW 16

/* Edits the debug scene or entrance, returning -1 to exit and -2 to */
/* cancel, else the scene after this frame's keys. Up and Down switch the */
/* field (`field`: 0 scene, 1 entrance), Left and Right step by one and wrap */
/* at either end. R and L step a scene by DEBUG_LARGE_STEP, setting the */
/* entrance to 0, and past either end go to the other; they step an entrance */
/* as far, wrapping by DEBUG_ENTRANCE_MAX (95 + 10 gives 6). */
s32 mnDebugSceneSelectHandleRowInput(s32 window, s16 scene, s16 *entrance,
									 s16 *field) {
	if(InputWork.repeat & KEY_A)
		return -1;
	if(InputWork.repeat & KEY_B)
		return -2;
	if(InputWork.repeat & KEY_DOWN) {
		*field ^= 1;
	}
	else if(InputWork.repeat & KEY_UP) {
		*field ^= 1;
	}
	else if(InputWork.repeat & KEY_RIGHT) {
		if(*field == 0) {
			scene++;
		}
		else {
			(*entrance)++;
			if(*entrance > DEBUG_ENTRANCE_MAX)
				*entrance = 0;
		}
		if(scene > DEBUG_SCENE_MAX)
			scene = 0;
		mnDebugSceneSelectDrawRow(window, scene, entrance);
	}
	else if(InputWork.repeat & KEY_LEFT) {
		if(*field == 0) {
			scene--;
		}
		else {
			(*entrance)--;
			if(*entrance < 0)
				*entrance = DEBUG_ENTRANCE_MAX;
		}
		if(scene < 0)
			scene = DEBUG_SCENE_MAX;
		mnDebugSceneSelectDrawRow(window, scene, entrance);
	}
	else if(InputWork.repeat & KEY_R) {
		if(*field == 0) {
			*entrance = 0;
			scene += DEBUG_LARGE_STEP;
		}
		else {
			*entrance += DEBUG_LARGE_STEP;
			if(*entrance > DEBUG_ENTRANCE_MAX)
				*entrance -= DEBUG_ENTRANCE_MAX;
		}
		if(scene > DEBUG_SCENE_MAX)
			scene = 0;
		mnDebugSceneSelectDrawRow(window, scene, entrance);
	}
	else if(InputWork.repeat & KEY_L) {
		if(*field == 0) {
			*entrance = 0;
			scene -= DEBUG_LARGE_STEP;
		}
		else {
			*entrance -= DEBUG_LARGE_STEP;
			if(*entrance < 0)
				*entrance += DEBUG_ENTRANCE_MAX;
		}
		if(scene < 0)
			scene = DEBUG_SCENE_MAX;
		mnDebugSceneSelectDrawRow(window, scene, entrance);
	}
	return scene;
}

/* The flag browser: shows a page of flags and takes input until closed, */
/* starting on page 8. Returns 0. */
s32 mnDebugFlagEditorRun(void) {
	MENU_SPR cursorSprite;
	s32 cursor[2]; /* column and row */
	s32 cursorSlot;
	s32 page;
	WINDOW *window;
	s32 change;

	page = 8;
	window = 0;
	cursor[0] = 0;
	cursor[1] = 0;
	window = mnWindowCreate(1, 0, 28, 20, WINDOW_FLAG_INSTANT);
	mnDebugFlagEditorDrawTable(window, page);
	mnArrowCreate(&cursorSprite, &cursorSlot);
	for(;;) {
		sySynch(1);
		change = mnDebugFlagEditorHandleInput(window, &page, cursor);
		/* B closes the table; a change of flag or page redraws it. */
		if(change == -1)
			break;
		if(change == 1)
			mnDebugFlagEditorDrawTable(window, page);
		mnArrowSetPos(&cursorSprite, cursor[0] * 8 + 58, cursor[1] * 8 + 20);
	}
	mnArrowRelease(cursorSlot);
	mnWindowClose(window, 2);
	return 0;
}

/* Writes the lowest count digits of value (up to five), most significant */
/* first, to out as uppercase hexadecimal, without a terminating zero. */
void mnStringFormatHexDigits(u32 value, u32 count, u8 *out) {
	u8 digits[8];
	u32 i;

	if(count > 5)
		count = 5;

	for(i = 0; i != count; i++) {
		u32 digit = value & 0xf;
		if(digit <= 9)
			digits[i] = digit + '0';
		else
			digits[i] = digit + 'A' - 10;
		value >>= 4;
	}

	for(i = count - 1; (s32)i >= 0; i--)
		*out++ = digits[i];
}

/* Draws one page of 256 flags as sixteen rows of hexadecimal labels and bits.
 */
/* Each row: its first flag's number in three hex digits, then its 16 flags */
/* as 0 or 1, lowest first, under a heading of hex digits. */
void mnDebugFlagEditorDrawTable(s32 window, s32 page) {
	s32 row;
	s32 y;
	s32 flag;
	u8 label[5];
	char bits[17];

	/* UB: called with no argument; the window it redraws is this one, left */
	/* where the argument would be. The ROM's code makes the call with no */
	/* declaration in scope; the cast to an unprototyped function keeps it so.
	 */
#ifdef UBFIX
	mnWindowSprPrepareForRedraw(window);
#else
	((void (*)())mnWindowSprPrepareForRedraw)();
#endif
	mnTextDrawStringInWindow((s32)Menu_HexDigitsString, window, 48, 0);

	flag = page << 8;
	for(row = 0; row != DEBUG_FLAG_ROWS; row++) {
		s32 i;

		y = row * 8 + 16;

		for(i = 0; i != 5; i++) {
			label[i] = 0;
		}
		mnStringFormatHexDigits(flag, 3, label);
		mnTextDrawStringInWindow((s32)label, window, 0, y);
		mnTextDrawStringInWindow((s32)Menu_ColonString, window, 32, y);

		for(i = 0; i < DEBUG_FLAGS_PER_ROW; i++) {
			s32 set = syGameFlagGet(flag);
			bits[i] = (set != 0) + '0';
			flag++;
		}
		bits[i] = 0;
		mnTextDrawStringInWindow((s32)bits, window, 48, y);
	}
}

/* Moves within the flag grid, changes pages, or toggles the selected flag. */
/* Returns 1 when flags/page need redrawing, -1 to close, and 0 otherwise. */
/* A toggles flag page * 256 + row * 16 + column; B or Select closes. L and */
/* R turn a page, wrapping at either end; with Start held they jump */
/* DEBUG_LARGE_STEP, to the other end when that passes it (5 - 10 gives 15). */
s32 mnDebugFlagEditorHandleInput(s32 window, s32 *page, s32 *cursor) {
	s32 *row = &cursor[1];

	if(InputWork.repeat & KEY_A) {
		s32 flag = ((*page << 4) + *row << 4) + cursor[0];

		if(syGameFlagGet(flag))
			syGameFlagClear(flag);
		else
			syGameFlagSet(flag);
		return 1;
	}
	if((InputWork.pressed & KEY_B) || (InputWork.repeat & KEY_SELECT))
		return -1;
	if(InputWork.repeat & KEY_UP) {
		if(--*row < 0)
			*row = DEBUG_FLAG_ROWS - 1;
	}
	else if(InputWork.repeat & KEY_DOWN) {
		if(++*row > DEBUG_FLAG_ROWS - 1)
			*row = 0;
	}
	else if(InputWork.repeat & KEY_LEFT) {
		if(--cursor[0] < 0)
			cursor[0] = DEBUG_FLAGS_PER_ROW - 1;
	}
	else if(InputWork.repeat & KEY_RIGHT) {
		if(++cursor[0] > DEBUG_FLAGS_PER_ROW - 1)
			cursor[0] = 0;
	}
	else if((InputWork.repeat & KEY_L) && (InputWork.repeat & KEY_START)) {
		*page -= DEBUG_LARGE_STEP;
		if(*page < 0)
			*page = DEBUG_FLAG_PAGES - 1;
		return 1;
	}
	else if((InputWork.repeat & KEY_R) && (InputWork.repeat & KEY_START)) {
		*page += DEBUG_LARGE_STEP;
		if(*page > DEBUG_FLAG_PAGES - 1)
			*page = 0;
		return 1;
	}
	else if(InputWork.repeat & KEY_L) {
		if(--*page < 0)
			*page = DEBUG_FLAG_PAGES - 1;
		return 1;
	}
	else if(InputWork.repeat & KEY_R) {
		if(++*page > DEBUG_FLAG_PAGES - 1)
			*page = 0;
		return 1;
	}
	return 0;
}

/* Unused: an empty function, the twin of the window object's */
/* mnSelectionMenuReservedNoOp. Nothing calls it. */
UNUSED void mnReservedNoOp(void) {}

/* Sets the Djinni's collected flag, then adds it to the specified party */
/* member. The flag is FLAG_DJINN_FIRST + DJINN_PER_ELEMENT * element + index.
 */
void mnDjinnSetFoundFlagAndAdd(s32 member, s32 element, s32 index) {
	syGameFlagSet((element * DJINN_PER_ELEMENT) + index + FLAG_DJINN_FIRST);
	gmDjinnGiveStub(member, element, index);
}
