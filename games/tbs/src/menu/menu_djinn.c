/* The Djinn menu: its screen, the Djinn list, help, and choosing the Djinn to
   set, put on standby, give or trade. Part of menu.c. */

#include "menu_internal.h"

/* The editions outside Japanese widen the Djinn list's highlight and lay the
   stat preview out for fewer digits; in every edition but the Japanese a trade
   preview shows its second side in another mode. */
#define DJINN_CURSOR_WIDTH JAPANESE_OR(5, 6)

/* DJINN_MENU_STATE.pairEntries before a Djinni is chosen: every element and
   number bit set, which no list entry has. */
#define DJINN_ENTRY_NONE (DJINN_ENTRY_ELEMENT | DJINN_ENTRY_INDEX)

/* The tile of BG character block 1 the backdrop's digits are copied to. */
#define DJINN_BACKDROP_DIGIT_TILE 128

/* The palettes the Djinn menu takes over, kept in DJINN_MENU_BACKDROP.pal while
   it is open: from DJINN_BACKDROP_PALETTE, a highlighted row's element icons
   (the highlight's palette bit turns BASIC_COLORS_PALETTE into it, as it turns
   WINDOW_PALETTE into HIGHLIGHT_PALETTE), the element icons' own, the dimmed
   window colours a chooser puts behind it and the copy of the window colours
   they are darkened from. */
#define DJINN_BACKDROP_PALETTE	  4
#define DJINN_DIM_PALETTE		  6
#define DJINN_WINDOW_COPY_PALETTE 7

/* The mode of a trade preview's second side. */
#define DJINN_SWAP_PREVIEW_MODE \
	WHEN(DJINN_GIVE_PREVIEW_RECEIVER_AS_TRADE, DJINN_PREVIEW_TRADE, \
		DJINN_PREVIEW_ADD)

/* The stat preview's columns; an arrow stands at x (in tiles) or, in the
   international editions, at the pixel column iconX the preview sets. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_PREVIEW_DIGITS			3
#define DJINN_PREVIEW_CURRENT_X			48
#define DJINN_PREVIEW_MAX_X				80
#define DJINN_PREVIEW_SLASH_X			72
#define DJINN_PREVIEW_LUCK_X			56
#define DJINN_PREVIEW_ARROW_X(x, iconX) (iconX)
#define DJINN_PREVIEW_PP_X				88
#else
#define DJINN_PREVIEW_DIGITS			4
#define DJINN_PREVIEW_CURRENT_X			32
#define DJINN_PREVIEW_MAX_X				72
#define DJINN_PREVIEW_SLASH_X			64
#define DJINN_PREVIEW_LUCK_X			48
#define DJINN_PREVIEW_ARROW_X(x, iconX) ((x) * 8 + 64)
#define DJINN_PREVIEW_PP_X				56
#endif

/* The y of the preview's stat row `row` (0 to 5: HP, PP, Attack, Defense,
   Agility and Luck) in a window whose top is tile row y. */
#define DJINN_PREVIEW_STAT_Y(y, row) ((y) * 8 + 56 + (row) * 8)
/* The x of a changed stat's new value on that row, for a window whose left is
   tile column x. */
#define DJINN_PREVIEW_VALUE_X(x)	 ((x) * 8 + 72)

/* The party's Djinn columns, one a member, 7 tiles wide, and the x of a
   column's figure. */
#define DJINN_COLUMN_TILES	   7
#define DJINN_COLUMN_WIDTH	   (DJINN_COLUMN_TILES * 8)
#define DJINN_FIGURE_X(column) ((column) * DJINN_COLUMN_WIDTH + 48)

/* The cursor packed in DJINN_MENU_STATE.selection: column + row * 10. */
#define DJINN_CURSOR_ROW_STRIDE 10

/* Wraps a cursor index that has stepped one past either end of modulus entries:
   -1 becomes modulus - 1 and modulus becomes 0. */
Sint32 mnWrapIndex(Sint32 value, Sint32 modulus)
{
	return (value + modulus) % modulus;
}

/* Where the first party figure stands, over its Djinn column. */
#define DJINN_MEMBER_FIGURE_X 32
#define DJINN_MEMBER_FIGURE_Y 61

/* Places the four party figures over their Djinn columns, at screen y
   DJINN_MEMBER_FIGURE_Y plus yOfs. Returns that y. */
Sint32 mnDjinnMenuPlacePartyFigures(Sint32 yOfs)
{
	DJINN_MENU_STATE *menu;
	Sint32			  memberX;
	Sint32			  memberIdx;
	menu = MenuState;
	yOfs += DJINN_MEMBER_FIGURE_Y;
	memberX = DJINN_MEMBER_FIGURE_X;
	for(memberIdx = 0; memberIdx < 4; memberIdx++) {
		menu->memberX[memberIdx] = memberX;
		menu->memberY[memberIdx] = yOfs;
		memberX += DJINN_COLUMN_WIDTH;
	}
	return yOfs;
}

/* Runs the Djinn menu over the field: the field menu's Djinn screen
   (mnFieldMenuRun, FIELD_MENU_DJINN), and the Djinn tutorial, which the world
   map opens when Flint joins. */
Sint32 mnDjinnMenuRun(void)
{
	DJINN_MENU_STATE	*menu;
	Uint32				 savedMsgSpeed;
	DJINN_MENU_BACKDROP *backdrop;

	menu = syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	savedMsgSpeed = MainRoutineStateWork.msgSpeed;
	MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_FAST;
	ActorSystemStateWork->suspended = 1;
	mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	sySynch(1);
	mnInitState(0);
	backdrop = syHeapAllocEWRAM(sizeof(*backdrop));
	menu->backdrop = backdrop;
	backdrop->tutorialTmr = 0;
	backdrop->tutorialStep = DJINN_TUTORIAL_OFF;
	/* The tutorial's lesson, from the flags the world map sets for each. */
	if(gmFlagGetStub(FLAG_DJINN_LESSON)) {
		if(!gmFlagGetStub(FLAG_DJINN_LESSON_PART_A)) {
			if(!gmFlagGetStub(FLAG_DJINN_LESSON_PART_B))
				backdrop->tutorialStep = DJINN_TUTORIAL_GIVE;
			else
				backdrop->tutorialStep = DJINN_TUTORIAL_POWERS;
		} else {
			if(!gmFlagGetStub(FLAG_DJINN_LESSON_PART_B))
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
	mnDjinnFiguresInitBottomRow((Sint32)menu->helpWin, 0);
	mnDjinnMenuRunScreen();
	mnDjinnFiguresReleaseBottomRow();
	mnIconMenuReleaseArrows();
	sySynch(1);
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	ActorSystemStateWork->suspended = 0;
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy(BG_CHAR_BLOCK(1), backdrop->tiles, sizeof(backdrop->tiles));
	syWordsCopy((void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
		backdrop->pal, sizeof(backdrop->pal));
	sySynch(1);
	mnResumeFieldEvents();
	mnWindowEraseBorderRectStub(0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
	syHeapRelease(menu->backdrop);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	fdPartyClearUnusableShortcutsStub();
	MainRoutineStateWork.msgSpeed = savedMsgSpeed;
	return 1;
}

/* Finds the moved Djinni in the list and selects its new position. */
#define DJINN_RESELECT_PAIR_ENTRY(menu, i) \
	do { \
		Sint32 column; \
		Sint32 found; \
		Uint8  entry; \
\
		column = (Uint16)((Uint32)(menu)->selection[1] % DJINN_LIST_ROWS); \
		found = 0; \
		entry = (menu)->pairEntries[0]; \
		for((i) = 0; (i) < (menu)->backdrop->list.cnts[column]; (i)++) { \
			if(entry == (Uint8)(menu)->backdrop->list.ids[column][(i)]) { \
				found = (i); \
				break; \
			} \
		} \
		(menu)->selection[0] = column + found * DJINN_LIST_ROWS; \
	} while(0)

/* The screen chooses an operation, optionally previews it, then commits it. */
typedef enum DJINN_MENU_STEP {
	/* After a chooser: a negative result closes the screen, any other goes back
	   to choosing a Djinni. */
	DJINN_STEP_HANDLE_RESULT = 0,
	DJINN_STEP_RESERVED = 1, /* nothing sets it */
	DJINN_STEP_SELECT_DJINN = 2,
	DJINN_STEP_SELECT_ACTION = 3, /* what to do with the chosen Djinni */
	/* After a second choice: back to the Djinn, or on to a trade's second
	   Djinni or a gift's confirmation. */
	DJINN_STEP_HANDLE_PAIR_RESULT = 4,
	DJINN_STEP_CONFIRM_SET = 5,
	DJINN_STEP_CONFIRM_STANDBY = 6,
	DJINN_STEP_CONFIRM_GIVE = 7,
	DJINN_STEP_SELECT_SECOND = 8, /* the other Djinni of a trade */
	DJINN_STEP_CONFIRM_TRADE = 9,
	/* the status of the chosen column's party member */
	DJINN_STEP_SHOW_STATUS = 10,
	DJINN_STEP_APPLY_SET = 11,
	DJINN_STEP_APPLY_STANDBY = 12,
	DJINN_STEP_APPLY_GIVE = 13,
	DJINN_STEP_APPLY_TRADE = 14,
	DJINN_STEP_SHOW_HELP = 15
} DJINN_MENU_STEP;

/* Chooses a Djinni, previews changes, then sets, stands by, gives or trades
   it. */
Sint32 mnDjinnMenuRunScreen(void)
{
	DJINN_MENU_STATE *menu;
	WINDOW_SPR		 *cursor;
	Sint32			  result;
	Sint32			  step;
	Sint32			  menuResult;
	Bool			  done;
	Sint32			  listIdx;

	done = FALSE;
	result = 0;
	menuResult = 0;
	menu = MenuState;
	cursor = menu->cursor;
	cursor->motion = ICON_HIDDEN;
	cursor->anmStep = 0;
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
		case DJINN_STEP_RESERVED:
			break;
		case DJINN_STEP_SELECT_DJINN:
			mnDjinnMenuPlacePartyFigures(0);
			mnDjinnFiguresSetSlotPos(1, 0, MENU_FIGURE_HIDDEN_Y, 0);
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
			result = mnDjinnMenuShowMemberStat();
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_SHOW_HELP:
			result = mnDjinnMenuShowHelp();
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_SELECT_SECOND:
			step = DJINN_STEP_HANDLE_RESULT;
			if(menu->pairCnt == 0)
				break;
			result = mnDjinnMenuSelectDjinn(DJINN_SELECT_ACTION);
			if(result == DJINN_RESULT_START)
				done = TRUE;
			step = DJINN_STEP_HANDLE_PAIR_RESULT;
			if(result < 0)
				break;
			step = DJINN_STEP_CONFIRM_TRADE;
			break;
		/* Choose what to do with it. The figures go up 8 pixels while this
		   chooser runs. */
		case DJINN_STEP_SELECT_ACTION:
			mnDjinnMenuDrawElementList(&menu->backdrop->list);
			mnDjinnMenuPlacePartyFigures(-8);
			menu->member = menu->members[menu->selectedColumn[0]];
			menu->shownMember[0] = menu->members[menu->selectedColumn[0]];
			mnDjinnFiguresSetSlotPos(
				0, DJINN_FIGURE_X(menu->selectedColumn[0]), 54, 0);
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
			result = (Sint32)gmDjinnRecoveryAddStandbyStub(
				menu->shownMember[0], menu->pairElement[0], menu->pairIdx[0]);
			gmUnitCalcStub(menu->shownMember[0]);
			menu->cursor->motion = ICON_HIDDEN;
			mnWindowSprClearListStub(menu->listWin);
			menu->cursor->motion = ICON_SHOWN;
			step = DJINN_STEP_SELECT_DJINN;
			break;
		case DJINN_STEP_CONFIRM_TRADE:
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
		case DJINN_STEP_HANDLE_PAIR_RESULT:
			if(result == DJINN_RESULT_BACK) {
				menuResult = result;
				step = DJINN_STEP_SELECT_DJINN;
			} else if(menu->pending & DJINN_PENDING_SECOND)
				step = DJINN_STEP_SELECT_SECOND;
			else if(menu->pending & DJINN_PENDING_CONFIRM)
				step = DJINN_STEP_CONFIRM_GIVE;
			break;
		case DJINN_STEP_CONFIRM_SET:
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

/* Writes copies of WINDOW_PALETTE, BASIC_COLORS_PALETTE and
   DJINN_WINDOW_COPY_PALETTE, each brightnessDelta lighter in every channel
   (clamped to 0 to RGB_LEVEL_MAX), into the palette before each:
   HIGHLIGHT_PALETTE, DJINN_BACKDROP_PALETTE and DJINN_DIM_PALETTE. The last
   copy is 12 levels darker instead. */
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
			paletteNum = BASIC_COLORS_PALETTE;
		} else {
			paletteNum = DJINN_WINDOW_COPY_PALETTE;
			brightnessDelta = -12;
		}
	}
}

/* mnPsynergyListDiff's marks on an action id: Psynergy the change gives, drawn
   in TEXT_COLOR_YELLOW, and Psynergy it takes away, in TEXT_COLOR_RED. */
#define DJINN_PREVIEW_GAINED 0x8000
#define DJINN_PREVIEW_LOST	 0x4000

/* Draws the backdrop behind the Djinn menu's list: its window, tiles and
   palettes. */
MENU_BACKDROP_RESULT mnOpenBackdropScreen(void)
{
	/* UB: the implicit int call conflicts with DrawElementList's void
	   definition. The sole caller ignores this wrapper's result. */
	DJINN_MENU_STATE	*menu = MenuState;
	DJINN_MENU_BACKDROP *backdrop = menu->backdrop;

	mnWindowUpdateOrCreate(&menu->listWin, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
	sySynch(1);
	syWordsCopy(backdrop->tiles, BG_CHAR_BLOCK(1), sizeof(backdrop->tiles));
	syWordsCopy(backdrop->pal,
		(void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
		sizeof(backdrop->pal));
	syWordsFill(BG_CHAR_BLOCK(1), sizeof(backdrop->tiles), PIXELS_4BPP(3));
	/* Every colour of the backdrop's palettes RGB(21, 10, 21), two to a
	   word. */
	syWordsFill((void *)&BG_PLTT_COLOR(DJINN_BACKDROP_PALETTE, 0),
		sizeof(backdrop->pal), RGB(21, 10, 21) << 16 | RGB(21, 10, 21));
	mnGraphicsExpandVRAMTilesByColTableStub((Uint16 *)(BG_CHAR_BLOCK(1) +
		DJINN_BACKDROP_DIGIT_TILE * TILE_SIZE_4BPP));
	syWordsCopy(BG_CHAR_BLOCK(1) + DJINN_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
		Menu_ChangeArrowTile, TILE_SIZE_4BPP);
	syDMAXCopy(syPaletteGetBasicSprCols(),
		(void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
		(void *)&BG_PLTT_COLOR(DJINN_WINDOW_COPY_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	mnDjinnMenuAdjustPalettes(8);
	BG_PLTT_COLOR(DJINN_WINDOW_COPY_PALETTE, WINDOW_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	BG_PLTT_COLOR(DJINN_DIM_PALETTE, WINDOW_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
#if FIX_FUNCTION_SIGNATURES
	mnDjinnMenuDrawElementList(&backdrop->list);
#else
	return mnDjinnMenuDrawElementList(&backdrop->list);
#endif
}

/* Lists the actions after a preview, marking gains, then appends the losses.
   The two output counts distinguish gained and lost actions. */
Sint32 mnPsynergyListDiff(PSYNERGY_ENTRY *before, PSYNERGY_ENTRY *after,
	Uint16 *actionsOut, Sint32 *gainedCntOut, Sint32 *lostCntOut)
{
	Sint32 actionIdx;
	Sint32 otherIdx;
	Sint32 actionCnt;
	Sint32 gainedCnt;
	Sint32 lostCnt;

	actionCnt = 0;
	gainedCnt = 0;
	lostCnt = 0;
	for(actionIdx = 0;
		actionIdx < PSYNERGY_SLOTS && after[actionIdx].encodedAction != 0;
		actionIdx++) {
		actionsOut[actionCnt] =
			after[actionIdx].encodedAction & PSYNERGY_ABILITY_MASK;
		actionCnt++;
		for(otherIdx = 0; otherIdx < PSYNERGY_SLOTS; otherIdx++) {
			if(((after[actionIdx].encodedAction ^
					before[otherIdx].encodedAction) &
				   PSYNERGY_ABILITY_MASK) == 0)
				break;
		}
		if(otherIdx == PSYNERGY_SLOTS) {
			gainedCnt++;
			actionsOut[actionCnt - 1] |= DJINN_PREVIEW_GAINED;
		}
	}
	for(actionIdx = 0;
		actionIdx < PSYNERGY_SLOTS && before[actionIdx].encodedAction != 0;
		actionIdx++) {
		for(otherIdx = 0; otherIdx < PSYNERGY_SLOTS; otherIdx++) {
			if(((before[actionIdx].encodedAction ^
					after[otherIdx].encodedAction) &
				   PSYNERGY_ABILITY_MASK) == 0)
				break;
		}
		if(otherIdx == PSYNERGY_SLOTS) {
			lostCnt++;
			actionsOut[actionCnt] =
				(before[actionIdx].encodedAction & PSYNERGY_ABILITY_MASK) |
				DJINN_PREVIEW_LOST;
			actionCnt++;
		}
	}
	*gainedCntOut = gainedCnt;
	*lostCntOut = lostCnt;
	return actionCnt;
}

/* Rebuilds the Djinn lists in table: each party member's packed Djinn entries
   and their count. */
Sint32 mnDjinnMenuBuildLists(void *tbl)
{
	DJINN_MENU_STATE *menu;
	Sint32			  memberIdx;
	Uint16 (*entries)[DJINN_LIST_ROWS];
	Uint16 *members;
	Sint8  *cnts;
	Sint16	entryCnt;
	Sint32	memberCnt;

	menu = MenuState;
	memberIdx = 0;
	if(memberIdx < menu->memberCnt) {
		cnts = ((DJINN_LIST_TABLE *)tbl)->cnts;
		members = menu->members;
		entries = ((DJINN_LIST_TABLE *)tbl)->ids;
		do {
			entryCnt = mnDjinnListUnitEntries(*entries, *members, -1);
			memberCnt = menu->memberCnt;
			memberIdx += 1;
			*cnts = entryCnt;
			members += 1;
			cnts += 1;
			entries++;
		} while(memberIdx < memberCnt);
	}
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* Draws each party member's Djinn column, including set and recovering entries.
   Rebuilds table's lists first. */
void mnDjinnMenuDrawElementList(DJINN_LIST_TABLE *tbl)
{
	DJINN_MENU_STATE *menu;
	Sint32			  listIdx;
	Sint32			  row;
	Sint32			  element;
	WIN_STATE		 *state;
	Sint32			  lineIdx;
	Sint32			  canChangeState;
	Uint16			 *entries;

	menu = MenuState;
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW);
	state->holdTilemapUpload = 1;
	listIdx = 0;
	if(menu->memberCnt != 0) {
		do {
			tbl->cnts[listIdx] = mnDjinnListUnitEntries(
				tbl->ids[listIdx], menu->members[listIdx], -1);
			listIdx++;
		} while(listIdx < menu->memberCnt);
	}
	mnWindowSprRedrawSavedRectStub(menu->listWin);
	mnTextDrawMessageAsTilesStub(MsgUnleashEffect, menu->listWin, 0, 80);
	for(row = 0; row < menu->memberCnt; row++) {
		entries = tbl->ids[row];
		lineIdx = 0;
		for(element = 0; element < DJINN_ELEMENTS; element++) {
			for(listIdx = 0; listIdx < tbl->cnts[row]; listIdx++) {
				if(element != DJINN_ELEMENT(entries[listIdx]))
					continue;
				/* TEXT_COLOR_RED when not set; TEXT_COLOR_YELLOW over it when
				   it is neither set nor ready to set, that is recovering (or
				   held by a summon). */
				if((entries[listIdx] & DJINN_ENTRY_IS_SET) == 0)
					mnTextSetColStub(TEXT_COLOR_RED);
				canChangeState = 0;
				if(gmDjinnCanSetStub(DJINN_MEMBER(entries[listIdx]),
					   DJINN_ELEMENT(entries[listIdx]),
					   DJINN_NUMBER(entries[listIdx])) ||
					gmDjinnIsSetStub(DJINN_MEMBER(entries[listIdx]),
						DJINN_ELEMENT(entries[listIdx]),
						DJINN_NUMBER(entries[listIdx])))
					canChangeState = 1;
				if(!canChangeState)
					mnTextSetColStub(TEXT_COLOR_YELLOW);
				mnWindowSetTilemapEntryStub(menu->listWin,
					DJINN_ELEMENT(entries[listIdx]) + DJINN_ELEMENT_TILE,
					row * DJINN_COLUMN_TILES + 1, lineIdx + 2,
					TILEMAP_ENTRY_PLAIN);
				mnTextDrawMessageAsTilesStub(MsgDjinnName +
						DJINN_ELEMENT(entries[listIdx]) * DJINN_PER_ELEMENT +
						DJINN_NUMBER(entries[listIdx]),
					menu->listWin, row * DJINN_COLUMN_WIDTH + 16,
					lineIdx * 8 + 16);
				lineIdx++;
				mnTextSetColStub(TEXT_COLOR_NORMAL);
			}
		}
	}
	mnWindowDrawDividerLineStub(menu->listWin, 0, 10, 28, 10);
	WINDOW_STATE->dirty = 1;
	state->holdTilemapUpload = 0;
}

/* Sets the palette of a rectangle of window's interior, (x, y) and its size in
   tiles from the interior's corner, through mnWindowHighlightRectStub. */
Sint32 mnWindowSetInteriorPaletteBitRect(const WINDOW *win, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Sint32 palette)
{
	mnWindowHighlightRectStub(
		win->x + x + 1, win->y + y + 1, width, height, palette);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* Moves each tile of a screen rectangle (in tiles) that shows WINDOW_PALETTE to
   palette, and marks each row changed for upload. */
void mnWindowSetRectPalette(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette)
{
	WIN_STATE *state;
	Uint16	  *tileCursor;
	Sint32	   column;
	Uint32	   tile;
	Uint32	   masked;

	state = WINDOW_STATE;
	palette = BG_TILE_PALETTE(palette);
	if(x < 0) {
		width += x;
		x = 0;
	}
	if(x + width > DISPLAY_TILE_WIDTH - 1)
		width = DISPLAY_TILE_WIDTH - x;
	if(y < 0) {
		height += y;
		y = 0;
	}
	/* BUG: tests the bottom against the screen's width but clips to its rows,
	   so a rectangle ending in rows 20 to 28 is kept whole and recolours past
	   the 32 by 20 tilemap. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1)
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1)
#endif
		height = DISPLAY_TILE_HEIGHT - y;

	if(width > 0 && height > 0) {
		do {
			tileCursor = &((Uint16 (*)[BG_SCREEN_WIDTH])state->tilemap)[y][x];
			for(column = width; column != 0; column--) {
				tile = *tileCursor;
				if(BG_TILE_PALETTE_OF(tile) == WINDOW_PALETTE) {
					masked = tile & ~BG_TILE_PALETTE(0xf);
					*tileCursor = masked | palette;
				}
				tileCursor++;
			}
			mnWindowMarkTilemapRow(state, y);
			height--;
			y++;
		} while(height != 0);
	}
}

/* mnWindowSetRectPalette on a rectangle of window's interior, (x, y) and its
   size in tiles from the interior's corner. */
void mnWindowSetInteriorRectPalette(
	void *win, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 palette)
{
	mnWindowSetRectPalette(((WINDOW *)win)->x + x + 1,
		((WINDOW *)win)->y + y + 1, width, height, palette);
}

/* The help's topics: Djinn, Set, Standby, Summon, Recovery, Class and
   Psynergy. */
#define DJINN_HELP_TOPICS 7

/* The Djinn menu's help, opened by Select on the first chooser: the list of
   topics with the chosen one's text typed beside it. */
Sint32 mnDjinnMenuShowHelp(void)
{
	WINDOW			 *helpWin;
	WINDOW			 *statWin;
	DJINN_MENU_STATE *menu;
	WIN_STATE		 *winState;
	Sint32			  result;
	Sint32			  prev;
	Sint32			  selection;
	WINDOW			 *list;
	MESSAGE			 *typing;
	Sint32			  msg;
	Sint32			  helpIdx;
	Sint32			  listMsg;

	menu = MenuState;
	winState = HEAP_SLOT_BLOCK(HEAP_SLOT_WINDOW);
	result = 0;
	prev = 0;
	selection = 0;
	mnWindowSprClearListStub(menu->listWin);
	sySynch(1);
	mnWindowSprPrepareForRedrawStub(menu->helpWin);
	msg = MsgDjinnInfoPrompt;
	mnTextDrawMessageAsTilesStub(msg, menu->helpWin, 0, 0);
	msg++;
	mnTextDrawMessageAsTilesStub(msg, menu->helpWin, 0, 16);
	/* Dim the party window and the Djinn list behind the help. */
	mnWindowSetRectPalette(1, 1, 11, 3, DJINN_DIM_PALETTE);
	mnWindowSetInteriorRectPalette(
		(void *)menu->listWin, 0, 0, 28, 10, DJINN_DIM_PALETTE);
	list = mnWindowCreateStub(
		0, 9, 8, 10, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	helpWin = mnWindowCreateStub(8, 12, 22, 7, WINDOW_FLAG_INSTANT);
	statWin = mnWindowCreateStub(8, 9, 22, 3, WINDOW_FLAG_INSTANT);
	mnWindowMarkVisibleTextTilesStub();
	for(helpIdx = 0, listMsg = MsgDjinnInfoTopic; helpIdx < DJINN_HELP_TOPICS;
		helpIdx++)
		mnTextDrawMessageAsTilesStub(helpIdx + listMsg, list, 0, helpIdx * 8);
	do {
		mnWindowSprPrepareForRedrawStub(statWin);
		mnTextDrawMessageStub(selection + MsgDjinnInfoTopic, statWin, 0, 0);
		typing = (MESSAGE *)mnMessageOpenDecodedStub(
			helpWin, selection + MsgDjinnInfoText);
		mnWindowSetInteriorPaletteBitRect(list, 0, prev, 6, 1, WINDOW_PALETTE);
		mnWindowSetInteriorPaletteBitRect(
			list, 0, selection, 6, 1, HIGHLIGHT_PALETTE);
		prev = selection;
		for(;;) {
			mnCursorPos(-12, (list->y + selection) * 8 + 8);
			sySynch(1);
			if(PerRepeat & (KEY_RIGHT | KEY_DOWN)) {
				selection++;
				selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				break;
			}
			if(PerRepeat & (KEY_LEFT | KEY_UP)) {
				selection--;
				selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				break;
			} else if(PerPush & KEY_START) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = DJINN_RESULT_START;
				break;
			} else if(PerPush & (KEY_B | KEY_SELECT)) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = DJINN_RESULT_BACK;
				break;
			} else if(PerPush & KEY_A) {
				if(mnMessageAllTypedStub()) {
					selection++;
					selection = mnWrapIndex(selection, DJINN_HELP_TOPICS);
					sySndPlayStub(SE_MENU_CONFIRM);
					break;
				}
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		/* Before the next topic: free the page arrow's VRAM slot the text took,
		   and stop its typing. */
		if(winState->glyphResource != NO_GLYPH_RESOURCE) {
			syVRAMCacheFreeEntry(winState->glyphResource);
			winState->glyphResource = NO_GLYPH_RESOURCE;
		}
		WINDOW_STATE->pageArrowShown = 0;
		mnWindowSprPrepareForRedrawStub(helpWin);
		{
			WINDOW *win = typing->win;

			win->zoomStage = 0;
			win->duration = 0;
			win->state = 0;
		}
		typing->win = NULL;
	} while(result == 0);
	WINDOW_STATE->holdTilemapUpload = 1;
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
		WINDOW_STATE->holdTilemapUpload = 0;
	}
	sySetEvent(mnPlacePartyFigures, EVENT_PASS_TICK);
	return result;
}

/* DJINN_ENTRY_IS_SET read as a signed halfword, for entries built in Sint32. */
#define DJINN_ENTRY_SET ((Sint16)DJINN_ENTRY_IS_SET)

/* The flags mnDjinnMenuSelectDjinn keeps for each party member's column. */
typedef enum DJINN_COLUMN_FLAG {
	DJINN_COLUMN_ON_HEADER = 1, /* the cursor is on the column's header */
	DJINN_COLUMN_HEADER_CHOOSABLE = 2, /* its header can be chosen */
	DJINN_COLUMN_EMPTY = 4			   /* no Djinni to choose */
} DJINN_COLUMN_FLAG;

/* What mnDjinnMenuSelectDjinn redraws: everything, or after a cursor move,
   which also makes the Djinni figure grow in again. */
typedef enum DJINN_REDRAW {
	DJINN_REDRAW_ALL = 1,
	DJINN_REDRAW_MOVED = 2
} DJINN_REDRAW;

/* The Djinn list's chooser. DJINN_SELECT_DJINNI picks a Djinni in any party
   member's column: R sets it or puts it on standby at once, R held with Select
   does so for every Djinni, L shows the member's status and Select the help.
   DJINN_SELECT_ACTION picks what to do with it, by the column chosen. While the
   tutorial runs, its steps press the buttons. */
Sint32 mnDjinnMenuSelectDjinn(Sint32 mode)
{
	Sint8				 columnFlg[DJINN_LIST_MEMBERS];
	Uint8				 requiresTrade[16];
	DJINN_MENU_STATE	*menu = MenuState;
	DJINN_MENU_BACKDROP *backdrop = menu->backdrop;
	UNIT				*unit;
	WINDOW				*win;
	Sint32				 redraw = DJINN_REDRAW_ALL;
	Uint32				 refresh;
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
	Sint32 result;
	Sint32 msg;
	Uint32 pressed;
	Sint32 cnt;
	Uint32 repeat;
	WINDOW *msgWin;
	Sint32	tutorialStep;
	Sint8  *stat;

	/* Each column's DJINN_COLUMN_ flags. */
	selectedRow = -1;
	stat = columnFlg;
	for(i = 0; i < DJINN_LIST_MEMBERS; i++)
		stat[i] = 0;
	if(mode == DJINN_SELECT_DJINNI) {
		mnDjinnMenuDrawElementList(&backdrop->list);
		for(i = 0; i < menu->memberCnt; i++) {
			if(backdrop->list.cnts[i] == 0)
				stat[i] = DJINN_COLUMN_EMPTY;
		}
		savedRow = djinnRow;
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
				if(backdrop->list.cnts[i] == 0)
					stat[i] = columnFlg[i] | DJINN_COLUMN_EMPTY;
			}
		}
	}
	/* DJINN_SELECT_ACTION: keep the first Djinni highlighted, dim the rest of
	   its column, and label each column's header with what choosing it does. */
	if(mode == DJINN_SELECT_ACTION) {
		Sint32 fromX;
		Sint32 fromY;
		Uint16 srcCursor;
		Sint32 n;
		srcCursor = menu->selection[0];
		fromX = (Uint16)((Uint32)srcCursor % (Uint32)DJINN_CURSOR_ROW_STRIDE);
		fromY = (Uint16)((Uint32)srcCursor / (Uint32)DJINN_CURSOR_ROW_STRIDE);
		mnWindowSetInteriorPaletteBitRect(menu->listWin,
			fromX * DJINN_COLUMN_TILES + 1, fromY + 2, DJINN_CURSOR_WIDTH, 1,
			HIGHLIGHT_PALETTE);
		mnWindowSetInteriorRectPalette(menu->listWin,
			fromX * DJINN_COLUMN_TILES + 1, 2, DJINN_CURSOR_WIDTH, 7,
			DJINN_DIM_PALETTE);
		for(n = 0; n < menu->memberCnt; n++) {
			if(n == menu->selectedColumn[0]) {
				if(menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
					msg = MsgDjinnStandbyAction;
				else
					msg = MsgDjinnSetAction;
			} else if(columnFlg[n] & DJINN_COLUMN_HEADER_CHOOSABLE) {
				msg = MsgDjinnGiveAction;
			} else {
				msg = MsgDjinnTradeAction;
			}
			mnTextDrawMessageAsTilesStub(
				msg, menu->listWin, n * DJINN_COLUMN_WIDTH + 8, 8);
		}
	}
	mnWindowSprRedrawSavedRectStub(menu->helpWin);
	menu->cursor->motion = ICON_SHOWN;
	for(;;) {
		if(redraw != 0) {
			refresh = redraw;
			redraw = 0;
			selectedRow = -1;
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER))
				selectedRow = djinnRow;
			win = menu->statWin;
			unit = gmUnitGetStub(menu->members[memberColumn]);
			mnWindowSprRedrawSavedRectStub(win);
			mnTextDrawStringAtOffsetStub(unit->name, win, 0, 0);
			mnTextDrawMessageAsTilesStub(
				unit->classId + MsgClassName, win, 0, 8);
			mnTextDrawStringAtOffsetStub(DjinnMenuTextLevel, win, 48, 0);
			mnTextDrawNumInWindowStub(unit->level, 2, win, 72, 0);
			if(mode == DJINN_SELECT_DJINNI)
				mnTextDrawMessageAsTilesStub(
					MsgDjinnCharacterStatus, win, 0, 16);
			if(selectedRow != -1)
				djinn = backdrop->list.ids[memberColumn][selectedRow];
			mnWindowSprRedrawSavedRectStub(menu->helpWin);
			/* DJINN_SELECT_ACTION's help: `<member>'s`, the element icon and
			   the Djinni's name, an ability name argument (its action, from
			   ABILITY_DJINNI_FIRST), then "What will you do?". */
			if(mode == DJINN_SELECT_ACTION) {
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || \
	LANGUAGE == LANGUAGE_ITALIAN
				mnWindowSetTilemapEntryStub(menu->helpWin,
					DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE, 0,
					0, TILEMAP_ENTRY_PLAIN);
				mnMessageArgumentPushStub(
					menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(
					DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
						DJINN_NUMBER(menu->pairEntries[0]) +
						ABILITY_DJINNI_FIRST,
					MESSAGE_ARGUMENT_ABILITY);
				mnTextDrawMessageAsTilesStub(
					MsgDjinnOwnerLabel, menu->helpWin, 8, 0);
#else
				mnMessageArgumentPushStub(
					menu->shownMember[0], MESSAGE_ARGUMENT_ACTOR);
				mnTextDrawMessageAsTilesStub(
					MsgDjinnOwnerLabel, menu->helpWin, 0, 0);
				mnMessageArgumentPushStub(
					DJINN_ELEMENT(menu->pairEntries[0]) * DJINN_PER_ELEMENT +
						DJINN_NUMBER(menu->pairEntries[0]) +
						ABILITY_DJINNI_FIRST,
					MESSAGE_ARGUMENT_ABILITY);
				mnWindowSetTilemapEntryStub(menu->helpWin,
					DJINN_ELEMENT(menu->pairEntries[0]) + DJINN_ELEMENT_TILE, 6,
					0, TILEMAP_ENTRY_PLAIN);
				mnTextDrawMessageAsTilesStub(
					MsgDjinnOwnerLabel + 1, menu->helpWin, 56, 0);
#endif
				mnTextDrawMessageAsTilesStub(
					MsgDjinnOwnerLabel + 2, menu->helpWin, 0, 8);
			}
			/* The Djinni's figure, its element's: standing for one on standby
			   or recovering, walking for one that is set. */
			if(selectedRow == -1) {
				mnDjinnFiguresSetSlotPos(mode, 0, MENU_FIGURE_HIDDEN_Y, 0);
			} else if(mode == DJINN_SELECT_DJINNI) {
				if(groupMode) {
					if(setAll == 0) {
						mnTextDrawMessageAsTilesStub(
							MsgDjinnSetAll, menu->helpWin, 0, 0);
					} else {
						mnTextDrawMessageAsTilesStub(
							MsgDjinnAllStandby, menu->helpWin, 0, 0);
					}
					if(gmDjinnCanSetStub(DJINN_MEMBER(djinn),
						   DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) ||
						gmDjinnIsSetStub(DJINN_MEMBER(djinn),
							DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
						if(gmDjinnCanSetStub(DJINN_MEMBER(djinn),
							   DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
							mnDjinnFiguresReplaceSlot(
								mode, DJINN_ELEMENT(djinn), ANIM_STAND);
						else
							mnDjinnFiguresReplaceSlot(
								mode, DJINN_ELEMENT(djinn), ANIM_WALK);
						mnDjinnFiguresSetSlotPos(
							mode, DJINN_FIGURE_X(memberColumn), 62, 0);
					} else {
						mnTextDrawMessageAsTilesStub(
							MsgDjinnRecovering, menu->helpWin, 0, 16);
						mnDjinnFiguresReplaceSlot(
							mode, DJINN_ELEMENT(djinn), ANIM_STAND);
						mnDjinnFiguresSetSlotPos(
							mode, DJINN_FIGURE_X(memberColumn), 62, 1);
					}
					if(refresh & DJINN_REDRAW_MOVED)
						mnDjinnFiguresSetSlotPhase(mode, 0);
				} else {
					mnTextDrawMessageAsTilesStub(
						MsgDjinnChoose, menu->helpWin, 0, 0);
					if(gmDjinnCanSetStub(DJINN_MEMBER(djinn),
						   DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) ||
						gmDjinnIsSetStub(DJINN_MEMBER(djinn),
							DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
						if(gmDjinnCanSetStub(DJINN_MEMBER(djinn),
							   DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
							mnTextDrawMessageAsTilesStub(
								MsgDjinnChoose + 3, menu->helpWin, 0, 16);
							mnDjinnFiguresReplaceSlot(
								mode, DJINN_ELEMENT(djinn), ANIM_STAND);
						} else {
							mnTextDrawMessageAsTilesStub(
								MsgDjinnChoose + 2, menu->helpWin, 0, 16);
							mnDjinnFiguresReplaceSlot(
								mode, DJINN_ELEMENT(djinn), ANIM_WALK);
						}
						mnDjinnFiguresSetSlotPos(
							mode, DJINN_FIGURE_X(memberColumn), 62, 0);
					} else {
						mnTextDrawMessageAsTilesStub(
							MsgDjinnChoose + 4, menu->helpWin, 0, 16);
						mnDjinnFiguresReplaceSlot(
							mode, DJINN_ELEMENT(djinn), ANIM_STAND);
						mnDjinnFiguresSetSlotPos(
							mode, DJINN_FIGURE_X(memberColumn), 62, 1);
					}
					if(refresh & DJINN_REDRAW_MOVED)
						mnDjinnFiguresSetSlotPhase(mode, 0);
				}
			} else {
				if(gmDjinnCanSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
					   DJINN_NUMBER(djinn)) ||
					gmDjinnIsSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
						DJINN_NUMBER(djinn))) {
					if(gmDjinnCanSetStub(DJINN_MEMBER(djinn),
						   DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
						mnDjinnFiguresReplaceSlot(
							mode, DJINN_ELEMENT(djinn), ANIM_STAND);
					else
						mnDjinnFiguresReplaceSlot(
							mode, DJINN_ELEMENT(djinn), ANIM_WALK);
					mnDjinnFiguresSetSlotPos(
						mode, DJINN_FIGURE_X(memberColumn), 54, 0);
				} else {
					mnDjinnFiguresReplaceSlot(
						mode, DJINN_ELEMENT(djinn), ANIM_STAND);
					mnDjinnFiguresSetSlotPos(
						mode, DJINN_FIGURE_X(memberColumn), 54, 1);
				}
				if(refresh & DJINN_REDRAW_MOVED)
					mnDjinnFiguresSetSlotPhase(mode, 0);
			}
			mnWindowSprClearListStub(menu->listWin);
			if(selectedRow != -1) {
				mnTextDrawMessageAsTilesStub(
					MsgUnleashEffect, menu->listWin, 0, 80);
#if LANGUAGE != LANGUAGE_JAPANESE
				mnWindowClearInteriorTilesStub(menu->listWin, 0, 96, 224, 104);
				mnTextDrawMessageAsTilesStub(
					DJINN_ELEMENT(djinn) * DJINN_PER_ELEMENT +
						DJINN_NUMBER(djinn) + MsgDjinnEffect,
					menu->listWin, 0, 96);
#else
				mnTextDrawMessageStub(DJINN_ELEMENT(djinn) * DJINN_PER_ELEMENT +
						DJINN_NUMBER(djinn) + MsgDjinnEffect,
					menu->listWin, 0, 96);
#endif
			}
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER))
				mnWindowSetInteriorPaletteBitRect(menu->listWin,
					memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
					DJINN_CURSOR_WIDTH, 1, HIGHLIGHT_PALETTE);
			WINDOW_STATE->dirty = 1;
		}
		if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER)
			mnCursorPos(memberColumn * DJINN_COLUMN_WIDTH - 8, 52);
		else
			mnCursorPos(
				memberColumn * DJINN_COLUMN_WIDTH - 8, djinnRow * 8 + 60);
		sySynch(1);
		/* Holding R after R on a Djinni keeps the group commands up; they end
		   when R is let go or pressed again (the latch is cleared when they
		   began). */
		if(!(PerData & KEY_R) || (PerPushLatch & KEY_R)) {
			if(groupMode)
				redraw = DJINN_REDRAW_ALL;
			groupMode = FALSE;
			setAll = 0;
		}
		repeat = PerRepeat;
		pressed = PerPush;
		/* The tutorial plays the menu itself: the steps press A, B, Right and
		   Left, after waits counted in ticks. */
		tutorialStep = backdrop->tutorialStep;
		if(tutorialStep != DJINN_TUTORIAL_OFF) {
			pressed = 0;
			backdrop->tutorialTmr++;
			repeat = 0;
			switch(tutorialStep) {
			case DJINN_TUTORIAL_CLOSE:
				while(!(PerPush & KEY_A)) {
					mnCursorPos(150, 26);
					sySynch(1);
				}
				pressed = KEY_B;
				repeat = KEY_B;
				break;
			/* The recovery lesson: after its two messages, three recovery
			   countdowns and Isaac's Flint goes back on standby. */
			case DJINN_TUTORIAL_RECOVERY:
				if(backdrop->tutorialTmr == 60) {
					/* "After a Djinni has been used to summon, it must spend
					   time in recovery." */
					msgWin = mnMessageOpenWindowStub(
						MsgDjinnTutorialRecovery, 9, 9, MESSAGE_WINDOW_ZOOM);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
					sySynch(1);
					/* "And after we recover, we will set ourselves again and be
					   ready to attack." */
					msgWin = mnMessageOpenWindowStub(
						MsgDjinnTutorialSetAgain, 9, 9, MESSAGE_WINDOW_ZOOM);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
					backdrop->tutorialTmr = 0;
					btDjinnRecoveryCountDownStub();
					btDjinnRecoveryCountDownStub();
					btDjinnRecoveryCountDownStub();
					gmDjinnPutOnStandbyStub(ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
					gmDjinnRecoveryAddStandbyStub(
						ISAAC, ELEMENT_VENUS, DJINNI_FLINT);
					gmUnitCalcStub(ISAAC);
					repeat = KEY_B;
					pressed = KEY_B;
				}
				break;
			case DJINN_TUTORIAL_GIVE:
				if(backdrop->tutorialTmr == 60) {
					/* "To give your Djinni to someone else, move it like
					   this." */
					msgWin = mnMessageOpenWindowStub(
						MsgDjinnTutorialGive, 9, 9, MESSAGE_WINDOW_ZOOM);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
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
			case DJINN_TUTORIAL_SET_AFTER_GIVE:
			case DJINN_TUTORIAL_SET_SPARE:
				if(backdrop->tutorialTmr == 60) {
					/* "After choosing who will take the Djinni, you can set
					   it." */
					msgWin =
						mnMessageOpenWindowStub(MsgDjinnTutorialSetAfterGive, 9,
							9, MESSAGE_WINDOW_ZOOM);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_SET_PICK;
				}
				break;
			case DJINN_TUTORIAL_SET_DONE:
				if(backdrop->tutorialTmr == 40) {
					backdrop->tutorialTmr = 0;
					repeat = KEY_B;
					pressed = KEY_B;
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
					/* "Each Djinni has different powers." */
					msgWin = mnMessageOpenWindowStub(
						MsgDjinnTutorialPowers, 9, 9, MESSAGE_WINDOW_ZOOM);
					mnCursorSlide(2, 146);
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					while(!(PerPush & KEY_A)) {
						mnCursorPos(2, 146);
						sySynch(1);
					}
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
					sySynch(1);
					/* "Once we are set, you can unleash us to use our
					   powers." */
					msgWin = mnMessageOpenWindowStub(
						MsgDjinnTutorialUnleash, 9, 9, MESSAGE_WINDOW_ZOOM);
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					while(!(PerPush & KEY_A)) {
						mnCursorPos(2, 146);
						sySynch(1);
					}
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
					sySynch(1);
					/* "So, let me demonstrate the power of my attack,
					   Isaac." */
					msgWin = mnMessageOpenWindowStub(
						MsgDjinnTutorialDemonstrate, 9, 9, MESSAGE_WINDOW_ZOOM);
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					while(!(PerPush & KEY_A)) {
						mnCursorPos(2, 146);
						sySynch(1);
					}
					mnWindowCloseStub(msgWin, 1);
					mnDjinnMenuDrawElementList(&backdrop->list);
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
					backdrop->tutorialTmr = 0;
					repeat = KEY_LEFT;
					pressed = KEY_LEFT;
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
		/* DJINN_SELECT_DJINNI's R: put the Djinni on standby or set it at once,
		   unless it is recovering. */
		if(mode == DJINN_SELECT_DJINNI && (pressed & KEY_R)) {
			if(selectedRow == -1) {
				sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			i = 0;
			if(gmDjinnCanSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
				   DJINN_NUMBER(djinn)) ||
				gmDjinnIsSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
					DJINN_NUMBER(djinn)))
				i = 1;
			groupMode = TRUE;
			PerPushLatch = 0;
			if(i == 0) {
				sySndPlayStub(SE_MENU_ERROR);
				mnWindowSprClearListStub(menu->listWin);
				mnWindowClearInteriorTilesStub(menu->listWin, 0, 80, 216, 104);
				mnTextDrawMessageStub(
					MsgDjinnNowRecovering, menu->listWin, 0, 96);
				redraw = DJINN_REDRAW_ALL;
				continue;
			}
			if(djinn & DJINN_ENTRY_IS_SET) {
				sySndPlayStub(SE_NOTE_HIGH);
				gmDjinnPutOnStandbyStub(DJINN_MEMBER(djinn),
					DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
				gmDjinnRecoveryAddStandbyStub(DJINN_MEMBER(djinn),
					DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
			} else {
				sySndPlayStub(SE_DJINNI_SET);
				gmDjinnSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
					DJINN_NUMBER(djinn));
				gmDjinnRecoveryRemoveStub(DJINN_MEMBER(djinn),
					DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
			}
			gmUnitCalcStub(DJINN_MEMBER(djinn));
			mnDjinnMenuDrawElementList(&backdrop->list);
			redraw = DJINN_REDRAW_ALL;
		}
		if(mode == DJINN_SELECT_DJINNI && (pressed & KEY_L)) {
			result = DJINN_RESULT_STATUS;
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
		if((pressed & KEY_A) ||
			(mode == DJINN_SELECT_ACTION && (pressed & KEY_R))) {
			i = 1;
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER)) {
				if(selectedRow == -1) {
					sySndPlayStub(SE_MENU_ERROR);
					continue;
				}
				i = 0;
				if(gmDjinnCanSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
					   DJINN_NUMBER(djinn)) ||
					gmDjinnIsSetStub(DJINN_MEMBER(djinn), DJINN_ELEMENT(djinn),
						DJINN_NUMBER(djinn)))
					i = 1;
			}
			if(i == 0) {
				sySndPlayStub(SE_MENU_ERROR);
				mnWindowSprClearListStub(menu->listWin);
				mnWindowClearInteriorTilesStub(menu->listWin, 0, 80, 216, 104);
				mnTextDrawMessageStub(
					MsgDjinnNowRecovering, menu->listWin, 0, 96);
				continue;
			}
			if(mode == DJINN_SELECT_ACTION) {
				if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER) {
					if(memberColumn == menu->selectedColumn[0]) {
						if(menu->pairEntries[0] & DJINN_ENTRY_IS_SET)
							result = DJINN_RESULT_STANDBY;
						else
							result = DJINN_RESULT_SET;
					} else {
						result = DJINN_RESULT_GIVE;
					}
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
		/* Select: with R held, set every Djinni that is not recovering, or, the
		   next time, put every one on standby; otherwise open the help. */
		if(mode == DJINN_SELECT_DJINNI && (pressed & KEY_SELECT)) {
			if(groupMode) {
				setAll ^= 1;
				if(setAll)
					sySndPlayStub(SE_DJINNI_SET);
				else
					sySndPlayStub(SE_NOTE_HIGH);
				for(i = 0; i < menu->memberCnt; i++) {
					for(j = 0; j < backdrop->list.cnts[i]; j++) {
						Uint32 entry = backdrop->list.ids[i][j];
						Bool   usable;
						usable = FALSE;
						if(gmDjinnCanSetStub(DJINN_MEMBER(entry),
							   DJINN_ELEMENT(entry), DJINN_NUMBER(entry)) ||
							gmDjinnIsSetStub(DJINN_MEMBER(entry),
								DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
							usable = TRUE;
						if(usable) {
							if(setAll) {
								if(!(entry & DJINN_ENTRY_IS_SET)) {
									gmDjinnSetStub(DJINN_MEMBER(entry),
										DJINN_ELEMENT(entry),
										DJINN_NUMBER(entry));
									gmDjinnRecoveryRemoveStub(
										DJINN_MEMBER(entry),
										DJINN_ELEMENT(entry),
										DJINN_NUMBER(entry));
									gmUnitCalcStub(DJINN_MEMBER(entry));
								}
							} else if(entry & DJINN_ENTRY_IS_SET) {
								gmDjinnPutOnStandbyStub(DJINN_MEMBER(entry),
									DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
								gmDjinnRecoveryAddStandbyStub(
									DJINN_MEMBER(entry), DJINN_ELEMENT(entry),
									DJINN_NUMBER(entry));
								gmUnitCalcStub(DJINN_MEMBER(entry));
							}
						}
					}
				}
				mnDjinnMenuDrawElementList(&backdrop->list);
				redraw = DJINN_REDRAW_ALL;
			} else {
				result = DJINN_RESULT_HELP;
				mnWindowSetInteriorPaletteBitRect(menu->listWin,
					memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
					DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
				sySndPlayStub(SE_MENU_CONFIRM);
				break;
			}
		}
		if(repeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY)
				continue;
			mnWindowSetInteriorPaletteBitRect(menu->listWin,
				memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
				DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
			if(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY)
				continue;
			if(columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER) {
				columnFlg[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
				djinnRow = 0;
			} else if(djinnRow == 0 &&
				(columnFlg[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
				columnFlg[memberColumn] |= DJINN_COLUMN_ON_HEADER;
				redraw = DJINN_REDRAW_MOVED;
				continue;
			}
			djinnRow--;
			cnt = backdrop->list.cnts[memberColumn];
			if(cnt == 0)
				cnt = 1;
			djinnRow = mnWrapIndex(djinnRow, cnt);
			savedRow = djinnRow;
			redraw = DJINN_REDRAW_MOVED;
			continue;
		}
		if(repeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY)
				continue;
			mnWindowSetInteriorPaletteBitRect(menu->listWin,
				memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
				DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
			djinnRow++;
			cnt = backdrop->list.cnts[memberColumn];
			if(cnt == 0)
				cnt = 1;
			djinnRow = mnWrapIndex(djinnRow, cnt);
			if((columnFlg[memberColumn] & DJINN_COLUMN_ON_HEADER) &&
				!(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY)) {
				columnFlg[memberColumn] &= ~DJINN_COLUMN_ON_HEADER;
				djinnRow = 0;
			} else if(djinnRow == 0 &&
				(columnFlg[memberColumn] & DJINN_COLUMN_HEADER_CHOOSABLE)) {
				columnFlg[memberColumn] |= DJINN_COLUMN_ON_HEADER;
			}
			savedRow = djinnRow;
			redraw = DJINN_REDRAW_MOVED;
			continue;
		}
		if(repeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY))
				mnWindowSetInteriorPaletteBitRect(menu->listWin,
					memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
					DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
			memberColumn--;
			memberColumn = mnWrapIndex(memberColumn, menu->memberCnt);
			if(mode == DJINN_SELECT_DJINNI) {
				for(i = 0; i < menu->memberCnt; i++) {
					if(columnFlg[memberColumn] == DJINN_COLUMN_EMPTY) {
						memberColumn--;
						memberColumn =
							mnWrapIndex(memberColumn, menu->memberCnt);
					}
				}
			}
			djinnRow = savedRow;
			cnt = backdrop->list.cnts[memberColumn];
			if(cnt == 0)
				cnt = 1;
			djinnRow = mnWrapIndex(djinnRow, cnt);
			redraw = DJINN_REDRAW_MOVED;
			continue;
		}
		if(repeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			if(!(columnFlg[memberColumn] & DJINN_COLUMN_EMPTY))
				mnWindowSetInteriorPaletteBitRect(menu->listWin,
					memberColumn * DJINN_COLUMN_TILES + 1, djinnRow + 2,
					DJINN_CURSOR_WIDTH, 1, WINDOW_PALETTE);
			memberColumn++;
			memberColumn = mnWrapIndex(memberColumn, menu->memberCnt);
			if(mode == DJINN_SELECT_DJINNI) {
				for(i = 0; i < menu->memberCnt; i++) {
					if(columnFlg[memberColumn] == DJINN_COLUMN_EMPTY) {
						memberColumn++;
						memberColumn =
							mnWrapIndex(memberColumn, menu->memberCnt);
					}
				}
			}
			djinnRow = savedRow;
			cnt = backdrop->list.cnts[memberColumn];
			if(cnt == 0)
				cnt = 1;
			djinnRow = mnWrapIndex(djinnRow, cnt);
			redraw = DJINN_REDRAW_MOVED;
		}
	}
	menu->selectedColumn[mode] = memberColumn;
	if(selectedRow != -1) {
		Uint32 id;
		Uint32 entry;

		menu->pairEntries[mode] = id =
			backdrop->list.ids[memberColumn][selectedRow];
		entry = (Uint16)id;
		menu->pairIdx[mode] = entry % (DJINN_ENTRY_INDEX + 1);
		menu->pairElement[mode] = DJINN_ELEMENT(entry);
		menu->pairMember[mode] = DJINN_MEMBER(entry);
	}
	menu->selection[mode] = memberColumn + djinnRow * DJINN_CURSOR_ROW_STRIDE;
	return result;
}

/* Lists owned Djinn by element and index, marking those that are set. For
   element -1, entries also encode their party member. */
Sint32 mnDjinnListUnitEntries(
	Uint16 *entriesOut, Sint32 memberId, Sint32 element)
{
	UNIT  *unit = gmUnitGetStub(memberId);
	Sint32 cnt = 0;
	Sint32 elementIdx;
	Sint32 djinnIdx;
	Sint32 encodedDjinn;

	if(element == -1) {
		for(elementIdx = 0; elementIdx < DJINN_ELEMENTS; elementIdx++) {
			for(djinnIdx = 0; djinnIdx < DJINN_PER_ELEMENT; djinnIdx++) {
				if(unit->djinnSet[elementIdx] & (1 << djinnIdx)) {
					encodedDjinn =
						(elementIdx << 5) | djinnIdx | DJINN_ENTRY_SET;
					encodedDjinn |= memberId << 8;
					entriesOut[cnt++] = encodedDjinn;
				} else if(unit->djinnHeld[elementIdx] & (1 << djinnIdx)) {
					entriesOut[cnt++] =
						(elementIdx << 5) | djinnIdx | (memberId << 8);
				}
			}
		}
	} else {
		for(djinnIdx = 0; djinnIdx < DJINN_PER_ELEMENT; djinnIdx++) {
			if(unit->djinnSet[element] & (1 << djinnIdx))
				entriesOut[cnt++] = (element << 5) | djinnIdx | DJINN_ENTRY_SET;
			else if(unit->djinnHeld[element] & (1 << djinnIdx))
				entriesOut[cnt++] = (element << 5) | djinnIdx;
		}
	}
	return cnt;
}

/* Draws both sides of the give or trade preview for the selected page: the
   receiver in the second window and the giver in the first. mode is a
   DJINN_COMPARE_OPERATION; only DJINN_COMPARE_TRADE differs, swapping the two
   Djinn. Returns 1. */
Sint32 mnDjinnMenuDrawPairPreview(Sint32 mode, Sint32 pageIdx)
{
	DJINN_MENU_STATE *menu;

	menu = MenuState;
	if(mode == DJINN_COMPARE_TRADE) {
		mnDjinnMenuDrawStatPreview(menu->secondWin, 0, 0, menu->pairMember[1],
			1, mode, DJINN_PREVIEW_TRADE, pageIdx, 1);
		mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->pairMember[0], mode,
			1, DJINN_PREVIEW_TRADE, pageIdx, mode);
	} else {
		mnDjinnMenuDrawStatPreview(menu->secondWin, 0, 0, menu->shownMember[1],
			1, 0, DJINN_SWAP_PREVIEW_MODE, pageIdx, 1);
		mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->shownMember[0], 0, 0,
			DJINN_PREVIEW_UNSET, pageIdx, 0);
	}
	return 1;
}

/* Copies a full unit for a temporary Djinn-stat preview. */
static __inline__ void mnUnitCopy(UNIT *dst, UNIT *src)
{
	syWordsCopy(dst, src, sizeof(UNIT));
}

/* Saves the unit, applies a hypothetical Djinn change, draws its differences
   and restores the original unit before returning. */
Sint32 mnDjinnMenuDrawStatPreview(WINDOW *win, Sint32 x, Sint32 y,
	Sint32 memberId, Sint32 removePairSlot, Sint32 addPairSlot,
	Sint32 previewMode, Sint32 page, Sint32 side)
{
	UNIT  *unit;
	UNIT  *originalUnit;
	Sint32 giveElement;
	Sint32 giveIdx;
	Sint32 giveSet;
	Sint32 takeElement;
	Sint32 takeIdx;
	Sint32 takeSet;
	Sint32 lost;
	Sint32 gained;
	/* mnPsynergyListDiff can write up to 64 entries here in the worst case (32
	   kept or gained and 32 lost). */
	Uint16			  actions[48];
	DJINN_MENU_STATE *menu;

	unit = gmUnitGetStub(memberId);
	menu = MenuState;
	giveElement = menu->pairElement[removePairSlot];
	giveIdx = menu->pairIdx[removePairSlot];
	giveSet = menu->pairEntries[removePairSlot] & DJINN_ENTRY_IS_SET;
	takeElement = menu->pairElement[addPairSlot];
	takeIdx = menu->pairIdx[addPairSlot];
	takeSet = menu->pairEntries[addPairSlot] & DJINN_ENTRY_IS_SET;
	originalUnit = syHeapAlloc(sizeof(UNIT));
	mnUnitCopy(originalUnit, unit);

	if(page == 0) {
		if(previewMode == DJINN_PREVIEW_NO_CHANGE) {
			mnTextDrawNumInWindowStub(unit->hpMax, DJINN_PREVIEW_DIGITS, win,
				x * 8 + DJINN_PREVIEW_MAX_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawNumInWindowStub(unit->ppMax, DJINN_PREVIEW_DIGITS, win,
				x * 8 + DJINN_PREVIEW_MAX_X, DJINN_PREVIEW_STAT_Y(y, 1));
			mnTextDrawNumInWindowStub(unit->hp, DJINN_PREVIEW_DIGITS, win,
				x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawNumInWindowStub(unit->pp, DJINN_PREVIEW_DIGITS, win,
				x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 1));
			mnTextDrawStringAtOffsetStub(DjinnMenuTextSlash, win,
				x * 8 + DJINN_PREVIEW_SLASH_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawStringAtOffsetStub(DjinnMenuTextSlash, win,
				x * 8 + DJINN_PREVIEW_SLASH_X, DJINN_PREVIEW_STAT_Y(y, 1));
		} else {
			mnTextDrawNumInWindowStub(unit->hp, DJINN_PREVIEW_DIGITS, win,
				x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 0));
			mnTextDrawNumInWindowStub(unit->pp, DJINN_PREVIEW_DIGITS, win,
				x * 8 + DJINN_PREVIEW_CURRENT_X, DJINN_PREVIEW_STAT_Y(y, 1));
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

	/* Only the temporary unit changes; the saved record is restored below. */
	switch(previewMode) {
	case DJINN_PREVIEW_ADD_AND_SET:
		gmDjinnGiveStub(memberId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		gmDjinnSetStub(memberId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		break;
	case DJINN_PREVIEW_UNSET:
		giveIdx &= DJINN_ENTRY_INDEX;
		gmDjinnPutOnStandbyStub(memberId, giveElement, giveIdx);
		break;
	case DJINN_PREVIEW_TRADE:
		if(giveSet) {
			giveIdx &= DJINN_ENTRY_INDEX;
			gmDjinnPutOnStandbyStub(memberId, giveElement, giveIdx);
		}
		gmDjinnGiveStub(memberId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		if(takeSet)
			gmDjinnSetStub(memberId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		break;
	case DJINN_PREVIEW_ADD:
		gmDjinnGiveStub(memberId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		if(takeSet)
			gmDjinnSetStub(memberId, takeElement, takeIdx & DJINN_ENTRY_INDEX);
		break;
	}

	gmUnitCalcStub(memberId);
	unit = gmUnitGetStub(memberId);

	if(page == 0) {
		Sint32 label;

		mnTextDrawStringAtOffsetStub(unit->name, win, x * 8 + 40, y * 8);
		mnTextDrawStringAtOffsetStub(
			DjinnMenuTextLevel, win, x * 8 + 40, y * 8 + 16);
		mnTextDrawNumInWindowStub(unit->level, 2, win, x * 8 + 88, y * 8 + 16);
		label = MsgPreviewStatLabel;
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
			MsgClassName + originalUnit->classId, win, x * 8, y * 8 + 32);
	}

	{
		Sint32 i = 0;

		if(page == 0) {
			Sint32 column;
#if LANGUAGE != LANGUAGE_JAPANESE
			Sint32 iconX;
#endif
			Sint32 textX;

			if(originalUnit->classId != unit->classId) {
				textX = x * 8;
				mnTextDrawMessageAsTilesStub(
					MsgClassName + unit->classId, win, textX, y * 8 + 48);
				/* The down arrow from the class to the next, a tile of BG
				   character block 1, the block after the windows' own. */
				mnWindowSetTilemapEntryStub(win,
					WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP +
						DJINN_CHANGE_ARROW_TILE),
					x + 2, 5, TILEMAP_ENTRY_PLAIN);
			}
			column = x;
			if(originalUnit->classId != unit->classId)
				column += 5;
			/* Each element's icon and its count of set Djinn: WINDOW_TILE('0')
			   + n is the window font's digit n in WINDOW_PALETTE. */
			for(i = 0; i < DJINN_ELEMENTS; i++) {
				mnWindowSetTilemapEntryStub(win, DJINN_ELEMENT_TILE + i,
					column + i * 2, y + 5, TILEMAP_ENTRY_PLAIN);
				mnWindowSetTilemapEntryStub(win,
					unit->djinnSetCnts[i] + WINDOW_TILE('0'),
					column + i * 2 + 1, y + 5, TILEMAP_ENTRY_PLAIN);
			}
#if LANGUAGE != LANGUAGE_JAPANESE
			iconX = x * 8 + 70;
#endif
			if(unit->hp != originalUnit->hp) {
				mnTextDrawNumInWindowStub(unit->hp, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 0));
				if(unit->hp > originalUnit->hp)
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 0), STAT_ARROW_DOWN);
			}
			if(unit->pp != originalUnit->pp) {
				mnTextDrawNumInWindowStub(unit->pp, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 1));
				if(unit->pp > originalUnit->pp)
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 1), STAT_ARROW_DOWN);
			}
			if(unit->attack != originalUnit->attack) {
				mnTextDrawNumInWindowStub(unit->attack, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 2));
				if(unit->attack > originalUnit->attack)
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 2), STAT_ARROW_DOWN);
			}
			if(unit->defense != originalUnit->defense) {
				mnTextDrawNumInWindowStub(unit->defense, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 3));
				if(unit->defense > originalUnit->defense)
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 3), STAT_ARROW_DOWN);
			}
			if(unit->agility != originalUnit->agility) {
				mnTextDrawNumInWindowStub(unit->agility, 4, win,
					DJINN_PREVIEW_VALUE_X(x), DJINN_PREVIEW_STAT_Y(y, 4));
				if(unit->agility > originalUnit->agility)
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 4), STAT_ARROW_DOWN);
			}
			if(unit->luck != originalUnit->luck) {
				mnTextDrawNumInWindowStub(
					unit->luck, 2, win, x * 8 + 88, DJINN_PREVIEW_STAT_Y(y, 5));
				if(unit->luck > originalUnit->luck)
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_UP);
				else
					mnIconCreateDjinnArrow(win, DJINN_PREVIEW_ARROW_X(x, iconX),
						DJINN_PREVIEW_STAT_Y(y, 5), STAT_ARROW_DOWN);
			}
		}
	}

	/* Pages 1 on: the Psynergy after the change, five to a page (six with no
	   change), then the key below: "* Psynergy Gained" in TEXT_COLOR_YELLOW, "*
	   Psynergy Lost" in TEXT_COLOR_RED, or "* No change". */
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
			mnTextDrawMessageAsTilesStub(
				MsgAbilityName + (actions[first] & PSYNERGY_ABILITY_MASK), win,
				x * 8 + 16, (y + line * 2) * 8 + 8);
			mnTextDrawNumAtOffsetStub(
				gmAbilityGetDirectStub(actions[first])->ppCost, 2, win,
				x * 8 + 88, (y + line * 2) * 8 + 8);
			line++;
		}
		mnTextSetColStub(TEXT_COLOR_NORMAL);
		mnTextDrawMessageAsTilesStub(
			MsgPsynergyPp, win, x * 8 + DJINN_PREVIEW_PP_X, y * 8);
		if(previewMode != DJINN_PREVIEW_NO_CHANGE) {
			Sint32 lines;

			lines = 0;
			if(gained) {
				mnTextSetColStub(TEXT_COLOR_YELLOW);
				mnTextDrawMessageAsTilesStub(
					MsgPsynergyGained, win, x * 8, y * 8 + 88);
				lines = 1;
			}
			if(lost) {
				mnTextSetColStub(TEXT_COLOR_RED);
				mnTextDrawMessageAsTilesStub(
					MsgPsynergyLost, win, x * 8, (y + lines) * 8 + 88);
				lines++;
			}
			if(lines == 0)
				mnTextDrawMessageAsTilesStub(
					MsgPsynergyUnchanged, win, x * 8, y * 8 + 88);
			mnTextSetColStub(TEXT_COLOR_NORMAL);
			mnWindowDrawDividerLineStub(win, 0, 11, 13, 11);
		}
		WINDOW_STATE->dirty = 1;
	}

	if(page == 0)
		mnPortraitCreateStub(memberId, 0, side, win, 0, 0);
	/* None of the preview's class, stat or action changes escape this call. */
	mnUnitCopy(unit, originalUnit);
	syHeapRelease(originalUnit);
	return 1;
}

#undef DJINN_ENTRY_NONE
#undef DJINN_BACKDROP_DIGIT_TILE
#undef DJINN_BACKDROP_PALETTE
#undef DJINN_DIM_PALETTE
#undef DJINN_WINDOW_COPY_PALETTE
#undef DJINN_HELP_TOPICS
#undef DJINN_MEMBER_FIGURE_X
#undef DJINN_MEMBER_FIGURE_Y
#undef DJINN_CURSOR_WIDTH
#undef DJINN_SWAP_PREVIEW_MODE
#undef DJINN_PREVIEW_DIGITS
#undef DJINN_PREVIEW_CURRENT_X
#undef DJINN_PREVIEW_MAX_X
#undef DJINN_PREVIEW_SLASH_X
#undef DJINN_PREVIEW_LUCK_X
#undef DJINN_PREVIEW_ARROW_X
#undef DJINN_PREVIEW_PP_X
#undef DJINN_RESELECT_PAIR_ENTRY
#undef DJINN_PREVIEW_GAINED
#undef DJINN_PREVIEW_LOST
#undef DJINN_ENTRY_SET
#undef DJINN_COLUMN_TILES
#undef DJINN_COLUMN_WIDTH
#undef DJINN_FIGURE_X
#undef DJINN_CURSOR_ROW_STRIDE
#undef DJINN_PREVIEW_VALUE_X
