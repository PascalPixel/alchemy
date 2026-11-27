/* The Pause menu's choices, a tilemap palette-bit rectangle, and the debug
   browsers of icons and portraits. Part of screens.c. */

/* Handles Save your quest, Sleep and Change settings from the Pause menu.
   Saving is refused, with its message, while FLAG_CANNOT_SAVE_HERE or
   FLAG_NO_PLACE_TO_SAVE is set. A refused or cancelled save or a cancelled
   settings menu reopens the Pause menu on the same entry; cancelling the Pause
   menu returns -1. Sleep shows its message and sets SleepRequested. Otherwise
   returns 0. */
Sint32 mnPauseMenuRun(void)
{
	Sint32 selection = 0;

again:
	selection = mnPauseMenuChoose(selection);
	if(selection == -1)
		return -1;
	if(selection == PAUSE_MENU_SAVE_QUEST) {
		if(syGameFlagGet(FLAG_CANNOT_SAVE_HERE)) {
			/* "Cannot save here" */
			mnFieldMessageShow((Sint32)MsgCannotSaveHere, FIELD_MESSAGE_ZOOM);
			goto again;
		}
		if(syGameFlagGet(FLAG_NO_PLACE_TO_SAVE)) {
			/* "This is no place for that!" */
			mnFieldMessageShow((Sint32)MsgNoPlaceForThat, FIELD_MESSAGE_ZOOM);
			goto again;
		}
		if(mnSaveToChosenFile() == -1)
			goto again;
	} else if(selection == PAUSE_MENU_SLEEP) {
		/* "Activating Sleep Mode. Press Select and the L & R Buttons to
		   exit." */
		mnFieldMessageShow((Sint32)MsgSleepMode, FIELD_MESSAGE_ZOOM);
		SleepRequested = selection;
	} else if(selection == PAUSE_MENU_CHANGE_SETTINGS) {
		if(mnSettingsMenuRun() == -1)
			goto again;
	}
	return 0;
}

/* Sets (`set` 1) or clears (0) bit 12 of the tilemap entries of the `width` x
   `height` tile rectangle at (x, y) inside `win`, clipped to the screen: bit
   12 is the lowest bit of the entry's palette number, so this switches the
   rectangle between two palettes, such as 15 and 14. Nothing calls it. */
UNUSED void mnWindowSetTextTileBitRect(const WINDOW *win, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Uint32 set)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->winState;

	x += win->x + 1;
	y += win->y + 1;
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
		do {
			Uint16 *cell = &state->tilemap[y][x];
			Sint32	remaining = width;
			while(remaining != 0) {
				Uint32 entry = *cell;
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

/* The icon browser's kinds (items, Psynergy and status patterns), and its pages
   of ICON_BROWSER_PAGE_ICONS icons, ICON_BROWSER_ROW_ICONS to a row. */
#define ICON_BROWSER_KINDS		3
#define ICON_BROWSER_PAGES		12
#define ICON_BROWSER_PAGE_ICONS 32
#define ICON_BROWSER_ROW_ICONS	8

/* Browses pages of item, Psynergy and status-pattern icons, suspending the
   field's actor system while the browser owns its window and cache entries. L
   and R switch the kind, Left and Right turn its pages (item and Psynergy icons
   from page * ICON_BROWSER_PAGE_ICONS on; the status patterns are the first
   page's on every page, whatever the heading says), A or B closes it. Returns
   0. The debug rooms open it (debug/level.c, debug/psynergy_test.c,
   debug/shop_test.c). */
Sint32 mnDebugBrowserIcons(void)
{
	Bool	redraw;
	Sint32	tile;
	Sint32	page;
	Sint32	kind;
	WINDOW *win;
	Sint32	slot;
	Sint32	base;
	Sint32	i;
	Sint32	row;
	Sint32	x;
	Sint32	y;

	win = NULL;
	page = 0;
	kind = 0;
	redraw = TRUE;
	RamHeapSlots->actorSystem->suspended = redraw;
	sySynch(1);

	while(1) {
		if(PerWork.repeat & KEY_LEFT) {
			page--;
			redraw = TRUE;
		}
		if(PerWork.repeat & KEY_RIGHT) {
			redraw = TRUE;
			page++;
		}
		if(PerWork.repeat & KEY_L) {
			kind--;
			redraw = TRUE;
		}
		if(PerWork.repeat & KEY_R) {
			redraw = TRUE;
			kind++;
		}
		if(PerWork.repeat & KEY_A)
			break;
		if(PerWork.repeat & KEY_B)
			break;
		if(redraw != 0) {
			redraw = FALSE;
			page = (page + ICON_BROWSER_PAGES) % ICON_BROWSER_PAGES;
			kind = (kind + ICON_BROWSER_KINDS) % ICON_BROWSER_KINDS;
			mnWindowClose(win, 2);
			win = mnWindowCreate(10, 0, 18, 12, WINDOW_FLAG_INSTANT);
			if(kind == 0)
				mnTextDrawStringInWindow(IconBrowserItemTitle, win, 0, 0);
			else if(kind == 1)
				mnTextDrawStringInWindow(IconBrowserEnergyTitle, win, 0, 0);
			else
				mnTextDrawStringInWindow(IconBrowserStatusTitle, win, 0, 0);
			mnTextDrawStringInWindow(IconBrowserPageLabel, win, 0, 8);
			mnTextDrawNumInWindow(page, 0, win, 40, 8);
			base = page * ICON_BROWSER_PAGE_ICONS;
			mnTextDrawNumInWindow(base, 3, win, 64, 8);
			mnTextDrawStringInWindow(IconBrowserRangeDash, win, 88, 8);
			mnTextDrawNumInWindow(
				base + ICON_BROWSER_PAGE_ICONS - 1, 3, win, 96, 8);
			for(i = 0; i < ICON_BROWSER_PAGE_ICONS; i++) {
				slot = -1;
				row = i / ICON_BROWSER_ROW_ICONS;
				x = (i - row * ICON_BROWSER_ROW_ICONS) * 16;
				y = row * 16 + 16;
				if(kind == 0) {
					mnIconBuildItemIconTiles(base + i, 1, &slot, &tile, kind);
					mnWindowSprCreate(
						slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
				} else if(kind == 1) {
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
	RamHeapSlots->actorSystem->suspended = 0;
	return 0;
}

/* A debug browser of the portraits: left and right step through
   PortraitCharacterMap and then PortraitSpriteMap, L and R by
   DEBUG_LARGE_STEP, each drawn with its number and its name from
   MsgNamesMasterPoi; A or B leaves. */
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

	state = (WIN_STATE *)RamHeapSlots->winState;
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
		if(PerWork.repeat & KEY_LEFT) {
			redraw = TRUE;
			idx--;
		}
		if(PerWork.repeat & KEY_RIGHT) {
			redraw = TRUE;
			idx++;
		}
		if(PerWork.repeat & KEY_L) {
			redraw = TRUE;
			idx -= DEBUG_LARGE_STEP;
		}
		if(PerWork.repeat & KEY_R) {
			redraw = TRUE;
			idx += DEBUG_LARGE_STEP;
		}
		if(PerWork.repeat & KEY_A)
			break;
		if(PerWork.repeat & KEY_B)
			break;
		if(redraw) {
			redraw = FALSE;
			idx = (idx + entryCnt) % entryCnt;
			mnWindowSprPrepareForRedraw(win);
			if(idx < characterCnt)
				glyph = PortraitCharacterMap[idx * 2 + 1];
			else
				glyph = PortraitSpriteMap[(idx - characterCnt) * 2 + 1] +
					PORTRAIT_FIRST_SPRITE_PORTRAIT;
			slot = state->portraitSprs[portraitSlot];
			mnIconLoadWithPalette(glyph, 0, &slot, &tile, palette, 1);
			mnTextDrawNumInWindow(idx, 2, win, 0, 0);
			mnTextDrawMessageAsTiles(
				idx + (Sint32)MsgNamesMasterPoi, win, 24, 0);
		}
		sySynch(1);
	}
	mnWindowClose((WINDOW *)win, 2);
	mnWindowClose((WINDOW *)portrait, 2);
	sySynch(1);
	return 0;
}
