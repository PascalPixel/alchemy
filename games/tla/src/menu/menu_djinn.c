/* The Djinn menu: its backdrop screen, the Djinn list and the screen's loop.
   Part of menu.c. */

/* The party's Djinn columns, one a member, 7 tiles wide, and the x of a
   column's figure. */
#define DJINN_COLUMN_TILES		7
#define DJINN_COLUMN_WIDTH		(DJINN_COLUMN_TILES * 8)
#define DJINN_FIGURE_X(column)	((column) * DJINN_COLUMN_WIDTH + 48)
/* The y of the chosen Djinni's figure over its column, and its y while the
   Djinni is being chosen. */
#define DJINN_FIGURE_Y			54
#define DJINN_FIGURE_CHOOSING_Y 62

/* DJINN_MENU_STATE.pairEntries before a Djinni is chosen: every element and
   number bit set, which no list entry has. */
#define DJINN_ENTRY_NONE (DJINN_ENTRY_ELEMENT | DJINN_ENTRY_INDEX)

/* The tile of BG character block 1 the backdrop's digits are copied to. */
#define DJINN_BACKDROP_DIGIT_TILE 128

/* The cursor packed in DJINN_MENU_STATE.selection: column + row * 10. */
#define DJINN_CURSOR_ROW_STRIDE 10

#include "menu/wrap_index.c"

/* Places the four party figures over their Djinn columns, 56 pixels apart from
   x 32, at screen y 61 plus yOfs. Returns that y. */
Sint32 mnDjinnMenuPlacePartyFigures(Sint32 yOfs)
{
	DJINN_MENU_STATE *menu;
	Sint32			  memberX;
	Sint32			  memberIdx;
	menu = RamHeapSlots->menuState;
	yOfs += 61;
	memberX = 32;
	for(memberIdx = 0; memberIdx < 4; memberIdx++) {
		menu->memberX[memberIdx] = memberX;
		menu->memberY[memberIdx] = yOfs;
		memberX += DJINN_COLUMN_WIDTH;
	}
	return yOfs;
}

/* Returns the Djinn tutorial's first line: the second copy's once
   FLAG_ALREADY_KNOWS_DJINN is set. */
Sint32 mnGetDjinnGiveMessage(void)
{
	if(syGameFlagGet(FLAG_ALREADY_KNOWS_DJINN))
		return (Sint32)MsgDjinnGiveTutorialB;
	return (Sint32)MsgDjinnGiveTutorialA;
}

void mnLoadDjinnArrows(void);

Sint32 mnDjinnMenuRunScreen(void);

/* Runs the Djinn menu over the field: the field menu's Djinn screen
   (mnFieldMenuRun, FIELD_MENU_DJINN), and the Djinn tutorial. Types messages at
   MESSAGE_SPEED_FAST while it runs (the tutorial's steps set
   MESSAGE_SPEED_NORMAL for their messages, which stays until the menu closes)
   and puts the player's speed back. Takes the menu state (HEAP_SLOT_MENU) and
   the backdrop (DJINN_MENU_BACKDROP, from the EWRAM pool), and frees both
   before returning, after putting back the field tiles and palettes the
   backdrop saved. Returns 1, which makes the field menu open again. */
Sint32 mnDjinnMenuRun(void)
{
	DJINN_MENU_STATE	*menu;
	Uint32				 savedMsgSpeed;
	Sint32				 enabled;
	DJINN_MENU_BACKDROP *backdrop;

	menu = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	savedMsgSpeed = MainRoutineStateWork.msgSpeed;
	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_FAST;
	enabled = RamHeapSlots->actorSystem->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);
	backdrop = syHeapAllocEWRAM(sizeof(*backdrop));
	menu->backdrop = backdrop;
	backdrop->tutorialTmr = 0;
	backdrop->tutorialStep = DJINN_TUTORIAL_OFF;
	/* The tutorial's lesson, from the flags the world map sets for each. */
	if(syGameFlagGet(FLAG_DJINN_LESSON)) {
		if(!syGameFlagGet(FLAG_DJINN_LESSON_PART_A)) {
			/* DJINN_TUTORIAL_GIVE */
			if(!syGameFlagGet(FLAG_DJINN_LESSON_PART_B))
				backdrop->tutorialStep = enabled;
			else
				backdrop->tutorialStep = DJINN_TUTORIAL_POWERS;
		} else {
			if(!syGameFlagGet(FLAG_DJINN_LESSON_PART_B))
				backdrop->tutorialStep = DJINN_TUTORIAL_RECOVERY;
			else
				backdrop->tutorialStep = DJINN_TUTORIAL_CLOSE;
		}
	}
	mnSuspendFieldEvents();
	mnTextUseExtraTilesStub(1);
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	menu->memberCnt = gmPartyListMembersStub(menu->members);
	mnLoadDjinnArrows();
	mnItemMenuInit(0, 3, 0, 7);
	mnDjinnMenuPlacePartyFigures(0);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	menu->helpWin = mnWindowCreateStub(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	menu->pairEntries[0] = DJINN_ENTRY_NONE;
	menu->selectedColumn[0] = 0;
	menu->selectedColumn[1] = 0;
	menu->selection[0] = 0;
	menu->selection[1] = 0;
	mnDjinnFiguresInitTopRow();
	mnDjinnMenuRunScreen();
	mnDjinnFiguresReleaseTopRow();
	mnFreeArrows();
	sySynch(1);
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	RamHeapSlots->actorSystem->suspended = 0;
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy(
		(void *)BG_CHAR_BLOCK(1), backdrop->tiles, sizeof(backdrop->tiles));
	syWordsCopy(
		(void *)&BG_PLTT_COLOR(4, 0), backdrop->pal, sizeof(backdrop->pal));
	sySynch(1);
	mnResumeFieldEvents();
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	syHeapRelease(menu->backdrop);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	fdPartyClearUnusableShortcutsStub();
	MainRoutineStateWork.msgSpeed = savedMsgSpeed;
	return 1;
}

void mnOpenBackdropScreen(void);

Sint32 mnDjinnMenuSelectDjinn(Sint32 mode);
Sint32 mnDjinnMenuShowMemberStat(void);
Sint32 mnDjinnMenuShowHelp(void);
Sint32 mnDjinnMenuRunPreview(Uint32 mode);
/* Moves a Djinni from owner to recipient, set or on standby as it was; returns
   -1 when it cannot. */
Sint32 gmDjinnTransferStub(
	Sint32 owner, Sint32 element, Sint32 idx, Sint32 recipient);

Uint32 gmDjinnPutOnStandbyStub(Sint32 owner, Sint32 element, Sint32 idx);
Sint32 gmDjinnRecoveryAddStandbyStub(Sint32 owner, Sint32 element, Sint32 idx);

void mnDjinnMenuBuildLists(DJINN_MENU_BACKDROP *backdrop);

/* Find the moved Djinni in the list and select its new position. */
#define DJINN_RESELECT_PAIR_ENTRY(menu, i) \
	do { \
		Sint32 column; \
		Sint32 found; \
		Uint8  entry; \
\
		column = \
			(Uint16)((Uint32)(menu)->selection[1] % DJINN_CURSOR_ROW_STRIDE); \
		found = 0; \
		entry = (menu)->pairEntries[0]; \
		for((i) = 0; (i) < (menu)->backdrop->cnts[column]; (i)++) { \
			if(entry == (Uint8)(menu)->backdrop->ids[column][(i)]) { \
				found = (i); \
				break; \
			} \
		} \
		(menu)->selection[0] = column + found * DJINN_CURSOR_ROW_STRIDE; \
	} while(0)

/* The screen chooses an operation, optionally previews it, then commits it. The
   Broken Seal's steps 1, 4 and 8, its trade's second choice, are gone. */
typedef enum DJINN_MENU_STEP {
	/* After a chooser: a negative result closes the screen, any other goes back
	   to choosing a Djinni. */
	DJINN_STEP_HANDLE_RESULT = 0,
	DJINN_STEP_SELECT_DJINN = 2,
	DJINN_STEP_SELECT_ACTION = 3, /* what to do with the chosen Djinni */
	DJINN_STEP_CONFIRM_SET = 5,
	DJINN_STEP_CONFIRM_STANDBY = 6,
	DJINN_STEP_CONFIRM_GIVE = 7,
	DJINN_STEP_CONFIRM_TRADE = 9,
	/* The status of the chosen column's party member. */
	DJINN_STEP_SHOW_STATUS = 10,
	DJINN_STEP_APPLY_SET = 11,
	DJINN_STEP_APPLY_STANDBY = 12,
	DJINN_STEP_APPLY_GIVE = 13,
	DJINN_STEP_APPLY_TRADE = 14,
	DJINN_STEP_SHOW_HELP = 15
} DJINN_MENU_STEP;

/* The Djinn menu's screen loop, one step at a time: choose a Djinni, then what
   to do with it (set it, put it on standby, give it or trade it), preview the
   change and apply it; L shows the party member's status and Select the help.
   Returns -1 once the player leaves the menu. */
Sint32 mnDjinnMenuRunScreen(void)
{
	/* icon holds the cursor, then each arrow in turn. */
	DJINN_MENU_STATE *menu;
	WINDOW_SPR		 *icon;
	Sint32			  result;
	Sint32			  step;
	Sint32			  menuResult;
	Bool			  done;
	Sint32			  listIdx;

	done = FALSE;
	result = 0;
	menuResult = 0;
	menu = RamHeapSlots->menuState;
	icon = menu->cursor;
	icon->motion = ICON_HIDDEN;
	icon->anmStep = 0;
	mnOpenBackdropScreen();
	sySynch(1);
	step = DJINN_STEP_SELECT_DJINN;
	do {
		switch(step) {
		case DJINN_STEP_HANDLE_RESULT:
			if(result < 0) {
				menuResult = -1;
				done = TRUE;
			}
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_SELECT_DJINN:
			mnDjinnMenuPlacePartyFigures(0);
			mnDjinnFiguresSetSlotPos(1, 0, MENU_FIGURE_HIDDEN_Y);
			result = mnDjinnMenuSelectDjinn(DJINN_SELECT_DJINNI);
			step = DJINN_STEP_SHOW_HELP;
			if(result == DJINN_RESULT_HELP)
				break;
			step = DJINN_STEP_HANDLE_RESULT;
			if(result < 0)
				break;
			menu->selection[1] = menu->selectedColumn[0];
			step = DJINN_STEP_SHOW_STATUS;
			if(result == DJINN_RESULT_STATUS)
				break;
			step = DJINN_STEP_SELECT_ACTION;
			break;
		case DJINN_STEP_SHOW_STATUS:
			menu->member = menu->members[menu->selectedColumn[0]];
			menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
			icon = menu->pageArrows[0];
			icon->motion = ICON_HIDDEN;
			icon = menu->pageArrows[1];
			icon->motion = ICON_HIDDEN;
			result = mnDjinnMenuShowMemberStat();
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_SHOW_HELP:
			icon = menu->pageArrows[0];
			icon->motion = ICON_HIDDEN;
			icon = menu->pageArrows[1];
			icon->motion = ICON_HIDDEN;
			result = mnDjinnMenuShowHelp();
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_SELECT_ACTION:
			mnDjinnMenuPlacePartyFigures(-8);
			menu->member = menu->members[menu->selectedColumn[0]];
			menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
			mnDjinnFiguresSetSlotPos(
				0, DJINN_FIGURE_X(menu->selectedColumn[0]), DJINN_FIGURE_Y);
			result = mnDjinnMenuSelectDjinn(DJINN_SELECT_ACTION);
			for(listIdx = 0; listIdx < menu->memberCnt; listIdx++)
				menu->memberY[listIdx] += 8;
			if(result == DJINN_RESULT_START)
				done = TRUE;
			if(result < 0) {
				step = DJINN_STEP_SELECT_DJINN;
				break;
			}
			if(result == DJINN_RESULT_GIVE || result == DJINN_RESULT_TRADE ||
				result == DJINN_RESULT_GIVE_NOW ||
				result == DJINN_RESULT_TRADE_NOW)
				menu->shownMember[1] = menu->members[menu->selectedColumn[1]];
			if(result < 0)
				step = DJINN_STEP_SELECT_DJINN;
			else if(result == DJINN_RESULT_SET)
				step = DJINN_STEP_CONFIRM_SET;
			else if(result == DJINN_RESULT_STANDBY)
				step = DJINN_STEP_CONFIRM_STANDBY;
			else if(result == DJINN_RESULT_GIVE) {
				menu->pending = DJINN_PENDING_CONFIRM;
				step = DJINN_STEP_CONFIRM_GIVE;
			} else if(result == DJINN_RESULT_TRADE) {
				menu->pending = DJINN_PENDING_CONFIRM;
				step = DJINN_STEP_CONFIRM_TRADE;
			} else if(result == DJINN_RESULT_SET_NOW)
				step = DJINN_STEP_APPLY_SET;
			else if(result == DJINN_RESULT_STANDBY_NOW)
				step = DJINN_STEP_APPLY_STANDBY;
			else if(result == DJINN_RESULT_GIVE_NOW) {
				menu->pending = DJINN_PENDING_CONFIRM;
				step = DJINN_STEP_APPLY_GIVE;
			} else if(result == DJINN_RESULT_TRADE_NOW) {
				menu->pending = DJINN_PENDING_CONFIRM;
				step = DJINN_STEP_APPLY_TRADE;
			}
			break;
		case DJINN_STEP_CONFIRM_GIVE:
			icon = menu->pageArrows[0];
			icon->motion = ICON_HIDDEN;
			icon = menu->pageArrows[1];
			icon->motion = ICON_HIDDEN;
			result = mnDjinnMenuRunPreview(DJINN_COMPARE_GIVE);
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_ACTION;
			if(result < 0)
				break;
			/* Confirmed: apply it at once. Fall through. */
		case DJINN_STEP_APPLY_GIVE:
			sySndPlayStub(SE_RECOVERY);
			result = gmDjinnTransferStub(menu->shownMember[0],
				menu->pairElement[0], menu->pairIdx[0], menu->shownMember[1]);
			gmUnitCalcStub(menu->shownMember[0]);
			gmUnitCalcStub(menu->shownMember[1]);
			menu->cursor->motion = ICON_HIDDEN;
			mnWindowSprClearListStub(menu->listWin);
			mnDjinnMenuBuildLists(menu->backdrop);
			DJINN_RESELECT_PAIR_ENTRY(menu, listIdx);
			menu->cursor->motion = ICON_SHOWN;
			step = DJINN_STEP_HANDLE_RESULT;
			break;
		case DJINN_STEP_CONFIRM_STANDBY:
			icon = menu->pageArrows[0];
			icon->motion = ICON_HIDDEN;
			icon = menu->pageArrows[1];
			icon->motion = ICON_HIDDEN;
			result = mnDjinnMenuRunPreview(DJINN_COMPARE_STANDBY);
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_ACTION;
			if(result < 0)
				break;
			/* Confirmed: apply it at once. Fall through. */
		case DJINN_STEP_APPLY_STANDBY:
			sySndPlayStub(SE_NOTE_HIGH);
			gmDjinnPutOnStandbyStub(
				menu->shownMember[0], menu->pairElement[0], menu->pairIdx[0]);
			result = gmDjinnRecoveryAddStandbyStub(
				menu->shownMember[0], menu->pairElement[0], menu->pairIdx[0]);
			gmUnitCalcStub(menu->shownMember[0]);
			menu->cursor->motion = ICON_HIDDEN;
			mnWindowSprClearListStub(menu->listWin);
			menu->cursor->motion = ICON_SHOWN;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_CONFIRM_TRADE:
			icon = menu->pageArrows[0];
			icon->motion = ICON_HIDDEN;
			icon = menu->pageArrows[1];
			icon->motion = ICON_HIDDEN;
			result = mnDjinnMenuRunPreview(DJINN_COMPARE_TRADE);
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_ACTION;
			if(result < 0)
				break;
			/* Confirmed: apply it at once. Fall through. */
		case DJINN_STEP_APPLY_TRADE:
			sySndPlayStub(SE_RECOVERY);
			gmDjinnTransferStub(menu->shownMember[0], menu->pairElement[0],
				menu->pairIdx[0], menu->shownMember[1]);
			result = gmDjinnTransferStub(menu->shownMember[1],
				menu->pairElement[1], menu->pairIdx[1], menu->shownMember[0]);
			gmUnitCalcStub(menu->shownMember[0]);
			gmUnitCalcStub(menu->shownMember[1]);
			mnDjinnMenuBuildLists(menu->backdrop);
			DJINN_RESELECT_PAIR_ENTRY(menu, listIdx);
			menu->cursor->motion = ICON_SHOWN;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_CONFIRM_SET:
			icon = menu->pageArrows[0];
			icon->motion = ICON_HIDDEN;
			icon = menu->pageArrows[1];
			icon->motion = ICON_HIDDEN;
			result = mnDjinnMenuRunPreview(DJINN_COMPARE_SET);
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_ACTION;
			if(result < 0)
				break;
			/* Confirmed: apply it at once. Fall through. */
		case DJINN_STEP_APPLY_SET:
			sySndPlayStub(SE_DJINNI_SET);
			gmDjinnSetStub(
				menu->shownMember[0], menu->pairElement[0], menu->pairIdx[0]);
			result = gmDjinnRecoveryRemoveStub(
				menu->shownMember[0], menu->pairElement[0], menu->pairIdx[0]);
			gmUnitCalcStub(menu->shownMember[0]);
			menu->cursor->motion = ICON_HIDDEN;
			mnWindowSprClearListStub(menu->listWin);
			menu->cursor->motion = ICON_SHOWN;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		default:
			done = TRUE;
			break;
		}
	} while(done == 0);
	return menuResult;
}

#undef DJINN_RESELECT_PAIR_ENTRY

/* The palette mnDjinnMenuAdjustPalettes darkens WINDOW_PALETTE into: the Djinn
   list behind the help, and the column a Djinni is taken from. */
#define DJINN_DIM_PALETTE 6
/* The Djinn help's topics: Djinn, Set, Standby, Summon, Recovery, Class and
   Psynergy. */
#define DJINN_HELP_TOPICS 7

/* Writes copies of background palettes 15, 5 and 7, each brightnessDelta
   lighter in every channel (clamped to 0-31), into palettes 14, 4 and 6;
   palette 7's copy is 12 darker instead. */
void mnDjinnMenuAdjustPalettes(Sint32 brightnessDelta)
{
	Sint32 copyIdx;
	Sint32 paletteNum;
	Sint32 colIdx;
	Sint32 paletteIdx;

	paletteNum = WINDOW_PALETTE;
	for(copyIdx = 0; copyIdx <= 2; copyIdx++) {
		for(colIdx = 0; colIdx < PALETTE_COLORS; colIdx++) {
			Uint32 col;
			Sint32 blue;
			Sint32 green;
			Sint32 red;
			Sint32 adjustedCol;

			paletteIdx = paletteNum * PALETTE_COLORS + colIdx;
			col = ((Uint16 *)BG_PLTT)[paletteIdx];
			blue = RGB_BLUE(col);
			green = RGB_GREEN(col);
			red = RGB_RED(col);
			blue += brightnessDelta;
			green += brightnessDelta;
			red += brightnessDelta;
			if(blue > RGB_LEVEL_MAX)
				blue = RGB_LEVEL_MAX;
			if(green > RGB_LEVEL_MAX)
				green = RGB_LEVEL_MAX;
			if(red > RGB_LEVEL_MAX)
				red = RGB_LEVEL_MAX;
			if(blue < 0)
				blue = 0;
			if(green < 0)
				green = 0;
			if(red < 0)
				red = 0;
			adjustedCol = RGB(red, green, blue);
			((Sint16 *)BG_PLTT - PALETTE_COLORS)[paletteIdx] = adjustedCol;
		}
		if(copyIdx == 0) {
			paletteNum = 5;
		} else {
			paletteNum = 7;
			brightnessDelta = -12;
		}
	}
}

/* Copies the window font's ten digit tiles twice into tiles, the first copy
   with colour 1 drawn as colour 10. */
void mnGraphicsExpandVRAMTilesByColTableStub(void *tiles);

/* Opens the Djinn menu's backdrop screen: saves the tiles and palettes it
   covers, draws the backdrop, lists the party's Djinn (mnDjinnMenuBuildLists),
   then makes the list's cursor and the two page arrows and places the arrows at
   the list window's corners. */
void mnOpenBackdropScreen(void)
{
	DJINN_MENU_STATE	*menu = RamHeapSlots->menuState;
	DJINN_MENU_BACKDROP *backdrop = menu->backdrop;
	WINDOW				*win;
	WINDOW_SPR			*icon;
	Sint32				 entry;
	Sint32				 x;
	Sint32				 y;

	mnWindowUpdateOrCreate(&menu->listWin, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
	sySynch(1);
	syWordsCopy(backdrop->tiles, BG_CHAR_BLOCK(1), sizeof(backdrop->tiles));
	syWordsCopy(
		backdrop->pal, (void *)&BG_PLTT_COLOR(4, 0), sizeof(backdrop->pal));
	syWordsFill(BG_CHAR_BLOCK(1), sizeof(backdrop->tiles), PIXELS_4BPP(3));
	syWordsFill(
		(void *)&BG_PLTT_COLOR(4, 0), sizeof(backdrop->pal), PIXELS_4BPP(5));
	mnGraphicsExpandVRAMTilesByColTableStub(
		BG_CHAR_BLOCK(1) + DJINN_BACKDROP_DIGIT_TILE * TILE_SIZE_4BPP);
	syWordsCopy(
		DJINN_BACKDROP_BORDER_VRAM, MenuBackdropBorderTile, TILE_SIZE_4BPP);
	syDMAXCopy(syPaletteGetBasicSprCols(),
		(void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0), DMA_COPY_HALFWORDS(16),
		REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
		(void *)&BG_PLTT_COLOR(7, 0), DMA_COPY_HALFWORDS(16), REG_DMA3);
	mnDjinnMenuAdjustPalettes(8);
	BG_PLTT_COLOR(7, WINDOW_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	BG_PLTT_COLOR(6, WINDOW_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	mnDjinnMenuBuildLists(backdrop);

	win = menu->statWin;
	mnWindowSprClearListStub(win);
	icon = mnIconCreateHandCursor(win, -8, 11);
	icon->motion = ICON_HIDDEN;
	menu->cursor = icon;
	entry = syVRAMCacheFindFreeEntry();
	if(entry < VRAM_CACHE_ENTRY_COUNT) {
		mnVRAMBlockLoadResourceStub(
			entry, ICON_TILE_BYTES, RESOURCE_SHOP_GEM_TILES);
		icon =
			mnWindowSprCreateStub(entry, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		icon->motion = ICON_HIDDEN;
		menu->pageArrows[0] = icon;
		icon->spr.oam.matrix |= 8;
		icon =
			mnWindowSprCreateStub(entry, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		icon->motion = ICON_HIDDEN;
		menu->pageArrows[1] = icon;
	}

	win = menu->listWin;
	icon = menu->pageArrows[0];
	x = (Uint16)win->x * 8;
	icon->spr.oam.x = icon->x = x;
	y = (Uint16)win->y * 8 + 4;
	icon->spr.oam.y = icon->y = y;
	icon = menu->pageArrows[1];
	x += 224;
	icon->spr.oam.x = icon->x = x;
	icon->spr.oam.y = icon->y = y;
}

/* mnPsynergyListDiff's marks on an action id: Psynergy the change gives, drawn
   in TEXT_COLOR_YELLOW, and Psynergy it takes away, in TEXT_COLOR_RED. */
#define DJINN_PREVIEW_GAINED 0x8000
#define DJINN_PREVIEW_LOST	 0x4000

#include "menu/psynergy_list_diff.c"

Sint32 mnDjinnListUnitEntries(
	Uint16 *entriesOut, Sint32 memberId, Sint32 element);

/* Lists each party member's Djinn of every element (mnDjinnListUnitEntries)
   into the backdrop's lists, with their counts. */
void mnDjinnMenuBuildLists(DJINN_MENU_BACKDROP *backdrop)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)RamHeapSlots->menuState;
	Sint32			 i;

	for(i = 0; i < menu->partyCnt; i++) {
		Sint32 owner = (Uint16)menu->memberIds[i];

		backdrop->cnts[i] = mnDjinnListUnitEntries(backdrop->ids[i], owner, -1);
	}
}

Sint32 gmDjinnCanSetStub(Sint32 owner, Sint32 element, Sint32 idx);
Sint32 gmDjinnIsSetStub(Sint32 owner, Sint32 element, Sint32 idx);

/* The state of a Djinni's list entry (DJINNI_STATE): recovering when it can be
   neither set nor is set, otherwise set or on standby by DJINN_ENTRY_IS_SET.
   mnDjinnMenuDrawPage colours each name by it. */
Sint32 mnDjinnMenuGetEntryState(Uint32 entry)
{
	Sint32 owner = DJINN_MEMBER(entry);
	Sint32 element = DJINN_ENTRY_ELEMENT_OF(entry);
	Sint32 index = DJINN_NUMBER(entry);
	Sint32 state;

	if(gmDjinnCanSetStub(owner, element, index) ||
		gmDjinnIsSetStub(owner, element, index)) {
		if(entry & DJINN_ENTRY_IS_SET)
			state = DJINNI_STATE_SET;
		else
			state = DJINNI_STATE_STANDBY;
	} else {
		state = DJINNI_STATE_RECOVERING;
	}
	return state;
}

/* Draws a packed djinni entry in a window: its name 8 pixels right of (x, y)
   and its element's icon tile at the tile under (x, y), then resets the text
   colour. The names run 20 to an element. */
void mnDjinnMenuDrawEntry(Sint32 win, Sint32 x, Sint32 y, Sint32 entry)
{
	Sint32 element = (entry & DJINN_ENTRY_ELEMENT) / (DJINN_ENTRY_INDEX + 1);

	mnTextDrawMessageAsTilesStub((Sint32)MsgDjinnName +
			element * DJINN_PER_ELEMENT + DJINN_NUMBER(entry),
		win, x + 8, y);
	mnWindowSetTilemapEntryStub(
		win, element + ELEMENT_ICON_TILE, x / 8, y / 8, 0);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
}

/* Draws four party members' Djinn into the list window, one column of 56 pixels
   each from party member page: each member's Djinn grouped by element, standby
   in red, set in white and the rest in yellow. Over the list it labels the
   unleash effects when the party has few enough Djinn to show them. */
void mnDjinnMenuDrawPage(DJINN_MENU_BACKDROP *backdrop, Sint32 page)
{
	DJINN_MENU_STATE *menu = RamHeapSlots->menuState;
	WIN_STATE		 *state = RamHeapSlots->winState;
	Sint32			  win = menu->listWin;
	Sint32			  owners;
	Sint32			  column;
	Sint32			  element;
	Sint32			  i;
	Sint32			  x;
	Sint32			  y;
	Sint32			  djinn;

	state->holdTilemapUpload = 1;
	mnWindowSprRedrawSavedRectStub(win);
	owners = gmPartyCountMembersStub();
	djinn = gmPartySumDjinnCountsStub(-1);
	if(owners != 0 && (djinn - 1) / owners <= 6) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnUnleashEffectLabel, win, 0, 80);
		mnWindowDrawDividerLineStub(win, 0, 10, 28, 10);
	}
	mnDjinnMenuBuildLists(backdrop);
	for(column = 0, x = 8; column <= 3; column++, x += DJINN_COLUMN_WIDTH) {
		Uint16 *entries = backdrop->ids[page + column];

		y = 16;
		if(menu->memberSprs[column] != NULL) {
			for(element = 0; element < ELEMENT_COUNT; element++) {
				for(i = 0; i < backdrop->cnts[page + column]; i++) {
					if(element == DJINN_ENTRY_ELEMENT_OF(entries[i])) {
						switch(mnDjinnMenuGetEntryState(entries[i])) {
						case DJINNI_STATE_STANDBY:
							mnTextSetColStub(TEXT_COLOR_RED);
							break;
						case DJINNI_STATE_SET:
							mnTextSetColStub(TEXT_COLOR_NORMAL);
							break;
						default:
							mnTextSetColStub(TEXT_COLOR_YELLOW);
							break;
						}
						mnDjinnMenuDrawEntry(win, x, y, entries[i]);
						y += 8;
					}
				}
			}
			/* In the Spanish and Italian editions each column waits a tick
			   after it is drawn. */
#if DJINN_COLUMN_WAIT_AFTER_DRAW
			sySynch(1);
#endif
		}
	}
	/* The Japanese release reads the window tiles again for the first store and
	   does not wait; in every edition but the Japanese the page waits three
	   ticks. */
#if DJINN_PAGE_WAIT_AFTER_DRAW
	state->dirty = 1;
	state->holdTilemapUpload = 0;
	sySynch(3);
#else
	((WIN_STATE *)RamHeapSlots->winState)->dirty = 1;
	state->holdTilemapUpload = 0;
#endif
}

/* A grid cell's highlight is a tile narrower in Japanese. */
#define GRID_CELL_WIDTH JAPANESE_OR(5, 6)

/* Draws the Djinn list's first page, the first four party members. */
void mnDjinnMenuDrawFirstPage(DJINN_MENU_BACKDROP *backdrop)
{
	mnDjinnMenuDrawPage(backdrop, 0);
}

/* Sets the palette of a rectangle of panel's interior, (x, y) and its size in
   tiles from the interior's corner, through mnWindowHighlightRectStub. */
void mnWindowSetInteriorPaletteBitRect(WINDOW *panel, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Sint32 palette)
{
	mnWindowHighlightRectStub(
		panel->x + x + 1, panel->y + y + 1, width, height, palette);
}

/* Highlights the cell of a four-column grid at index's column and row's line,
   in HIGHLIGHT_PALETTE when selected and WINDOW_PALETTE otherwise. */
void mnUiPanelHighlightGridCell(
	WINDOW *panel, Sint32 idx, Sint32 row, Sint32 selected)
{
	Sint32 palette;

	if(!selected)
		palette = WINDOW_PALETTE;
	else
		palette = HIGHLIGHT_PALETTE;
	mnWindowSetInteriorPaletteBitRect(panel, idx % 4 * DJINN_COLUMN_TILES + 1,
		row + 2, GRID_CELL_WIDTH, 1, palette);
}

#undef GRID_CELL_WIDTH

/* Gives every cell of a tile rectangle that uses WINDOW_PALETTE the palette
   given instead, clipped to the screen, and marks its rows for upload. */
void mnWindowSetRectPalette(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette)
{
	WIN_STATE *tiles = (WIN_STATE *)RamHeapSlots->winState;

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
	/* BUG: tests the bottom against the screen's width but clips to its rows,
	   so a rectangle ending in rows 20 to 28 is kept whole and recolours past
	   the 32 by 20 tilemap. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}

	if(width > 0 && height > 0) {
		do {
			Uint16 *entry = &tiles->tilemap[y][x];
			Sint32	column = width;

			if(column != 0) {
				do {
					Uint32 tile = *entry;

					if(BG_TILE_PALETTE_OF(tile) == WINDOW_PALETTE) {
						tile &= ~BG_TILE_PALETTE(0xf);
						tile |= palette;
						*entry = tile;
					}
					column--;
					entry++;
				} while(column != 0);
			}
			WINDOW_MARK_TILEMAP_ROW(tiles, y);

			height--;
			y++;
		} while(height != 0);
	}
}

/* mnWindowSetRectPalette on a rectangle of window's interior, (x, y) and its
   size in tiles from the interior's corner. */
void mnWindowSetInteriorRectPalette(WINDOW *panel, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Sint32 palette)
{
	mnWindowSetRectPalette(
		panel->x + x + 1, panel->y + y + 1, width, height, palette);
}

/* Builds a message's render entries and queues them in window; returns 0 when
   the message is empty or there is no window. */
MESSAGE *mnMessageOpenDecodedStub(void *win, Sint32 msg);

/* The Djinn menu's help, opened by Select on the first chooser: a list of seven
   topics (Djinn, Set, Standby, Summon, Recovery, Class, Psynergy) with the
   chosen one's text typed beside it. The d-pad moves through the topics, and A
   moves on once the text has finished. Returns DJINN_RESULT_BACK for B or
   Select, back to the list, or DJINN_RESULT_START for Start, which closes the
   Djinn menu. */
Sint32 mnDjinnMenuShowHelp(void)
{
	WINDOW			 *helpWin;
	WINDOW			 *statWin;
	DJINN_MENU_STATE *menu;
	WIN_STATE		 *render;
	Sint32			  result;
	Sint32			  prev;
	Sint32			  selection;
	WINDOW			 *list;
	MESSAGE			 *channel;
	Sint32			  msg;
	Sint32			  helpIdx;
	Sint32			  listMsg;

	menu = RamHeapSlots->menuState;
	selection = 0;
	render = (WIN_STATE *)RamHeapSlots->winState;
	result = 0;
	prev = 0;
	mnWindowSprClearListStub(menu->listWin);
	sySynch(1);
	mnWindowSprPrepareForRedrawStub(menu->helpWin);
	msg = (Sint32)MsgDjinnInfoPrompt;
	mnTextDrawMessageAsTilesStub(msg, menu->helpWin, 0, 0);
	msg++;
	mnTextDrawMessageAsTilesStub(msg, menu->helpWin, 0, 16);
	/* "Select the items" and "Select:Djinni selection" */
	mnWindowSetRectPalette(1, 1, 11, 3, DJINN_DIM_PALETTE);
	mnWindowSetInteriorRectPalette(
		menu->listWin, 0, 0, 28, 10, DJINN_DIM_PALETTE);
	list = mnWindowCreateStub(0, 9, 8, 10, 6);
	helpWin = mnWindowCreateStub(8, 12, 22, 7, WINDOW_FLAG_INSTANT);
	statWin = mnWindowCreateStub(8, 9, 22, 3, WINDOW_FLAG_INSTANT);
	mnWindowMarkVisibleTextTilesStub();
	listMsg = (Sint32)MsgDjinnInfoTopic;
	for(helpIdx = 0; helpIdx < DJINN_HELP_TOPICS; helpIdx++) {
		Sint32 y = helpIdx * 8;

		mnTextDrawMessageAsTilesStub(helpIdx + listMsg, list, 0, y);
	}
	do {
		mnWindowSprPrepareForRedrawStub(statWin);
		mnTextDrawMessageStub(
			selection + (Sint32)MsgDjinnInfoTopic, statWin, 0, 0);
		channel = mnMessageOpenDecodedStub(
			helpWin, selection + (Sint32)MsgDjinnInfoText);
		mnWindowSetInteriorPaletteBitRect(list, 0, prev, 6, 1, WINDOW_PALETTE);
		mnWindowSetInteriorPaletteBitRect(
			list, 0, selection, 6, 1, HIGHLIGHT_PALETTE);
		prev = selection;
		for(;;) {
			mnCursorPos(-12, (list->y + selection) * 8 + 8);
			sySynch(1);
			if(PerWork.repeat & (KEY_RIGHT | KEY_DOWN)) {
				selection++;
				selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				break;
			}
			if(PerWork.repeat & (KEY_LEFT | KEY_UP)) {
				selection--;
				selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				break;
			} else if(PerWork.push & KEY_START) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = DJINN_RESULT_START;
				break;
			} else if(PerWork.push & (KEY_B | KEY_SELECT)) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = DJINN_RESULT_BACK;
				break;
			} else if(PerWork.push & KEY_A) {
				if(mnMessageAllTypedStub()) {
					selection++;
					selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
					sySndPlayStub(SE_MENU_CONFIRM);
					break;
				}
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		if(render->glyphResource != NO_GLYPH_RESOURCE) {
			syVRAMCacheFreeEntry(render->glyphResource);
			render->glyphResource = NO_GLYPH_RESOURCE;
		}
		((WIN_STATE *)RamHeapSlots->winState)->pageArrowShown = 0;
		mnWindowSprPrepareForRedrawStub(helpWin);
		{
			WINDOW *win = channel->win;

			win->zoomStage = 0;
			win->duration = 0;
			win->state = 0;
		}
		channel->win = 0;
	} while(result == 0);
	((WIN_STATE *)RamHeapSlots->winState)->holdTilemapUpload = 1;
	mnWindowSprClearListStub(statWin);
	mnWindowSprClearListStub(helpWin);
	sySynch(1);
	mnWindowCloseStub(statWin, 1);
	mnWindowCloseStub(list, 1);
	mnWindowCloseStub(helpWin, 1);
	mnWindowMarkVisibleTextTilesStub();
	if(result == DJINN_RESULT_START) {
		mnWindowSprPrepareForRedrawStub(menu->helpWin);
		mnWindowSprPrepareForRedrawStub(menu->listWin);
		mnWindowSprPrepareForRedrawStub(menu->statWin);
		((WIN_STATE *)RamHeapSlots->winState)->holdTilemapUpload = 0;
	}
	sySetEvent(mnUpdatePartyFigures, EVENT_PASS_TICK);
	return result;
}

/* A grid cell's highlight is a tile narrower in Japanese. */
#define GRID_CELL_WIDTH JAPANESE_OR(5, 6)

/* Spanish, French and Italian name the Djinni before its owner in one message
   on the trade's help line, so the Djinni's icon leads the line. */
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || \
	LANGUAGE == LANGUAGE_ITALIAN
#define TRADE_HELP_DJINNI_FIRST 1
#else
#define TRADE_HELP_DJINNI_FIRST 0
#endif

/* The level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern Uint8 DjinnMenuTextLevel[];

Sint32 mnDjinnMarkTradeOnlyRecipients(Sint8 *tbl, Sint32 self);

void mnSetPageArrowsLeftRight(Sint32 idx, Sint32 cnt);

void   btDjinnRecoveryCountDownStub(void);
Sint32 mnWrapIndex(Sint32 value, Sint32 modulus) __attribute__((const));

/* Shows one tutorial message and waits for its text to finish. */
#define DJINN_TUTORIAL_MESSAGE(window, offset) \
	do { \
		(window) = mnMessageOpenWindowStub( \
			mnGetDjinnGiveMessage() + (offset), 9, 9, 1); \
		MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL; \
		while(mnMessageAllTypedStub() == 0) \
			sySynch(1); \
	} while(0)

/* The flags mnDjinnMenuSelectDjinn keeps for each party member's column. */
enum DJINN_COLUMN_FLAG {
	DJINN_COLUMN_ON_HEADER = 1, /* the cursor is on the column's header */
	DJINN_COLUMN_HEADER_CHOOSABLE = 2, /* its header can be chosen */
	DJINN_COLUMN_EMPTY = 4			   /* no Djinni to choose */
};

/* What mnDjinnMenuSelectDjinn redraws on its next pass. */
enum DJINN_REDRAW {
	DJINN_REDRAW_CURSOR = 1, /* the cursor, the figure and the help lines */
	DJINN_REDRAW_DJINNI = 2, /* as for the cursor, the figure from its start */
	DJINN_REDRAW_PAGE = 4	 /* the four columns of the page as well */
};

/* The Djinn list's two choosers (DJINN_SELECT_MODE). Choosing a Djinni, the
   d-pad moves over every party member's column, R sets the Djinni or puts it on
   standby at once, R held with Select does so to them all, L shows the party
   member's status and Select the help. Choosing what to do with it, the cursor
   picks its own column to set it or put it on standby, another member's header
   to give it, or one of their Djinn to trade. While the tutorial runs, it
   presses the keys itself. Returns an DJINN_MENU_RESULT. */
Sint32 mnDjinnMenuSelectDjinn(Sint32 mode)
{
	Sint8				 columnFlg[8];
	Uint8				 requiresTrade[16];
	DJINN_MENU_STATE	*menu = RamHeapSlots->menuState;
	DJINN_MENU_BACKDROP *backdrop = menu->backdrop;
	Sint32				 helpWin = menu->helpWin;
	Sint32				 listWin = menu->listWin;
	UNIT				*unit;
	Sint32				 win;
	Uint32				 redraw = DJINN_REDRAW_PAGE;
	Sint32				 i;
	Sint32				 j;
	Sint32				 selectedRow;
	Sint32				 memberColumn =
		(Uint16)(menu->selection[mode] % DJINN_CURSOR_ROW_STRIDE);
	Sint32 djinnRow = (Uint16)(menu->selection[mode] / DJINN_CURSOR_ROW_STRIDE);
	Sint32 savedRow = 0;
	Uint32 djinn = 0;
	Bool   groupMode = FALSE;
	Sint32 setAll = 0;
	Sint32 owner = 0;
	Sint32 element = 0;
	Sint32 num = 0;
	Sint32 state = 0;
	Sint32 result;
	Sint32 msg;
	Uint32 pressed;
	Uint32 repeat;
	Sint32 msgWin;
	Sint8 *stat;

	selectedRow = -1;
	stat = columnFlg;
	for(i = 0; i < 8; i++)
		stat[i] = 0;
	if(mode == DJINN_SELECT_DJINNI) {
		savedRow = djinnRow;
		for(i = 0; i < menu->memberCnt; i++) {
			if(backdrop->cnts[i] == 0)
				stat[i] = DJINN_COLUMN_EMPTY;
		}
		for(i = 0; i < menu->memberCnt; i++) {
			if(stat[memberColumn] == DJINN_COLUMN_EMPTY) {
				memberColumn++;
				memberColumn = mnWrapIndex(memberColumn, menu->memberCnt);
			}
		}
	} else {
		mnDjinnMarkTradeOnlyRecipients(
			(Sint8 *)requiresTrade, menu->selectedColumn[0]);
		for(i = 0; i < menu->memberCnt; i++) {
			if(i == menu->selectedColumn[0]) {
				columnFlg[i] = DJINN_COLUMN_EMPTY |
					DJINN_COLUMN_HEADER_CHOOSABLE | DJINN_COLUMN_ON_HEADER;
			} else if(requiresTrade[i] != 0) {
				columnFlg[i] = 0;
			} else {
				columnFlg[i] =
					DJINN_COLUMN_HEADER_CHOOSABLE | DJINN_COLUMN_ON_HEADER;
				if(backdrop->cnts[i] == 0)
					stat[i] = columnFlg[i] | DJINN_COLUMN_EMPTY;
			}
		}
	}
	menu->cursor->motion = ICON_SHOWN;
	for(;;) {
		if(redraw != 0) {
			selectedRow = -1;
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER))
				selectedRow = djinnRow;
			if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER)
				mnCursorPos(memberColumn % 4 * DJINN_COLUMN_WIDTH - 8, 52);
			else
				mnCursorPos(memberColumn % 4 * DJINN_COLUMN_WIDTH - 8,
					djinnRow * 8 + 60);
			if(redraw >> 2) { /* DJINN_REDRAW_PAGE */
				Sint32 page = memberColumn / 4 * 4;

				mnCreatePartyFigures(page);
				mnDjinnMenuDrawPage(backdrop, page);
				if(mode == DJINN_SELECT_ACTION) {
					Sint32 fromX;
					Sint32 fromY;
					Uint16 srcCursor;
					Sint32 x;
					Sint32 height;

					srcCursor = menu->selection[0];
					fromX = (Uint16)((Uint32)srcCursor %
						(Uint32)DJINN_CURSOR_ROW_STRIDE);
					fromY = (Uint16)((Uint32)srcCursor /
						(Uint32)DJINN_CURSOR_ROW_STRIDE);
					if(fromX / 4 * 4 == page) {
						height = backdrop->cnts[fromX];
						mnUiPanelHighlightGridCell(listWin, fromX, fromY, 1);
						mnWindowSetInteriorRectPalette(listWin,
							(fromX - page) * DJINN_COLUMN_TILES + 1, 2,
							GRID_CELL_WIDTH, height, DJINN_DIM_PALETTE);
						menu->slotY[0] = DJINN_FIGURE_Y;
					} else {
						menu->slotY[0] = MENU_FIGURE_HIDDEN_Y;
					}
					for(i = 0, x = 8; i <= 3; i++, x += DJINN_COLUMN_WIDTH) {
						if(menu->memberSprs[i] != NULL) {
							if(page + i == menu->selectedColumn[0]) {
								if(menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
									msg = (Sint32)MsgDjinnStandbyAction;
								else
									msg = (Sint32)MsgDjinnSetAction;
							} else if(columnFlg[page + i] &
								DJINN_COLUMN_HEADER_CHOOSABLE) {
								msg = (Sint32)MsgDjinnGiveAction;
							} else {
								msg = (Sint32)MsgDjinnTradeAction;
							}
							mnTextDrawMessageAsTilesStub(msg, listWin, x, 8);
						}
					}
				}
				mnSetPageArrowsLeftRight(memberColumn, menu->memberCnt);
#if !DJINN_MENU_WAITS_ON_EVERY_REDRAW
				sySynch(1);
#endif
			}
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER))
				mnUiPanelHighlightGridCell(listWin, memberColumn, djinnRow, 1);
			if(selectedRow != -1)
				djinn = backdrop->ids[memberColumn][selectedRow];
			owner = DJINN_MEMBER(djinn);
			element = DJINN_ENTRY_ELEMENT_OF(djinn);
			num = DJINN_NUMBER(djinn);
			state = mnDjinnMenuGetEntryState(djinn);
			if(selectedRow == -1) {
				mnDjinnFiguresSetSlotPos(mode, 0, MENU_FIGURE_HIDDEN_Y);
			} else {
				Sint32 facing;
				Sint32 anm;

				switch(state) {
				case DJINNI_STATE_STANDBY:
					anm = 1;
					facing = FACING_SOUTH;
					break;
				case DJINNI_STATE_SET:
					anm = 2;
					facing = FACING_SOUTH;
					break;
				default:
					anm = 1;
					facing = FACING_WEST;
					break;
				}
				mnDjinnFiguresSetSlotPos(mode, DJINN_FIGURE_X(memberColumn % 4),
					mode == DJINN_SELECT_DJINNI ? DJINN_FIGURE_CHOOSING_Y
												: DJINN_FIGURE_Y);
				mnDjinnFiguresSetSlotFacing(mode, facing);
				if(redraw & DJINN_REDRAW_DJINNI)
					mnDjinnFiguresSetSlotPhase(mode, 0);
				mnDjinnFiguresReplaceSlot(mode, element, anm);
			}
			win = menu->statWin;
			unit = syUnitGet(menu->members[memberColumn]);
			mnWindowSprRedrawSavedRectStub(win);
			mnTextDrawStringAtOffsetStub(unit->name, win, 0, 0);
			mnTextDrawMessageAsTilesStub(
				unit->classId + (Sint32)MsgClassName, win, 0, 8);
			mnTextDrawStringAtOffsetStub(DjinnMenuTextLevel, win, 48, 0);
			mnTextDrawNumInWindowStub(unit->level, 2, win, 72, 0);
			if(mode == DJINN_SELECT_DJINNI)
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDjinnCharacterStatus, win, 0, 16);
			mnWindowSprRedrawSavedRectStub(helpWin);
			if(mode == DJINN_SELECT_DJINNI && selectedRow != -1) {
				if(groupMode) {
					if(setAll == 0)
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgDjinnSetAll, helpWin, 0, 0);
					else
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgDjinnAllStandby, helpWin, 0, 0);
					if(state == DJINNI_STATE_RECOVERING)
						mnTextDrawMessageAsTilesStub(
							(Sint32)MsgDjinnRecovering, helpWin, 0, 16);
				} else {
					mnTextDrawMessageAsTilesStub(
						(Sint32)MsgDjinnChoose, helpWin, 0, 0);
					switch(state) {
					/* "R:Set Select:Help" */
					case DJINNI_STATE_STANDBY:
						msg = (Sint32)MsgDjinnChoose + 3;
						break;
					/* "R:Standby Select:Help" */
					case DJINNI_STATE_SET:
						msg = (Sint32)MsgDjinnChoose + 2;
						break;
					default:
						msg = (Sint32)MsgDjinnRecovering;
						break;
					}
					mnTextDrawMessageAsTilesStub(msg, helpWin, 0, 16);
				}
			}
#if DJINN_MENU_WAITS_ON_EVERY_REDRAW
			sySynch(1);
#endif
			if(mode == DJINN_SELECT_ACTION) {
#if TRADE_HELP_DJINNI_FIRST
				mnWindowSetTilemapEntryStub(helpWin,
					DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) +
						ELEMENT_ICON_TILE,
					0, 0, 0);
				mnMessageArgumentPushStub(
					menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(
					DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) *
							DJINN_PER_ELEMENT +
						DJINN_NUMBER(menu->pairEntries[0]) +
						ABILITY_DJINNI_FIRST,
					MESSAGE_ARGUMENT_ABILITY);
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDjinnOwnerLabel, helpWin, 8, 0);
#else
				mnMessageArgumentPushStub(
					menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDjinnOwnerLabel, helpWin, 0, 0);
				mnMessageArgumentPushStub(
					DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) *
							DJINN_PER_ELEMENT +
						DJINN_NUMBER(menu->pairEntries[0]) +
						ABILITY_DJINNI_FIRST,
					MESSAGE_ARGUMENT_ABILITY);
				mnWindowSetTilemapEntryStub(helpWin,
					DJINN_ENTRY_ELEMENT_OF(menu->pairEntries[0]) +
						ELEMENT_ICON_TILE,
					6, 0, 0);
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDjinnOwnerLabel + 1, helpWin, 56, 0);
#endif
				/* "What will you do?" */
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDjinnOwnerLabel + 2, helpWin, 0, 8);
			}
#if DJINN_MENU_WAITS_ON_EVERY_REDRAW
			sySynch(1);
#endif
			mnWindowSprClearListStub(listWin);
			if(selectedRow != -1) {
#if LANGUAGE != LANGUAGE_JAPANESE
				mnWindowClearInteriorTilesStub(listWin, 0, 96, 224, 104);
#endif
				/* Japanese draws the effect as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
				mnTextDrawMessageStub(
					element * DJINN_PER_ELEMENT + num + (Sint32)MsgDjinnEffect,
					listWin, 0, 96);
#else
				mnTextDrawMessageAsTilesStub(
					element * DJINN_PER_ELEMENT + num + (Sint32)MsgDjinnEffect,
					listWin, 0, 96);
#endif
#if DJINN_EFFECT_CLEARED_WITHOUT_DJINNI
			} else {
				mnWindowClearInteriorTilesStub(listWin, 0, 96, 224, 104);
#endif
			}
			((WIN_STATE *)RamHeapSlots->winState)->dirty = 1;
			redraw = 0;
		}
		sySynch(1);
		if(!(PerWork.data & KEY_R) || (PerWork.pendingPush & KEY_R)) {
			if(groupMode)
				redraw = DJINN_REDRAW_CURSOR;
			groupMode = FALSE;
			setAll = 0;
		}
		repeat = PerWork.repeat;
		pressed = PerWork.push;
		if(backdrop->tutorialStep != DJINN_TUTORIAL_OFF) {
			pressed = 0;
			repeat = 0;
			backdrop->tutorialTmr++;
			switch(backdrop->tutorialStep) {
			case DJINN_TUTORIAL_CLOSE:
				while(!(PerWork.push & KEY_A)) {
					mnCursorPos(150, 26);
					sySynch(1);
				}
				pressed = KEY_B;
				repeat = KEY_B;
				break;
			case DJINN_TUTORIAL_RECOVERY:
				if(backdrop->tutorialTmr == 60) {
					DJINN_TUTORIAL_MESSAGE(
						msgWin, DJINN_TUTORIAL_LINE_RECOVERY);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					sySynch(1);
					DJINN_TUTORIAL_MESSAGE(
						msgWin, DJINN_TUTORIAL_LINE_SET_AGAIN);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					backdrop->tutorialTmr = 0;
					btDjinnRecoveryCountDownStub();
					btDjinnRecoveryCountDownStub();
					btDjinnRecoveryCountDownStub();
					gmDjinnPutOnStandbyStub(FELIX, ELEMENT_VENUS, DJINNI_ECHO);
					gmDjinnRecoveryAddStandbyStub(
						FELIX, ELEMENT_VENUS, DJINNI_ECHO);
					gmUnitCalcStub(FELIX);
					repeat = KEY_B;
					pressed = KEY_B;
				}
				break;
			case DJINN_TUTORIAL_GIVE:
				if(backdrop->tutorialTmr == 60) {
					DJINN_TUTORIAL_MESSAGE(msgWin, DJINN_TUTORIAL_LINE_GIVE);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_GIVE_PICK;
				}
				break;
			case DJINN_TUTORIAL_GIVE_PICK:
			case DJINN_TUTORIAL_GIVE_DROP:
			case DJINN_TUTORIAL_SET_PICK:
			case DJINN_TUTORIAL_SET_CHOOSE:
			case DJINN_TUTORIAL_POWERS_CHOOSE:
				if(backdrop->tutorialTmr == 90) {
					repeat = KEY_A;
					pressed = KEY_A;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep++;
				}
				break;
			case DJINN_TUTORIAL_GIVE_MOVE:
				if(backdrop->tutorialTmr == 90) {
					repeat = KEY_RIGHT;
					pressed = KEY_RIGHT;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_GIVE_DROP;
				}
				break;
			case DJINN_TUTORIAL_SET:
			case DJINN_TUTORIAL_SET_SPARE:
				if(backdrop->tutorialTmr == 60) {
					DJINN_TUTORIAL_MESSAGE(
						msgWin, DJINN_TUTORIAL_LINE_SET_AFTER_GIVE);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_SET_PICK;
				}
				break;
			case DJINN_TUTORIAL_SET_DONE:
				if(backdrop->tutorialTmr == 40) {
					repeat = KEY_B;
					pressed = KEY_B;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep++;
				}
				break;
			case DJINN_TUTORIAL_POWERS:
			case DJINN_TUTORIAL_POWERS_WAIT:
			case DJINN_TUTORIAL_POWERS_WAIT_MORE:
				if(backdrop->tutorialTmr == 40) {
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep++;
				}
				break;
			case DJINN_TUTORIAL_POWERS_TALK:
				if(backdrop->tutorialTmr == 60) {
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					msgWin = mnMessageOpenWindowStub(
						mnGetDjinnGiveMessage() + DJINN_TUTORIAL_LINE_POWERS, 9,
						9, 1);
					mnCursorSlide(2, 146);
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					while(!(PerWork.push & KEY_A)) {
						mnCursorPos(2, 146);
						sySynch(1);
					}
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					sySynch(1);
					msgWin = mnMessageOpenWindowStub(
						mnGetDjinnGiveMessage() + DJINN_TUTORIAL_LINE_UNLEASH,
						9, 9, 1);
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					while(!(PerWork.push & KEY_A)) {
						mnCursorPos(2, 146);
						sySynch(1);
					}
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					sySynch(1);
					msgWin = mnMessageOpenWindowStub(mnGetDjinnGiveMessage() +
							DJINN_TUTORIAL_LINE_DEMONSTRATE,
						9, 9, 1);
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					while(!(PerWork.push & KEY_A)) {
						mnCursorPos(2, 146);
						sySynch(1);
					}
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawFirstPage(backdrop);
					sySynch(1);
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_WAIT;
				}
				break;
			case DJINN_TUTORIAL_POWERS_PICK:
			case DJINN_TUTORIAL_POWERS_PICK_SPARE:
				if(backdrop->tutorialTmr == 90) {
					repeat = KEY_A;
					pressed = KEY_A;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_MOVE;
				}
				break;
			case DJINN_TUTORIAL_POWERS_MOVE:
				if(backdrop->tutorialTmr == 90) {
					repeat = KEY_LEFT;
					pressed = KEY_LEFT;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_CHOOSE;
				}
				break;
			case DJINN_TUTORIAL_POWERS_DONE:
				if(backdrop->tutorialTmr == 60) {
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_POWERS_END;
					repeat = KEY_B;
					pressed = KEY_B;
				}
				break;
			}
		}
		/* R on a Djinni: put it on standby or set it at once. */
		if(mode == DJINN_SELECT_DJINNI && (pressed & KEY_R)) {
			if(selectedRow == -1) {
				sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			groupMode = TRUE;
			PerWork.pendingPush = 0;
			switch(state) {
			case DJINNI_STATE_STANDBY:
				sySndPlayStub(SE_DJINNI_SET);
				gmDjinnSetStub(owner, element, num);
				gmDjinnRecoveryRemoveStub(owner, element, num);
				break;
			case DJINNI_STATE_SET:
				sySndPlayStub(SE_NOTE_HIGH);
				gmDjinnPutOnStandbyStub(owner, element, num);
				gmDjinnRecoveryAddStandbyStub(owner, element, num);
				break;
			default:
				sySndPlayStub(SE_MENU_ERROR);
				redraw = DJINN_REDRAW_CURSOR;
				continue;
			}
			gmUnitCalcStub(owner);
			mnDjinnMenuDrawPage(backdrop, memberColumn / 4 * 4);
			redraw = DJINN_REDRAW_CURSOR;
		}
		if(mode == DJINN_SELECT_DJINNI && (pressed & KEY_L)) {
			result = DJINN_RESULT_STATUS;
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if((pressed & KEY_A) ||
			(mode == DJINN_SELECT_ACTION && (pressed & KEY_R))) {
			if(selectedRow != -1 && state == DJINNI_STATE_RECOVERING) {
				sySndPlayStub(SE_MENU_ERROR);
				mnWindowSprClearListStub(listWin);
#if LANGUAGE != LANGUAGE_JAPANESE
				mnWindowClearInteriorTilesStub(listWin, 0, 96, 224, 104);
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
				mnTextDrawMessageStub(
					(Sint32)MsgDjinnNowRecovering, listWin, 0, 96);
#else
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgDjinnNowRecovering, listWin, 0, 96);
#endif
				continue;
			}
			if(mode == DJINN_SELECT_ACTION) {
				if(memberColumn == menu->selectedColumn[0]) {
					if(menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
						result = DJINN_RESULT_STANDBY;
					else
						result = DJINN_RESULT_SET;
				} else if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER) {
					result = DJINN_RESULT_GIVE;
				} else {
					result = DJINN_RESULT_TRADE;
				}
			} else {
				result = DJINN_RESULT_CHOSEN;
			}
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if(pressed & KEY_START) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_START;
			break;
		}
		if(pressed & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_BACK;
			break;
		}
		/* Select: with R held, set every Djinni not recovering or, the next
		   time, put every one on standby; otherwise open the help. */
		if(mode == DJINN_SELECT_DJINNI && (pressed & KEY_SELECT)) {
			if(groupMode) {
				setAll ^= 1;
				if(setAll)
					sySndPlayStub(SE_DJINNI_SET);
				else
					sySndPlayStub(SE_NOTE_HIGH);
				for(i = 0; i < menu->memberCnt; i++) {
					for(j = 0; j < backdrop->cnts[i]; j++) {
						Uint32 entry = backdrop->ids[i][j];
						Sint32 entryOwner = DJINN_MEMBER(entry);
						Sint32 entryElement = DJINN_ENTRY_ELEMENT_OF(entry);
						Sint32 entryNumber = DJINN_NUMBER(entry);

						switch(mnDjinnMenuGetEntryState(entry)) {
						case DJINNI_STATE_STANDBY:
							if(setAll) {
								gmDjinnSetStub(
									entryOwner, entryElement, entryNumber);
								gmDjinnRecoveryRemoveStub(
									entryOwner, entryElement, entryNumber);
								gmUnitCalcStub(entryOwner);
							}
							break;
						case DJINNI_STATE_SET:
							if(!setAll) {
								gmDjinnPutOnStandbyStub(
									entryOwner, entryElement, entryNumber);
								gmDjinnRecoveryAddStandbyStub(
									entryOwner, entryElement, entryNumber);
								gmUnitCalcStub(entryOwner);
							}
							break;
						}
					}
				}
				mnDjinnMenuDrawPage(backdrop, memberColumn / 4 * 4);
				redraw = DJINN_REDRAW_CURSOR;
			} else {
				mnUiPanelHighlightGridCell(listWin, memberColumn, djinnRow, 0);
				result = DJINN_RESULT_HELP;
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
		}
		if(repeat & KEY_UP) {
			if(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY)
				continue;
			if(backdrop->cnts[memberColumn] == 0)
				continue;
			mnUiPanelHighlightGridCell(listWin, memberColumn, djinnRow, 0);
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER) {
				columnFlg[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
				djinnRow = 0;
			} else if(djinnRow == 0 &&
				(columnFlg[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
				columnFlg[memberColumn] |= DJINN_COLUMN_ON_HEADER;
				redraw = DJINN_REDRAW_DJINNI;
				continue;
			}
			djinnRow--;
			djinnRow = mnWrapIndex(djinnRow, backdrop->cnts[memberColumn]);
			savedRow = djinnRow;
			redraw = DJINN_REDRAW_DJINNI;
			continue;
		}
		if(repeat & KEY_DOWN) {
			if(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY)
				continue;
			if(backdrop->cnts[memberColumn] == 0)
				continue;
			mnUiPanelHighlightGridCell(listWin, memberColumn, djinnRow, 0);
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			djinnRow++;
			djinnRow = mnWrapIndex(djinnRow, backdrop->cnts[memberColumn]);
			if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER) {
				columnFlg[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
				djinnRow = 0;
			} else if(djinnRow == 0 &&
				(columnFlg[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
				columnFlg[memberColumn] |= DJINN_COLUMN_ON_HEADER;
			}
			savedRow = djinnRow;
			redraw = DJINN_REDRAW_DJINNI;
			continue;
		}
		if(repeat & KEYS_HORIZONTAL) {
			Sint32 oldColumn;

			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			oldColumn = memberColumn;
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY))
				mnUiPanelHighlightGridCell(listWin, memberColumn, djinnRow, 0);
			if(repeat & KEY_LEFT)
				memberColumn--;
			else
				memberColumn++;
			if(memberColumn < 0)
				memberColumn = 0;
			if(memberColumn > menu->memberCnt - 1)
				memberColumn = menu->memberCnt - 1;
			if(mode == DJINN_SELECT_DJINNI) {
				for(i = 0; i < menu->memberCnt; i++) {
					if(columnFlg[memberColumn] != DJINN_COLUMN_EMPTY)
						break;
					if(repeat & KEY_LEFT)
						memberColumn--;
					else
						memberColumn++;
					if(memberColumn < 0 || memberColumn >= menu->memberCnt) {
						memberColumn = oldColumn;
						break;
					}
				}
			}
			djinnRow = savedRow;
			result = backdrop->cnts[memberColumn];
			if(result == 0)
				result = 1;
			djinnRow = mnWrapIndex(djinnRow, result);
			redraw = DJINN_REDRAW_DJINNI;
			if(oldColumn / 4 * 4 != memberColumn / 4 * 4)
				redraw = DJINN_REDRAW_PAGE | DJINN_REDRAW_DJINNI;
		}
	}
	menu->selectedColumn[mode] = memberColumn;
	if(selectedRow != -1) {
		Uint32 id;
		Uint32 entry;

		menu->pairEntries[mode] = id = backdrop->ids[memberColumn][selectedRow];
		entry = (Uint16)id;
		menu->pairIdx[mode] = entry % (DJINN_ENTRY_INDEX + 1);
		menu->pairElement[mode] = DJINN_ENTRY_ELEMENT_OF(entry);
		menu->pairMember[mode] = DJINN_MEMBER(entry);
	}
	menu->selection[mode] = memberColumn + djinnRow * DJINN_CURSOR_ROW_STRIDE;
	mnDjinnFiguresSetSlotFacing(mode, FACING_SOUTH);
	return result;
}

#undef DJINN_TUTORIAL_MESSAGE

/* DJINN_ENTRY_IS_SET as a negative int, for entries built in Sint32. */
#define DJINN_ENTRY_SET (-DJINN_ENTRY_IS_SET)

/* Lists party member memberId's Djinn of element as list entries, set ones
   marked; for element -1, every element's, each entry with its party member. */
Sint32 mnDjinnListUnitEntries(
	Uint16 *entriesOut, Sint32 memberId, Sint32 element)
{
	UNIT  *unit = syUnitGet(memberId);
	Sint32 cnt = 0;
	Sint32 elementIdx;
	Sint32 djinnIdx;
	Sint32 encodedDjinn;

	if(element == -1) {
		for(elementIdx = 0; elementIdx < DJINN_ELEMENTS; elementIdx++) {
			for(djinnIdx = 0; djinnIdx < DJINN_PER_ELEMENT; djinnIdx++) {
				if(unit->djinnSet[elementIdx] & (1 << djinnIdx)) {
					encodedDjinn = (elementIdx << DJINN_ENTRY_ELEMENT_SHIFT) |
						djinnIdx | DJINN_ENTRY_SET;
					encodedDjinn |= memberId << DJINN_ENTRY_MEMBER_SHIFT;
					entriesOut[cnt++] = encodedDjinn;
				} else if(unit->djinnHeld[elementIdx] & (1 << djinnIdx)) {
					entriesOut[cnt++] =
						(elementIdx << DJINN_ENTRY_ELEMENT_SHIFT) | djinnIdx |
						(memberId << DJINN_ENTRY_MEMBER_SHIFT);
				}
			}
		}
	} else {
		for(djinnIdx = 0; djinnIdx < DJINN_PER_ELEMENT; djinnIdx++) {
			if(unit->djinnSet[element] & (1 << djinnIdx))
				entriesOut[cnt++] = (element << DJINN_ENTRY_ELEMENT_SHIFT) |
					djinnIdx | DJINN_ENTRY_SET;
			else if(unit->djinnHeld[element] & (1 << djinnIdx))
				entriesOut[cnt++] =
					(element << DJINN_ENTRY_ELEMENT_SHIFT) | djinnIdx;
		}
	}
	return cnt;
}

#undef DJINN_ENTRY_SET

Sint32 mnDjinnMenuDrawStatPreview(Sint32 win, Sint32 x, Sint32 y,
	Sint32 ownerId, Sint32 removePairSlot, Sint32 addPairSlot,
	Sint32 previewMode, Sint32 page, Sint32 side);

/* Draws both sides of a Djinn preview for page pageIdx: the second member in
   the second window and the first in the first. Mode 0 draws the pair members,
   both as a trade (DJINN_PREVIEW_TRADE); any other mode the shown members, the
   second gaining the Djinni and the first losing it. Returns 1. */
Sint32 mnDjinnMenuDrawPairPreview(Sint32 mode, Sint32 pageIdx)
{
	Sint32			  savedMode;
	Uint8			  first;
	Uint8			  owner;
	DJINN_MENU_STATE *menu;

	menu = RamHeapSlots->menuState;
	if(mode == DJINN_COMPARE_TRADE) {
		savedMode = mode;
		first = menu->pairMember[1];
		mnDjinnMenuDrawStatPreview((Sint32)menu->secondWin, 0, 0, first, 1,
			mode, DJINN_PREVIEW_TRADE, pageIdx, 1);
		mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->pairMember[0], mode,
			1, DJINN_PREVIEW_TRADE, pageIdx, mode);
	} else {
		owner = menu->shownMember[1];
		mnDjinnMenuDrawStatPreview((Sint32)menu->secondWin, 0, 0, owner, 1, 0,
			DJINN_PREVIEW_ADD, pageIdx, 1);
		mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->shownMember[0], 0, 0,
			DJINN_PREVIEW_UNSET, pageIdx, 0);
	}
	return 1;
}

/* The stat preview's columns, in pixels from x * 8: the Japanese edition draws
   Attack, Defense and Agility with four digits from HP's column and Luck a tile
   further left, sets every arrow in one column, and puts the Psynergy page's PP
   heading further left.

   The Japanese text leaves the preview's messages unnamed, so each is written
   by its place after a named one: "HP", the first of the six stat labels, 13
   after "Luck rises"; "* Psynergy Gained" and "* Psynergy Lost", the two after
   the status help; "* No change", the one after "Trade". */
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_PREVIEW_DIGITS	   3
#define DJINN_PREVIEW_CURRENT_X	   48
#define DJINN_PREVIEW_LUCK_X	   56
#define DJINN_PREVIEW_POOL_ARROW_X 62 /* beside HP and PP */
#define DJINN_PREVIEW_STAT_ARROW_X 70 /* beside the other stats */
#define DJINN_PREVIEW_PP_X		   88
#define STAT_LABELS_MESSAGE		   ((Sint32)MsgPreviewStatLabel)
#define PSYNERGY_GAINED_MESSAGE	   ((Sint32)MsgPsynergyGained)
#define PSYNERGY_LOST_MESSAGE	   ((Sint32)MsgPsynergyLost)
#define NO_CHANGE_MESSAGE		   ((Sint32)MsgPsynergyUnchanged)
#else
#define DJINN_PREVIEW_DIGITS	   4
#define DJINN_PREVIEW_CURRENT_X	   32
#define DJINN_PREVIEW_LUCK_X	   48
#define DJINN_PREVIEW_POOL_ARROW_X 64
#define DJINN_PREVIEW_STAT_ARROW_X 64
#define DJINN_PREVIEW_PP_X		   56
#define STAT_LABELS_MESSAGE		   ((Sint32)MsgLuckRises + 13)
#define PSYNERGY_GAINED_MESSAGE	   ((Sint32)MsgDjinnStatusHelp + 1)
#define PSYNERGY_LOST_MESSAGE	   ((Sint32)MsgDjinnStatusHelp + 2)
#define NO_CHANGE_MESSAGE		   ((Sint32)MsgDjinnCompareTrade + 1)
#endif

/* HP and PP, four digits wide in every edition, and their maximum and the slash
   before it when nothing changes. */
#define DJINN_PREVIEW_POOL_X		 32
#define DJINN_PREVIEW_MAX_X			 72
#define DJINN_PREVIEW_SLASH_X		 64
/* The y of the preview's stat row `row` (0 to 5: HP, PP, Attack, Defense,
   Agility and Luck) in a window whose top is tile row y. */
#define DJINN_PREVIEW_STAT_Y(y, row) ((y) * 8 + 56 + (row) * 8)
/* The x of a changed stat's new value on that row, for a window whose left is
   tile column x. */
#define DJINN_PREVIEW_VALUE_X(x)	 ((x) * 8 + 72)

/* `/` between the current and the maximum HP and PP in the Djinn preview. */
extern const char DjinnMenuTextSlash[];

void mnIconCreatePsynergyIcon(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 resourceId);
Sint32 mnIconCreateDjinnArrow(WINDOW *win, Sint32 x, Sint32 y, Sint32 variant);

/* Previews a djinni change on a member's status: the unit is saved, the change
   applied and the unit recalculated; page 0 draws the stats with an arrow
   beside each one that changes, later pages the Psynergy gained and lost; then
   the saved unit is copied back. */
Sint32 mnDjinnMenuDrawStatPreview(Sint32 win, Sint32 x, Sint32 y,
	Sint32 ownerId, Sint32 removePairSlot, Sint32 addPairSlot,
	Sint32 previewMode, Sint32 page, Sint32 side)
{
	/* One constant for its whole life: the djinn tile loop starts from it. */
	Sint32 firstElement = 0;
	/* One constant for its whole life: the closing copy's size. */
	Sint32			  size;
	UNIT			 *unit;
	UNIT			 *originalUnit;
	Sint32			  giveElement;
	Sint32			  giveIdx;
	Sint32			  giveSet;
	Sint32			  takeElement;
	Sint32			  takeIdx;
	Sint32			  takeSet;
	Sint32			  lost;
	Sint32			  gained;
	Uint16			  actions[48];
	DJINN_MENU_STATE *menu;

	unit = syUnitGet(ownerId);
	menu = RamHeapSlots->menuState;
	giveElement = menu->pairElement[removePairSlot];
	giveIdx = menu->pairIdx[removePairSlot];
	giveSet = menu->pairEntries[removePairSlot] & DJINN_ENTRY_IS_SET;
	takeElement = menu->pairElement[addPairSlot];
	takeIdx = menu->pairIdx[addPairSlot];
	takeSet = menu->pairEntries[addPairSlot] & DJINN_ENTRY_IS_SET;
	originalUnit = syHeapAlloc(sizeof(UNIT));
	syWordsCopyEntry(originalUnit, unit, sizeof(UNIT));

	if(page == 0) {
		if(previewMode == DJINN_PREVIEW_NO_CHANGE) {
			mnTextDrawNumInWindowStub(unit->hpMax, 4, win,
				x * 8 + DJINN_PREVIEW_MAX_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawNumInWindowStub(unit->ppMax, 4, win,
				x * 8 + DJINN_PREVIEW_MAX_X, DJINN_PREVIEW_STAT_Y(y, 1));
			mnTextDrawNumInWindowStub(unit->hp, 4, win,
				x * 8 + DJINN_PREVIEW_POOL_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawNumInWindowStub(unit->pp, 4, win,
				x * 8 + DJINN_PREVIEW_POOL_X, DJINN_PREVIEW_STAT_Y(y, 1));
			mnTextDrawStringAtOffsetStub((Sint32)DjinnMenuTextSlash, win,
				x * 8 + DJINN_PREVIEW_SLASH_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawStringAtOffsetStub((Sint32)DjinnMenuTextSlash, win,
				x * 8 + DJINN_PREVIEW_SLASH_X, DJINN_PREVIEW_STAT_Y(y, 1));
		} else {
			mnTextDrawNumInWindowStub(unit->hp, 4, win,
				x * 8 + DJINN_PREVIEW_POOL_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawNumInWindowStub(unit->pp, 4, win,
				x * 8 + DJINN_PREVIEW_POOL_X, DJINN_PREVIEW_STAT_Y(y, 1));
		}
		mnTextDrawNumInWindowStub(unit->attack, DJINN_PREVIEW_DIGITS, win,
			x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 2));
		mnTextDrawNumInWindowStub(unit->defense, DJINN_PREVIEW_DIGITS, win,
			x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 3));
		mnTextDrawNumInWindowStub(unit->agility, DJINN_PREVIEW_DIGITS, win,
			x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 4));
		mnTextDrawNumInWindowStub(unit->luck, 2, win,
			x * 8 + DJINN_PREVIEW_LUCK_X, DJINN_PREVIEW_STAT_Y(y, 5));
	}

	switch(previewMode) {
	case DJINN_PREVIEW_ADD_AND_SET:
		gmDjinnGiveStub(ownerId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		gmDjinnSetStub(ownerId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		break;
	case DJINN_PREVIEW_UNSET:
		giveIdx &= DJINN_ENTRY_INDEX;
		gmDjinnPutOnStandbyStub(ownerId, giveElement, giveIdx);
		break;
	case DJINN_PREVIEW_TRADE:
		if(giveSet) {
			giveIdx &= DJINN_ENTRY_INDEX;
			gmDjinnPutOnStandbyStub(ownerId, giveElement, giveIdx);
		}
		gmDjinnGiveStub(ownerId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		if(takeSet)
			gmDjinnSetStub(ownerId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		break;
	case DJINN_PREVIEW_ADD:
		gmDjinnGiveStub(ownerId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		if(takeSet)
			gmDjinnSetStub(ownerId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		break;
	}

	gmUnitCalcStub(ownerId);
	unit = syUnitGet(ownerId);

	if(page == 0) {
		Sint32 label;

		mnTextDrawStringAtOffsetStub(
			(Sint32)unit->name, win, x * 8 + 40, y * 8);
		mnTextDrawStringAtOffsetStub(
			(Sint32)DjinnMenuTextLevel, win, x * 8 + 40, y * 8 + 16);
		mnTextDrawNumInWindowStub(unit->level, 2, win, x * 8 + 88, y * 8 + 16);
		label = STAT_LABELS_MESSAGE;
		mnTextDrawMessageAsTilesStub(
			label, win, x * 8, DJINN_PREVIEW_STAT_Y(y, 0));
		mnTextDrawMessageAsTilesStub(
			label + 1, win, x * 8, DJINN_PREVIEW_STAT_Y(y, 1));
		mnTextDrawMessageAsTilesStub(
			label + 2, win, x * 8, DJINN_PREVIEW_STAT_Y(y, 2));
		mnTextDrawMessageAsTilesStub(
			label + 3, win, x * 8, DJINN_PREVIEW_STAT_Y(y, 3));
		mnTextDrawMessageAsTilesStub(
			label + 4, win, x * 8, DJINN_PREVIEW_STAT_Y(y, 4));
		mnTextDrawMessageAsTilesStub(
			label + 5, win, x * 8, DJINN_PREVIEW_STAT_Y(y, 5));
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgClassName + originalUnit->classId, win, x * 8,
			y * 8 + 32);
	}

	{
		Sint8 i = 0;

		if(page == 0) {
			Sint32 column;

			if(originalUnit->classId != unit->classId) {
				mnTextDrawMessageAsTilesStub(
					(Sint32)MsgClassName + unit->classId, win, x * 8,
					y * 8 + 48);
				/* The down arrow from the class to the next, a tile of BG
				   character block 1, the block after the windows' own. */
				mnWindowSetTilemapEntryStub(win,
					WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP +
						DJINN_BACKDROP_BORDER_TILE),
					x + 2, 5, 0);
			}
			column = x;
			if(originalUnit->classId != unit->classId)
				column += 5;
			for(i = firstElement; i < ELEMENT_COUNT; i++) {
				Sint32 tileColumn;
				Sint32 tile = ELEMENT_ICON_TILE + i;

				tileColumn = column + i * 2;
				mnWindowSetTilemapEntryStub(win, tile, tileColumn, y + 5, 0);
				tileColumn++;
				mnWindowSetTilemapEntryStub(win,
					unit->djinnSetCnts[i] + WINDOW_TILE('0'), tileColumn, y + 5,
					0);
			}
			/* FAKEMATCH(dead): an empty once-only block after the tile loop; as
			 * no block. */
			do {
			} while(0);
			if(unit->hp != originalUnit->hp) {
				mnTextDrawNumInWindowStub(unit->hp, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 0));
				if(unit->hp > originalUnit->hp)
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_DOWN);
			}
			if(unit->pp != originalUnit->pp) {
				mnTextDrawNumInWindowStub(unit->pp, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 1));
				if(unit->pp > originalUnit->pp)
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_POOL_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_DOWN);
			}
			if(unit->attack != originalUnit->attack) {
				mnTextDrawNumInWindowStub(unit->attack, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 2));
				if(unit->attack > originalUnit->attack)
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_DOWN);
			}
			if(unit->defense != originalUnit->defense) {
				mnTextDrawNumInWindowStub(unit->defense, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 3));
				if(unit->defense > originalUnit->defense)
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_DOWN);
			}
			if(unit->agility != originalUnit->agility) {
				mnTextDrawNumInWindowStub(unit->agility, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 4));
				if(unit->agility > originalUnit->agility)
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_DOWN);
			}
			if(unit->luck != originalUnit->luck) {
				mnTextDrawNumInWindowStub(
					unit->luck, 2, win, x * 8 + 88, DJINN_PREVIEW_STAT_Y(y, 5));
				if(unit->luck > originalUnit->luck)
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win,
						x * 8 + DJINN_PREVIEW_STAT_ARROW_X,
						DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_DOWN);
			}
		}
	}

	if(page > 0) {
		Sint32 rows;
		Sint32 first;
		Sint8  cnt;
		Sint8  row;
		Sint8  line;

		rows = 6 - (previewMode != DJINN_PREVIEW_NO_CHANGE);
		first = rows * (page - 1);
		cnt = mnPsynergyListDiff(
			originalUnit->psynergy, unit->psynergy, actions, &gained, &lost);
		line = 0;
		for(row = 0; first < cnt && row < rows; row++, first++) {
			mnIconCreatePsynergyIcon(win, x * 8, (y + line * 2) * 8 + 4,
				actions[first] & PSYNERGY_ABILITY_MASK);
			if(actions[first] & DJINN_PREVIEW_GAINED)
				mnTextSetColStub(TEXT_COLOR_YELLOW);
			else if(actions[first] & DJINN_PREVIEW_LOST)
				mnTextSetColStub(TEXT_COLOR_RED);
			else
				mnTextSetColStub(TEXT_COLOR_NORMAL);
			mnTextDrawMessageAsTilesStub((Sint32)MsgAbilityName +
					(actions[first] & PSYNERGY_ABILITY_MASK),
				win, x * 8 + 16, (y + line * 2) * 8 + 8);
			mnTextDrawNumAtOffsetStub(
				gmAbilityGetDirectStub(actions[first])->ppCost, 2, win,
				x * 8 + 88, (y + line * 2) * 8 + 8);
			line++;
		}
		mnTextSetColStub(TEXT_COLOR_NORMAL);
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgPsynergyPp, win, x * 8 + DJINN_PREVIEW_PP_X, y * 8);
		if(previewMode != DJINN_PREVIEW_NO_CHANGE) {
			Sint32 lines;

			lines = 0;
			if(gained) {
				mnTextSetColStub(TEXT_COLOR_YELLOW);
				mnTextDrawMessageAsTilesStub(
					PSYNERGY_GAINED_MESSAGE, win, x * 8, y * 8 + 88);
				lines = 1;
			}
			if(lost) {
				mnTextSetColStub(TEXT_COLOR_RED);
				mnTextDrawMessageAsTilesStub(
					PSYNERGY_LOST_MESSAGE, win, x * 8, (y + lines) * 8 + 88);
				lines++;
			}
			if(lines == 0)
				mnTextDrawMessageAsTilesStub(
					NO_CHANGE_MESSAGE, win, x * 8, y * 8 + 88);
			mnTextSetColStub(TEXT_COLOR_NORMAL);
			mnWindowDrawDividerLineStub(win, 0, 11, 13, 11);
		}
		((WIN_STATE *)RamHeapSlots->winState)->dirty = 1;
	}

	size = sizeof(UNIT);
	if(page == 0)
		mnPortraitCreateStub(ownerId, 0, side, win, 0, 0);
	syWordsCopyEntry(unit, originalUnit, size);
	syHeapRelease(originalUnit);
	return 1;
}

#undef NO_CHANGE_MESSAGE
