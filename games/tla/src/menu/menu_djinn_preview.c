/* @file */
/* The Djinn menu's preview of a set, standby, give or trade before it is made,
 */
/* the party member's "Char. Status" page, and the check that keeps the party's
 */
/* Djinn balanced. */
/* Part of menu.c. */

void mnCreatePairFigures(s32 first, s32 second);

/* The Djinni figures of a give or trade preview hop across between the two */
/* party members once every DJINN_PREVIEW_HOP_PERIOD frames, a few frames into
 */
/* it: a hop takes DJINN_PREVIEW_HOP_FRAMES frames along half a sine wave of */
/* DJINN_PREVIEW_HOP_SINE_HEIGHT pixels, cut flat at DJINN_PREVIEW_HOP_HEIGHT.
 */
/* A figure may sink to DJINN_PREVIEW_HOP_DEPTH below its line, which the half
 */
/* sine never does, and its slide would wrap after DJINN_PREVIEW_SLIDE_STEPS */
/* steps, which a hop never reaches. */
#define DJINN_PREVIEW_HOP_PERIOD	  60
#define DJINN_PREVIEW_HOP_FRAMES	  30
#define DJINN_PREVIEW_HOP_DELAY		  5
#define DJINN_PREVIEW_HOP_SINE_HEIGHT 16
#define DJINN_PREVIEW_HOP_HEIGHT	  6
#define DJINN_PREVIEW_HOP_DEPTH		  12
#define DJINN_PREVIEW_SLIDE_STEPS	  35

/* The Psynergy one page of the preview lists: a row fewer when the change */
/* has its gains and losses to sum up below them. */
#define DJINN_PREVIEW_PSYNERGY_ROWS 6

/* The tilemap rows at the top of the screen that a tutorial message covers, */
/* saved before it opens and put back after it closes. */
#define DJINN_TUTORIAL_COVERED_ROWS 8

/* Shows or hides the menu's cursor while a line of the Djinn tutorial points */
/* at the preview. */
#define syDjinnTutorialSetCursor(menu, value)                                  \
	do {                                                                       \
		(menu)->cursor->motion = (value);                                      \
	} while(0)

/* Previews a give, trade, set or standby (mode, DJINN_COMPARE_OPERATION) */
/* before it is made: each party member's stats and Psynergy as they would be,
 */
/* the given Djinni hopping across, and "Is this OK?". R turns between both */
/* members' stats and each one's Psynergy, Left and Right their pages. Returns
 */
/* DJINN_RESULT_CONFIRMED for Yes, DJINN_RESULT_BACK for No or B, and */
/* DJINN_RESULT_START for Start. */
s32 mnDjinnMenuRunPreview(u32 mode) {
	DJINN_MENU_STATE *menu;
	DJINN_MENU_BACKDROP *backdrop;
	WIN_STATE *state;
	DJINN_PREVIEW_WINDOW *list;
	s32 pageCounts[2];
	s32 selectedPages[2];
	u16 owners[14];
	s32 confirmationChoice;
	u32 redraw;
	s32 frame;
	s32 page;
	s32 help;
	s32 previewMode;
	s32 addPairSlot;
	s32 step;
	s32 result;
	s32 index;
	s32 memberIndex;
	s32 first;
	s32 second;
	u32 chosenEntry;
	u32 otherEntry;
	s32 row;

	menu = Ram_HeapSlots->menuState;
	confirmationChoice = YES;
	result = 0;
	redraw = 1;
	frame = 0;
	page = DJINN_COMPARE_PAGE_BOTH;
	help = menu->helpWin;
	menu->cursor->motion = ICON_HIDDEN;
	selectedPages[0] = 0;
	selectedPages[1] = 0;
	gmPartyListMembersStub(owners);
	first = owners[menu->selectedColumn[0]];
	second = owners[menu->selectedColumn[1]];
	mnCreatePairFigures(first, second);
	for(memberIndex = 0; memberIndex < 4; memberIndex++)
		menu->memberY[memberIndex] = MENU_FIGURE_HIDDEN_Y;
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
	pageCounts[0] = 1;
	pageCounts[1] = 1;
	if(mode <= DJINN_COMPARE_GIVE) {
		u16 *diffState;
		UNIT *original;
		UNIT *unit;
		s32 memberId;
		s32 firstElement;
		s32 firstIndex;
		s32 secondElement;
		s32 secondIndex;
		s32 firstWasSet;
		s32 secondWasSet;
		s32 lostCount;
		s32 gainedCount;
		s32 size;

		diffState = syHeapAllocEwram(96);
		size = sizeof(UNIT);
		original = syHeapAllocEwram(size);
		secondElement = 0;
		secondIndex = 0;
		memberId = menu->shownMember[0];
		firstElement = menu->pairElement[0];
		firstIndex = menu->pairIdx[0] & DJINN_ENTRY_INDEX;
		firstWasSet = (u16)(menu->pairEntries[0] & DJINN_ENTRY_IS_SET);
		unit = syUnitGet(memberId);
		syWordsCopyEntry(original, unit, size);
		gmDjinnPutOnStandbyStub(memberId, firstElement, firstIndex);
		if(mode == DJINN_COMPARE_TRADE) {
			secondElement = menu->pairElement[1];
			secondIndex = menu->pairIdx[1] & DJINN_ENTRY_INDEX;
			secondWasSet = (u16)(menu->pairEntries[1] & DJINN_ENTRY_IS_SET);
			gmDjinnGiveStub(memberId, secondElement, secondIndex);
			if(secondWasSet != 0)
				gmDjinnSetStub(memberId, secondElement, secondIndex);
		}
		gmUnitCalcStub(memberId);
		pageCounts[0] = mnPsynergyListDiff(original->psynergy, unit->psynergy,
										   diffState, &gainedCount, &lostCount);
		syWordsCopyEntry(unit, original, size);

		memberId = menu->shownMember[1];
		unit = syUnitGet(memberId);
		syWordsCopyEntry(original, unit, size);
		if(mode == DJINN_COMPARE_TRADE)
			gmDjinnPutOnStandbyStub(memberId, secondElement, secondIndex);
		gmDjinnGiveStub(memberId, firstElement, firstIndex);
		if(firstWasSet != 0)
			gmDjinnSetStub(memberId, firstElement, firstIndex);
		gmUnitCalcStub(memberId);
		pageCounts[1] = mnPsynergyListDiff(original->psynergy, unit->psynergy,
										   diffState, &gainedCount, &lostCount);
		syWordsCopyEntry(unit, original, size);
		syHeapRelease(original);
		syHeapRelease(diffState);
	}
	else if(mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
		u16 *diffState;
		UNIT *original;
		UNIT *unit;
		s32 memberId;
		s32 firstElement;
		s32 firstIndex;
		s32 applySecond;
		s32 lostCount;
		s32 gainedCount;

		diffState = syHeapAllocEwram(96);
		original = syHeapAllocEwram(sizeof(UNIT));
		memberId = menu->shownMember[0];
		firstElement = menu->pairElement[0];
		firstIndex = menu->pairIdx[0] & DJINN_ENTRY_INDEX;
		applySecond = (mode == DJINN_COMPARE_SET);
		unit = syUnitGet(memberId);
		syWordsCopyEntry(original, unit, sizeof(UNIT));
		gmDjinnPutOnStandbyStub(memberId, firstElement, firstIndex);
		if(applySecond != 0)
			gmDjinnSetStub(memberId, firstElement, firstIndex);
		gmUnitCalcStub(memberId);
		pageCounts[0] = mnPsynergyListDiff(original->psynergy, unit->psynergy,
										   diffState, &gainedCount, &lostCount);
		syWordsCopyEntry(unit, original, sizeof(UNIT));
		pageCounts[1] = pageCounts[0];
		page = DJINN_COMPARE_PAGE_GIVER;
		syHeapRelease(original);
		syHeapRelease(diffState);
	}
	pageCounts[0] = (pageCounts[0] - 1) / (DJINN_PREVIEW_PSYNERGY_ROWS - 1) + 1;
	if(pageCounts[0] == 0)
		pageCounts[0] = 1;
	pageCounts[1] = (pageCounts[1] - 1) / (DJINN_PREVIEW_PSYNERGY_ROWS - 1) + 1;
	if(pageCounts[1] == 0)
		pageCounts[1] = 1;
	mnWindowUpdateOrCreate(&menu->win, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
	mnWindowUpdateOrCreate(&menu->secondWin, 15, 5, 15, 15,
						   WINDOW_FLAG_INSTANT);
	mnWindowSprRedrawSavedRectStub(help);
	if(mode == DJINN_COMPARE_STANDBY) {
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareStandby, help, 96, 0);
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareConfirm, help, 96, 16);
	}
	else if(mode == DJINN_COMPARE_SET) {
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareSet, help, 96, 0);
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareConfirm, help, 96, 16);
	}
	else if(mode == DJINN_COMPARE_TRADE) {
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareTrade, help, 128, 0);
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareConfirm, help, 128, 8);
	}
	else {
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareGive, help, 128, 0);
		mnTextDrawMessageAsTilesStub((s32)MsgDjinnCompareConfirm, help, 128, 8);
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
	}
	else if(mode == DJINN_COMPARE_STANDBY || mode == DJINN_COMPARE_SET) {
		if((chosenEntry & DJINN_ENTRY_IS_SET) != 0)
			mnTextSetColStub(TEXT_COLOR_RED);
		mnDjinnMenuDrawEntry(help, 40, 16, chosenEntry);
		/* The blinking down arrow between the two, a tile of BG character block
		 */
		/* 1, the block after the windows' own. */
		mnWindowSetTilemapEntryStub(help,
									WINDOW_TILE(BG_CHAR_SIZE / TILE_SIZE_4BPP +
												DJINN_BACKDROP_BORDER_TILE),
									7, 1, 0);
	}

	for(;;) {
		u32 pressed;
		u32 repeat;

		state = (WIN_STATE *)Ram_HeapSlots->winState;
		backdrop = menu->backdrop;
		pressed = InputWork.pressed;
		repeat = InputWork.repeat;
		if(backdrop->tutorialStep != DJINN_TUTORIAL_OFF) {
			u8 *saved;
			s32 size;

			size = DJINN_TUTORIAL_COVERED_ROWS * sizeof(state->tilemap[0]);
			saved = syHeapAllocEwram(size);
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
					s32 window;

					syWordsCopyEntry(saved, state->tilemap, size);
					window = mnMessageOpenWindowStub(
						mnGetDjinnGiveMessage() +
							DJINN_TUTORIAL_LINE_SET_CHANGES,
						8, 0, MESSAGE_WINDOW_ZOOM);
					syDjinnTutorialSetCursor(menu, ICON_SHOWN);
					mnCursorSlide(2, 96);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(window, 1);
					syWordsCopyEntry(state->tilemap, saved, size);
					mnWindowMarkVisibleTextTilesStub();
					state->dirty = 1;
					redraw = 1;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_CLASS_PSYNERGY;
					syDjinnTutorialSetCursor(menu, ICON_HIDDEN);
				}
				break;
			case DJINN_TUTORIAL_CLASS_PSYNERGY:
				if(backdrop->tutorialTmr == 60) {
					s32 window;

					syWordsCopyEntry(saved, state->tilemap, size);
					window = mnMessageOpenWindowStub(
						mnGetDjinnGiveMessage() +
							DJINN_TUTORIAL_LINE_CLASS_PSYNERGY,
						8, 0, MESSAGE_WINDOW_ZOOM);
					syDjinnTutorialSetCursor(menu, ICON_SHOWN);
					mnCursorSlide(106, 56);
					MainRoutineStateWork.msgSpeed = MESSAGE_SPEED_NORMAL;
					while(mnMessageAllTypedStub() == 0)
						sySynch(1);
					mnWindowCloseStub(window, 1);
					syWordsCopyEntry(state->tilemap, saved, size);
					mnWindowMarkVisibleTextTilesStub();
					state->dirty = 1;
					sySynch(1);
					redraw = 1;
					backdrop->tutorialTmr = 0;
					backdrop->tutorialStep = DJINN_TUTORIAL_SET_PREVIEW;
					syDjinnTutorialSetCursor(menu, ICON_HIDDEN);
				}
				break;
			}
			syHeapRelease(saved);
		}

		if(redraw != 0) {
			addPairSlot = 1;
			if(mode <= DJINN_COMPARE_GIVE) {
				mnWindowClearInteriorTilesStub(help, 128, 16, 224, 24);
				if(page == DJINN_COMPARE_PAGE_RECEIVER) {
					mnTextDrawMessageAsTilesStub((s32)MsgDjinnStatusHelp, help,
												 128, 16);
				}
				else {
					mnMessageArgumentPushStub(
						menu->shownMember[page != DJINN_COMPARE_PAGE_BOTH],
						MESSAGE_ARGUMENT_ACTOR);
					mnTextDrawMessageAsTilesStub((s32)MsgDjinnPsyHelp, help,
												 128, 16);
				}
			}
			state->holdTilemapUpload = 1;
			mnWindowSprPrepareForRedrawStub(menu->win);
			mnWindowSprPrepareForRedrawStub((s32)menu->secondWin);
			if((redraw >> 1) != 0)
				sySynch(1);
			if(mode == DJINN_COMPARE_SET) {
				previewMode = DJINN_PREVIEW_ADD_AND_SET;
				addPairSlot = 0;
			}
			else if(mode == DJINN_COMPARE_STANDBY) {
				previewMode = DJINN_PREVIEW_UNSET;
				addPairSlot = 0;
			}
			else {
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
										   menu->shownMember[1], page, 0,
										   previewMode, 0, 0);
				mnDjinnMenuDrawStatPreview(
					(s32)menu->secondWin, 0, 0, menu->shownMember[1], page, 0,
					previewMode, selectedPages[1] + 1, 0);
				break;
			case DJINN_COMPARE_PAGE_GIVER:
				if(mode == DJINN_COMPARE_GIVE) {
					previewMode = DJINN_PREVIEW_UNSET;
					addPairSlot = 0;
				}
				mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->pairMember[0],
										   page, addPairSlot, previewMode, page,
										   1);
				mnDjinnMenuDrawStatPreview(
					(s32)menu->secondWin, 0, 0, menu->pairMember[0], page,
					addPairSlot, previewMode, selectedPages[0] + 1, 1);
				break;
			}
			state->holdTilemapUpload = 0;
		}

		if(page <= DJINN_COMPARE_PAGE_RECEIVER) {
			list = menu->secondWin;
			if(pageCounts[page] > 1) {
				for(index = 0; index < pageCounts[page]; index++) {
					s32 tile;

					tile = WINDOW_TILE('1') + index;
					if(index > 8)
						tile = WINDOW_TILE('?');
					if(index == selectedPages[page])
						tile -=
							BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
					mnWindowSetTilemapEntryStub(
						(s32)list, tile,
						list->width - pageCounts[page] + index - 2, -1, 0);
				}
				mnWindowSetTilemapEntryStub((s32)list, PAGE_ARROW_LEFT,
											list->width - pageCounts[page] - 3,
											-1, 0);
				mnWindowSetTilemapEntryStub((s32)list, PAGE_ARROW_RIGHT,
											list->width - 2, -1, 0);
				WINDOW_MARK_TILEMAP_ROW(state, list->y);
			}
		}

		frame++;
		step = frame % DJINN_PREVIEW_HOP_PERIOD - DJINN_PREVIEW_HOP_DELAY;
		if(step < 0)
			step = 0;
		if(step > DJINN_PREVIEW_HOP_FRAMES - 1)
			step = DJINN_PREVIEW_HOP_FRAMES - 1;
		/* Both figures held at three quarters of their size. */
		mnDjinnFiguresSetSlotPhase(0, -(FIXED_ONE * 3 / 4));
		mnDjinnFiguresSetSlotPhase(1, -(FIXED_ONE * 3 / 4));
		if(mode <= DJINN_COMPARE_GIVE) {
			FIXED rise;
			s32 slide;

			rise = -syMulFX(DJINN_PREVIEW_HOP_SINE_HEIGHT,
							sySin(step % DJINN_PREVIEW_HOP_FRAMES *
								  (DEGtoANG(180) / DJINN_PREVIEW_HOP_FRAMES)));
			if(rise < -DJINN_PREVIEW_HOP_HEIGHT)
				rise = -DJINN_PREVIEW_HOP_HEIGHT;
			/* DEAD: rise is never positive, the hop's half sine wave staying
			 * above */
			/* the line, so DJINN_PREVIEW_HOP_DEPTH never applies; the ROM keeps
			 * its */
			/* test. */
			if(rise > DJINN_PREVIEW_HOP_DEPTH)
				rise = DJINN_PREVIEW_HOP_DEPTH;
			slide = step % DJINN_PREVIEW_SLIDE_STEPS * 2;
			mnDjinnFiguresSetSlotPos(0, slide + 34, rise + 20);
			if(mode == DJINN_COMPARE_TRADE)
				mnDjinnFiguresSetSlotPos(1, 99 - slide, 36 - rise);
		}
		else {
			mnDjinnFiguresSetSlotPos(0, 32, 30);
		}
		if(redraw != 0) {
			redraw = 0;
			confirmationChoice = mnWrapIndex(confirmationChoice, 2);
		}
		mnCursorPos(confirmationChoice * 56 + 80, 16);
		if((frame & 3) == 0) {
			void *tile;
			u32 fill = TEXT_CANVAS_BACKGROUND_WORD;

			tile = (void *)DJINN_BACKDROP_BORDER_VRAM;
			if((frame & 4) != 0) {
				syWordsCopy(tile, Menu_BackdropBorderTile, TILE_SIZE_4BPP);
			}
			else {
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
		}
		else if((pressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = DJINN_RESULT_BACK;
			break;
		}
		else if((pressed & KEY_R) != 0) {
			if(mode <= DJINN_COMPARE_GIVE) {
				page = (page + 1) % 3;
				redraw = 2;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
			}
		}
		else if((repeat & KEY_LEFT) != 0) {
			if(page <= DJINN_COMPARE_PAGE_RECEIVER) {
				selectedPages[page]--;
				selectedPages[page] =
					mnWrapIndex(selectedPages[page], pageCounts[page]);
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				redraw = 1;
			}
		}
		else if((repeat & KEY_RIGHT) != 0) {
			if(page <= DJINN_COMPARE_PAGE_RECEIVER) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				selectedPages[page]++;
				redraw = 1;
				selectedPages[page] =
					mnWrapIndex(selectedPages[page], pageCounts[page]);
			}
		}
		sySynch(1);
	}

	mnDjinnFiguresSetSlotPhase(0, 0);
	mnDjinnFiguresSetSlotPhase(1, 0);
	sySetEvent(mnUpdatePartyFigures, TASK_PASS_FRAME);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 1;
	mnWindowCloseIfOpen(&menu->helpWin, 1);
	sySynch(1);
	mnWindowUpdateOrCreate(&menu->helpWin, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	mnWindowCloseIfOpen(&menu->win, 1);
	mnWindowCloseIfOpen(&menu->secondWin, 1);
	mnWindowSprRedrawSavedRectStub(menu->listWin);
	mnWindowSprRedrawSavedRectStub(menu->unopenedWin);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 0;
	sySynch(1);
	return result;
}

/* Shows the chosen party member's current stats and paged Psynergy list. */
/* Opened by L on the Djinn list ("Char. Status"): the party member's stats on
 */
/* the left, their Psynergy six to a page on the right, which Left and Right */
/* turn. Returns DJINN_RESULT_BACK for A, B, L or R, and DJINN_RESULT_START for
 */
/* Start, which closes the Djinn menu. */
s32 mnDjinnMenuShowMemberStat(void) {
	s32 selectedPages[2];
	s32 pageCounts[2];
	s32 gained;
	s32 lost;
	s32 redraw;
	s32 frame;
	void *leftWindowSlot;
	void *rightWindowSlot;
	s32 *helpWindowSlot;
	WIN_STATE *render;
	u32 pressedKeys;
	u32 repeatedKeys;
	DJINN_MENU_STATE *menu;
	UNIT *unit;
	DJINN_PREVIEW_WINDOW *window;
	u16 *listedActions;
	void *saved;
	s32 result;
	s32 tile;
	s32 pageIndex;
	s32 side;
	s32 hopStep;

	menu = Ram_HeapSlots->menuState;
	redraw = 1;
	result = 0;
	frame = 0;
	menu->cursor->motion = ICON_HIDDEN;
	selectedPages[0] = 0;
	selectedPages[1] = 0;
	for(side = 3; side >= 0; side--)
		menu->memberY[side] = MENU_FIGURE_HIDDEN_Y;
	mnWindowSprClearListStub(menu->listWin);
	sySynch(1);
	pageCounts[0] = 1;
	pageCounts[1] = 1;
	listedActions = syHeapAllocEwram(96);
	saved = syHeapAllocEwram(sizeof(UNIT));
	unit = syUnitGet(menu->shownMember[0]);
	pageCounts[0] = mnPsynergyListDiff(unit->psynergy, unit->psynergy,
									   listedActions, &gained, &lost);
	pageCounts[1] = pageCounts[0];
	syHeapRelease(saved);
	syHeapRelease(listedActions);
	pageCounts[0] = (pageCounts[0] - 1) / DJINN_PREVIEW_PSYNERGY_ROWS + 1;
	if(pageCounts[0] == 0)
		pageCounts[0] = 1;
	pageCounts[1] = (pageCounts[1] - 1) / DJINN_PREVIEW_PSYNERGY_ROWS + 1;
	if(pageCounts[1] == 0)
		pageCounts[1] = 1;
	leftWindowSlot = &menu->win;
	mnWindowUpdateOrCreate(leftWindowSlot, 0, 5, 15, 15, WINDOW_FLAG_INSTANT);
	rightWindowSlot = &menu->secondWin;
	mnWindowUpdateOrCreate(rightWindowSlot, 15, 5, 15, 15, WINDOW_FLAG_INSTANT);
	helpWindowSlot = &menu->helpWin;
	mnWindowSprRedrawSavedRectStub(*helpWindowSlot);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	mnTextDrawMessageAsTilesStub((s32)MsgCurrentStatus, menu->statWin, 0, 0);
	mnTextDrawMessageAsTilesStub((s32)MsgCurrentStatus + 2, menu->statWin, 0,
								 16);
	side = 0;
	while(1) {
		render = (WIN_STATE *)Ram_HeapSlots->winState;
		pressedKeys = InputWork.pressed;
		repeatedKeys = InputWork.repeat;
		if(redraw) {
			render->holdTilemapUpload = 1;
			mnWindowSprPrepareForRedrawStub(menu->win);
			mnWindowSprPrepareForRedrawStub((s32)menu->secondWin);
			mnDjinnMenuDrawStatPreview(menu->win, 0, 0, menu->pairMember[0], 0,
									   0, DJINN_PREVIEW_NO_CHANGE, 0, 1);
			mnDjinnMenuDrawStatPreview(
				(s32)menu->secondWin, 0, 0, menu->pairMember[0], 0, 0,
				DJINN_PREVIEW_NO_CHANGE, selectedPages[0] + 1, 1);
			render->holdTilemapUpload = 0;
		}
		/* The page numbers, written out as in mnDjinnMenuRunPreview: one
		 * function */
		/* shared by both does not keep the ROM's bytes. */
		if(pageCounts[side] > 1) {
			window = menu->secondWin;
			for(pageIndex = 0; pageIndex < pageCounts[side]; pageIndex++) {
				tile = WINDOW_TILE('1') + pageIndex;
				if(pageIndex > 8)
					tile = WINDOW_TILE('?');
				if(pageIndex == selectedPages[side])
					tile -= BG_TILE_PALETTE(WINDOW_PALETTE - HIGHLIGHT_PALETTE);
				mnWindowSetTilemapEntryStub(
					(s32)window, tile,
					window->width - pageCounts[side] + pageIndex - 2, -1, 0);
			}
			mnWindowSetTilemapEntryStub((s32)window, PAGE_ARROW_LEFT,
										window->width - pageCounts[side] - 3,
										-1, 0);
			mnWindowSetTilemapEntryStub((s32)window, PAGE_ARROW_RIGHT,
										window->width - 2, -1, 0);
			WINDOW_MARK_TILEMAP_ROW(render, window->y);
		}
		frame++;
		/* The preview's hop step, which no figure on this page uses. */
		hopStep = frame % DJINN_PREVIEW_HOP_PERIOD - DJINN_PREVIEW_HOP_DELAY;
		CLAMP(hopStep, 0, DJINN_PREVIEW_HOP_FRAMES - 1);
		mnDjinnFiguresSetSlotPos(0, 32, MENU_FIGURE_HIDDEN_Y);
		/* DEAD: the test cannot change the result, redraw = 0 alone does the
		 * same; */
		/* the ROM keeps the compare (its twin also steps a phase under it). */
		if(redraw)
			redraw = 0;
		if((frame & 3) == 0) {
			if(frame & 4)
				syWordsCopyEntry((void *)DJINN_BACKDROP_BORDER_VRAM,
								 Menu_BackdropBorderTile, TILE_SIZE_4BPP);
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
				mnWrapIndex(selectedPages[side], pageCounts[side]);
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			redraw = 1;
		}
		else if(repeatedKeys & KEY_RIGHT) {
			selectedPages[side]++;
			selectedPages[side] =
				mnWrapIndex(selectedPages[side], pageCounts[side]);
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			redraw = 1;
		}
		sySynch(1);
	}
	sySetEvent(mnUpdatePartyFigures, TASK_PASS_FRAME);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 1;
	mnWindowCloseIfOpen(helpWindowSlot, 1);
	sySynch(1);
	mnWindowUpdateOrCreate(helpWindowSlot, 13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	mnWindowCloseIfOpen(leftWindowSlot, 1);
	mnWindowCloseIfOpen(rightWindowSlot, 1);
	mnWindowSprRedrawSavedRectStub(menu->listWin);
	mnWindowSprRedrawSavedRectStub(menu->unopenedWin);
	mnWindowSprRedrawSavedRectStub(menu->statWin);
	((WIN_STATE *)Ram_HeapSlots->winState)->holdTilemapUpload = 0;
	sySynch(1);
	return result;
}

s32 mnDjinnCheckGiveBalance(s32 self, s32 other);

/* Marks the party members the second Djinn chooser offers only Trade: */
/* requiresTrade[i] becomes 1 for each other party member i who, given one of */
/* sourceMemberColumn's Djinn, would hold two or more more than the giver */
/* (mnDjinnCheckGiveBalance), else 0. Returns how many there are. */
s32 mnDjinnMarkTradeOnlyRecipients(s8 *requiresTrade, s32 sourceMemberColumn) {
	s32 tradeCount;
	s32 memberColumn;
	s8 *entry;
	DJINN_MENU_STATE *menu;

	menu = Ram_HeapSlots->menuState;
	tradeCount = 0;
	memberColumn = 0;
	if(tradeCount < menu->memberCnt) {
		entry = requiresTrade;
		do {
			*entry = 0;
			if(memberColumn != sourceMemberColumn) {
				if(mnDjinnCheckGiveBalance(sourceMemberColumn, memberColumn) ==
				   0) {
					*entry = 1;
					tradeCount += 1;
				}
			}
			memberColumn += 1;
			entry += 1;
		} while(memberColumn < menu->memberCnt);
	}
	return tradeCount;
}

void mnDjinnCountOwned(u8 *counts);

/* Returns whether column from could give column to a Djinni: not when the */
/* receiver would then hold two or more Djinn more than the giver. */
s32 mnDjinnCheckGiveBalance(s32 from, s32 to) {
	s8 counts[16];
	s32 balanced;

	mnDjinnCountOwned((u8 *)counts);
	counts[from] -= 1;
	counts[to] += 1;
	balanced = 1;
	if(counts[from] + 2 <= counts[to])
		balanced = 0;
	return balanced;
}

/* Writes how many Djinn each party member holds, set or not, into */
/* ownedCounts by list column. */
void mnDjinnCountOwned(u8 *ownedCounts) {
	DJINN_MENU_STATE *menu;
	UNIT *unit;
	s32 memberIndex;
	s32 element;
	s32 djinnIndex;
	s32 ownedCount;

	menu = Ram_HeapSlots->menuState;
	for(memberIndex = 0; memberIndex < menu->memberCnt; memberIndex++) {
		unit = syUnitGet(menu->members[memberIndex]);
		ownedCount = 0;
		for(element = 0; element < ELEMENT_COUNT; element++) {
			for(djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
				if(unit->djinnSet[element] & (1 << djinnIndex))
					ownedCount++;
				else if(unit->djinnHeld[element] & (1 << djinnIndex))
					ownedCount++;
			}
		}
		ownedCounts[memberIndex] = ownedCount;
	}
}
