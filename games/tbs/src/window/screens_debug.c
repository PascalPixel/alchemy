/* @file */
/* The debug menus (scenes, flags, icon browsers) and the Pause menu's */
/* choices. */
/* Part of screens.c. */

/* The last scene the debug scene selector offers, the last of the game's 201,
 */
/* and its last entrance. */
#define DEBUG_SCENE_LAST	200
#define DEBUG_ENTRANCE_LAST 99
/* BUG: an entrance moved past either end by DEBUG_MENU_JUMP wraps by */
/* DEBUG_ENTRANCE_LAST rather than by the DEBUG_ENTRANCE_LAST + 1 entrances, so
 */
/* 95 + 10 gives 6. */
#ifdef BUGFIX
#define DEBUG_ENTRANCE_WRAP (DEBUG_ENTRANCE_LAST + 1)
#else
#define DEBUG_ENTRANCE_WRAP DEBUG_ENTRANCE_LAST
#endif
/* How far L and R move in the debug menus, where Left and Right move by one. */
#define DEBUG_MENU_JUMP 10
/* The flag editor shows a flag number's three hex digits as its page, row and
 */
/* column, so it has sixteen of each. */
#define DEBUG_FLAG_DIGIT_VALUES 16
/* The icon browser's pages, each of ICON_BROWSER_PAGE_ICONS 16 by 16 icons in
 */
/* rows of ICON_BROWSER_COLUMNS. */
#define ICON_BROWSER_PAGES		8
#define ICON_BROWSER_PAGE_ICONS 32
#define ICON_BROWSER_COLUMNS	8

/* What the debug scene selector's keys return besides a scene. */
#define DEBUG_SCENE_GO	   -1 /* A: go to the scene and entrance shown */
#define DEBUG_SCENE_CANCEL -2 /* B: close without going */

/* What the icon browser shows, which L and R change. */
typedef enum ICON_BROWSER_MODE {
	ICON_BROWSER_ITEMS,
	ICON_BROWSER_PSYNERGY,
	ICON_BROWSER_STATUS,
	ICON_BROWSER_MODES
} ICON_BROWSER_MODE;

/* Unused: runs the debug scene and entrance selector as a task. */
UNUSED void mnDebugSceneSelectSchedule(void) {
	sySetEvent((TASK_FUNC)mnDebugSceneSelectRun, TASK_PASS_FRAME);
}

/* Draws the debug scene and entrance numbers with their scene code and area */
/* name. */
void mnDebugSceneSelectDrawRow(WINDOW *window, s32 sceneId,
							   const s16 *entrance) {
	s16 mapIndex = sceneId;
	s32 label = fdAreaGetNameIndexStub(mapIndex, *entrance) + MsgDebugAreaName;
	mnWindowSprPrepareForRedraw(window);
	mnTextDrawNum(mapIndex, 3, window, 0, 14);
	mnTextDrawNum(*entrance, 3, window, DEBUG_SCENE_SELECTOR_LABEL_X, 14);
	mnTextDrawString(Menu_ColonString, window, DEBUG_SCENE_SELECTOR_COLON_X, 0);
	mnTextDrawMessage(mapIndex + MsgDebugSceneCode, window, 0, 0);
	mnTextDrawString(Menu_ColonString, window, DEBUG_SCENE_SELECTOR_COLON_X,
					 14);
	mnTextDrawMessage(label, window, DEBUG_SCENE_SELECTOR_LABEL_X, 0);
}

/* The debug scene selector: a scene and an entrance number, starting from the
 */
/* scene being run (mnDebugSceneSelectHandleRowInput has the keys). */
#ifdef UBFIX
void mnDebugSceneSelectRun(void) {
#else
s16 mnDebugSceneSelectRun(void) {
#endif
	s32 cursorSlot;
	s16 selectedField;
	s16 entrance;
	MENU_SPR cursor;
	s16 sceneId;
	WINDOW *window;
	s16 result;

	window = 0;
	selectedField = 0;
	sceneId = MainRoutineStateWork.returnScene;
	entrance = MainRoutineStateWork.returnEntrance;
	window = mnWindowCreate(0, 7, 30, 5, WINDOW_FLAG_INSTANT);
	mnDebugSceneSelectDrawRow(window, sceneId, &entrance);
	mnArrowCreate(&cursor, &cursorSlot);

	/* Wait for the keys that opened it to be let go. */
	while(KeysHeld != 0)
		sySynch(1);

	for(;;) {
		result = mnDebugSceneSelectHandleRowInput(window, sceneId, &entrance,
												  &selectedField);
		if(result == DEBUG_SCENE_GO) {
			mnArrowRelease(cursorSlot);
			mnWindowClose(window, 2);
			fdEventSetDestinationStub(sceneId, entrance);
			RETURN_UNREAD(result);
		}
		if(result == DEBUG_SCENE_CANCEL) {
			mnArrowRelease(cursorSlot);
			mnWindowClose(window, 2);
			RETURN_UNREAD(result);
		}

		mnArrowSetPos(&cursor, DEBUG_SCENE_SELECTOR_COLON_X,
					  selectedField * 14 + DEBUG_SCENE_SELECTOR_FIRST_ROW_Y);
		sceneId = result;
		sySynch(1);
	}
}

/* Edits the debug scene or entrance by one frame's keys, returning the scene,
 */
/* DEBUG_SCENE_GO or DEBUG_SCENE_CANCEL. L and R move a scene by */
/* DEBUG_MENU_JUMP and set its entrance to 0; an entrance moved past either end
 */
/* wraps by DEBUG_ENTRANCE_WRAP. */
s32 mnDebugSceneSelectHandleRowInput(WINDOW *window, s16 sceneId, s16 *entrance,
									 s16 *selectedField) {
	if(KeysRepeat & KEY_A)
		return DEBUG_SCENE_GO;
	if(KeysRepeat & KEY_B)
		return DEBUG_SCENE_CANCEL;
	if(KeysRepeat & KEY_DOWN) {
		*selectedField ^= 1;
	}
	else if(KeysRepeat & KEY_UP) {
		*selectedField ^= 1;
	}
	else if(KeysRepeat & KEY_RIGHT) {
		if(*selectedField == 0) {
			sceneId++;
		}
		else {
			(*entrance)++;
			if(*entrance > DEBUG_ENTRANCE_LAST)
				*entrance = 0;
		}
		if(sceneId > DEBUG_SCENE_LAST)
			sceneId = 0;
		mnDebugSceneSelectDrawRow(window, sceneId, entrance);
	}
	else if(KeysRepeat & KEY_LEFT) {
		if(*selectedField == 0) {
			sceneId--;
		}
		else {
			(*entrance)--;
			if(*entrance < 0)
				*entrance = DEBUG_ENTRANCE_LAST;
		}
		if(sceneId < 0)
			sceneId = DEBUG_SCENE_LAST;
		mnDebugSceneSelectDrawRow(window, sceneId, entrance);
	}
	else if(KeysRepeat & KEY_R) {
		if(*selectedField == 0) {
			*entrance = 0;
			sceneId += DEBUG_MENU_JUMP;
		}
		else {
			*entrance += DEBUG_MENU_JUMP;
			if(*entrance > DEBUG_ENTRANCE_LAST)
				*entrance -= DEBUG_ENTRANCE_WRAP;
		}
		if(sceneId > DEBUG_SCENE_LAST)
			sceneId = 0;
		mnDebugSceneSelectDrawRow(window, sceneId, entrance);
	}
	else if(KeysRepeat & KEY_L) {
		if(*selectedField == 0) {
			*entrance = 0;
			sceneId -= DEBUG_MENU_JUMP;
		}
		else {
			*entrance -= DEBUG_MENU_JUMP;
			if(*entrance < 0)
				*entrance += DEBUG_ENTRANCE_WRAP;
		}
		if(sceneId < 0)
			sceneId = DEBUG_SCENE_LAST;
		mnDebugSceneSelectDrawRow(window, sceneId, entrance);
	}
	return sceneId;
}

/* Shows a 16x16 flag grid per page and processes input until closed. Starts on
 */
/* page 8 and always returns 0. */
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

/* Writes the lowest count digits of value, up to five, most significant first
 */
/* as uppercase hexadecimal in out, without a null terminator. */
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
void mnDebugFlagEditorDrawTable(WINDOW *window, s32 page) {
	s32 row;
	s32 y;
	s32 flag;
	char label[5];
	char bits[17];

	mnWindowSprPrepareForRedraw(window);
	mnTextDrawStringInWindow((u8 *)Menu_HexDigitsString, window, 48, 0);

	flag = page << 8;
	for(row = 0; row != DEBUG_FLAG_DIGIT_VALUES; row++) {
		s32 i;

		y = row * 8 + 16;

		for(i = 0; i != 5; i++) {
			label[i] = 0;
		}
		mnStringFormatHexDigits(flag, 3, label);
		mnTextDrawStringInWindow((u8 *)label, window, 0, y);
		mnTextDrawStringInWindow((u8 *)Menu_ColonString, window, 32, y);

		for(i = 0; i < DEBUG_FLAG_DIGIT_VALUES; i++) {
			s32 set = gmFlagGetStub(flag);
			bits[i] = (set != 0) + '0';
			flag++;
		}
		bits[i] = 0;
		mnTextDrawStringInWindow((u8 *)bits, window, 48, y);
	}
}

/* Moves within the flag grid, changes pages, or toggles the selected flag. */
/* Returns 1 when flags/page need redrawing, -1 to close, and 0 otherwise. */
s32 mnDebugFlagEditorHandleInput(WINDOW *window, s32 *page, s32 *cursor) {
	s32 *row = &cursor[1];

	if(KeysRepeat & KEY_A) {
		s32 flag = (((*page << 4) + *row) << 4) + cursor[0];

		if(gmFlagGetStub(flag))
			gmFlagClearStub(flag);
		else
			gmFlagSetStub(flag);
		return 1;
	}
	if((KeysPressed & KEY_B) || (KeysRepeat & KEY_SELECT))
		return -1;
	if(KeysRepeat & KEY_UP) {
		if(--*row < 0)
			*row = DEBUG_FLAG_DIGIT_VALUES - 1;
	}
	else if(KeysRepeat & KEY_DOWN) {
		if(++*row > DEBUG_FLAG_DIGIT_VALUES - 1)
			*row = 0;
	}
	else if(KeysRepeat & KEY_LEFT) {
		if(--cursor[0] < 0)
			cursor[0] = DEBUG_FLAG_DIGIT_VALUES - 1;
	}
	else if(KeysRepeat & KEY_RIGHT) {
		if(++cursor[0] > DEBUG_FLAG_DIGIT_VALUES - 1)
			cursor[0] = 0;
	}
	else if((KeysRepeat & KEY_L) && (KeysRepeat & KEY_START)) {
		*page -= DEBUG_MENU_JUMP;
		if(*page < 0)
			*page = DEBUG_FLAG_DIGIT_VALUES - 1;
		return 1;
	}
	else if((KeysRepeat & KEY_R) && (KeysRepeat & KEY_START)) {
		*page += DEBUG_MENU_JUMP;
		if(*page > DEBUG_FLAG_DIGIT_VALUES - 1)
			*page = 0;
		return 1;
	}
	else if(KeysRepeat & KEY_L) {
		if(--*page < 0)
			*page = DEBUG_FLAG_DIGIT_VALUES - 1;
		return 1;
	}
	else if(KeysRepeat & KEY_R) {
		if(++*page > DEBUG_FLAG_DIGIT_VALUES - 1)
			*page = 0;
		return 1;
	}
	return 0;
}

/* Unused: an empty function nothing calls. */
UNUSED void mnReservedNoOp(void) {}

/* Marks the Djinni found (FLAG_DJINNI), then gives it to party member */
/* `member`. */
void mnDjinnSetFoundFlagAndAdd(s32 member, s32 element, s32 index) {
	gmFlagSetStub(FLAG_DJINNI(element * DJINN_PER_ELEMENT + index));
	gmDjinnGiveStub(member, element, index);
}

/* The Pause menu: Save your quest, Sleep or Change settings. A cancelled save
 */
/* or settings menu reopens the Pause menu; cancelling the Pause menu returns */
/* -1. Sleep shows its message and sets SleepRequested. Otherwise returns */
/* 0. The field engine reaches it by stub call (field/field_sequence_run.c). */
s32 mnPauseMenuRun(void) {
	s32 result;

	result = mnPauseMenuChoose();
	if(result == -1) {
		return -1;
	}
	if(result == PAUSE_MENU_SAVE_QUEST) {
		if(mnSaveToChosenFile() == -1) {
			return mnPauseMenuRun();
		}
	}
	else if(result == PAUSE_MENU_SLEEP) {
		mnFieldMessageShow(MsgSleepMode, FIELD_MESSAGE_ZOOM);
		SleepRequested = result;
	}
	else if(result == PAUSE_MENU_CHANGE_SETTINGS) {
		if(mnSettingsMenuRun() == -1) {
			return mnPauseMenuRun();
		}
	}
	return 0;
}

/* Browses pages of item, Psynergy and status-pattern icons, suspending the */
/* field engine's actor system while the browser owns its window and cache */
/* entries. */
s32 mnDebugBrowserIcons(void) {
	s32 redraw;
	s32 tile;
	s32 page;
	s32 mode;
	WINDOW *window;
	s32 slot;
	s32 base;
	s32 i;
	s32 row;
	s32 x;
	s32 y;

	window = NULL;
	page = 0;
	mode = 0;
	redraw = 1;
	ActorSystemStateWork->suspended = redraw;
	sySynch(1);

	while(1) {
		if(KeysRepeat & KEY_LEFT) {
			page--;
			redraw = 1;
		}
		if(KeysRepeat & KEY_RIGHT) {
			redraw = 1;
			page++;
		}
		if(KeysRepeat & KEY_L) {
			mode--;
			redraw = 1;
		}
		if(KeysRepeat & KEY_R) {
			redraw = 1;
			mode++;
		}
		if(KeysRepeat & KEY_A)
			break;
		if(KeysRepeat & KEY_B)
			break;
		if(redraw != 0) {
			redraw = 0;
			page = (page + ICON_BROWSER_PAGES) % ICON_BROWSER_PAGES;
			mode = (mode + ICON_BROWSER_MODES) % ICON_BROWSER_MODES;
			mnWindowClose(window, 2);
			window = mnWindowCreate(10, 0, 18, 12, WINDOW_FLAG_INSTANT);
			if(mode == ICON_BROWSER_ITEMS)
				mnTextDrawStringInWindow(DebugMenu_ItemTitle, window, 0, 0);
			else if(mode == ICON_BROWSER_PSYNERGY)
				mnTextDrawStringInWindow(DebugMenu_EnergyTitle, window, 0, 0);
			else
				mnTextDrawStringInWindow(DebugMenu_StatusTitle, window, 0, 0);
			mnTextDrawStringInWindow(DebugMenu_PageLabel, window, 0, 8);
			mnTextDrawNumInWindow(page, 0, window, 40, 8);
			base = page * ICON_BROWSER_PAGE_ICONS;
			mnTextDrawNumInWindow(base, 3, window, 64, 8);
			mnTextDrawStringInWindow(DebugMenu_RangeDash, window, 88, 8);
			mnTextDrawNumInWindow(base + ICON_BROWSER_PAGE_ICONS - 1, 3, window,
								  96, 8);
			for(i = 0; i < ICON_BROWSER_PAGE_ICONS; i++) {
				slot = -1;
				row = i / ICON_BROWSER_COLUMNS;
				x = (i - row * ICON_BROWSER_COLUMNS) * 16;
				y = row * 16 + 16;
				if(mode == ICON_BROWSER_ITEMS) {
					mnIconBuildItemIconTiles(base + i, 1, &slot, &tile, mode);
					mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x,
									  y);
				}
				else if(mode == ICON_BROWSER_PSYNERGY) {
					mnIconBuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
					mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x,
									  y);
				}
				else {
					slot = syVramCacheFindFreeEntry();
					mnIconBuildMiscIconTiles(i, 0, slot);
					mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x,
									  y);
				}
			}
		}
		sySynch(1);
	}
	mnWindowClose(window, 2);
	ActorSystemStateWork->suspended = 0;
	return 0;
}

/* The message of the portrait browser's first label, which the German, */
/* French, Spanish and Italian editions point at other messages. */
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define DEBUG_GLYPH_NAME MsgDebugGlyphName
#elif LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
#define DEBUG_GLYPH_NAME MsgEnd
#else
#define DEBUG_GLYPH_NAME MsgDebugSecondForm
#endif

/* Browses the character and sprite portrait maps as one circular list. Shows */
/* each entry's glyph and label, then releases both windows on A or B. */
s32 mnDebugBrowserPortraits(void) {
	WIN_STATE *state;
	WINDOW *portrait;
	s32 redraw;
	WINDOW *window;
	s32 characterCount;
	s32 entryCount;
	s32 index;
	s32 i;
	s32 glyph;
	s32 tile;
	s32 slot;
	s32 portraitSlot = 1;
	s32 palette = PORTRAIT_PALETTE + portraitSlot;

	state = WINDOW_STATE;
	redraw = 1;
	portrait = mnPortraitOpenWindow(0, 0, 10, 5);
	window = mnWindowCreate(10, 10, 14, 3, WINDOW_FLAG_INSTANT);
	index = 0;
	for(i = 0; Portrait_CharacterMap[i * 2] != -1; i++) {
	}
	characterCount = i;
	for(i = 0; Portrait_SpriteMap[i * 2] != -1; i++) {
	}
	entryCount = characterCount + i;

	while(1) {
		if(KeysRepeat & KEY_LEFT) {
			redraw = 1;
			index--;
		}
		if(KeysRepeat & KEY_RIGHT) {
			redraw = 1;
			index++;
		}
		if(KeysRepeat & KEY_L) {
			redraw = 1;
			index -= DEBUG_MENU_JUMP;
		}
		if(KeysRepeat & KEY_R) {
			redraw = 1;
			index += DEBUG_MENU_JUMP;
		}
		if(KeysRepeat & KEY_A)
			break;
		if(KeysRepeat & KEY_B)
			break;
		if(redraw) {
			redraw = 0;
			index = (index + entryCount) % entryCount;
			mnWindowSprPrepareForRedraw(window);
			if(index < characterCount)
				glyph = Portrait_CharacterMap[index * 2 + 1];
			else
				glyph = Portrait_SpriteMap[(index - characterCount) * 2 + 1] +
						PORTRAIT_SPRITE_MAP_FIRST;
			slot = state->portraitSprs[portraitSlot];
			mnIconLoadWithPalette(glyph, 0, &slot, &tile, palette, 1);
			mnTextDrawNumInWindow(index, 2, window, 0, 0);
			mnTextDrawMessageAsTiles(index + DEBUG_GLYPH_NAME, window, 24, 0);
		}
		sySynch(1);
	}
	mnWindowClose(window, 2);
	mnWindowClose(portrait, 2);
	sySynch(1);
	return 0;
}
