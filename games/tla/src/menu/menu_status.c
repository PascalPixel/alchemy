/* The party member chooser and the Status menu: its party member row and its
   status, Psynergy, range and equipment pages. Part of menu.c. */

/* The steps of the Status menu (mnStatScreenRunPages). */
typedef enum STAT_SCREEN_STEP {
	STATUS_STEP_MEMBER = 0,	  /* choose a party member */
	STATUS_STEP_STATUS = 1,	  /* their status page */
	STATUS_STEP_PSYNERGY = 2, /* their Psynergy page */
	STATUS_STEP_EQUIPMENT = 3 /* their equipment page */
} STAT_SCREEN_STEP;

/* The page modes of mnStatScreenDrawStatPage and mnStatScreenDrawTopRows, in
   the mode's low byte: the party row's summary, or the full status page, with
   the elemental levels, Power and Resist. */
enum { STATUS_PAGE_SUMMARY = 0, STATUS_PAGE_FULL = 1 };

/* A bit over the page mode: draw without clearing the window first, as for a
   window just opened. */
#define STATUS_PAGE_NO_CLEAR  (1 << 8)
#define STATUS_PAGE_MODE_MASK (STATUS_PAGE_NO_CLEAR - 1)

/* The worn items' rows in mnItemMenuDrawEquipment's window, 16 pixels apart:
   weapon, helm, shield and armor from the top. A slot's label stands on its
   row, its item's name a line below, and its icon at EQUIPPED_ICON_X on
   screen. */
#define EQUIPPED_ROW_Y(row)	 ((row) * 16)
#define EQUIPPED_ICON_X		 216
#define EQUIPPED_ICON_Y(row) (EQUIPPED_ROW_Y(row) + 32)

/* The Djinn figures stand in a row 32 pixels apart, at y STATUS_FIGURE_ROW_Y,
   over the status page's Djinn counts. On the full status page those stand
   three tiles higher, and the figures at STATUS_FIGURE_FULL_PAGE_ROW_Y; the
   pages after it put them above the screen. */
#define STATUS_FIGURE_X(index)		  (130 + (index) * 32)
#define STATUS_FIGURE_ROW_Y			  128
#define STATUS_FIGURE_FULL_PAGE_ROW_Y 104
#define STATUS_FIGURE_HIDDEN_Y		  -16

/* The Djinn list (mnStatScreenShowDjinnList): a column for each element, its
   Djinni figure standing over it. */
#define DJINN_LIST_COLUMN_WIDTH		 56
#define DJINN_LIST_FIGURE_X(element) (32 + (element) * DJINN_LIST_COLUMN_WIDTH)
#define DJINN_LIST_FIGURE_Y			 56
/* The rows of each element's column on a page of the Djinn list. */
#define DJINN_LIST_PAGE_ROWS		 9
/* The window map entry of the first element icon, in palette 1 as
   TILEMAP_ENTRY_ICON gives it; the four elements follow it in order. */
#define STATUS_ELEMENT_TILE			 (BG_TILE_PALETTE(1) | 1)

/* The status page's four element columns, 4 tiles apart from tile 15, where
   each element's icon stands; its figures end left of it. */
#define STATUS_ELEMENT_X(element) (((element) * 4 + 15) * 8)

void mnStatScreenInit(
	Sint32 unusedA, Sint32 unusedB, Sint32 unusedC, Sint32 unusedD);

Sint32 mnPartyMemberChooserRun(void);

/* Lets the player choose one party member, over the field. Returns the chosen
   party member's id, or -1 when the player backs out. Owns the menu state
   (HEAP_SLOT_MENU) while it runs and frees it before returning. */
Sint32 mnPartyMemberChooserOpen(void)
{
	STAT_SCREEN_STATE *state =
		(STAT_SCREEN_STATE *)syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	Sint32 result;
	Sint32 idx;

	RamHeapSlots->actorSystem->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnInitState(0);

	state->partyCnt = gmPartyListMembersStub(state->memberIds);
	mnStatScreenInit(0, 3, 0, 7);
	state->selectorWin = mnWindowCreateStub(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	for(idx = 0; idx < 4; idx++)
		state->memberY[idx] = MENU_MEMBER_Y;
	state->page = STATUS_SCREEN_PAGE_CHOOSER;

	result = mnPartyMemberChooserRun();

	mnWindowSprClearListStub(state->statWin);
	mnItemMenuClose();
	RamHeapSlots->actorSystem->suspended = 0;
	sySynch(1);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	return result;
}

Sint32 mnStatScreenSelectMember(Sint32 slot);

/* The party member chooser's loop. Returns the chosen party member's id, or
   -1. */
Sint32 mnPartyMemberChooserRun(void)
{
	STAT_SCREEN_STATE *state = RamHeapSlots->menuState;
	Sint32			   result = 0;

	do {
		state->selectedSlots[0] = 0;
		if(mnStatScreenSelectMember(0) == -1) {
			result = -1;
			break;
		}
		result = state->selectedIds[0];
	} while(0);
	return result;
}

Sint32 mnStatScreenRunPages(void);

void mnIconLoadSlots(void);
void mnIconReleaseSlots(void);

/* The window tiles the Status menu saves and restores. */
#define STATUS_SAVED_TILE_BYTES 10240

/* The Status screen, the field menu's Status, over the field: saves the
   background palettes and the window tiles it draws over, sets up the party
   row, the five command icons and, when the party has Djinn, the Djinn figures,
   runs the status pages, then puts everything back. */
Sint32 mnStatScreenRun(void)
{
	STAT_SCREEN_STATE *state =
		(STAT_SCREEN_STATE *)syHeapSlotAlloc(HEAP_SLOT_MENU, MENU_STATE_SIZE);
	void					*palette = syHeapAllocEWRAM(64);
	void					*tiles = syHeapAllocEWRAM(STATUS_SAVED_TILE_BYTES);
	Sint32					 result = -1;
	HEAP_SLOTS				*heap;
	SCREEN_TRANSITION_STATE *transition;
	Uint32					 saved;
	Uint16					*page;

	heap = RamHeapSlots;
	transition = heap->screenTransition;
	heap->actorSystem->suspended = 1;
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	saved = (Uint16)transition->outsideLayers;
	/* Window 0 shows only the windows' layer and the sprites. */
	transition->outsideLayers = WININ_WIN0_BG0 | WININ_WIN0_OBJ;
	mnSuspendFieldEvents();
	mnInitState(0);
	page = &state->page;
	*page = 0;
	state->partyCnt = gmPartyListMembersStub(state->memberIds);
	mnStatScreenInit(0, 3, 0, 7);
	syWordsCopy(palette, (void *)BG_PLTT, 64);
	syWordsCopy(tiles, (void *)BG_CHAR_BLOCK(1), STATUS_SAVED_TILE_BYTES);
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	syDMAXCopy(
		(void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy(
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	mnTextUseExtraTilesStub(1);
	state->selectorWin = mnWindowCreateStub(13, 0, 17, 5, WINDOW_FLAG_INSTANT);
	if(gmPartySumDjinnCountsStub(result) != 0)
		mnDjinnFiguresInitTopRow();
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	mnIconLoadSlots();
	mnStartWatchingStart();
	*page = 0;

	result = mnStatScreenRunPages();

	mnStopWatchingStart();
	mnIconReleaseSlots();
	mnWindowSprClearListStub(state->statWin);
	mnDjinnFiguresReleaseTopRow();
	mnResumeFieldEvents();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	sySynch(1);
	mnWindowMarkVisibleTextTilesStub();
	mnTextUseExtraTilesStub(0);
	syWordsCopy((void *)BG_PLTT, palette, 64);
	syWordsCopy((void *)BG_CHAR_BLOCK(1), tiles, STATUS_SAVED_TILE_BYTES);
	syHeapRelease(tiles);
	syHeapRelease(palette);
	((WIN_STATE *)heap->winState)->holdTilemapUpload = 1;
	mnItemMenuClose();
	mnWindowDrawBorderStub(0, 0, 30, 20);
	syHeapSlotRelease(HEAP_SLOT_MENU);
	heap->actorSystem->suspended = 0;
	transition->outsideLayers = saved;
	sySynch(1);
	mnWindowEraseBorderRectStub(0, 0, 30, 20);
	((WIN_STATE *)heap->winState)->holdTilemapUpload = 0;
	return result;
}

void   mnIconSetActive(Sint32 active);
Sint32 mnStatScreenRunStatPage(void);
Sint32 mnStatScreenRunPsynergyPage(void);
Sint32 mnStatScreenRunEquipmentPage(void);

/* The Status menu's pages: choose a party member, then their status page, A for
   their Psynergy page and A again for their equipment page, whose A goes back
   round to the status page; B on any page goes back to the party member row.
   Returns -1 when the player backs out of the party member row or presses
   Start. */
Sint32 mnStatScreenRunPages(void)
{
	STAT_SCREEN_STATE *state = (STAT_SCREEN_STATE *)RamHeapSlots->menuState;
	Sint32			   step = STATUS_STEP_MEMBER;
	Sint32			   finished = step;
	Sint32			   result = 0;

	while(!finished && !syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		switch(step) {
		case STATUS_STEP_MEMBER:
			state->selectedSlots[0] = finished;
			sySetEvent(mnUpdatePartyFigures, EVENT_PASS_TICK);
			mnIconSetActive(1);
			step = mnStatScreenSelectMember(0);
			syCloseEvent(mnUpdatePartyFigures);
			if(step == -1) {
				result = step;
				finished = 1;
			}
			step = STATUS_STEP_STATUS;
			break;
		case STATUS_STEP_STATUS:
			mnIconSetActive(1);
			result = mnStatScreenRunStatPage();
			step = result == -1 ? STATUS_STEP_MEMBER : STATUS_STEP_PSYNERGY;
			break;
		case STATUS_STEP_PSYNERGY:
			mnIconSetActive(0);
			result = mnStatScreenRunPsynergyPage();
			step = STATUS_STEP_MEMBER;
			if(result != -1)
				step = STATUS_STEP_EQUIPMENT;
			break;
		case STATUS_STEP_EQUIPMENT:
			mnIconSetActive(0);
			result = mnStatScreenRunEquipmentPage();
			if(result == -1)
				step = STATUS_STEP_MEMBER;
			else
				step = STATUS_STEP_STATUS;
			break;
		default:
			finished = 1;
			break;
		}
	}

	if(syGameFlagGet(FLAG_MENU_START_PRESSED))
		result = -1;

	return result;
}

Sint32 mnPartyMemberChooserChooseMember(void);
Sint32 mnStatScreenRunPartyRow(void);

/* Lets the player choose a party member in slot, from the one chosen last: the
   party member chooser on the chooser page, else the Status menu's party
   row. */
Sint32 mnStatScreenSelectMember(Sint32 slot)
{
	STAT_SCREEN_STATE *menu;
	WINDOW_SPR		  *cursor;
	Sint32			   result;
	Sint32			   idx;

	menu = (STAT_SCREEN_STATE *)RamHeapSlots->menuState;
	idx = menu->memberIdx[slot];
	result = 0;
	cursor = menu->memberCursors[slot];
	cursor->motion = ICON_SHOWN;
	cursor->anmStep = 0;
	mnWindowSprRedrawSavedRectStub(menu->memberWin);
	if(syGameFlagGet(FLAG_LINK_BATTLE_ARRANGED))
		mnWindowDrawDividerLineStub(menu->memberWin, 9, 1, 9, 3);
	mnWindowDrawDividerLineStub(menu->memberWin, 0, 3, 12, 3);
	if(idx == -1)
		menu->memberIdx[slot] = 0;
	else
		mnCursorSlide(idx * 24 - 10, 16);
	if(menu->page == STATUS_SCREEN_PAGE_CHOOSER)
		result = mnPartyMemberChooserChooseMember();
	else
		result = mnStatScreenRunPartyRow();
	cursor = menu->memberCursors[slot];
	mnIconShow(cursor);
	sySynch(1);
	return result;
}

/* The Djinn list Select opens from the party row: every Djinni the party has
   found, a column for each element, nine a page over two pages, The Lost Age's
   own first and The Broken Seal's after them, until A, B or Select. */
void mnStatScreenShowDjinnList(void)
{
	DJINN_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			  page;
	Sint32			  redraw;
	Sint32			  win;
	Sint32			  element;
	Sint32			  row;
	Sint32			  djinn;
	Sint32			  i;
	Sint32			  pages;

	page = 0;
	pages = 2;
	sySndPlayStub(SE_MENU_CONFIRM);
	redraw = 1;
	mnWindowSprRedrawSavedRectStub(menu->helpWin);
	mnTextDrawMessageAsTilesStub(MsgButtonReturn, menu->helpWin, 0, 16);
	mnTextDrawMessageStub(MsgCurrentDjinn, menu->helpWin, -96, 132);
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	mnIconSetActive(0);
	menu->cursor->motion = ICON_HIDDEN;
	menu->secondCursor->motion = ICON_HIDDEN;
	for(i = 0; i < 4; i++) {
		menu->slotX[i] = DJINN_LIST_FIGURE_X(i);
		menu->slotY[i] = DJINN_LIST_FIGURE_Y;
		menu->memberY[i] = MENU_MEMBER_Y;
	}
	sySynch(1);
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			win = menu->win;
			mnWindowSprRedrawSavedRectStub(win);
			sySynch(1);
			mnWindowDrawDividerLineStub(win, 0, 11, 28, 11);
			mnPagedListDrawPageNumbers(win, pages, 1, page, 28);
			for(element = 0; element < 4; element++) {
				for(row = 0; row < DJINN_LIST_PAGE_ROWS; row++) {
					/* page * 9: a page of DJINN_LIST_PAGE_ROWS. */
					djinn = element * DJINN_PER_ELEMENT +
						(row + (page * 8 + page) + DJINN_OWN_FIRST) %
							DJINNI_SPARE_FIRST;
					if(syGameFlagGet(FLAG_DJINNI(djinn))) {
						mnWindowSetTilemapEntryStub(win,
							STATUS_ELEMENT_TILE + element,
							element * (DJINN_LIST_COLUMN_WIDTH / 8) + 1,
							row + 1, 0);
						mnTextDrawMessageAsTilesStub(djinn + MsgDjinnName, win,
							element * DJINN_LIST_COLUMN_WIDTH + 16,
							row * 8 + 8);
					}
				}
			}
			((WIN_STATE *)RamHeapSlots->winState)->dirty = 1;
			redraw = 0;
		}
		mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
		sySynch(1);
		if(PerWork.push & (KEY_A | KEY_B | KEY_SELECT))
			break;
		if(PerWork.repeat & KEYS_HORIZONTAL) {
			if(PerWork.repeat & KEY_RIGHT)
				page++;
			else
				page--;
			page = (page + pages) % pages;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			redraw = 1;
		}
	}
	mnWindowSprRedrawSavedRectStub(menu->win);
	mnWindowSprClearListStub(menu->helpWin);
	for(i = 0; i < 4; i++) {
		menu->slotX[i] = STATUS_FIGURE_X(i);
		menu->slotY[i] = STATUS_FIGURE_ROW_Y;
	}
	menu->cursor->motion = ICON_SHOWN;
	menu->secondCursor->motion = ICON_SHOWN;
	mnIconSetActive(1);
	sySndPlayStub(SE_MENU_CANCEL);
}

void mnStatScreenShowMemberStat(Sint32 resource, Sint32 frm);

Bool mnStatScreenMovePartyMember(Sint32 selection, Sint32 direction);

/* The Status menu's party row: Left and Right choose a party member, showing
   their status; L and R move the party member one place along the party order;
   Select lists the Djinn found (once the first is caught, FLAG_DJINN_FIRST); A
   chooses the party member and B backs out. */
Sint32 mnStatScreenRunPartyRow(void)
{
	STAT_SCREEN_STATE *menu;
	Sint32			   ownerIdx;
	Sint32			   ownerCnt;
	Sint32			   page;
	Sint32			   redraw;
	Sint32			   result;
	Sint32			   rowIdx;

	menu = RamHeapSlots->menuState;
	ownerIdx = menu->memberIdx[0];
	ownerCnt = menu->memberCnt[0];
	redraw = 2;
	result = 0;
	page = menu->page;
	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(menu->memberIds[ownerIdx]);
	for(rowIdx = 0; rowIdx < 4; rowIdx++) {
		menu->rowX[rowIdx] = STATUS_FIGURE_X(rowIdx);
		menu->rowY[rowIdx] = STATUS_FIGURE_ROW_Y;
	}
	mnPaletteLightenHighlight(HIGHLIGHT_PALETTE);
	syDMAXCopy(
		(void *)OBJ_PLTT, (void *)BG_PLTT, DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy(
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(0, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	syDMAXCopy((void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(1, 0),
		DMA_COPY_HALFWORDS(16), REG_DMA3);
	syDMAXCopy((void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(1, TEXT_BACKGROUND_COLOR), DMA_COPY_HALFWORDS(1),
		REG_DMA3);
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(redraw) {
			mnWindowSprRedrawSavedRectStub(menu->selectorWin);
			mnTextDrawMessageAsTilesStub(
				MsgRearrangeHelp, menu->selectorWin, 0, 0);
			/* The Djinn list opens once Echo, the first Djinni the party meets,
			   has joined. */
			if(syGameFlagGet(FLAG_DJINNI(DJINNI_ECHO)))
				mnTextDrawMessageAsTilesStub(
					MsgDjinnListHelp, menu->selectorWin, 0, 16);
			mnTextDrawMessageAsTilesStub(
				MsgRearrangeHelp - 3, menu->selectorWin, 0, 8);
			ownerIdx = (ownerIdx + ownerCnt) % ownerCnt;
			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(menu->memberIds[ownerIdx]);
			page = (page + 3) % 3;
			mnStatScreenShowMemberStat(menu->memberIds[ownerIdx], page);
			mnReloadMemberPortrait(menu, menu->memberIds[ownerIdx]);
			if(redraw == 2) {
				mnCreatePartyFigures(ownerIdx / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->memberWin, ownerIdx, ownerCnt);
			redraw = 0;
		}
		mnCursorPos((ownerIdx % 4) * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerWork.repeat & KEY_R) {
			if(mnStatScreenMovePartyMember(ownerIdx, 1)) {
				sySndPlayStub(SE_MENU_CONFIRM);
				ownerIdx++;
				mnCreatePartyFigures(ownerIdx / 4 * 4);
				mnMarkChosenMember(menu->memberWin, ownerIdx, ownerCnt);
			} else {
				sySndPlayStub(SE_MENU_ERROR);
			}
			sySynch(1);
		} else if(PerWork.repeat & KEY_L) {
			if(mnStatScreenMovePartyMember(ownerIdx, 0)) {
				sySndPlayStub(SE_MENU_CONFIRM);
				ownerIdx--;
				mnCreatePartyFigures(ownerIdx / 4 * 4);
				mnMarkChosenMember(menu->memberWin, ownerIdx, ownerCnt);
			} else {
				sySndPlayStub(SE_MENU_ERROR);
			}
			sySynch(1);
		} else if((PerWork.push & KEY_SELECT) &&
			syGameFlagGet(FLAG_DJINNI(DJINNI_ECHO))) {
			mnStatScreenShowDjinnList();
			redraw = 1;
		} else {
			redraw = mnMoveGridCursor(&ownerIdx, ownerCnt, 4);
		}
	}
	menu->memberIdx[0] = ownerIdx;
	menu->selectedMember = menu->memberIds[ownerIdx];
	menu->selectedIds[0] = menu->memberIds[ownerIdx];
	menu->pageArrows[0]->motion = ICON_HIDDEN;
	menu->pageArrows[1]->motion = ICON_HIDDEN;
	return result;
}

void mnPlaceEntryIconsInGrid(Sint32 x, Sint32 y, Sint32 columns);

/* The party member chooser (menu page 3): Left and Right choose a party member,
   showing their status and Psynergy icons; A chooses and B backs out. */
Sint32 mnPartyMemberChooserChooseMember(void)
{
	STAT_SCREEN_STATE *menu = (STAT_SCREEN_STATE *)RamHeapSlots->menuState;
	Sint32			   selection = menu->memberIdx[0];
	Sint32			   cnt = menu->memberCnt[0];
	Sint32			   pending = 2;
	Sint32			   page = menu->page;
	Sint32			   result;

	/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
	syUnitGet(menu->memberIds[selection]);
	mnWindowSprRedrawSavedRectStub(menu->selectorWin);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgChooseCharacter, menu->selectorWin, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgChooseCharacter + 1, menu->selectorWin, 0, 16);
	while(1) {
		if(pending) {
			selection = (selection + cnt) % cnt;
			/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
			syUnitGet(menu->memberIds[selection]);
			page = (page + 3) % 3;
			mnStatScreenShowMemberStat(menu->memberIds[selection], page);
			mnReloadMemberPortrait(menu, menu->memberIds[selection]);
			menu->entryCnt = mnPsynergyMenuCollectActions(
				syUnitGet(menu->memberIds[selection]), menu->entries,
				PSYNERGY_LIST_ALL);
			mnPsynergyMenuDrawPsynergyIcons(menu->entries);
			mnPlaceEntryIconsInGrid(96, 96, 8);
			mnHideEmptyEntryIcons(menu->entries);
			if(pending == 2) {
				mnCreatePartyFigures(selection / 4 * 4);
				sySynch(1);
			}
			mnMarkChosenMember(menu->memberWin, selection, cnt);
		}
		mnCursorPos((selection % 4) * 24 - 10, 16);
		sySynch(1);
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		pending = mnMoveGridCursor(&selection, cnt, 4);
	}
	menu->memberIdx[0] = selection;
	menu->selectedMember = menu->memberIds[selection];
	menu->selectedIds[0] = menu->memberIds[selection];
	return result;
}

/* Moves the party member at selectedIdx one place later (direction 1) or
   earlier (0) in the party order. Returns 1, or 0 when it cannot move. */
Bool mnStatScreenMovePartyMember(Sint32 selectedIdx, Sint32 direction)
{
	STAT_SCREEN_STATE *menu = RamHeapSlots->menuState;
	Uint32			   reordered[14];
	Sint32			   idx;

	if(menu->partyCnt <= 1)
		return FALSE;
	if(direction == 1) {
		if(selectedIdx == menu->partyCnt - 1)
			return FALSE;
	} else {
		if(selectedIdx == 0)
			return FALSE;
	}

	for(idx = 0; idx < (Sint32)ARRAY_COUNT(reordered); idx++)
		reordered[idx] = 0;
	for(idx = 0; idx < *(Uint8 *)&menu->partyCnt; idx++)
		reordered[idx] = menu->memberIds[idx];

	if(direction == 1) {
		idx = reordered[selectedIdx];
		reordered[selectedIdx] = reordered[selectedIdx + 1];
		reordered[selectedIdx + 1] = idx;
	} else {
		idx = reordered[selectedIdx];
		reordered[selectedIdx] = reordered[selectedIdx - 1];
		reordered[selectedIdx - 1] = idx;
	}

	for(idx = 0; idx < menu->partyCnt; idx++)
		gmPartyRemoveMemberStub(menu->memberIds[idx]);
	for(idx = 0; idx < menu->partyCnt; idx++)
		gmPartyAddMemberStub(reordered[idx]);
	menu->partyCnt = gmPartyListMembersStub(menu->memberIds);
	return TRUE;
}

/* Sets up the party row of the Status menu and the party member chooser: the
   top-left window with the hand cursor and the party's figures, no other window
   yet, and an 8 by 2 cursor grid. Its callers pass four values it does not
   read. */
void mnStatScreenInit(
	Sint32 unusedA, Sint32 unusedB, Sint32 unusedC, Sint32 unusedD)
{
	STAT_SCREEN_STATE *state = RamHeapSlots->menuState;

	mnShowPartyFigures(mnCursorCreate(state), 2, 2);
	state->unopenedWin = 0;
	state->statWin = 0;
	state->helpWin = 0;
	state->actionWin = 0;
	state->selectedColumn = 0;
	state->selectedRow = 0;
	state->columnCnt = 8;
	state->rowCnt = 2;
}

void mnStatScreenDrawStatPage(Sint32 win, Sint32 ownerId, Sint32 mode);

/* Shows party member resource's status in the party row's status window,
   opening it the first time with the party member's portrait (and, for the
   party member chooser, the Psynergy icons). tick is not used. */
void mnStatScreenShowMemberStat(Sint32 resource, Sint32 frm)
{
	Sint32			   created;
	Sint32			   handle;
	void			  *object;
	STAT_SCREEN_STATE *state;

	state = RamHeapSlots->menuState;
	created = 0;
	handle = state->statWin;
	if(handle == 0) {
		created = mnWindowUpdateOrCreate(
			&state->statWin, 0, 5, 30, 15, WINDOW_FLAG_INSTANT);
		handle = state->statWin;
	}
	if(created != 0) {
		object = mnPortraitCreateStub(resource, 0, 0, handle, 0, 0);
		state->statCursor = object;
		if((((WINDOW_SPR *)object)->depth = OAM_DEPTH_WINDOW, state->page) ==
			STATUS_SCREEN_PAGE_CHOOSER) {
			mnSpawnIconEntries(state, handle);
		}
		mnStatScreenDrawStatPage(handle, resource, STATUS_PAGE_NO_CLEAR);
		return;
	}
	mnStatScreenDrawStatPage(handle, resource, STATUS_PAGE_SUMMARY);
}

/* The switch-characters help follows the A-button label on the help line:
   Japanese starts it 64 pixels in, the other editions 80. */
#define SWITCH_CHARACTER_HELP_X JAPANESE_OR(64, 80)

void mnStatScreenDrawSelectionCursor(
	Sint32 mode, Sint32 selected, Uint8 *entries, Sint32 invert);

void mnStatScreenDrawSelectionLabels(
	Sint32 tgt, Sint32 selected, const Uint8 *entries);
void mnStatScreenShowMemberProgressMessage(void *win, Sint32 help, Sint32 full);

/* The Status menu's status page: Up and Down choose one of the party member's
   statuses, whose advice the help window gives, Right the other column, its
   entries explained in turn; L and R change party member. Returns 1 for A, -1
   for B. */
Sint32 mnStatScreenRunStatPage(void)
{
	STAT_SCREEN_STATE *menu;
	Sint32			   pane;
	Sint32			   result;
	Sint32			   selected;
	Sint8			   cnt;
	Bool			   hasAilments;
	Bool			   done;
	Bool			   hasDjinn;
	Bool			   redraw;
	Sint32			   member;
	Sint32			   win;
	Sint32			   helpWin;
	Sint32			   i;
	Uint8			   entries[8];

	menu = RamHeapSlots->menuState;
	pane = 0;
	selected = 0;
	result = 0;
	win = menu->statWin;
	mnWindowUpdateOrCreate(&menu->helpWin, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
	helpWin = menu->helpWin;
	if(gmPartySumDjinnCountsStub(-1) != 0)
		hasDjinn = TRUE;
	else
		hasDjinn = FALSE;
	for(i = 3; i >= 0; i--)
		menu->rowY[i] = STATUS_FIGURE_FULL_PAGE_ROW_Y;
	done = FALSE;
	mnCursorSlide(-10, 88);

	while(done == 0 && !syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		/* DEAD: syUnitGet's result is not used; the ROM keeps the call. */
		syUnitGet(menu->selectedIds[0]);
		mnStatScreenDrawStatPage(win, menu->selectedIds[0], STATUS_PAGE_FULL);
		cnt = mnStatScreenListStatuses(entries, 1, menu->selectedIds[0]);
		hasAilments = FALSE;
		if(cnt == 0)
			cnt = 1;
		else
			hasAilments = TRUE;
		redraw = TRUE;

		while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
			if(redraw) {
				Sint32 limit;

				redraw = FALSE;
				limit = cnt;
				pane = (pane + 2) % 2;
				mnWindowSprRedrawSavedRectStub(helpWin);
				if(pane == 0) {
					selected = (selected + limit) % limit;
					if(hasAilments == 0) {
						/* "A:Psynergy" and "L-R:Switch characters" */
						mnTextDrawMessageAsTilesStub(
							MsgSwitchCharacterHelp + 1, win, 0, -24);
						mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp,
							win, SWITCH_CHARACTER_HELP_X, -24);
					}
				} else {
					if(hasDjinn != 0)
						selected = (selected + 8) % 8;
					else
						selected = (selected + 7) % 7;
				}
				mnStatScreenDrawSelectionCursor(pane, selected, entries, 0);
				mnWindowSprClearListStub(helpWin);
				sySynch(1);
				if(pane == 0)
					mnStatScreenDrawSelectionLabels(helpWin, selected, entries);
				else
					mnStatScreenShowMemberProgressMessage(
						helpWin, selected, hasDjinn);
			}
			if(pane == 0)
				mnCursorPos(-10, selected * 16 + 88);
			else if(selected <= 3)
				mnCursorPos(24, selected * 8 + 48);
			else
				mnCursorPos(48, selected * 8 + 80);
			sySynch(1);
			if(PerWork.repeat & KEYS_DPAD)
				mnStatScreenDrawSelectionCursor(pane, selected, entries, 1);
			if(PerWork.push & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				done = TRUE;
				result = 1;
				break;
			}
			if(PerWork.push & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				done = TRUE;
				result = -1;
				break;
			}
			if(PerWork.repeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				selected--;
			}
			if(PerWork.repeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				selected++;
			}
			if(PerWork.repeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				pane++;
			}
			if(PerWork.repeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
				pane--;
			}
			if((PerWork.repeat & KEY_R) || (PerWork.repeat & KEY_L)) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				member = menu->memberIdx[0];
				if(PerWork.repeat & KEY_R)
					member++;
				else
					member--;
				member = (member + menu->partyCnt) % menu->partyCnt;
				menu->selectedMember = menu->memberIds[member];
				menu->selectedIds[0] = menu->memberIds[member];
				menu->memberIdx[0] = member;
				mnReloadMemberPortrait(menu, menu->memberIds[member]);
				break;
			}
		}
	}
	mnWindowSprPrepareForRedrawStub(menu->helpWin);
	mnWindowSprRedrawSavedRectStub(win);
	for(i = 3; i >= 0; i--)
		menu->rowY[i] = STATUS_FIGURE_HIDDEN_Y;
	return result;
}

#undef SWITCH_CHARACTER_HELP_X

/* Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
   status page: in side 0 the selected status row, as wide as its label
   (StatusScreenCursorWidths); in side 1 one of the first four status rows or
   of the rows below. */
void mnStatScreenDrawSelectionCursor(
	Sint32 mode, Sint32 selected, Uint8 *entries, Sint32 invert)
{
	STAT_SCREEN_STATE *menu = RamHeapSlots->menuState;
	Sint32			   cnt;
	Sint32			   idx;
	Sint32			   x;
	Sint32			   y;
	Sint32			   palette;
	Uint8			   width;

	if(mode == 0) {
		y = selected * 2 + 5;
		x = 0;
		width = 5;
		cnt = 0;
		for(idx = 0; idx < CHARACTER_AILMENT_COUNT; idx++) {
			if(entries[idx] != 0) {
				if(selected == cnt) {
					width = StatusScreenCursorWidths[idx];
					break;
				}
				cnt++;
			}
		}
	} else if(selected <= 3) {
		y = selected;
		x = 5;
		width = 13;
	} else {
		y = selected + 4;
		x = 8;
		width = 20;
	}

	/* HIGHLIGHT_PALETTE when invert is 0, WINDOW_PALETTE when it is 1. */
	if(invert == 1)
		palette = WINDOW_PALETTE;
	else
		palette = HIGHLIGHT_PALETTE;
	mnWindowSetHighlightRect(menu->statWin, x, y, width, 1, palette);
}

/* Writes the advice for the selected status (two lines, for example how to
   revive a party member who is down) into window target, or that the party
   member is well when they have no status. */
void mnStatScreenDrawSelectionLabels(
	Sint32 tgt, Sint32 selected, const Uint8 *entries)
{
	Sint32 labelCnt = 0;
	Sint32 labelIdx;

	for(labelIdx = 0; labelIdx < CHARACTER_AILMENT_COUNT; labelIdx++) {
		if(entries[labelIdx] != 0) {
			if(selected == labelCnt) {
				Sint32 msgId = MsgStatusAdvice + labelIdx * 2;
				mnTextDrawMessageStub(msgId, tgt, 0, -1);
				mnTextDrawMessageStub(msgId + 1, tgt, 0, 15);
			}
			labelCnt++;
		}
	}

	if(labelCnt == 0)
		mnTextDrawMessageStub(MsgStatusNormal, tgt, 0, 0);
}

/* The help line's length, in characters: longer in the European editions. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELP_LENGTH 80
#else
#define HELP_LENGTH 64
#endif

/* Returns the experience party member memberId needs for level. */
Uint32 gmUnitGetExperienceForLevelStub(Sint32 memberId, Sint32 level);
/* Copies a message's text, its arguments expanded, into buffer: up to length
   characters with its terminator. */
void mnMessageCopyExpandedStub(Sint32 msgId, void *buf, Sint32 length);
/* Draws a decoded string of 16-bit characters into window at pixel (x, y), all
   at once. */
void mnTextDrawWideStringAtOffsetStub(
	void *text, void *win, Sint32 x, Sint32 y);

/* The help lines of the status page's right-hand column, counted from
   MsgYourNameAndLevelExperience. */
typedef enum STAT_HELP {
	STATUS_HELP_NAME_AND_LEVEL = 0, /* "Your name and level. Experience..." */
	/* "Experience points. You need ... more to reach the next level." */
	STATUS_HELP_EXPERIENCE = 1,
	STATUS_HELP_HP = 2, /* "Your current and max Hit Points..." */
	STATUS_HELP_PP = 3, /* "Your Psynergy Points..." */
	/* "Your Djinn. This also shows which ones are set." */
	STATUS_HELP_DJINN = 4,
	STATUS_HELP_LEVELS = 5, /* "Elemental Levels indicate..." */
	STATUS_HELP_POWER = 6,	/* "Power reflects the damage you..." */
	STATUS_HELP_RESIST = 7, /* "Resist reflects your defensive..." */
	/* "These are your experience points.", at the highest level. */
	STATUS_HELP_EXPERIENCE_AT_MAX = 8
} STAT_HELP;

/* Writes help line help of the status page's right-hand column into window,
   skipping the Djinn line unless full says the party has Djinn. The experience
   line says how many points the next level needs, or at the highest level only
   what they are. */
void mnStatScreenShowMemberProgressMessage(void *win, Sint32 help, Sint32 full)
{
	STAT_SCREEN_STATE *menu = RamHeapSlots->menuState;
	Uint8			  *buf;

	if(full == 0 && help > STATUS_HELP_PP)
		help++;
	if(help == STATUS_HELP_EXPERIENCE) {
		Uint8 *owner = &menu->selectedIds[0];
		UNIT  *state = syUnitGet(*owner);

		if(state->level == UNIT_LEVEL_MAX)
			help = STATUS_HELP_EXPERIENCE_AT_MAX;
		else
			mnMessageArgumentPushStub(
				gmUnitGetExperienceForLevelStub(*owner, state->level + 1) -
					state->experience,
				MESSAGE_ARGUMENT_NUMBER);
	}
	buf = syHeapAlloc(HELP_LENGTH * 2);
	mnMessageCopyExpandedStub(
		help + (Sint32)MsgYourNameAndLevelExperience, buf, HELP_LENGTH);
	mnTextDrawWideStringAtOffsetStub(buf, win, 0, -1);
	syHeapRelease(buf);
}

#undef HELP_LENGTH

/* The Status screen's level label: `Lv`; `Nv.` in Spanish, `Niv` in French. */
extern const Uint8 MenuLvString[];
/* `/` between a current and a maximum HP or PP, and between a set and a held
   Djinn count. */
extern const Uint8 MenuSlashString[];

void mnStatScreenDrawTopRows(Sint32 win, Sint32 owner, Sint32 flg);

void mnPanelDrawEntryList(
	WINDOW *panel, Sint32 x, Sint32 y, const Uint8 *enabled);

Sint32 gmUnitGetElementLevelStub(Sint32 ownerId, Sint32 element);

/* Draws party member memberId's status page into window: the top rows
   (mnStatScreenDrawTopRows), the status list or "Normal", and except on the
   chooser page the Djinn counts by element and, in mode 1, the elemental
   levels, Power and Resist. With STATUS_PAGE_NO_CLEAR in mode the window is not
   cleared first. */
void mnStatScreenDrawStatPage(Sint32 win, Sint32 ownerId, Sint32 mode)
{
	STAT_SCREEN_STATE *menu = RamHeapSlots->menuState;
	UNIT			  *unit;
	Sint32			   element;
	Sint32			   djinnRow;
	Sint32			   keepDrawn;
	Sint32			   hasDjinn;
	Uint8			   ailments[8];

	if(gmPartySumDjinnCountsStub(-1) != 0)
		hasDjinn = 1;
	else
		hasDjinn = 0;
	unit = syUnitGet(ownerId);
	/* The Djinn row, in tiles: higher on the full page (mode 1), which has the
	   elemental rows below it. */
	if((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL)
		djinnRow = 7;
	else
		djinnRow = 10;
	menu->statCursor->motion = ICON_SHOWN;
	mnStatScreenDrawTopRows(win, ownerId, mode);
	mnStatScreenListStatuses(ailments, 1, ownerId);
	keepDrawn = mode & STATUS_PAGE_NO_CLEAR;
	if(keepDrawn == 0)
		mnWindowClearInteriorTilesStub(win, 0, 40, 96, 96);
	mnPanelDrawEntryList(win, 0, 40, ailments);
	if(menu->page == STATUS_SCREEN_PAGE_CHOOSER)
		return;
	if(keepDrawn == 0) {
		sySynch(1);
		mnWindowClearInteriorTilesStub(win, 64, 56, 224, 96);
	}
	/* The four element icons, then the Djinn counts and, in mode 1, the
	   elemental levels, Power and Resist. */
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	if(mode == STATUS_PAGE_FULL || hasDjinn == 1) {
		mnWindowSetTilemapEntryStub(win, 1, 15, djinnRow, TILEMAP_ENTRY_ICON);
		mnWindowSetTilemapEntryStub(win, 2, 19, djinnRow, TILEMAP_ENTRY_ICON);
		mnWindowSetTilemapEntryStub(win, 3, 23, djinnRow, TILEMAP_ENTRY_ICON);
		mnWindowSetTilemapEntryStub(win, 4, 27, djinnRow, TILEMAP_ENTRY_ICON);
	}
	if(hasDjinn)
		mnTextDrawMessageAsTilesStub(
			MsgDjinnLabel, win, 64, (djinnRow + 1) * 8);
	if(mode == STATUS_PAGE_FULL) {
		if(!hasDjinn)
			djinnRow--;
		MENU_DRAW_STATUS_LABEL(MenuLvString, win, 64, (djinnRow + 2) * 8);
		mnTextDrawMessageAsTilesStub(
			MsgPowerLabel, win, 64, (djinnRow + 3) * 8);
		mnTextDrawMessageAsTilesStub(
			MsgPowerLabel + 1, win, 64, (djinnRow + 4) * 8);
	}
	for(element = 0; element <= 3; element++) {
		if(hasDjinn)
			mnTextDrawNumInWindowStub(unit->djinnHeldCnts[element], 1, win,
				STATUS_ELEMENT_X(element), (djinnRow + 1) * 8);
		if((mode & STATUS_PAGE_MODE_MASK) == STATUS_PAGE_FULL) {
			if(hasDjinn) {
				mnTextDrawNumInWindowStub(unit->djinnSetCnts[element], 1, win,
					STATUS_ELEMENT_X(element) - 16, (djinnRow + 1) * 8);
				mnTextDrawStringInWindowStub(MenuSlashString, win,
					STATUS_ELEMENT_X(element) - 8, (djinnRow + 1) * 8);
			}
			mnTextDrawNumInWindowStub(
				gmUnitGetElementLevelStub(ownerId, element), 2, win,
				STATUS_ELEMENT_X(element) - 8, (djinnRow + 2) * 8);
			mnTextDrawNumInWindowStub(unit->element[element].power, 3, win,
				STATUS_ELEMENT_X(element) - 16, (djinnRow + 3) * 8);
			mnTextDrawNumInWindowStub(unit->element[element].resistance, 3, win,
				STATUS_ELEMENT_X(element) - 16, (djinnRow + 4) * 8);
		}
	}
}

/* The HP label: `HP`; `KP` in German, `PV` in Spanish and French, `PS` in
   Italian. */
extern const Uint8 MenuHpString[];
/* The PP label: `PP`; `EP` in German. */
extern const Uint8 MenuPpString[];

/* The stat column of the status page's top rows: three digits from 200 or four
   from 192, both ending at pixel 224. */
#define STATUS_STAT_DIGITS JAPANESE_OR(4, 3)
#define STATUS_STAT_X	   JAPANESE_OR(192, 200)

/* Stat label `stat`, 0 to 3, of "Attack", "Defense", "Agility" and "Luck",
   MsgStatLabel and the three after it, counted back from MsgExpLabel. */
#define STAT_LABEL(stat) \
	((Sint32)MsgExpLabel - (MSGNO(MsgExpLabel) - MSGNO(MsgStatLabel) - (stat)))

/* Draws the top rows of party member owner's status page into window: name,
   class, level, experience, HP and PP on the left, Attack, Defense, Agility and
   Luck on the right. */
void mnStatScreenDrawTopRows(Sint32 win, Sint32 owner, Sint32 flg)
{
	ITEM_MENU_STATE *menu;
	UNIT			*unit;

	menu = RamHeapSlots->menuState;
	unit = syUnitGet(owner);
	menu->cursor->motion = ICON_SHOWN;
	flg &= STATUS_PAGE_NO_CLEAR;
	if(flg == 0)
		mnWindowClearInteriorTilesStub(win, 0, 0, 128, 40);
	mnTextDrawStringAtOffsetStub(unit->name, win, 40, 0);
	mnTextDrawMessageAsTilesStub(
		unit->classId + (Sint32)MsgClassName, win, 0, 32);
	MENU_DRAW_STATUS_LABEL(MenuLvString, win, 104, 0);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	mnTextDrawNumInWindowStub(unit->level, 2, win, 128, 0);
	MENU_DRAW_STATUS_LABEL(MenuHpString, win, 40, 16);
	mnTextDrawNumInWindowStub(unit->hp, 4, win, 72, 16);
	mnTextDrawNumInWindowStub(unit->hpMax, 4, win, 112, 16);
	mnTextDrawStringInWindowStub(MenuSlashString, win, 104, 16);
	MENU_DRAW_STATUS_LABEL(MenuPpString, win, 40, 24);
	mnTextDrawNumInWindowStub(unit->pp, 4, win, 72, 24);
	mnTextDrawNumInWindowStub(unit->ppMax, 4, win, 112, 24);
	mnTextDrawStringInWindowStub(MenuSlashString, win, 104, 24);
	mnTextDrawMessageAsTilesStub((Sint32)MsgExpLabel, win, 40, 8);
	mnTextDrawNumInWindowStub(unit->experience, 8, win, 80, 8);
	if(flg == 0) {
		sySynch(1);
		mnWindowClearInteriorTilesStub(win, 144, 0, 224, 40);
	}
	/* "Attack", "Defense", "Agility" and "Luck". */
	mnTextDrawMessageAsTilesStub(STAT_LABEL(0), win, 152, 0);
	mnTextDrawMessageAsTilesStub(STAT_LABEL(1), win, 152, 8);
	mnTextDrawMessageAsTilesStub(STAT_LABEL(2), win, 152, 16);
	mnTextDrawMessageAsTilesStub(STAT_LABEL(3), win, 152, 24);
	mnTextDrawNumInWindowStub(
		unit->attack, STATUS_STAT_DIGITS, win, STATUS_STAT_X, 0);
	mnTextDrawNumInWindowStub(
		unit->defense, STATUS_STAT_DIGITS, win, STATUS_STAT_X, 8);
	mnTextDrawNumInWindowStub(
		unit->agility, STATUS_STAT_DIGITS, win, STATUS_STAT_X, 16);
	mnTextDrawNumInWindowStub(
		unit->luck, STATUS_STAT_DIGITS, win, STATUS_STAT_X, 24);
}

#undef STATUS_STAT_DIGITS
#undef STATUS_STAT_X
#undef STAT_LABEL

/* The rows one page of the list shows: Japanese fits six, the other editions
   five. */
#define STATUS_LIST_PAGE_ROWS JAPANESE_OR(6, 5)

/* Fills result with the item list of the party member in side index for the
   equipment pages: their unit, the menu's item count, and the page and row of
   the item chosen last for them, STATUS_LIST_PAGE_ROWS items to a page. Returns
   1. */
Sint32 mnPagedListBuild(PAGED_LIST *result, Sint32 idx)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	void			*owner;
	Sint32			 cnt;
	Sint32			 value;
	Sint32			 page;
	Sint32			 row;
	Sint32			 pages;

	owner = syUnitGet(menu->sideMember[idx]);
	cnt = menu->itemCnt;
	value = menu->selectedIdxByMember[menu->sideMember[idx]];

	if(value + 1 > cnt)
		value = cnt - 1;
	if(cnt == 0)
		value = 0;
	page = value / STATUS_LIST_PAGE_ROWS;
	row = value % STATUS_LIST_PAGE_ROWS;
	pages = cnt / STATUS_LIST_PAGE_ROWS;
	if(cnt % STATUS_LIST_PAGE_ROWS != 0)
		pages++;
	result->memberState = (Sint32)owner;
	result->page = page;
	result->pageCnt = pages;
	result->row = row;
	result->entryCnt = cnt;
	result->selectedIdx = value;
	return 1;
}

#undef STATUS_LIST_PAGE_ROWS

/* Pairs of 4-bit rows of dots in colour 5, each reaching further right than the
   last, that mnBuildPatternTiles XORs into its tile pairs (menu_tiles.s). */
extern const Uint32 MenuPatternRowMasks[][2];

/* The words of one range icon's two tiles. */
#define RANGE_PATTERN_PAIR_WORDS	 16
/* The first of the Status menu's range icon BG tiles, which
   mnPsynergyMenuDrawRange draws as tilemap entries; the battle menu builds its
   own elsewhere (mnRangeIconBuildTiles). */
#define STATUS_RANGE_ICON_FIRST_TILE 768

/* Builds the twelve range icons mnPsynergyMenuDrawRange draws, as the 24 BG
   tiles from STATUS_RANGE_ICON_FIRST_TILE: icon n is a centre tile and an end
   tile, filled with WINDOW_BACKGROUND_COLOR, then marked row by row by XOR with
   MenuPatternRowMasks, the first set's marks growing row by row, the second
   set's leaving row 1 bare. */
void mnBuildPatternTiles(void)
{
	Sint32 patternSet;
	Sint32 patternIdx;

	for(patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
		for(patternIdx = 0; patternIdx < RANGE_PATTERN_COUNT; patternIdx++) {
			Uint32 *tile = (Uint32 *)(BG_CHAR_BLOCK(0) +
							   STATUS_RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP) +
				patternSet * (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_WORDS) +
				patternIdx * RANGE_PATTERN_PAIR_WORDS;
			Sint32 row;

			syWordsFillEntry(
				tile, 2 * TILE_SIZE_4BPP, TEXT_CANVAS_BACKGROUND_WORD);
			for(row = 1; row <= 7; row++) {
				Sint32 srcPattern = patternIdx;

				if(patternSet == 1 && row <= 1)
					continue;
				if(patternSet == 0 && patternIdx > row - 2) {
					srcPattern = row - 2;
					if(srcPattern < 0)
						srcPattern = 0;
				}
				tile[row] =
					mnXorWord(tile[row], MenuPatternRowMasks[srcPattern][0]);
				tile[row + 8] = mnXorWord(
					tile[row + 8], MenuPatternRowMasks[srcPattern][1]);
			}
		}
	}
}

/* Draws a Psynergy range icon at tile (x, y) of window: three tiles from pair
   range of the range icon tiles (mnBuildPatternTiles), the side tile on both
   ends, mirrored on the left, and the centre tile between. range is the
   ability's range less one (0 for the aimed unit alone), or RANGE_ICON_ALL for
   ABILITY_RANGE_ALL (mnPsynergyMenuDrawListPage). */
void mnPsynergyMenuDrawRange(
	Sint32 win, Sint32 x, Sint32 y, Sint32 range, Sint32 unused)
{
	Sint32 n;
	Sint32 tile;

	n = range * 2;
	tile = n + WINDOW_TILE(STATUS_RANGE_ICON_FIRST_TILE) + 1;
	mnWindowSetTilemapEntryStub(win, TILE_HFLIP | tile, x, y, 0);
	mnWindowSetTilemapEntryStub(
		win, n + WINDOW_TILE(STATUS_RANGE_ICON_FIRST_TILE), x + 1, y, 0);
	mnWindowSetTilemapEntryStub(win, tile, x + 2, y, 0);
}

/* The Japanese page lists six abilities rather than five, a row higher, and
   says where the chosen ability can be used in short lines stacked at the
   window's left edge rather than in one sentence across it. FIRST_TILE_ROW is
   the tile row of the page's first ability; USABLE_TOP, USABLE_RIGHT and
   USABLE_BOTTOM bound that text in pixels inside the window. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define STATUS_LIST_PAGE_ROWS	 6
#define FIRST_TILE_ROW			 1
#define USABLE_TOP				 72
#define USABLE_RIGHT			 64
#define USABLE_BOTTOM			 96
/* まちやダンジョン ("towns and dungeons"), the message two after
   MsgExpLabel. */
#define USABLE_IN_FIELD_MESSAGE	 ((Sint32)MsgExpLabel + 2)
/* せんとう ("battle"), the message after MsgExpLabel. */
#define USABLE_IN_BATTLE_MESSAGE ((Sint32)MsgExpLabel + 1)
/* でつかえます ("can be used"), the message before MsgDjinnListHelp. */
#define CAN_BE_USED_MESSAGE		 ((Sint32)MsgDjinnListHelp - 1)
#else
#define STATUS_LIST_PAGE_ROWS 5
#define FIRST_TILE_ROW		  2
#define USABLE_TOP			  96
#define USABLE_RIGHT		  224
#define USABLE_BOTTOM		  104
#endif

/* Updates the Psynergy page for the row under the cursor: the Psynergy's
   description in the info window, where it can be used (in the field, in
   battle, or anywhere), and the row highlights with the range icons. */
Sint32 mnPsynergyMenuDrawRangePage(Sint32 win, Sint32 unused, PAGED_LIST *state)
{
	PSYNERGY_MENU_STATE *menu;
	ABILITY_DEFINITION	*ability;
	Sint32				 row;
	Sint32				 base;

	menu = (PSYNERGY_MENU_STATE *)RamHeapSlots->menuState;
	state->selectedIdx = state->page * STATUS_LIST_PAGE_ROWS + state->row;
	/* The Japanese release clears the info window; in every edition but the
	   Japanese it redraws the window's saved rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
	mnWindowSprRedrawSavedRectStub(menu->infoWin);
#else
	mnWindowSprClearListStub(menu->infoWin);
#endif
	sySynch(1);
	if(menu->psynergies[state->selectedIdx] != 0) {
		/* Japanese draws the description as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
		mnTextDrawMessageStub(
			(menu->psynergies[state->selectedIdx] & ACTION_ID_MASK) +
				(Sint32)MsgAbilityDescription,
			(Sint32)menu->infoWin, 0, 0);
#else
		mnTextDrawMessageAsTilesStub(
			(menu->psynergies[state->selectedIdx] & ACTION_ID_MASK) +
				(Sint32)MsgAbilityDescription,
			(Sint32)menu->infoWin, 0, 0);
#endif
		ability = gmAbilityGetDirectStub(
			menu->psynergies[state->selectedIdx] & ACTION_ID_MASK);
		mnWindowClearInteriorTilesStub(
			win, 0, USABLE_TOP, USABLE_RIGHT, USABLE_BOTTOM);
		row = 0;
#if LANGUAGE == LANGUAGE_JAPANESE
		/* One line for each place the ability works, then the end of the
		   sentence on the line below them. */
		if(ability->fieldPsynergy != 0 ||
			(ability->flg & ABILITY_USABLE_IN_FIELD) != 0) {
			mnTextDrawMessageAsTilesStub(
				USABLE_IN_FIELD_MESSAGE, win, 0, USABLE_TOP);
			row = 1;
		}
		if((ability->flg & ABILITY_USABLE_IN_BATTLE) != 0) {
			mnTextDrawMessageAsTilesStub(
				USABLE_IN_BATTLE_MESSAGE, win, 0, row * 8 + USABLE_TOP);
			row++;
		}
		mnTextDrawMessageAsTilesStub(
			CAN_BE_USED_MESSAGE, win, 0, row * 8 + USABLE_TOP);
#else
		/* row holds where it works for now: 2 in the field, 1 in battle. */
		if(ability->fieldPsynergy != 0 ||
			(ability->flg & ABILITY_USABLE_IN_FIELD) != 0) {
			row = 2;
		}
		if((ability->flg & ABILITY_USABLE_IN_BATTLE) != 0) {
			row |= 1;
		}
		if(row == 3) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUsableAnywhere, win, 0, USABLE_TOP);
		} else if(row == 2) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUsableInField, win, 0, USABLE_TOP);
		} else if(row == 1) {
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgUsableInBattle, win, 0, USABLE_TOP);
		}
#endif
	}

	base = state->page * STATUS_LIST_PAGE_ROWS;
	for(row = 0; row <= STATUS_LIST_PAGE_ROWS - 1; row++) {
		if(row == state->row) {
			ability = gmAbilityGetDirectStub(
				menu->psynergies[base + row] & ACTION_ID_MASK);
			if(ability->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(
					win, ability->element + 1, 24, row * 2 + FIRST_TILE_ROW, 0);
				mnWindowSetHighlightRect(
					win, 9, row * 2 + FIRST_TILE_ROW, 15, 1, HIGHLIGHT_PALETTE);
				mnWindowSetHighlightRect(
					win, 25, row * 2 + FIRST_TILE_ROW, 3, 1, HIGHLIGHT_PALETTE);
			} else {
				mnWindowSetHighlightRect(
					win, 9, row * 2 + FIRST_TILE_ROW, 19, 1, HIGHLIGHT_PALETTE);
			}
		} else {
			ability = gmAbilityGetDirectStub(
				menu->psynergies[base + row] & ACTION_ID_MASK);
			if(ability->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(win, ability->element + 1, 24,
					row * 2 + FIRST_TILE_ROW, TILEMAP_ENTRY_ICON);
				mnWindowSetHighlightRect(
					win, 9, row * 2 + FIRST_TILE_ROW, 15, 1, WINDOW_PALETTE);
				mnWindowSetHighlightRect(
					win, 25, row * 2 + FIRST_TILE_ROW, 3, 1, WINDOW_PALETTE);
			} else {
				mnWindowSetHighlightRect(
					win, 9, row * 2 + FIRST_TILE_ROW, 19, 1, WINDOW_PALETTE);
			}
		}
	}
	sySynch(1);
	return 1;
}

#undef STATUS_LIST_PAGE_ROWS
#undef USABLE_RIGHT
#undef USABLE_BOTTOM

/* The Japanese page lists six abilities rather than five, a row higher, and
   lays its heading, names, costs, page icons and "no Psynergy" message out on
   its own columns. */
#define STATUS_LIST_PAGE_ROWS		 JAPANESE_OR(6, 5)
#define PAGE_ICONS_X				 JAPANESE_OR(86, 80)
#define PAGE_ICONS_Y				 JAPANESE_OR(53, 58)
#define STATUS_PSYNERGY_PP_HEADING_X JAPANESE_OR(144, 176)
#define FIRST_ROW_Y					 JAPANESE_OR(8, 16)
#define STATUS_PSYNERGY_NAME_X		 JAPANESE_OR(96, 88)
#define STATUS_PSYNERGY_COST_X		 JAPANESE_OR(168, 176)
#define FIRST_RANGE_ROW				 JAPANESE_OR(1, 2)
#define EMPTY_MESSAGE_Y				 JAPANESE_OR(8, 17)

/* The Psynergy menu's list page: each ability's name, PP cost and range, "no
   Psynergy" when empty, then the owner's name, class and level. */
Sint32 mnPsynergyMenuDrawListPage(
	Sint32 win, Sint32 unused, const PAGED_LIST *res)
{
	PSYNERGY_MENU_STATE *menu = RamHeapSlots->menuState;
	UNIT				*member;
	Uint32				 firstEntry;
	Uint32				 rows;
	Uint8				 row;
	Sint32				 entryIdx;

	(void)unused;

	member = syUnitGet(menu->memberIds[0]);

	mnWindowSprRedrawSavedRectStub(win);

	firstEntry = res->page * STATUS_LIST_PAGE_ROWS;
	rows = (Uint8)(res->entryCnt - firstEntry);
	if(rows > STATUS_LIST_PAGE_ROWS) {
		rows = STATUS_LIST_PAGE_ROWS;
	}

	mnSetPageIcons(
		STATUS_LIST_PAGE_ROWS, firstEntry, win, PAGE_ICONS_X, PAGE_ICONS_Y);
	mnPagedListDrawPageNumbers(
		win, res->entryCnt, STATUS_LIST_PAGE_ROWS, res->page, 28);

	/* "PP" */
	mnTextDrawMessageAsTilesStub((Sint32)MsgPsynergyChooseTarget + 2, win,
		STATUS_PSYNERGY_PP_HEADING_X, 0);

	row = 0;
	if(rows > row) {
		entryIdx = firstEntry;
		do {
			ABILITY_DEFINITION *action;
			Sint32				msgId;
			Sint32				y;
			Sint32				range;
			action = gmAbilityGetDirectStub(
				PSYNERGY_ABILITY_MASK & menu->psynergies[entryIdx]);
			msgId = (menu->psynergies[entryIdx] & PSYNERGY_ABILITY_MASK) +
				(Sint32)MsgAbilityName;
			y = row * 16 + FIRST_ROW_Y;

			mnTextDrawMessageAsTilesStub(msgId, win, STATUS_PSYNERGY_NAME_X, y);
			mnTextDrawNumAtOffsetStub(
				action->ppCost, 2, win, STATUS_PSYNERGY_COST_X, y);

			/* ABILITY_RANGE_ALL takes the last range icon; range n takes n -
			   1. */
			range = action->range;
			if(range == ABILITY_RANGE_ALL) {
				range = RANGE_ICON_ALL;
			} else {
				range--;
			}
			mnPsynergyMenuDrawRange(
				win, 25, row * 2 + FIRST_RANGE_ROW, range, 0);

			row++;
			entryIdx++;
		} while(rows > row);
	}

	if(menu->psynergyCnt == 0) {
		/* "You have no Psynergy." */
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgPsynergyChooseForR - 1, win, 96, EMPTY_MESSAGE_Y);
	}

	mnTextDrawStringAtOffsetStub(member->name, win, 40, 0);
	mnTextDrawMessageAsTilesStub(
		member->classId + (Sint32)MsgClassName, win, 0, 32);
	/* Spanish and French draw the level label at an offset, as the owner's
	   name. */
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH
	mnTextDrawStringAtOffsetStub(MenuLvString, win, 0, 48);
#else
	mnTextDrawStringInWindowStub(MenuLvString, win, 0, 48);
#endif
	mnTextDrawNumInWindowStub(member->level, 2, win, 24, 48);

	return 1;
}

#undef STATUS_LIST_PAGE_ROWS
#undef PAGE_ICONS_X
#undef STATUS_PSYNERGY_PP_HEADING_X
#undef STATUS_PSYNERGY_NAME_X
#undef STATUS_PSYNERGY_COST_X
#undef FIRST_RANGE_ROW
#undef EMPTY_MESSAGE_Y

/* The rows of a page of the list and where the cursor points at its first row:
   the Japanese page lists six abilities, a row higher. */
#define STATUS_LIST_PAGE_ROWS	 JAPANESE_OR(6, 5)
#define STATUS_PSYNERGY_CURSOR_X JAPANESE_OR(58, 55)
#define CURSOR_Y				 JAPANESE_OR(52, 60)

/* The Status menu's Psynergy page: the party member's Psynergy,
   STATUS_LIST_PAGE_ROWS to a page, each with its PP cost and range icon; the
   cursor's Psynergy is described in the info window with where it can be
   used. */
Sint32 mnStatScreenRunPsynergyPage(void)
{
	PSYNERGY_MENU_STATE *menu;
	Sint32				 result;
	Sint32				 win;
	Sint32				 refresh;
	Bool				 listNeedsRedraw;
	Sint32				 navigation;
	Sint32				 ownerIdx;
	PAGED_LIST			 state;

	menu = RamHeapSlots->menuState;
	win = menu->statWin;
	result = 0;
	refresh = 2;
	listNeedsRedraw = TRUE;
	mnWindowUpdateOrCreate(&menu->infoWin, 0, 0, 30, 5, WINDOW_FLAG_INSTANT);
	mnSpawnIconEntries(menu, menu->msgWin);
	/* "A:Item" and "L-R:Switch characters" */
	mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp + 2, win, 0, -24);
	mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp, win, 64, -24);
	mnBuildPatternTiles();
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(refresh) {
			if(refresh == 2) {
				menu->psynergyCnt =
					mnPsynergyMenuCollectActions(syUnitGet(menu->memberIds[0]),
						menu->psynergies, PSYNERGY_LIST_ALL);
				mnPagedListBuild(&state, 0);
				mnItemMenuHideAllIcons();
				mnPsynergyMenuDrawPsynergyIcons(menu->psynergies);
				listNeedsRedraw = TRUE;
				sySynch(1);
			}
			if(listNeedsRedraw) {
				listNeedsRedraw = FALSE;
				mnPsynergyMenuDrawListPage(win, 0, &state);
			}
			mnPsynergyMenuDrawRangePage(win, 0, &state);
#if PSYNERGY_PAGE_REDRAWS_HELP
			/* German, Spanish, French and Italian draw the help again after
			   each page. */
			mnTextDrawMessageAsTilesStub(
				MsgSwitchCharacterHelp + 2, win, 0, -24);
			mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp, win, 64, -24);
#endif
			refresh = 0;
		}
		sySynch(1);
		navigation = mnPagedListHandleInput(
			0, state.entryCnt, STATUS_LIST_PAGE_ROWS, &state.row, &state.page);
		mnCursorPos(STATUS_PSYNERGY_CURSOR_X, state.row * 16 + CURSOR_Y);
		if(navigation == 1) {
			refresh = 1;
			listNeedsRedraw = TRUE;
		}
		if(navigation == 0)
			refresh = 1;
		if(navigation == -1)
			refresh = 0;
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if((PerWork.repeat & KEY_R) || (PerWork.repeat & KEY_L)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			ownerIdx = menu->tabIdx[0];
			menu->selectedIdxByMember[menu->memberTbl[ownerIdx]] =
				state.selectedIdx;
			if(PerWork.repeat & KEY_R)
				ownerIdx++;
			else
				ownerIdx--;
			ownerIdx = (ownerIdx + menu->memberCnt) % menu->memberCnt;
			menu->selectedMember = menu->memberTbl[ownerIdx];
			menu->memberIds[0] = menu->memberTbl[ownerIdx];
			menu->tabIdx[0] = ownerIdx;
			mnReloadMemberPortrait(menu, menu->memberTbl[ownerIdx]);
			refresh = 2;
		}
	}
	mnWindowSprPrepareForRedrawStub(menu->infoWin);
	mnWindowSprRedrawSavedRectStub(win);
	mnWindowSprClearListStub(menu->msgWin);
	return result;
}

#undef STATUS_LIST_PAGE_ROWS
#undef STATUS_PSYNERGY_CURSOR_X
#undef CURSOR_Y

/* The rows one page of the item list shows: Japanese fits six, the other
   editions five. */
#define STATUS_LIST_PAGE_ROWS JAPANESE_OR(6, 5)

/* The tile column the equipment slots' highlight starts at. */
#define EQUIP_SLOT_X	 2
/* The equipment slots' highlight is a tile wider in the European editions. */
#define EQUIP_SLOT_WIDTH BY_LANGUAGE(9, 9, 10, 10, 10, 10)

/* Updates the Status menu's equipment page for the row under the cursor: the
   item's plain name in the info window, the row highlights, and in the
   equipment window the highlight of the slot the item is worn in, if it is. */
Sint32 mnItemMenuDrawEquipPage(Sint32 win, Sint32 unused, PAGED_LIST *state)
{
	ITEM_MENU_STATE *menu;
	ITEM_DEFINITION *item;
	Sint32			 base;
	Sint32			 row;
	Uint32			 entry;
	Uint32			 mask;

	menu = RamHeapSlots->menuState;
	base = state->page * STATUS_LIST_PAGE_ROWS;
	state->selectedIdx = base + state->row;
	/* The Japanese release clears the name's window; in every edition but the
	   Japanese it redraws the window's saved rectangle. */
#if LANGUAGE != LANGUAGE_JAPANESE
	mnWindowSprRedrawSavedRectStub(menu->infoWin);
#else
	mnWindowSprClearListStub(menu->infoWin);
#endif
	sySynch(1);
	if(menu->items[state->selectedIdx] != 0) {
		/* Japanese draws the name as a resource string. */
#if LANGUAGE == LANGUAGE_JAPANESE
		mnTextDrawMessageStub(
			(menu->items[state->selectedIdx] & INVENTORY_ITEM_ID_MASK) +
				(Sint32)MsgItemDescription,
			menu->infoWin, 0, 0);
#else
		mnTextDrawMessageAsTilesStub(
			(menu->items[state->selectedIdx] & INVENTORY_ITEM_ID_MASK) +
				MsgItemDescription,
			menu->infoWin, 0, 0);
#endif
	}

	for(row = 0; row < STATUS_LIST_PAGE_ROWS; row++) {
		if(row == state->row) {
			entry = menu->items[state->selectedIdx];
			mask = INVENTORY_ITEM_ID_MASK;
			mask &= entry;
			item = gmItemGetDirectStub(mask);
			if(item->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(
					win, item->element + 1, 27, row * 2 + 1, 0);
				mnWindowSetHighlightRect(
					win, 14, row * 2 + 1, 13, 1, HIGHLIGHT_PALETTE);
			} else {
				mnWindowSetHighlightRect(
					win, 14, row * 2 + 1, 14, 1, HIGHLIGHT_PALETTE);
			}
		} else {
			entry = menu->items[base + row];
			mask = INVENTORY_ITEM_ID_MASK;
			mask &= entry;
			item = gmItemGetDirectStub(mask);
			if(item->element != ELEMENT_NONE) {
				mnWindowSetTilemapEntryStub(win, item->element + 1, 27,
					row * 2 + 1, TILEMAP_ENTRY_ICON);
				mnWindowSetHighlightRect(
					win, 14, row * 2 + 1, 13, 1, WINDOW_PALETTE);
			} else {
				mnWindowSetHighlightRect(
					win, 14, row * 2 + 1, 14, 1, WINDOW_PALETTE);
			}
		}
	}

	for(row = 0; row <= 3; row++)
		mnWindowSetHighlightRect(menu->equipWin, EQUIP_SLOT_X, row * 2 + 1,
			EQUIP_SLOT_WIDTH, 1, WINDOW_PALETTE);

	if(menu->items[state->selectedIdx] & INVENTORY_EQUIPPED) {
		item = gmItemGetDirectStub(
			menu->items[state->selectedIdx] & INVENTORY_ITEM_ID_MASK);
		switch(item->type) {
		case ITEM_TYPE_WEAPON:
			mnWindowSetHighlightRect(menu->equipWin, EQUIP_SLOT_X, 1,
				EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		case ITEM_TYPE_HELM:
			mnWindowSetHighlightRect(menu->equipWin, EQUIP_SLOT_X, 3,
				EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		case ITEM_TYPE_SHIELD:
			mnWindowSetHighlightRect(menu->equipWin, EQUIP_SLOT_X, 5,
				EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		case ITEM_TYPE_ARMOR:
			mnWindowSetHighlightRect(menu->equipWin, EQUIP_SLOT_X, 7,
				EQUIP_SLOT_WIDTH, 1, HIGHLIGHT_PALETTE);
			break;
		}
	}
	sySynch(1);
	return 1;
}

#undef STATUS_LIST_PAGE_ROWS
#undef EQUIP_SLOT_WIDTH

/* The rows of a page of the item list: six in Japanese. */
#define STATUS_LIST_PAGE_ROWS JAPANESE_OR(6, 5)

/* The page icons, the item names, the "no items" message and the Attack and
   Defense figures, as each translation lays them out. */
#define PAGE_ICONS_X	BY_LANGUAGE(123, 123, 123, 119, 123, 119)
#define ITEM_NAME_X		BY_LANGUAGE(136, 136, 136, 128, 136, 128)
#define EMPTY_MESSAGE_X GERMAN_OR(112, 120)
#define STAT_X			BY_LANGUAGE(72, 72, 72, 80, 80, 72)

/* Draws one page of the member's items into window: STATUS_LIST_PAGE_ROWS
   names, or "no items", then the member's name, Attack and Defense. */
Sint32 mnItemMenuDrawItemPage(Sint32 win, Sint32 unused, PAGED_LIST *state)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	UNIT			*unit;
	Uint16			*inventoryEntry;
	Sint32			 firstEntry;
	Uint32			 rowCnt;
	Uint32			 rowIdx;
	Uint8			*msg;

	unit = syUnitGet(menu->sideMember[0]);
	mnWindowClearInteriorTilesStub(win, 128, 8, 224, 96);
	firstEntry = state->page * STATUS_LIST_PAGE_ROWS;
	rowCnt = (Uint8)(state->entryCnt - firstEntry);
	if(rowCnt > STATUS_LIST_PAGE_ROWS)
		rowCnt = STATUS_LIST_PAGE_ROWS;
	mnSetPageIcons(STATUS_LIST_PAGE_ROWS, firstEntry, win, PAGE_ICONS_X, 52);
	mnPagedListDrawPageNumbers(
		win, state->entryCnt, STATUS_LIST_PAGE_ROWS, state->page, 28);
	if(menu->itemCnt == 0) {
		mnTextDrawMessageAsTilesStub(MsgItemMenuEmpty, win, EMPTY_MESSAGE_X, 8);
	} else {
		rowIdx = 0;
		if(rowCnt > rowIdx) {
			inventoryEntry = &menu->items[firstEntry];
			do {
				mnTextDrawMessageAsTilesStub(
					(*inventoryEntry & INVENTORY_ITEM_ID_MASK) + MsgItemName,
					win, ITEM_NAME_X, (rowIdx << 4) + 8);
				rowIdx = (Uint8)(rowIdx + 1);
				inventoryEntry++;
			} while(rowCnt > rowIdx);
		}
	}
	mnTextDrawStringAtOffsetStub(unit->name, win, 40, 0);
	msg = MsgStatLabel;
	mnTextDrawMessageAsTilesStub(msg, win, 32, 16);
	mnTextDrawMessageAsTilesStub(msg + 1, win, 32, 24);
	mnTextDrawNumInWindowStub(unit->attack, 3, win, STAT_X, 16);
	mnTextDrawNumInWindowStub(unit->defense, 3, win, STAT_X, 24);
	return 1;
}

#undef STATUS_LIST_PAGE_ROWS
#undef EMPTY_MESSAGE_X
#undef STAT_X

/* The equip window's width in tiles, narrower in Japanese. */
#define EQUIP_WINDOW_WIDTH JAPANESE_OR(13, 15)

/* The rows one page of the item list shows: Japanese fits six, the other
   editions five. */
#define STATUS_LIST_PAGE_ROWS JAPANESE_OR(6, 5)

/* The French list puts its cursor 4 pixels further right. */
#define STATUS_EQUIPMENT_CURSOR_X FRENCH_OR(100, 96)

/* The Status menu's equipment page: the party member's items,
   STATUS_LIST_PAGE_ROWS to a page, beside the equipment window of what they
   wear. */
Sint32 mnStatScreenRunEquipmentPage(void)
{
	ITEM_MENU_STATE *menu;
	UNIT			*owner;
	Sint32			 result;
	Sint32			 win;
	Sint32			 refresh;
	Bool			 listNeedsRedraw;
	Sint32			 navigation;
	Sint32			 ownerIdx;
	Sint32			 i;
	PAGED_LIST		 state;
	Uint16			 savedInventory[INVENTORY_SLOTS];

	menu = RamHeapSlots->menuState;
	win = menu->statWin;
	result = 0;
	refresh = 2;
	listNeedsRedraw = TRUE;
	owner = syUnitGet(menu->sideMember[0]);
	mnWindowUpdateOrCreate(
		&menu->equipWin, 0, 10, EQUIP_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
	mnSpawnIconEntries(menu, menu->questionWin);
	/* "A:Status" and "L-R:Switch characters" */
	mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp + 3, win, 0, -24);
	mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp, win, 64, -24);
	while(!syGameFlagGet(FLAG_MENU_START_PRESSED)) {
		if(refresh) {
			if(refresh == 2) {
				owner = syUnitGet(menu->sideMember[0]);
				menu->itemCnt = mnItemMenuCollectItems(owner, menu->items, 0);
				mnWindowSprRedrawSavedRectStub(win);
				for(i = 0; i < INVENTORY_SLOTS; i++)
					savedInventory[i] = owner->inventory[i];
				mnItemMenuSort(menu->items, 0);
				mnPagedListBuild(&state, 0);
				mnItemMenuDrawIcons(menu->items, 0);
				mnWindowSprRedrawSavedRectStub(menu->equipWin);
				mnItemMenuDrawEquipment(menu->equipWin, menu->sideMember[0], 1);
				listNeedsRedraw = TRUE;
				sySynch(1);
			}
			if(listNeedsRedraw) {
				listNeedsRedraw = FALSE;
				mnItemMenuDrawItemPage(win, 0, &state);
			}
			mnItemMenuDrawEquipPage(win, 0, &state);
#if EQUIPMENT_PAGE_REDRAWS_HELP
			mnTextDrawMessageAsTilesStub(
				MsgSwitchCharacterHelp + 3, win, 0, -24);
			mnTextDrawMessageAsTilesStub(MsgSwitchCharacterHelp, win, 64, -24);
#endif
			refresh = 0;
		}
		sySynch(1);
		navigation = mnPagedListHandleInput(
			0, state.entryCnt, STATUS_LIST_PAGE_ROWS, &state.row, &state.page);
		mnCursorPos(STATUS_EQUIPMENT_CURSOR_X, state.row * 16 + 52);
		if(navigation == 1) {
			refresh = 1;
			listNeedsRedraw = TRUE;
		}
		if(navigation == 0)
			refresh = 1;
		if(navigation == -1)
			refresh = 0;
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			result = 1;
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if((PerWork.repeat & KEY_R) || (PerWork.repeat & KEY_L)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			ownerIdx = menu->sideIdx[0];
			menu->selectedIdxByMember[(Uint16)menu->memberIds[ownerIdx]] =
				state.selectedIdx;
			if(PerWork.repeat & KEY_R)
				ownerIdx++;
			else
				ownerIdx--;
			for(i = 0; i < INVENTORY_SLOTS; i++)
				owner->inventory[i] = savedInventory[i];
			ownerIdx = (ownerIdx + menu->partyCnt) % menu->partyCnt;
			menu->selectedMember = (Uint16)menu->memberIds[ownerIdx];
			menu->sideMember[0] = (Uint16)menu->memberIds[ownerIdx];
			menu->sideIdx[0] = ownerIdx;
			mnReloadMemberPortrait(menu, (Uint16)menu->memberIds[ownerIdx]);
			refresh = 2;
		}
	}
	mnWindowSprPrepareForRedrawStub(menu->infoWin);
	mnWindowSprRedrawSavedRectStub(win);
	mnWindowSprClearListStub(menu->questionWin);
	return result;
}

#undef STATUS_LIST_PAGE_ROWS
#undef STATUS_EQUIPMENT_CURSOR_X

/* Writes the four equipment slot labels (Weapon, Head, Shield and Chest, one a
   16-pixel row) and the names of the items worn in them into window; mode 0
   also shows their icons at the right. */
void mnItemMenuDrawEquipment(Sint32 win, Sint32 ownerId, Sint32 mode)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Uint8			*items;

	mnHideEntryIcons();
	mnItemMenuHideAllIcons();
	mnTextDrawMessageAsTilesStub(MsgEquipSlotLabels, win, 0, 0);
	mnTextDrawMessageAsTilesStub(MsgEquipSlotLabels + 1, win, 0, 32);
	mnTextDrawMessageAsTilesStub(MsgEquipSlotLabels + 2, win, 0, 16);
	mnTextDrawMessageAsTilesStub(MsgEquipSlotLabels + 3, win, 0, 48);
	items = (Uint8 *)menu->items;
	mnItemMenuDrawEquippedItemNames(win, items);
	if(mode == 0) {
		sySynch(1);
		mnItemMenuDrawIcons((Uint16 *)items, 1);
		mnItemMenuArrangeCategoryItemIcons((Uint16 *)items);
	}
}

/* Writes the name of each equipped item of the inventory into window, below its
   slot's label: the weapon, the helm, the shield, then the armor. */
void mnItemMenuDrawEquippedItemNames(Sint32 win, Uint16 *items)
{
	Sint32 i;
	Sint32 item;

	for(i = 0; i < INVENTORY_SLOTS; i++) {
		if(items[i] & INVENTORY_EQUIPPED) {
			item = items[i] & INVENTORY_ITEM_ID_MASK;
			switch(gmItemGetDirectStub(item)->type) {
			case ITEM_TYPE_WEAPON:
				mnTextDrawMessageAsTilesStub(item + MsgItemName, win, 16, 8);
				break;
			case ITEM_TYPE_ARMOR:
				mnTextDrawMessageAsTilesStub(item + MsgItemName, win, 16, 56);
				break;
			case ITEM_TYPE_SHIELD:
				mnTextDrawMessageAsTilesStub(item + MsgItemName, win, 16, 40);
				break;
			case ITEM_TYPE_HELM:
				mnTextDrawMessageAsTilesStub(item + MsgItemName, win, 16, 24);
				break;
			}
		}
	}
}

/* Places each of the 32 entry icons on the grid. */
void mnPlaceEntryIconsInGrid(Sint32 originX, Sint32 originY, Sint32 columns)
{
	Sint32		 entryIdx;
	WINDOW_SPR	*icon;
	WINDOW_SPR **icons;

	for(entryIdx = 0,
	icons = ((ITEM_MENU_STATE *)RamHeapSlots->menuState)->entryIcons;
		entryIdx <= MENU_ENTRY_ICONS - 1; entryIdx++) {
		icon = *icons++;
		if(icon != NULL) {
			mnPlaceEntryIconInGrid(icon, entryIdx, originX, originY, columns);
		}
	}
}

/* The side of a cell of the grids mnPlaceEntryIconInGrid places icons in. */
#define ENTRY_ICON_CELL_SIZE 16

/* Places icon at cell index (an index above 31 is taken as 0) of a grid of
   ENTRY_ICON_CELL_SIZE square cells, columns wide, from (originX, originY), and
   shows it. */
void mnPlaceEntryIconInGrid(WINDOW_SPR *icon, Sint32 idx, Sint32 originX,
	Sint32 originY, Sint32 columns)
{
	Sint32 entryIdx;

	entryIdx = idx;
	if(entryIdx > MENU_ENTRY_ICONS - 1) {
		entryIdx = 0;
	}
	icon->y = (entryIdx / columns * ENTRY_ICON_CELL_SIZE) + originY;
	icon->x = (entryIdx % columns * ENTRY_ICON_CELL_SIZE) + originX;
	mnIconShow(icon);
}

/* Places the icon of each worn weapon, armor, shield and helm at x 216, on its
   slot's row of mnItemMenuDrawEquipment's window; the others stay off
   screen. */
void mnItemMenuArrangeCategoryItemIcons(Uint16 *items)
{
	ITEM_MENU_STATE *menu;
	WINDOW_SPR		*icon;
	Sint32			 slotIdx;

	menu = RamHeapSlots->menuState;
	mnHideEntryIcons();
	for(slotIdx = 0; slotIdx < INVENTORY_SLOTS; slotIdx++) {
		if(items[slotIdx] != 0 && (items[slotIdx] & INVENTORY_EQUIPPED) != 0) {
			icon = menu->entryIcons[slotIdx];
			if(icon != 0) {
				switch(
					gmItemGetDirectStub(items[slotIdx] & INVENTORY_ITEM_ID_MASK)
						->type) {
				case ITEM_TYPE_WEAPON:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(0);
					break;
				case ITEM_TYPE_ARMOR:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(3);
					break;
				case ITEM_TYPE_SHIELD:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(2);
					break;
				case ITEM_TYPE_HELM:
					icon->x = EQUIPPED_ICON_X;
					icon->y = EQUIPPED_ICON_Y(1);
					break;
				}
				mnIconShow(icon);
			}
		}
	}
}

/* Moves all 32 entry icons off screen, to (248, 168), before a screen lays them
   out again. */
void mnHideEntryIcons(void)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	Sint32			 x = MENU_ICON_HIDDEN_X;
	WINDOW_SPR	   **entry = menu->entryIcons;
	Sint32			 y = MENU_ICON_HIDDEN_Y;
	Sint32			 i;

	for(i = 0; i < MENU_ENTRY_ICONS; i++) {
		WINDOW_SPR *object = *entry++;

		if(object != 0) {
			object->x = x;
			object->y = y;
			mnIconShow(object);
		}
	}
}
