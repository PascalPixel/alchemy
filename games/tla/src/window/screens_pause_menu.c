/* @file */
/* The Pause menu's choices, a tilemap palette-bit rectangle, and the debug */
/* browsers of icons and portraits. */
/* Part of screens.c. */

/* Handles Save your quest, Sleep and Change settings from the Pause menu. */
/* Saving is refused, with its message, while FLAG_CANNOT_SAVE_HERE or */
/* FLAG_NO_PLACE_TO_SAVE is set. A refused or cancelled save or a cancelled */
/* settings menu reopens the Pause menu on the same entry; cancelling the Pause
 */
/* menu returns -1. Sleep shows its message and sets SleepRequested. Otherwise
 */
/* returns 0. */
s32 mnPauseMenuRun(void) {
	s32 selection = 0;

again:
	selection = mnPauseMenuChoose(selection);
	if(selection == -1)
		return -1;
	if(selection == PAUSE_MENU_SAVE_QUEST) {
		if(syGameFlagGet(FLAG_CANNOT_SAVE_HERE)) {
			/* "Cannot save here" */
			mnFieldMessageShow((s32)MsgCannotSaveHere, FIELD_MESSAGE_ZOOM);
			goto again;
		}
		if(syGameFlagGet(FLAG_NO_PLACE_TO_SAVE)) {
			/* "This is no place for that!" */
			mnFieldMessageShow((s32)MsgNoPlaceForThat, FIELD_MESSAGE_ZOOM);
			goto again;
		}
		if(mnSaveToChosenFile() == -1)
			goto again;
	}
	else if(selection == PAUSE_MENU_SLEEP) {
		/* "Activating Sleep Mode. Press Select and the L & R Buttons to exit."
		 */
		mnFieldMessageShow((s32)MsgSleepMode, FIELD_MESSAGE_ZOOM);
		SleepRequested = selection;
	}
	else if(selection == PAUSE_MENU_CHANGE_SETTINGS) {
		if(mnSettingsMenuRun() == -1)
			goto again;
	}
	return 0;
}

/* Sets (`set` 1) or clears (0) bit 12 of the tilemap entries of the `width` x
 */
/* `height` tile rectangle at (x, y) inside `window`, clipped to the screen: */
/* bit 12 is the lowest bit of the entry's palette number, so this switches the
 */
/* rectangle between two palettes, such as 15 and 14. Nothing calls it. */
UNUSED void mnWindowSetTextTileBitRect(const WINDOW *window, s32 x, s32 y,
									   s32 width, s32 height, u32 set) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;

	x += window->x + 1;
	y += window->y + 1;
	set &= 1;
	set = BG_TILE_PALETTE(set);
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
	/* BUG: tests the bottom against the screen's width, so a window reaching */
	/* below the screen draws into the map's unseen rows. */
#ifdef BUGFIX
	if(y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}
	if(width > 0 && height > 0) {
		do {
			u16 *cell = &state->tilemap[y][x];
			s32 remaining = width;
			while(remaining != 0) {
				u32 entry = *cell;
				entry &= ~BG_TILE_PALETTE(1);
				entry |= set;
				remaining--;
				*cell = entry;
				cell++;
			}
			height--;
			y++;
		} while(height != 0);
		state->dirty = 1;
	}
}

/* The icon browser's kinds (items, Psynergy and status patterns), and its */
/* pages of ICON_BROWSER_PAGE_ICONS icons, ICON_BROWSER_ROW_ICONS to a row. */
#define ICON_BROWSER_KINDS		3
#define ICON_BROWSER_PAGES		12
#define ICON_BROWSER_PAGE_ICONS 32
#define ICON_BROWSER_ROW_ICONS	8

/* Browses pages of item, Psynergy and status-pattern icons, suspending the */
/* field's actor system while the browser owns its window and cache entries. */
/* L and R switch the kind, Left and Right turn its pages (item and Psynergy */
/* icons from page * ICON_BROWSER_PAGE_ICONS on; the status patterns are the */
/* first page's on every page, whatever the heading says), A or B closes it. */
/* Returns 0. The debug rooms open it (debug/level.c, debug/psynergy_test.c, */
/* debug/shop_test.c). */
s32 mnDebugBrowserIcons(void) {
	s32 redraw;
	s32 tile;
	s32 page;
	s32 kind;
	WINDOW *window;
	s32 slot;
	s32 base;
	s32 i;
	s32 row;
	s32 x;
	s32 y;

	window = NULL;
	page = 0;
	kind = 0;
	redraw = 1;
	Ram_HeapSlots->actorSystem->suspended = redraw;
	sySynch(1);

	while(1) {
		if(InputWork.repeat & KEY_LEFT) {
			page--;
			redraw = 1;
		}
		if(InputWork.repeat & KEY_RIGHT) {
			redraw = 1;
			page++;
		}
		if(InputWork.repeat & KEY_L) {
			kind--;
			redraw = 1;
		}
		if(InputWork.repeat & KEY_R) {
			redraw = 1;
			kind++;
		}
		if(InputWork.repeat & KEY_A)
			break;
		if(InputWork.repeat & KEY_B)
			break;
		if(redraw != 0) {
			redraw = 0;
			page = (page + ICON_BROWSER_PAGES) % ICON_BROWSER_PAGES;
			kind = (kind + ICON_BROWSER_KINDS) % ICON_BROWSER_KINDS;
			mnWindowClose(window, 2);
			window = mnWindowCreate(10, 0, 18, 12, WINDOW_FLAG_INSTANT);
			if(kind == 0)
				mnTextDrawStringInWindow(IconBrowser_ItemTitle, window, 0, 0);
			else if(kind == 1)
				mnTextDrawStringInWindow(IconBrowser_EnergyTitle, window, 0, 0);
			else
				mnTextDrawStringInWindow(IconBrowser_StatusTitle, window, 0, 0);
			mnTextDrawStringInWindow(IconBrowser_PageLabel, window, 0, 8);
			mnTextDrawNumInWindow(page, 0, window, 40, 8);
			base = page * ICON_BROWSER_PAGE_ICONS;
			mnTextDrawNumInWindow(base, 3, window, 64, 8);
			mnTextDrawStringInWindow(IconBrowser_RangeDash, window, 88, 8);
			mnTextDrawNumInWindow(base + ICON_BROWSER_PAGE_ICONS - 1, 3, window,
								  96, 8);
			for(i = 0; i < ICON_BROWSER_PAGE_ICONS; i++) {
				slot = -1;
				row = i / ICON_BROWSER_ROW_ICONS;
				x = (i - row * ICON_BROWSER_ROW_ICONS) * 16;
				y = row * 16 + 16;
				if(kind == 0) {
					mnIconBuildItemIconTiles(base + i, 1, &slot, &tile, kind);
					mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, window, x,
									  y);
				}
				else if(kind == 1) {
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
	Ram_HeapSlots->actorSystem->suspended = 0;
	return 0;
}

/* A debug browser of the portraits: left and right step through */
/* Portrait_CharacterMap and then Portrait_SpriteMap, L and R by */
/* DEBUG_LARGE_STEP, each drawn with its number and its name from */
/* MsgNamesMasterPoi; A or B leaves. */
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

	state = (WIN_STATE *)Ram_HeapSlots->winState;
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
		if(InputWork.repeat & KEY_LEFT) {
			redraw = 1;
			index--;
		}
		if(InputWork.repeat & KEY_RIGHT) {
			redraw = 1;
			index++;
		}
		if(InputWork.repeat & KEY_L) {
			redraw = 1;
			index -= DEBUG_LARGE_STEP;
		}
		if(InputWork.repeat & KEY_R) {
			redraw = 1;
			index += DEBUG_LARGE_STEP;
		}
		if(InputWork.repeat & KEY_A)
			break;
		if(InputWork.repeat & KEY_B)
			break;
		if(redraw) {
			redraw = 0;
			index = (index + entryCount) % entryCount;
			mnWindowSprPrepareForRedraw(window);
			if(index < characterCount)
				glyph = Portrait_CharacterMap[index * 2 + 1];
			else
				glyph = Portrait_SpriteMap[(index - characterCount) * 2 + 1] +
						PORTRAIT_FIRST_SPRITE_PORTRAIT;
			slot = state->portraitSprs[portraitSlot];
			mnIconLoadWithPalette(glyph, 0, &slot, &tile, palette, 1);
			mnTextDrawNumInWindow(index, 2, window, 0, 0);
			mnTextDrawMessageAsTiles(index + (s32)MsgNamesMasterPoi, window, 24,
									 0);
		}
		sySynch(1);
	}
	mnWindowClose((WINDOW *)window, 2);
	mnWindowClose((WINDOW *)portrait, 2);
	sySynch(1);
	return 0;
}
