/* The debug menus (scenes, flags, icon browsers) and the Pause menu's choices.
   Part of screens.c. */

/* The last scene the debug scene selector offers, the last of the game's 201,
   and its last entrance. */
#define DEBUG_SCENE_LAST	200
#define DEBUG_ENTRANCE_LAST 99
/* BUG: an entrance moved past either end by DEBUG_MENU_JUMP wraps by
   DEBUG_ENTRANCE_LAST rather than by the DEBUG_ENTRANCE_LAST + 1 entrances, so
   95 + 10 gives 6. */
#define DEBUG_ENTRANCE_WRAP \
	WHEN(FIX_DEBUG_ENTRANCE_JUMP_WRAP, (DEBUG_ENTRANCE_LAST + 1), \
		DEBUG_ENTRANCE_LAST)
/* How far L and R move in the debug menus, where Left and Right move by one. */
#define DEBUG_MENU_JUMP			10
/* The flag editor shows a flag number's three hex digits as its page, row and
   column, so it has sixteen of each. */
#define DEBUG_FLAG_DIGIT_VALUES 16
/* The icon browser's pages, each of ICON_BROWSER_PAGE_ICONS 16 by 16 icons in
   rows of ICON_BROWSER_COLUMNS. */
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

/* Unused: runs the debug scene and entrance selector as an event. */
UNUSED void mnDebugSceneSelectSchedule(void)
{
	sySetEvent((EVENT_FUNC)mnDebugSceneSelectRun, EVENT_PASS_TICK);
}

/* Draws the debug scene and entrance numbers with their scene code and area
   name. */
void mnDebugSceneSelectDrawRow(
	WINDOW *win, Sint32 sceneId, const Sint16 *entrance)
{
	Sint16 mapIdx = sceneId;
	Sint32 label = fdAreaGetNameIndexStub(mapIdx, *entrance) + MsgDebugAreaName;
	mnWindowSprPrepareForRedraw(win);
	mnTextDrawNum(mapIdx, 3, win, 0, 14);
	mnTextDrawNum(*entrance, 3, win, DEBUG_SCENE_SELECTOR_LABEL_X, 14);
	mnTextDrawString(MenuColonString, win, DEBUG_SCENE_SELECTOR_COLON_X, 0);
	mnTextDrawMessage(mapIdx + MsgDebugSceneCode, win, 0, 0);
	mnTextDrawString(MenuColonString, win, DEBUG_SCENE_SELECTOR_COLON_X, 14);
	mnTextDrawMessage(label, win, DEBUG_SCENE_SELECTOR_LABEL_X, 0);
}

/* The debug scene selector: a scene and an entrance number, starting from the
   scene being run (mnDebugSceneSelectHandleRowInput has the keys). */
#if FIX_FUNCTION_SIGNATURES
void mnDebugSceneSelectRun(void)
{
#else
Sint16 mnDebugSceneSelectRun(void)
{
#endif
	Sint32	 cursorSlot;
	Sint16	 selectedField;
	Sint16	 entrance;
	MENU_SPR cursor;
	Sint16	 sceneId;
	WINDOW	*win;
	Sint16	 result;

	win = 0;
	selectedField = 0;
	sceneId = MainRoutineStateWork.returnScene;
	entrance = MainRoutineStateWork.returnEntrance;
	win = mnWindowCreate(0, 7, 30, 5, WINDOW_FLAG_INSTANT);
	mnDebugSceneSelectDrawRow(win, sceneId, &entrance);
	mnArrowCreate(&cursor, &cursorSlot);

	/* Wait for the keys that opened it to be let go. */
	while(PerData != 0)
		sySynch(1);

	for(;;) {
		result = mnDebugSceneSelectHandleRowInput(
			win, sceneId, &entrance, &selectedField);
		if(result == DEBUG_SCENE_GO) {
			mnArrowRelease(cursorSlot);
			mnWindowClose(win, 2);
			fdScriptSetDestinationStub(sceneId, entrance);
			RETURN_UNREAD(result);
		}
		if(result == DEBUG_SCENE_CANCEL) {
			mnArrowRelease(cursorSlot);
			mnWindowClose(win, 2);
			RETURN_UNREAD(result);
		}

		mnArrowSetPos(&cursor, DEBUG_SCENE_SELECTOR_COLON_X,
			selectedField * 14 + DEBUG_SCENE_SELECTOR_FIRST_ROW_Y);
		sceneId = result;
		sySynch(1);
	}
}

/* Edits the debug scene or entrance by one tick's keys, returning the scene,
   DEBUG_SCENE_GO or DEBUG_SCENE_CANCEL. L and R move a scene by DEBUG_MENU_JUMP
   and set its entrance to 0; an entrance moved past either end wraps by
   DEBUG_ENTRANCE_WRAP. */
Sint32 mnDebugSceneSelectHandleRowInput(
	WINDOW *win, Sint16 sceneId, Sint16 *entrance, Sint16 *selectedField)
{
	if(PerRepeat & KEY_A)
		return DEBUG_SCENE_GO;
	if(PerRepeat & KEY_B)
		return DEBUG_SCENE_CANCEL;
	if(PerRepeat & KEY_DOWN) {
		*selectedField ^= 1;
	} else if(PerRepeat & KEY_UP) {
		*selectedField ^= 1;
	} else if(PerRepeat & KEY_RIGHT) {
		if(*selectedField == 0) {
			sceneId++;
		} else {
			(*entrance)++;
			if(*entrance > DEBUG_ENTRANCE_LAST)
				*entrance = 0;
		}
		if(sceneId > DEBUG_SCENE_LAST)
			sceneId = 0;
		mnDebugSceneSelectDrawRow(win, sceneId, entrance);
	} else if(PerRepeat & KEY_LEFT) {
		if(*selectedField == 0) {
			sceneId--;
		} else {
			(*entrance)--;
			if(*entrance < 0)
				*entrance = DEBUG_ENTRANCE_LAST;
		}
		if(sceneId < 0)
			sceneId = DEBUG_SCENE_LAST;
		mnDebugSceneSelectDrawRow(win, sceneId, entrance);
	} else if(PerRepeat & KEY_R) {
		if(*selectedField == 0) {
			*entrance = 0;
			sceneId += DEBUG_MENU_JUMP;
		} else {
			*entrance += DEBUG_MENU_JUMP;
			if(*entrance > DEBUG_ENTRANCE_LAST)
				*entrance -= DEBUG_ENTRANCE_WRAP;
		}
		if(sceneId > DEBUG_SCENE_LAST)
			sceneId = 0;
		mnDebugSceneSelectDrawRow(win, sceneId, entrance);
	} else if(PerRepeat & KEY_L) {
		if(*selectedField == 0) {
			*entrance = 0;
			sceneId -= DEBUG_MENU_JUMP;
		} else {
			*entrance -= DEBUG_MENU_JUMP;
			if(*entrance < 0)
				*entrance += DEBUG_ENTRANCE_WRAP;
		}
		if(sceneId < 0)
			sceneId = DEBUG_SCENE_LAST;
		mnDebugSceneSelectDrawRow(win, sceneId, entrance);
	}
	return sceneId;
}

/* Shows a 16x16 flag grid per page and processes input until closed. Starts on
   page 8 and always returns 0. */
Sint32 mnDebugFlagEditorRun(void)
{
	MENU_SPR cursorSpr;
	Sint32	 cursor[2]; /* column and row */
	Sint32	 cursorSlot;
	Sint32	 page;
	WINDOW	*win;
	Sint32	 change;

	page = 8;
	win = 0;
	cursor[0] = 0;
	cursor[1] = 0;
	win = mnWindowCreate(1, 0, 28, 20, WINDOW_FLAG_INSTANT);
	mnDebugFlagEditorDrawTable(win, page);
	mnArrowCreate(&cursorSpr, &cursorSlot);
	for(;;) {
		sySynch(1);
		change = mnDebugFlagEditorHandleInput(win, &page, cursor);
		/* B closes the table; a change of flag or page redraws it. */
		if(change == -1)
			break;
		if(change == 1)
			mnDebugFlagEditorDrawTable(win, page);
		mnArrowSetPos(&cursorSpr, cursor[0] * 8 + 58, cursor[1] * 8 + 20);
	}
	mnArrowRelease(cursorSlot);
	mnWindowClose(win, 2);
	return 0;
}

/* Writes the lowest count digits of value, up to five, most significant first
   as uppercase hexadecimal in out, without a null terminator. */
void mnStringFormatHexDigits(Uint32 value, Uint32 cnt, Uint8 *out)
{
	Uint8  digits[8];
	Uint32 i;

	if(cnt > 5)
		cnt = 5;

	for(i = 0; i != cnt; i++) {
		Uint32 digit = value & 0xf;
		if(digit <= 9)
			digits[i] = digit + '0';
		else
			digits[i] = digit + 'A' - 10;
		value >>= 4;
	}

	for(i = cnt - 1; (Sint32)i >= 0; i--)
		*out++ = digits[i];
}

/* Draws one page of 256 flags as sixteen rows of hexadecimal labels and
   bits. */
void mnDebugFlagEditorDrawTable(WINDOW *win, Sint32 page)
{
	Sint32 row;
	Sint32 y;
	Sint32 flg;
	char   label[5];
	char   bits[17];

	mnWindowSprPrepareForRedraw(win);
	mnTextDrawStringInWindow((Uint8 *)MenuHexDigitsString, win, 48, 0);

	flg = page << 8;
	for(row = 0; row != DEBUG_FLAG_DIGIT_VALUES; row++) {
		Sint32 i;

		y = row * 8 + 16;

		for(i = 0; i != 5; i++) {
			label[i] = 0;
		}
		mnStringFormatHexDigits(flg, 3, label);
		mnTextDrawStringInWindow((Uint8 *)label, win, 0, y);
		mnTextDrawStringInWindow((Uint8 *)MenuColonString, win, 32, y);

		for(i = 0; i < DEBUG_FLAG_DIGIT_VALUES; i++) {
			Sint32 set = gmFlagGetStub(flg);
			bits[i] = (set != 0) + '0';
			flg++;
		}
		bits[i] = 0;
		mnTextDrawStringInWindow((Uint8 *)bits, win, 48, y);
	}
}

/* Moves within the flag grid, changes pages, or toggles the selected flag.
   Returns 1 when flags/page need redrawing, -1 to close, and 0 otherwise. */
Sint32 mnDebugFlagEditorHandleInput(WINDOW *win, Sint32 *page, Sint32 *cursor)
{
	Sint32 *row = &cursor[1];

	if(PerRepeat & KEY_A) {
		Sint32 flg = (((*page << 4) + *row) << 4) + cursor[0];

		if(gmFlagGetStub(flg))
			gmFlagClearStub(flg);
		else
			gmFlagSetStub(flg);
		return 1;
	}
	if((PerPush & KEY_B) || (PerRepeat & KEY_SELECT))
		return -1;
	if(PerRepeat & KEY_UP) {
		if(--*row < 0)
			*row = DEBUG_FLAG_DIGIT_VALUES - 1;
	} else if(PerRepeat & KEY_DOWN) {
		if(++*row > DEBUG_FLAG_DIGIT_VALUES - 1)
			*row = 0;
	} else if(PerRepeat & KEY_LEFT) {
		if(--cursor[0] < 0)
			cursor[0] = DEBUG_FLAG_DIGIT_VALUES - 1;
	} else if(PerRepeat & KEY_RIGHT) {
		if(++cursor[0] > DEBUG_FLAG_DIGIT_VALUES - 1)
			cursor[0] = 0;
	} else if((PerRepeat & KEY_L) && (PerRepeat & KEY_START)) {
		*page -= DEBUG_MENU_JUMP;
		if(*page < 0)
			*page = DEBUG_FLAG_DIGIT_VALUES - 1;
		return 1;
	} else if((PerRepeat & KEY_R) && (PerRepeat & KEY_START)) {
		*page += DEBUG_MENU_JUMP;
		if(*page > DEBUG_FLAG_DIGIT_VALUES - 1)
			*page = 0;
		return 1;
	} else if(PerRepeat & KEY_L) {
		if(--*page < 0)
			*page = DEBUG_FLAG_DIGIT_VALUES - 1;
		return 1;
	} else if(PerRepeat & KEY_R) {
		if(++*page > DEBUG_FLAG_DIGIT_VALUES - 1)
			*page = 0;
		return 1;
	}
	return 0;
}

/* Unused: an empty function nothing calls. */
UNUSED void mnReservedNoOperation(void)
{
}

/* Marks the Djinni found (FLAG_DJINNI), then gives it to party member
   `member`. */
void mnDjinnSetFoundFlagAndAdd(Sint32 member, Sint32 element, Sint32 idx)
{
	gmFlagSetStub(FLAG_DJINNI(element * DJINN_PER_ELEMENT + idx));
	gmDjinnGiveStub(member, element, idx);
}

/* The Pause menu: Save your quest, Sleep or Change settings. A cancelled save
   or settings menu reopens the Pause menu; cancelling the Pause menu returns
   -1. Sleep shows its message and sets SleepRequested. Otherwise returns
   0. The field engine reaches it by stub call (field/field_mode_run.c). */
Sint32 mnPauseMenuRun(void)
{
	Sint32 result;

	result = mnPauseMenuChoose();
	if(result == -1) {
		return -1;
	}
	if(result == PAUSE_MENU_SAVE_QUEST) {
		if(mnSaveToChosenFile() == -1) {
			return mnPauseMenuRun();
		}
	} else if(result == PAUSE_MENU_SLEEP) {
		mnFieldMessageShow(MsgSleepMode, FIELD_MESSAGE_ZOOM);
		SleepRequested = result;
	} else if(result == PAUSE_MENU_CHANGE_SETTINGS) {
		if(mnSettingsMenuRun() == -1) {
			return mnPauseMenuRun();
		}
	}
	return 0;
}

/* Browses pages of item, Psynergy and status-pattern icons, suspending the
   field engine's actor system while the browser owns its window and cache
   entries. */
Sint32 mnDebugBrowserIcons(void)
{
	Bool	redraw;
	Sint32	tile;
	Sint32	page;
	Sint32	mode;
	WINDOW *win;
	Sint32	slot;
	Sint32	base;
	Sint32	i;
	Sint32	row;
	Sint32	x;
	Sint32	y;

	win = NULL;
	page = 0;
	mode = 0;
	redraw = TRUE;
	ActorSystemStateWork->suspended = redraw;
	sySynch(1);

	while(1) {
		if(PerRepeat & KEY_LEFT) {
			page--;
			redraw = TRUE;
		}
		if(PerRepeat & KEY_RIGHT) {
			redraw = TRUE;
			page++;
		}
		if(PerRepeat & KEY_L) {
			mode--;
			redraw = TRUE;
		}
		if(PerRepeat & KEY_R) {
			redraw = TRUE;
			mode++;
		}
		if(PerRepeat & KEY_A)
			break;
		if(PerRepeat & KEY_B)
			break;
		if(redraw != 0) {
			redraw = FALSE;
			page = (page + ICON_BROWSER_PAGES) % ICON_BROWSER_PAGES;
			mode = (mode + ICON_BROWSER_MODES) % ICON_BROWSER_MODES;
			mnWindowClose(win, 2);
			win = mnWindowCreate(10, 0, 18, 12, WINDOW_FLAG_INSTANT);
			if(mode == ICON_BROWSER_ITEMS)
				mnTextDrawStringInWindow(DebugMenuItemTitle, win, 0, 0);
			else if(mode == ICON_BROWSER_PSYNERGY)
				mnTextDrawStringInWindow(DebugMenuEnergyTitle, win, 0, 0);
			else
				mnTextDrawStringInWindow(DebugMenuStatusTitle, win, 0, 0);
			mnTextDrawStringInWindow(DebugMenuPageLabel, win, 0, 8);
			mnTextDrawNumInWindow(page, 0, win, 40, 8);
			base = page * ICON_BROWSER_PAGE_ICONS;
			mnTextDrawNumInWindow(base, 3, win, 64, 8);
			mnTextDrawStringInWindow(DebugMenuRangeDash, win, 88, 8);
			mnTextDrawNumInWindow(
				base + ICON_BROWSER_PAGE_ICONS - 1, 3, win, 96, 8);
			for(i = 0; i < ICON_BROWSER_PAGE_ICONS; i++) {
				slot = -1;
				row = i / ICON_BROWSER_COLUMNS;
				x = (i - row * ICON_BROWSER_COLUMNS) * 16;
				y = row * 16 + 16;
				if(mode == ICON_BROWSER_ITEMS) {
					mnIconBuildItemIconTiles(base + i, 1, &slot, &tile, mode);
					mnWindowSprCreate(
						slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
				} else if(mode == ICON_BROWSER_PSYNERGY) {
					mnIconBuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
					mnWindowSprCreate(
						slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
				} else {
					slot = syVRAMCacheFindFreeEntry();
					mnIconBuildMiscIconTiles(i, 0, slot);
					mnWindowSprCreate(
						slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
				}
			}
		}
		sySynch(1);
	}
	mnWindowClose(win, 2);
	ActorSystemStateWork->suspended = 0;
	return 0;
}

/* The message of the portrait browser's first label, which the German, French,
   Spanish and Italian editions point at other messages. */
#define DEBUG_GLYPH_NAME \
	BY_LANGUAGE(MsgDebugGlyphName, MsgDebugGlyphName, MsgEnd, \
		MsgDebugSecondForm, MsgEnd, MsgDebugSecondForm)

/* Browses the character and sprite portrait maps as one circular list. Shows
   each entry's glyph and label, then releases both windows on A or B. */
Sint32 mnDebugBrowserPortraits(void)
{
	WIN_STATE *state;
	WINDOW	  *portrait;
	Bool	   redraw;
	WINDOW	  *win;
	Sint32	   characterCnt;
	Sint32	   entryCnt;
	Sint32	   idx;
	Sint32	   i;
	Sint32	   glyph;
	Sint32	   tile;
	Sint32	   slot;
	Sint32	   portraitSlot = 1;
	Sint32	   palette = PORTRAIT_PALETTE + portraitSlot;

	state = WINDOW_STATE;
	redraw = TRUE;
	portrait = mnPortraitOpenWindow(0, 0, 10, 5);
	win = mnWindowCreate(10, 10, 14, 3, WINDOW_FLAG_INSTANT);
	idx = 0;
	for(i = 0; PortraitCharacterMap[i * 2] != -1; i++) {
	}
	characterCnt = i;
	for(i = 0; PortraitSpriteMap[i * 2] != -1; i++) {
	}
	entryCnt = characterCnt + i;

	while(1) {
		if(PerRepeat & KEY_LEFT) {
			redraw = TRUE;
			idx--;
		}
		if(PerRepeat & KEY_RIGHT) {
			redraw = TRUE;
			idx++;
		}
		if(PerRepeat & KEY_L) {
			redraw = TRUE;
			idx -= DEBUG_MENU_JUMP;
		}
		if(PerRepeat & KEY_R) {
			redraw = TRUE;
			idx += DEBUG_MENU_JUMP;
		}
		if(PerRepeat & KEY_A)
			break;
		if(PerRepeat & KEY_B)
			break;
		if(redraw) {
			redraw = FALSE;
			idx = (idx + entryCnt) % entryCnt;
			mnWindowSprPrepareForRedraw(win);
			if(idx < characterCnt)
				glyph = PortraitCharacterMap[idx * 2 + 1];
			else
				glyph = PortraitSpriteMap[(idx - characterCnt) * 2 + 1] +
					PORTRAIT_SPRITE_MAP_FIRST;
			slot = state->portraitSprs[portraitSlot];
			mnIconLoadWithPalette(glyph, 0, &slot, &tile, palette, 1);
			mnTextDrawNumInWindow(idx, 2, win, 0, 0);
			mnTextDrawMessageAsTiles(idx + DEBUG_GLYPH_NAME, win, 24, 0);
		}
		sySynch(1);
	}
	mnWindowClose(win, 2);
	mnWindowClose(portrait, 2);
	sySynch(1);
	return 0;
}
