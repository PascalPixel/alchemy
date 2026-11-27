/* The Djinn menu's preview of a set, standby, give or trade before it is made,
   the party member's "Char. Status" page, and the check that keeps the party's
   Djinn balanced. Part of menu.c. */

void mnCreatePairFigures(Sint32 first, Sint32 second);

/* The Djinni figures of a give or trade preview hop across between the two
   party members once every DJINN_PREVIEW_HOP_PERIOD ticks, a few ticks into
   it: a hop takes DJINN_PREVIEW_HOP_TICKS ticks along half a sine wave of
   DJINN_PREVIEW_HOP_SINE_HEIGHT pixels, cut flat at DJINN_PREVIEW_HOP_HEIGHT. A
   figure may sink to DJINN_PREVIEW_HOP_DEPTH below its line, which the half
   sine never does, and its slide would wrap after DJINN_PREVIEW_SLIDE_STEPS
   steps, which a hop never reaches. */
#define DJINN_PREVIEW_HOP_PERIOD	  60
#define DJINN_PREVIEW_HOP_TICKS		  30
#define DJINN_PREVIEW_HOP_DELAY		  5
#define DJINN_PREVIEW_HOP_SINE_HEIGHT 16
#define DJINN_PREVIEW_HOP_HEIGHT	  6
#define DJINN_PREVIEW_HOP_DEPTH		  12
#define DJINN_PREVIEW_SLIDE_STEPS	  35

/* The Psynergy one page of the preview lists: a row fewer when the change has
   its gains and losses to sum up below them. */
#define DJINN_PREVIEW_PSYNERGY_ROWS 6

/* The tilemap rows at the top of the screen that a tutorial message covers,
   saved before it opens and put back after it closes. */
#define DJINN_TUTORIAL_COVERED_ROWS 8

/* Shows or hides the menu's cursor while a line of the Djinn tutorial points at
   the preview. */
#define DJINN_TUTORIAL_SET_CURSOR(menu, value) \
	do { \
		(menu)->cursor->motion = (value); \
	} while(0)

/* Previews a give, trade, set or standby (mode, DJINN_COMPARE_OPERATION) before
   it is made: each party member's stats and Psynergy as they would be, the
   given Djinni hopping across, and "Is this OK?". R turns between both members'
   stats and each one's Psynergy, Left and Right their pages. Returns
   DJINN_RESULT_CONFIRMED for Yes, DJINN_RESULT_BACK for No or B, and
   DJINN_RESULT_START for Start. */
Sint32 mnDjinnMenuRunPreview(Uint32 mode)
{
	DJINN_MENU_STATE	 *menu;
	DJINN_MENU_BACKDROP	 *backdrop;
	WIN_STATE			 *state;
	DJINN_PREVIEW_WINDOW *list;
	Sint32				  pageCnts[2];
	Sint32				  selectedPages[2];
	Uint16				  owners[14];
	Sint32				  confirmationChoice;
	Uint32				  redraw;
	Sint32				  frm;
	Sint32				  page;
	Sint32				  help;
	Sint32				  previewMode;
	Bool				  addPairSlot;
	Sint32				  step;
	Sint32				  result;
	Sint32				  idx;
	Sint32				  memberIdx;
	Sint32				  first;
	Sint32				  second;
	Uint32				  chosenEntry;
	Uint32				  otherEntry;
	Sint32				  row;

	menu = RamHeapSlots->menuState;
	confirmationChoice = YES;
	result = 0;
	redraw = 1;
	frm = 0;
	page = DJINN_COMPARE_PAGE_BOTH;
	help = menu->helpWin;
	menu->cursor->motion = ICON_HIDDEN;
	selectedPages[0] = 0;
	selectedPages[1] = 0;
	gmPartyListMembersStub(owners);
	first = owners[menu->selectedColumn[0]];
	second = owners[menu->selectedColumn[1]];
	mnCreatePairFigures(first, second);
	for(memberIdx = 0; memberIdx < 4; memberIdx++)
		menu->memberY[memberIdx] = MENU_FIGURE_HIDDEN_Y;
	menu->memberX[0] = 16;
	menu->memberY[0] = 32;
	if(first != second) {
		menu->memberX[1] = 120;
		menu->memberY[1] = 32;
	}
	mnWindowSprClearListStub(menu->listWin);
	sySynch(1);
	mnWindowCloseIfOpen(&menu->helpWin, 1);
	mnWindowUpdateOrCreate(&menu->helpWin, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
	pageCnts[0] = 1;
	pageCnts[1] = 1;
	if(mode <= DJINN_COMPARE_GIVE) {
		Uint16 *diffState;
		UNIT   *original;
		UNIT   *unit;
		Sint32	memberId;
		Sint32	firstElement;
		Sint32	firstIdx;
		Sint32	secondElement;
		Sint32	secondIdx;
		Sint32	firstWasSet;
		Sint32	secondWasSet;
		Sint32	lostCnt;
		Sint32	gainedCnt;
		Sint32	size;

		diffState = syHeapAllocEWRAM(96);
		size = sizeof(UNIT);
		original = syHeapAllocEWRAM(size);
		secondElement = 0;
		secondIdx = 0;
		memberId = menu->shownMember[0];
		firstElement = menu->pairElement[0];
		firstIdx = menu->pairIdx[0] & DJINN_ENTRY_INDEX;
		firstWasSet = (Uint16)(menu->pairEntries[0] & DJINN_ENTRY_IS_SET);
		unit = syUnitGet(memberId);
		syWordsCopyEntry(original, unit, size);
		gmDjinnPutOnStandbyStub(memberId, firstElement, firstIdx);
		if(mode == DJINN_COMPARE_TRADE) {
			secondElement = menu->pairElement[1];
			secondIdx = menu->pairIdx[1] & DJINN_ENTRY_INDEX;
			secondWasSet = (Uint16)(menu->pairEntries[1] & DJINN_ENTRY_IS_SET);
			gmDjinnGiveStub(memberId, secondElement, secondIdx);
			if(secondWasSet != 0)
				gmDjinnSetStub(memberId, secondElement, secondIdx);
		}
		gmUnitCalcStub(memberId);
		pageCnts[0] = mnPsynergyListDiff(original->psynergy, unit->psynergy,
			diffState, &gainedCnt, &lostCnt);
		syWordsCopyEntry(unit, original, size);

		memberId = menu->shownMember[1];
		unit = syUnitGet(memberId);
		syWordsCopyEntry(original, unit, size);
		if(mode == DJINN_COMPARE_TRADE)
			gmDjinnPutOnStandbyStub(memberId, secondElement, secondIdx);
		gmDjinnGiveStub(memberId, firstElement, firstIdx);
		if(firstWasSet != 0)
			gmDjinnSetStub(memberId, firstElement, firstIdx);
		gmUnitCalcStub(memberId);
		pageCnts[1] = mnPsynergyListDiff(original->psynergy, unit->psynergy,
			diffState, &gainedCnt, &lostCnt);
		syWordsCopyEntry(unit, original, size);
		syHeapRelease(original);
		syHeapRelease(diffState);
	} else if(mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
		Uint16 *diffState;
		UNIT   *original;
		UNIT   *unit;
		Sint32	memberId;
		Sint32	firstElement;
		Sint32	firstIdx;
		Sint32	applySecond;
		Sint32	lostCnt;
		Sint32	gainedCnt;

		diffState = syHeapAllocEWRAM(96);
		original = syHeapAllocEWRAM(sizeof(UNIT));
		memberId = menu->shownMember[0];
		firstElement = menu->pairElement[0];
		firstIdx = menu->pairIdx[0] & DJINN_ENTRY_INDEX;
		applySecond = (mode == DJINN_COMPARE_SET);
		unit = syUnitGet(memberId);
		syWordsCopyEntry(original, unit, sizeof(UNIT));
		gmDjinnPutOnStandbyStub(memberId, firstElement, firstIdx);
		if(applySecond != 0)
			gmDjinnSetStub(memberId, firstElement, firstIdx);
		gmUnitCalcStub(memberId);
		pageCnts[0] = mnPsynergyListDiff(original->psynergy, unit->psynergy,
			diffState, &gainedCnt, &lostCnt);
		syWordsCopyEntry(unit, original, sizeof(UNIT));
		pageCnts[1] = pageCnts[0];
		page = DJINN_COMPARE_PAGE_GIVER;
		syHeapRelease(original);
		syHeapRelease(diffState);
	}
	pageCnts[0] = (pageCnts[0] - 1) / (DJINN_PREVIEW_PSYNERGY_ROWS - 1) + 1;
	if(pageCnts[0] == 0)
		pageCnts[0] = 1;
	pageCnts[1] = (pageCnts[1] - 1) / (DJINN_PREVIEW_PSYNERGY_ROWS - 1) + 1;
	if(pageCnts[1] == 0)
		pageCnts[1] = 1;
	mnWindowUpdateOrCreate(&menu->win, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
	mnWindowUpdateOrCreate(
		&menu->secondWin, 15, 5, 15, 15, WINDOW_FLAG_INSTANT);
	mnWindowSprRedrawSavedRectStub(help);
	if(mode == DJINN_COMPARE_STANDBY) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnCompareStandby, help, 96, 0);
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnCompareConfirm, help, 96, 16);
	} else if(mode == DJINN_COMPARE_SET) {
		mnTextDrawMessageAsTilesStub((Sint32)MsgDjinnCompareSet, help, 96, 0);
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnCompareConfirm, help, 96, 16);
	} else if(mode == DJINN_COMPARE_TRADE) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnCompareTrade, help, 128, 0);
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnCompareConfirm, help, 128, 8);
	} else {
		mnTextDrawMessageAsTilesStub((Sint32)MsgDjinnCompareGive, help, 128, 0);
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgDjinnCompareConfirm, help, 128, 8);
	}
	row = 0;
	chosenEntry = menu->pairEntries[0];
	otherEntry = menu->pairEntries[1];
	if(mode == DJINN_COMPARE_GIVE)
		row = 1;
	if((chosenEntry & DJINN_ENTRY_IS_SET) == 0)
		mnTextSetColStub(TEXT_COLOR_RED);
	mnDjinnMenuDrawEntry(help, 40, row * 8, chosenEntry);
	if(mode == DJINN_COMPARE_TRADE) {
		if((otherEntry & DJINN_ENTRY_IS_SET) == 0)
			mnTextSetColStub(TEXT_COLOR_RED);
		mnDjinnMenuDrawEntry(help, 40, 16, otherEntry);
	} else if(mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
		if((chosenEntry & DJINN_ENTRY_IS_SET) != 0)
			mnTextSetColStub(TEXT_COLOR_RED);
		mnDjinnMenuDrawEntry(help, 40, 16, chosenEntry);
		/* The blinking down arrow between the two, a tile of BG character block
		   1, the block after the windows' own. */
		mnWindowSetTilemapEntryStub(help,
			WINDOW_TILE(
				BG_CHAR_SIZE / TILE_SIZE_4BPP + DJINN_BACKDROP_BORDER_TILE),
			7, 1, 0);
	}

	for(;;) {
		Uint32 pressed;
		Uint32 repeat;

		state = (WIN_STATE *)RamHeapSlots->winState;
		backdrop = menu->backdrop;
		pressed = PerWork.push;
		repeat = PerWork.repeat;
		if(backdrop->tutorialStep != DJINN_TUTORIAL_OFF) {
			Uint8 *saved;
			Sint32 size;

			size = DJINN_TUTORIAL_COVERED_ROWS * sizeof(state->tilemap[0]);
			saved = syHeapAllocEWRAM(size);
			pressed = 0;
			repeat = 0;
			backdrop->tutorialTmr++;
			switch(backdrop->tutorialStep) {
			case DJINN_TUTORIAL_GIVE_PREVIEW:
			case DJINN_TUTORIAL_SET_PREVIEW:
			case DJINN_TUTORIAL_POWERS_PREVIEW:
				if(backdrop->tutorialTmr == 100) {
					repeat = KEY_A;
					pressed = KEY_A;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep++;
				}
				break;
			case DJINN_TUTORIAL_SET_CHANGES:
				if(backdrop->tutorialTmr == 60) {
					Sint32 win;

					syWordsCopyEntry(saved, state->tilemap, size);
					win = mnMessageOpenWindowStub(mnGetDjinnGiveMessage() +
							DJINN_TUTORIAL_LINE_SET_CHANGES,
						8, 0, MESSAGE_WINDOW_ZOOM);
					DJINN_TUTORIAL_SET_CURSOR(menu, ICON_SHOWN);
					mnCursorSlide(2, 96);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(win, 1);
					syWordsCopyEntry(state->tilemap, saved, size);
					mnWindowMarkVisibleTextTilesStub();
					state->dirty = 1;
					redraw = 1;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_CLASS_PSYNERGY;
					DJINN_TUTORIAL_SET_CURSOR(menu, ICON_HIDDEN);
				}
				break;
			case DJINN_TUTORIAL_CLASS_PSYNERGY:
				if(backdrop->tutorialTmr == 60) {
					Sint32 win;

					syWordsCopyEntry(saved, state->tilemap, size);
					win = mnMessageOpenWindowStub(mnGetDjinnGiveMessage() +
							DJINN_TUTORIAL_LINE_CLASS_PSYNERGY,
						8, 0, MESSAGE_WINDOW_ZOOM);
					DJINN_TUTORIAL_SET_CURSOR(menu, ICON_SHOWN);
					mnCursorSlide(106, 56);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(win, 1);
					syWordsCopyEntry(state->tilemap, saved, size);
					mnWindowMarkVisibleTextTilesStub();
					state->dirty = 1;
					sySynch(1);
					redraw = 1;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_SET_PREVIEW;
					DJINN_TUTORIAL_SET_CURSOR(menu, ICON_HIDDEN);
				}
				break;
			}
			syHeapRelease(saved);
		}

		if(redraw != 0) {
			addPairSlot = TRUE;
			if(mode <= DJINN_COMPARE_GIVE) {
				mnWindowClearInteriorTilesStub(help, 128, 16, 224, 24);
				if(page == DJINN_COMPARE_PAGE_RECEIVER) {
					mnTextDrawMessageAsTilesStub(
						(Sint32)MsgDjinnStatusHelp, help, 128, 16);
				} else {
					mnMessageArgumentPushStub(
						menu->shownMember[page != DJINN_COMPARE_PAGE_BOTH],
						MESSAGE_ARGUMENT_ACTOR);
					mnTextDrawMessageAsTilesStub(
						(Sint32)MsgDjinnPsyHelp, help, 128, 16);
				}
			}
			state->holdTilemapUpload = 1;
			mnWindowSprPrepareForRedrawStub(menu->win);
			mnWindowSprPrepareForRedrawStub((Sint32)menu->secondWin);
			if((redraw >> 1) != 0)
				sySynch(1);
			if(mode == DJINN_COMPARE_SET) {
				previewMode = DJINN_PREVIEW_ADD_AND_SET;
				addPairSlot = FALSE;
			} else if(mode == DJINN_COMPARE_STANDBY) {
				previewMode = DJINN_PREVIEW_UNSET;
				addPairSlot = FALSE;
			} else {
				previewMode = DJINN_PREVIEW_TRADE;
			}
			switch(page) {
			default:
				mnDjinnMenuDrawPairPreview(mode, 0);
				break;
			case DJINN_COMPARE_PAGE_RECEIVER:
				if(mode == DJINN_COMPARE_GIVE)
					previewMode = DJINN_PREVIEW_ADD;
				mnDjinnMenuDrawStatPreview(menu->win, 0, 0,
					menu->shownMember[1], page, 0, previewMode, 0, 0);
				mnDjinnMenuDrawStatPreview((Sint32)menu->secondWin, 0, 0,
					menu->shownMember[1], page, 0, previewMode,
					selectedPages[1] + 1, 0);
				break;
			case DJINN_COMPARE_PAGE_GIVER:
				if(mode == DJINN_COMPARE_GIVE) {
					previewMode = DJINN_PREVIEW_UNSET;
					addPairSlot = FALSE;
				}
				mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->pairMember[0],
					page, addPairSlot, previewMode, page, 1);
				mnDjinnMenuDrawStatPreview((Sint32)menu->secondWin, 0, 0,
					menu->pairMember[0], page, addPairSlot, previewMode,
					selectedPages[0] + 1, 1);
				break;
			}
			state->holdTilemapUpload = 0;
		}

		if(page <= DJINN_COMPARE_PAGE_RECEIVER) {
			list = menu->secondWin;
			if(pageCnts[page] > 1) {
				for(idx = 0; idx < pageCnts[page]; idx++) {
					Sint32 tile;

					tile = WINDOW_TILE('1') + idx;
					if(idx > 8)
						tile = WINDOW_TILE('?');
					if(idx == selectedPages[page])
						tile -=
							BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
					mnWindowSetTilemapEntryStub((Sint32)list, tile,
						list->width - pageCnts[page] + idx - 2, -1, 0);
				}
				mnWindowSetTilemapEntryStub((Sint32)list, PAGE_ARROW_LEFT,
					list->width - pageCnts[page] - 3, -1, 0);
				mnWindowSetTilemapEntryStub(
					(Sint32)list, PAGE_ARROW_RIGHT, list->width - 2, -1, 0);
				WINDOW_MARK_TILEMAP_ROW(state, list->y);
			}
		}

		frm++;
		step = frm % DJINN_PREVIEW_HOP_PERIOD - DJINN_PREVIEW_HOP_DELAY;
		if(step < 0)
			step = 0;
		if(step > DJINN_PREVIEW_HOP_TICKS - 1)
			step = DJINN_PREVIEW_HOP_TICKS - 1;
		/* Both figures held at three quarters of their size. */
		mnDjinnFiguresSetSlotPhase(0, -(toFIXED(3) / 4));
		mnDjinnFiguresSetSlotPhase(1, -(toFIXED(3) / 4));
		if(mode <= DJINN_COMPARE_GIVE) {
			FIXED  rise;
			Sint32 slide;

			rise = -syMulFX(DJINN_PREVIEW_HOP_SINE_HEIGHT,
				sySin(step % DJINN_PREVIEW_HOP_TICKS *
					(DEGtoANG(180) / DJINN_PREVIEW_HOP_TICKS)));
			if(rise < -DJINN_PREVIEW_HOP_HEIGHT)
				rise = -DJINN_PREVIEW_HOP_HEIGHT;
			/* DEAD: rise is never positive, the hop's half sine wave staying
			   above the line, so DJINN_PREVIEW_HOP_DEPTH never applies; the ROM
			   keeps its test. */
			if(rise > DJINN_PREVIEW_HOP_DEPTH)
				rise = DJINN_PREVIEW_HOP_DEPTH;
			slide = step % DJINN_PREVIEW_SLIDE_STEPS * 2;
			mnDjinnFiguresSetSlotPos(0, slide + 34, rise + 20);
			if(mode == DJINN_COMPARE_TRADE)
				mnDjinnFiguresSetSlotPos(1, 99 - slide, 36 - rise);
		} else {
			mnDjinnFiguresSetSlotPos(0, 32, 30);
		}
		if(redraw != 0) {
			redraw = 0;
			confirmationChoice = mnWrapIndex(confirmationChoice, 2);
		}
		mnCursorPos(confirmationChoice * 56 + 80, 16);
		if((frm & 3) == 0) {
			void  *tile;
			Uint32 fill = TEXT_CANVAS_BACKGROUND_WORD;

			tile = (void *)DJINN_BACKDROP_BORDER_VRAM;
			if((frm & 4) != 0) {
				syWordsCopy(tile, MenuBackdropBorderTile, TILE_SIZE_4BPP);
			} else {
				syWordsFill(tile, TILE_SIZE_4BPP, fill);
			}
		}

		if((pressed & KEY_A) != 0) {
			if(confirmationChoice == YES) {
				result = DJINN_RESULT_CONFIRMED;
				break;
			}
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_BACK;
			break;
		}
		if((pressed & KEY_START) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_START;
			break;
		} else if((pressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_BACK;
			break;
		} else if((pressed & KEY_R) != 0) {
			if(mode <= DJINN_COMPARE_GIVE) {
				page = (page + 1) % 3;
				redraw = 2;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		} else if((repeat & KEY_LEFT) != 0) {
			if(page <= DJINN_COMPARE_PAGE_RECEIVER) {
				selectedPages[page]--;
				selectedPages[page] =
					mnWrapIndex(selectedPages[page], pageCnts[page]);
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				redraw = 1;
			}
		} else if((repeat & KEY_RIGHT) != 0) {
			if(page <= DJINN_COMPARE_PAGE_RECEIVER) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				selectedPages[page]++;
				redraw = 1;
				selectedPages[page] =
					mnWrapIndex(selectedPages[page], pageCnts[page]);
			}
		}
		sySynch(1);
	}

	mnDjinnFiguresSetSlotPhase(0, 0);
	mnDjinnFiguresSetSlotPhase(1, 0);
	sySetEvent(mnUpdatePartyFigures, EVENT_PASS_TICK);
	((WIN_STATE *)RamHeapSlots->winState)->holdTilemapUpload = 1;
	mnWindowCloseIfOpen(&menu->helpWin, 1);
	sySynch(1);
	mnWindowUpdateOrCreate(&menu->helpWin, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	mnWindowCloseIfOpen(&menu->win, 1);
	mnWindowCloseIfOpen(&menu->secondWin, 1);
	mnWindowSprRedrawSavedRectStub(menu->listWin);
	mnWindowSprRedrawSavedRectStub(menu->unopenedWin);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	((WIN_STATE *)RamHeapSlots->winState)->holdTilemapUpload = 0;
	sySynch(1);
	return result;
}

/* Shows the chosen party member's current stats and paged Psynergy list. Opened
   by L on the Djinn list ("Char. Status"): the party member's stats on the
   left, their Psynergy six to a page on the right, which Left and Right turn.
   Returns DJINN_RESULT_BACK for A, B, L or R, and DJINN_RESULT_START for Start,
   which closes the Djinn menu. */
Sint32 mnDjinnMenuShowMemberStat(void)
{
	Sint32				  selectedPages[2];
	Sint32				  pageCnts[2];
	Sint32				  gained;
	Sint32				  lost;
	Bool				  redraw;
	Sint32				  frm;
	void				 *leftWinSlot;
	void				 *rightWinSlot;
	Sint32				 *helpWinSlot;
	WIN_STATE			 *render;
	Uint32				  pressedKeys;
	Uint32				  repeatedKeys;
	DJINN_MENU_STATE	 *menu;
	UNIT				 *unit;
	DJINN_PREVIEW_WINDOW *win;
	Uint16				 *listedActions;
	void				 *saved;
	Sint32				  result;
	Sint32				  tile;
	Sint32				  pageIdx;
	Sint32				  side;
	Sint32				  hopStep;

	menu = RamHeapSlots->menuState;
	redraw = TRUE;
	result = 0;
	frm = 0;
	menu->cursor->motion = ICON_HIDDEN;
	selectedPages[0] = 0;
	selectedPages[1] = 0;
	for(side = 3; side >= 0; side--)
		menu->memberY[side] = MENU_FIGURE_HIDDEN_Y;
	mnWindowSprClearListStub(menu->listWin);
	sySynch(1);
	pageCnts[0] = 1;
	pageCnts[1] = 1;
	listedActions = syHeapAllocEWRAM(96);
	saved = syHeapAllocEWRAM(sizeof(UNIT));
	unit = syUnitGet(menu->shownMember[0]);
	pageCnts[0] = mnPsynergyListDiff(
		unit->psynergy, unit->psynergy, listedActions, &gained, &lost);
	pageCnts[1] = pageCnts[0];
	syHeapRelease(saved);
	syHeapRelease(listedActions);
	pageCnts[0] = (pageCnts[0] - 1) / DJINN_PREVIEW_PSYNERGY_ROWS + 1;
	if(pageCnts[0] == 0)
		pageCnts[0] = 1;
	pageCnts[1] = (pageCnts[1] - 1) / DJINN_PREVIEW_PSYNERGY_ROWS + 1;
	if(pageCnts[1] == 0)
		pageCnts[1] = 1;
	leftWinSlot = &menu->win;
	mnWindowUpdateOrCreate(leftWinSlot, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
	rightWinSlot = &menu->secondWin;
	mnWindowUpdateOrCreate(rightWinSlot, 15, 5, 15, 15, WINDOW_FLAG_INSTANT);
	helpWinSlot = &menu->helpWin;
	mnWindowSprRedrawSavedRectStub(*helpWinSlot);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	mnTextDrawMessageAsTilesStub((Sint32)MsgCurrentStatus, menu->statWin, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgCurrentStatus + 2, menu->statWin, 0, 16);
	side = 0;
	while(1) {
		render = (WIN_STATE *)RamHeapSlots->winState;
		pressedKeys = PerWork.push;
		repeatedKeys = PerWork.repeat;
		if(redraw) {
			render->holdTilemapUpload = 1;
			mnWindowSprPrepareForRedrawStub(menu->win);
			mnWindowSprPrepareForRedrawStub((Sint32)menu->secondWin);
			mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->pairMember[0], 0,
				0, DJINN_PREVIEW_NO_CHANGE, 0, 1);
			mnDjinnMenuDrawStatPreview((Sint32)menu->secondWin, 0, 0,
				menu->pairMember[0], 0, 0, DJINN_PREVIEW_NO_CHANGE,
				selectedPages[0] + 1, 1);
			render->holdTilemapUpload = 0;
		}
		/* The page numbers, written out as in mnDjinnMenuRunPreview: one
		   function shared by both does not keep the ROM's bytes. */
		if(pageCnts[side] > 1) {
			win = menu->secondWin;
			for(pageIdx = 0; pageIdx < pageCnts[side]; pageIdx++) {
				tile = WINDOW_TILE('1') + pageIdx;
				if(pageIdx > 8)
					tile = WINDOW_TILE('?');
				if(pageIdx == selectedPages[side])
					tile -= BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
				mnWindowSetTilemapEntryStub((Sint32)win, tile,
					win->width - pageCnts[side] + pageIdx - 2, -1, 0);
			}
			mnWindowSetTilemapEntryStub((Sint32)win, PAGE_ARROW_LEFT,
				win->width - pageCnts[side] - 3, -1, 0);
			mnWindowSetTilemapEntryStub(
				(Sint32)win, PAGE_ARROW_RIGHT, win->width - 2, -1, 0);
			WINDOW_MARK_TILEMAP_ROW(render, win->y);
		}
		frm++;
		/* The preview's hop step, which no figure on this page uses. */
		hopStep = frm % DJINN_PREVIEW_HOP_PERIOD - DJINN_PREVIEW_HOP_DELAY;
		CLAMP(hopStep, 0, DJINN_PREVIEW_HOP_TICKS - 1);
		mnDjinnFiguresSetSlotPos(0, 32, MENU_FIGURE_HIDDEN_Y);
		/* DEAD: the test cannot change the result, redraw = 0 alone does the
		   same; the ROM keeps the compare (its twin also steps a phase under
		   it). */
		if(redraw)
			redraw = FALSE;
		if((frm & 3) == 0) {
			if(frm & 4)
				syWordsCopyEntry((void *)DJINN_BACKDROP_BORDER_VRAM,
					MenuBackdropBorderTile, TILE_SIZE_4BPP);
			else
				syWordsFillEntry((void *)DJINN_BACKDROP_BORDER_VRAM,
					TILE_SIZE_4BPP, TEXT_CANVAS_BACKGROUND_WORD);
		}
		if(pressedKeys & KEY_START) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_START;
			break;
		}
		if(pressedKeys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_BACK;
			break;
		}
		if(repeatedKeys & KEY_LEFT) {
			selectedPages[side]--;
			selectedPages[side] =
				mnWrapIndex(selectedPages[side], pageCnts[side]);
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			redraw = TRUE;
		} else if(repeatedKeys & KEY_RIGHT) {
			selectedPages[side]++;
			selectedPages[side] =
				mnWrapIndex(selectedPages[side], pageCnts[side]);
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			redraw = TRUE;
		}
		sySynch(1);
	}
	sySetEvent(mnUpdatePartyFigures, EVENT_PASS_TICK);
	((WIN_STATE *)RamHeapSlots->winState)->holdTilemapUpload = 1;
	mnWindowCloseIfOpen(helpWinSlot, 1);
	sySynch(1);
	mnWindowUpdateOrCreate(helpWinSlot, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	mnWindowCloseIfOpen(leftWinSlot, 1);
	mnWindowCloseIfOpen(rightWinSlot, 1);
	mnWindowSprRedrawSavedRectStub(menu->listWin);
	mnWindowSprRedrawSavedRectStub(menu->unopenedWin);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	((WIN_STATE *)RamHeapSlots->winState)->holdTilemapUpload = 0;
	sySynch(1);
	return result;
}

Sint32 mnDjinnCheckGiveBalance(Sint32 self, Sint32 other);

/* Marks the party members the second Djinn chooser offers only Trade:
   requiresTrade[i] becomes 1 for each other party member i who, given one of
   srcMemberColumn's Djinn, would hold two or more more than the giver
   (mnDjinnCheckGiveBalance), else 0. Returns how many there are. */
Sint32 mnDjinnMarkTradeOnlyRecipients(
	Sint8 *requiresTrade, Sint32 srcMemberColumn)
{
	Sint32			  tradeCnt;
	Sint32			  memberColumn;
	Sint8			 *entry;
	DJINN_MENU_STATE *menu;

	menu = RamHeapSlots->menuState;
	tradeCnt = 0;
	memberColumn = 0;
	if(tradeCnt < menu->memberCnt) {
		entry = requiresTrade;
		do {
			*entry = 0;
			if(memberColumn != srcMemberColumn) {
				if(mnDjinnCheckGiveBalance(srcMemberColumn, memberColumn) ==
					0) {
					*entry = 1;
					tradeCnt += 1;
				}
			}
			memberColumn += 1;
			entry += 1;
		} while(memberColumn < menu->memberCnt);
	}
	return tradeCnt;
}

void mnDjinnCountOwned(Uint8 *cnts);

/* Returns whether column from could give column to a Djinni: not when the
   receiver would then hold two or more Djinn more than the giver. */
Sint32 mnDjinnCheckGiveBalance(Sint32 from, Sint32 to)
{
	Sint8 cnts[16];
	Bool  balanced;

	mnDjinnCountOwned((Uint8 *)cnts);
	cnts[from] -= 1;
	cnts[to] += 1;
	balanced = TRUE;
	if(cnts[from] + 2 <= cnts[to])
		balanced = FALSE;
	return balanced;
}

/* Writes how many Djinn each party member holds, set or not, into ownedCnts
   by list column. */
void mnDjinnCountOwned(Uint8 *ownedCnts)
{
	DJINN_MENU_STATE *menu;
	UNIT			 *unit;
	Sint32			  memberIdx;
	Sint32			  element;
	Sint32			  djinnIdx;
	Sint32			  ownedCnt;

	menu = RamHeapSlots->menuState;
	for(memberIdx = 0; memberIdx < menu->memberCnt; memberIdx++) {
		unit = syUnitGet(menu->members[memberIdx]);
		ownedCnt = 0;
		for(element = 0; element < ELEMENT_COUNT; element++) {
			for(djinnIdx = 0; djinnIdx < DJINN_PER_ELEMENT; djinnIdx++) {
				if(unit->djinnSet[element] & (1 << djinnIdx))
					ownedCnt++;
				else if(unit->djinnHeld[element] & (1 << djinnIdx))
					ownedCnt++;
			}
		}
		ownedCnts[memberIdx] = ownedCnt;
	}
}
