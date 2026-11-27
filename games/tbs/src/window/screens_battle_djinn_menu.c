/* @file */
/* The battle Djinn menu. */
/* Part of screens.c. */

/* A recovering Djinni's turns left: its digits and their x in the list, in */
/* pixels. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define DJINN_RECOVERY_DIGITS 1
#define DJINN_RECOVERY_X	  48
#else
#define DJINN_RECOVERY_DIGITS 2
#define DJINN_RECOVERY_X	  40
#endif

/* The party status window's layout, whose Djinn column the menu blinks. */
#define PARTY_STATUS_LAYOUT (*(PARTY_STAT_WINDOW **)PartyStatusState)

/* A battle Djinn entry packs a Djinni's DJINN_COMMAND (bits 11..8 its element,
 */
/* 7..0 its number in the element, DJINN_COMMAND_INDEX), its standby bit and */
/* its recovery turns. */
#define BATTLE_DJINN_ENTRY_ELEMENT(entry) (((entry) & 0xf00) >> 8)
/* The Djinni is on standby or recovering, not Set. */
#define BATTLE_DJINN_ENTRY_STANDBY 0x10000
/* bits 21..17: turns until a recovering Djinni is back; 0 when ready. */
#define BATTLE_DJINN_ENTRY_RECOVERY_MASK  0x3e0000
#define BATTLE_DJINN_ENTRY_RECOVERY_SHIFT 17
#define BATTLE_DJINN_ENTRY_END			  0x80000000 /* ends the list */

/* The battle Djinn menu for unit `member`: its Djinn in element and number */
/* order, five to a page, set ones plain, standby ones in TEXT_COLOR_RED and */
/* recovering ones in TEXT_COLOR_YELLOW with their turns, the one under the */
/* cursor described above the list with its change preview beside it */
/* (mnBattleDjinnMenuShowChangePreview; holding R shows the Psynergy pages, */
/* Left and Right turn them). */
s32 mnBattleDjinnMenuRun(s32 member, s32 unused) {
	MENU_SPR cursorStorage;
	u16 messageText[64];
	WIN_STATE *state;
	WINDOW *previewWindow;
	s32 drawnRow;
	s32 cursorSlot;
	u32 *djinnEntries;
	s32 djinnCount;
	s32 drawnFirstEntry;
	WINDOW *messageWindow;
	s32 standbyElements;
	s32 preferredRow;
	s32 previewPage;
	s32 redraw;
	s32 alreadySelected;
	s32 previewPages;
	BATTLE_MENU_STATE *navigation;
	UNIT *unit;
	DJINN_RECOVERY_LIST *list;
	WINDOW *listWindow;
	WINDOW *iconWindow;
	PARTY_STAT_WINDOW *partyStatus;
	u32 selectedDjinn;
	u32 blinkPhase;
	s32 firstEntry;
	s32 selectedRow;
	u32 encodedDjinn;
	s32 listPages;
	s32 palette;
	MENU_POINT cursorPos;
	s32 pressed;
	s32 repeated;
	s32 held;
	s32 element;
	s32 djinnIndex;
	s32 i;
	s32 j;

	state = WINDOW_STATE;
	previewWindow = 0;
	drawnRow = -1;
	cursorSlot = syVramCacheAllocEntry(128);
	/* 84 words: room for 4 elements of 20 Djinn and the end. */
	djinnEntries = syHeapAllocEwram(84 * 4);
	djinnCount = 0;
	drawnFirstEntry = -1;
	/* Default: the chosen entry, or -1, is set as the loop ends. */
	encodedDjinn = 0;
	standbyElements = 0;
	previewPage = 0;
	previewPages = 0;
	redraw = 0;
	alreadySelected = 0;
	messageWindow =
		mnWindowCreate(0, 4, 30, 4,
					   WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
						   WINDOW_FLAG_BORDERED_CANVAS);
	mnTextUseExtraTiles(1);
	listWindow = mnWindowCreate(21, 9, 9, 11, MENU_WINDOW_FLAGS);
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	unit = gmUnitGetStub(member);

	/* Pack Djinn state and recovery turns before paging the selection list. */
	for(element = 0; element < ELEMENT_COUNT; element++) {
		for(djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
			if(unit->djinnSet[element] & (u32)(1 << djinnIndex)) {
				djinnEntries[djinnCount] =
					(u32)DJINN_COMMAND(element, djinnIndex);
				djinnCount++;
			}
			else if(unit->djinnHeld[element] & (u32)(1 << djinnIndex)) {
				u32 v;
				s32 k;
				/* Owned but not set: look for its recovery in its side's list.
				 */
				list = &((DJINN_RECOVERY_TABLE *)gmDjinnGetRecoveryTableStub(
							 (u32)member > ALLY_MAX ? DJINN_RECOVERY_OPPONENT
													: DJINN_RECOVERY_PARTY))
							->list;
				for(k = 0; k < list->cnt; k++) {
					if(list->entries[k].unitId == member &&
					   list->entries[k].element == element &&
					   list->entries[k].idx == djinnIndex) {
						break;
					}
				}
				v = (u32)(DJINN_COMMAND(element, djinnIndex) |
						  BATTLE_DJINN_ENTRY_STANDBY);
				djinnEntries[djinnCount] = v;
				if(list->entries[k].turns > 0) {
					v |= (u32)(list->entries[k].turns
							   << BATTLE_DJINN_ENTRY_RECOVERY_SHIFT);
					djinnEntries[djinnCount] = v;
				}
				djinnCount++;
			}
		}
	}
	djinnEntries[djinnCount] = BATTLE_DJINN_ENTRY_END;
	state->dirty = 1;

	for(;;) {
		/* FAKEMATCH(temporary): sprite holds &cursorStorage; as &cursorStorage
		 * at each use. */
		MENU_SPR *sprite = &cursorStorage;

		/* Rebuild descriptions only when the selection or preview page changes:
		 */
		/* the message ("Cannot use the same Djinni twice.", "Ready to summon."
		 */
		/* for a ready standby Djinni, the Djinni's effect otherwise, or "You
		 * have */
		/* no Djinn."), the preview, the list page, the page numbers and the
		 * row's */
		/* highlight. */
		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
		   redraw != 0) {
			selectedDjinn = djinnEntries[firstEntry + selectedRow];
			standbyElements = 0;
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(listWindow->x + 1,
								  listWindow->y + drawnRow * 2 + 1,
								  listWindow->width - 2, 1, WINDOW_PALETTE);
			if(previewPage != 0) {
				mnWindowClose(messageWindow, 1);
				messageWindow = mnWindowCreate(0, 4, 30, 4,
											   WINDOW_FLAG_INSTANT |
												   WINDOW_FLAG_TEXT_CANVAS |
												   WINDOW_FLAG_BORDERED_CANVAS);
				mnTextFillCanvasBackground();
			}
			redraw = 0;
			if(djinnCount != 0) {
				/* Default: 0, though the loop below sets it again. */
				s32 k = 0;
				mnMessageArgumentClearAll();
				alreadySelected = 0;
				for(k = 0; k <= 7; k++) {
					if((u32)BattleMenuStateWork->chosenDjinn[k] ==
					   selectedDjinn) {
						alreadySelected = 1;
						break;
					}
				}
				if(alreadySelected != 0) {
					BATTLE_MENU_MESSAGE_COPY(MsgBattleDjinnTwice, messageText,
											 BATTLE_MENU_MESSAGE_CAPACITY);
					if(previewWindow != 0) {
						mnWindowClose(previewWindow, 1);
						previewWindow = 0;
						previewPage = 0;
					}
				}
				else if(selectedDjinn & BATTLE_DJINN_ENTRY_STANDBY) {
					/* A recovering Djinni: its effect, with the turns as the
					 * argument */
					/* (MESSAGE_ARGUMENT_NUMBER), and no preview. */
					if(selectedDjinn & BATTLE_DJINN_ENTRY_RECOVERY_MASK) {
						mnMessageArgumentPush(
							(s32)((selectedDjinn &
								   BATTLE_DJINN_ENTRY_RECOVERY_MASK) >>
								  BATTLE_DJINN_ENTRY_RECOVERY_SHIFT),
							MESSAGE_ARGUMENT_NUMBER);
						BATTLE_MENU_MESSAGE_COPY(
							(s32)(BATTLE_DJINN_ENTRY_ELEMENT(selectedDjinn) *
									  DJINN_PER_ELEMENT +
								  DJINN_COMMAND_INDEX(selectedDjinn)) +
								MsgDjinnEffect,
							messageText, BATTLE_MENU_MESSAGE_CAPACITY);
						if(previewWindow != 0) {
							mnWindowClose(previewWindow, 1);
							previewWindow = 0;
							previewPage = 0;
						}
					}
					else {
						previewWindow =
							(WINDOW *)mnBattleDjinnMenuShowChangePreview(
								previewWindow, member, (s32)selectedDjinn,
								previewPage, &previewPages);
						mnMessageCopyExpanded(MsgBattleSummonStandby,
											  messageText,
											  BATTLE_MENU_MESSAGE_CAPACITY);
						standbyElements =
							1 << BATTLE_DJINN_ENTRY_ELEMENT(selectedDjinn);
					}
				}
				else {
					previewWindow =
						(WINDOW *)mnBattleDjinnMenuShowChangePreview(
							previewWindow, member, (s32)selectedDjinn,
							previewPage, &previewPages);
					BATTLE_MENU_MESSAGE_COPY(
						(s32)(BATTLE_DJINN_ENTRY_ELEMENT(selectedDjinn) *
								  DJINN_PER_ELEMENT +
							  DJINN_COMMAND_INDEX(selectedDjinn)) +
							MsgDjinnEffect,
						messageText, BATTLE_MENU_MESSAGE_CAPACITY);
				}
			}
			else {
				BATTLE_MENU_MESSAGE_COPY(MsgBattleNoDjinn, messageText,
										 BATTLE_MENU_MESSAGE_CAPACITY);
			}
			state->holdTilemapUpload = 0;
			if(previewPage == 0) {
				state->holdTilemapUpload = 1;
				mnWindowClose(messageWindow, 1);
				messageWindow = mnWindowCreate(0, 4, 30, 4,
											   WINDOW_FLAG_INSTANT |
												   WINDOW_FLAG_TEXT_CANVAS |
												   WINDOW_FLAG_BORDERED_CANVAS);
				state->holdTilemapUpload = (u8)previewPage;
			}
			mnTextDrawWideStringAtOffset(messageText, messageWindow, 0, 4);
			drawnRow = selectedRow;
			if(firstEntry != drawnFirstEntry) {
				mnWindowSprRedrawSavedRect(listWindow);
				j = 0;
				while((encodedDjinn = djinnEntries[firstEntry + j]) !=
					  BATTLE_DJINN_ENTRY_END) {
					mnWindowSetTilemapEntry(
						listWindow,
						(s32)BATTLE_DJINN_ENTRY_ELEMENT(encodedDjinn) +
							LIST_TILE_SUMMON_ELEMENT,
						0, j * 2, 0);
					if(encodedDjinn & BATTLE_DJINN_ENTRY_RECOVERY_MASK) {
						mnTextSetCol(TEXT_COLOR_YELLOW);
					}
					else if(encodedDjinn & BATTLE_DJINN_ENTRY_STANDBY) {
						mnTextSetCol(TEXT_COLOR_RED);
					}
					mnTextDrawMessageAsTiles(
						(s32)(BATTLE_DJINN_ENTRY_ELEMENT(encodedDjinn) *
								  DJINN_PER_ELEMENT +
							  DJINN_COMMAND_INDEX(encodedDjinn)) +
							MsgDjinnName,
						listWindow, 8, j * 16);
					if(encodedDjinn & BATTLE_DJINN_ENTRY_RECOVERY_MASK) {
						mnTextDrawNumAtOffset(
							(s32)((encodedDjinn &
								   BATTLE_DJINN_ENTRY_RECOVERY_MASK) >>
								  BATTLE_DJINN_ENTRY_RECOVERY_SHIFT),
							DJINN_RECOVERY_DIGITS, listWindow, DJINN_RECOVERY_X,
							j * 16);
					}
					mnTextSetCol(TEXT_COLOR_WHITE);
					j++;
					if(j >= BATTLE_LIST_ROWS) {
						break;
					}
				}
				drawnFirstEntry = firstEntry;
			}
			if(djinnCount > BATTLE_LIST_ROWS) {
				for(j = 0;
					j < (listPages = ((djinnCount + BATTLE_LIST_ROWS - 1) /
									  BATTLE_LIST_ROWS));
					j++) {
					s32 tile = j + PAGE_NUMBER_TILE;
					if(j == (firstEntry / BATTLE_LIST_ROWS)) {
						tile = j + PAGE_NUMBER_LIT_TILE;
					}
					mnWindowSetTilemapEntry(
						listWindow, tile, listWindow->width - listPages + j - 2,
						-1, 0);
				}
			}
			mnWindowHighlightRect(listWindow->x + 1,
								  listWindow->y + selectedRow * 2 + 1,
								  listWindow->width - 2, 1, HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		/* Each frame: the current page's number lit 12 frames in 16 (steady
		 * while */
		/* R is held), and the page arrows, which give way to the plain frame */
		/* while R turns the preview's pages instead. */
		if(djinnCount > BATTLE_LIST_ROWS) {
			for(j = 0;
				j < (djinnCount + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
				j++) {
				s32 tile = j + PAGE_NUMBER_TILE;
				if((KeysHeld & KEY_R) || (FrameCount & 15) <= 11) {
					if(j == (firstEntry / BATTLE_LIST_ROWS)) {
						tile = j + PAGE_NUMBER_LIT_TILE;
					}
				}
				mnWindowSetTilemapEntry(
					listWindow, tile,
					listWindow->width -
						((djinnCount + BATTLE_LIST_ROWS - 1) /
						 BATTLE_LIST_ROWS) +
						j - 2,
					-1, 0);
			}
			if((KeysHeld & KEY_R) == 0) {
				mnWindowSetTilemapEntry(
					listWindow, LIST_TILE_PAGES_LEFT,
					listWindow->width -
						(djinnCount + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS -
						3,
					-1, 0);
				mnWindowSetTilemapEntry(listWindow, LIST_TILE_PAGES_RIGHT,
										listWindow->width - 2, -1, 0);
			}
			else {
				mnWindowSetTilemapEntry(
					listWindow, WINDOW_TILE_BORDER_TOP,
					listWindow->width -
						(djinnCount + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS -
						3,
					-1, 0);
				mnWindowSetTilemapEntry(listWindow,
										WINDOW_TILE_BORDER_TOP_RIGHT,
										listWindow->width - 2, -1, 0);
			}
			mnWindowMarkTilemapRow(state, listWindow->y - 1);
		}

		/* Tutorial input replaces live keys but uses the same navigation below.
		 */
		pressed = KeysPressed;
		repeated = KeysRepeat;
		held = KeysHeld;
		navigation = BattleMenuStateWork;
		if(navigation->tutorialStep != 0) {
			repeated = 0;
			pressed = 0;
			held = 0;
			if(navigation->tutorialDelay == 0) {
				navigation->tutorialDelay = 60;
				repeated = KEY_A;
				pressed = KEY_A;
			}
			else {
				navigation->tutorialDelay--;
			}
		}
		navigation = BattleMenuStateWork;
		if(navigation->tmr == 0 || (pressed & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			encodedDjinn = -1;
			break;
		}
		/* A takes a Djinni that is ready and not chosen already; a recovering
		 */
		/* one is refused with "Your Djinni is still recovering.". */
		if(pressed & KEY_A) {
			u32 selection;
			u32 turns;
			if(djinnCount != 0) {
				selection = djinnEntries[firstEntry + selectedRow];
				turns = selection & BATTLE_DJINN_ENTRY_RECOVERY_MASK;
				if(turns == 0) {
					if(alreadySelected == 0) {
						navigation->page = firstEntry;
						navigation->row = selectedRow;
						navigation->preferredRow = preferredRow;
						encodedDjinn = selection;
						break;
					}
				}
				else if(alreadySelected == 0) {
					mnTextFillCanvasBackground();
					mnMessageArgumentClearAll();
					mnMessageArgumentPush(
						(s32)((turns & BATTLE_DJINN_ENTRY_RECOVERY_MASK) >>
							  BATTLE_DJINN_ENTRY_RECOVERY_SHIFT),
						MESSAGE_ARGUMENT_NUMBER);
					mnMessageCopyExpanded(MsgBattleDjinnRecovering, messageText,
										  BATTLE_MENU_MESSAGE_CAPACITY);
					mnTextDrawWideStringAtOffset(messageText, messageWindow, 0,
												 4);
				}
				sySndPlayStub(SE_MENU_ERROR);
			}
			else {
				encodedDjinn = -1;
				break;
			}
		}

		if(djinnCount != 0) {
			if(repeated & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow++;
				if(selectedRow == BATTLE_LIST_ROWS ||
				   firstEntry + selectedRow == djinnCount) {
					selectedRow = 0;
				}
				preferredRow = selectedRow;
			}
			else if(repeated & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow--;
				if(selectedRow < 0) {
					if(firstEntry == ((djinnCount - 1) / BATTLE_LIST_ROWS) *
										 BATTLE_LIST_ROWS) {
						selectedRow = djinnCount - firstEntry - 1;
					}
					else {
						selectedRow = BATTLE_LIST_ROWS - 1;
					}
				}
				preferredRow = selectedRow;
			}
			else if(held & KEY_R) {
				/* R held: the preview's page numbers and arrows on its top
				 * edge; the */
				/* first frame turns to page 1, then Left and Right turn the
				 * pages. */
				if(previewWindow != 0) {
					for(i = 0; i < previewPages; i++) {
						s32 tile = i + PAGE_NUMBER_TILE;
						if((FrameCount & 15) <= 11 && i == previewPage - 1) {
							tile = previewPage - 1 + PAGE_NUMBER_LIT_TILE;
						}
						mnWindowSetTilemapEntry(
							previewWindow, tile,
							previewWindow->width - previewPages + i - 2, -1, 0);
					}
					mnWindowSetTilemapEntry(
						previewWindow, LIST_TILE_PAGES_LEFT,
						previewWindow->width - previewPages - 3, -1, 0);
					mnWindowSetTilemapEntry(previewWindow,
											LIST_TILE_PAGES_RIGHT,
											previewWindow->width - 2, -1, 0);
					mnWindowMarkTilemapRow(state, previewWindow->y);
				}
				if(previewPage == 0 && previewPages != 0) {
					if(previewWindow != 0) {
						mnWindowSprClearList(previewWindow);
					}
					previewPage = 1;
					redraw = 1;
				}
				else {
					if(previewPage > previewPages) {
						previewPage = previewPages;
					}
					if(previewPage != 0) {
						if(repeated & KEY_RIGHT) {
							sySndPlayStub(SE_MENU_CURSOR_MOVE);
							previewPage++;
							if(previewPage > previewPages) {
								previewPage = 1;
							}
							redraw = 1;
						}
						else if(repeated & KEY_LEFT) {
							sySndPlayStub(SE_MENU_CURSOR_MOVE);
							previewPage--;
							if(previewPage <= 0) {
								previewPage = previewPages;
							}
							redraw = 1;
						}
					}
				}
			}
			else if(previewPage != 0) {
				/* R let go: back to the stats page. */
				if(previewWindow != 0) {
					mnWindowSprClearList(previewWindow);
				}
				previewPage = 0;
				redraw = 1;
			}
			else if(repeated & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry + BATTLE_LIST_ROWS >= djinnCount) {
					if(firstEntry != 0) {
						firstEntry = 0;
						selectedRow = preferredRow;
					}
				}
				else {
					firstEntry += BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry == ((djinnCount - 1) / BATTLE_LIST_ROWS) *
										 BATTLE_LIST_ROWS) {
						selectedRow = djinnCount - firstEntry - 1;
						if(selectedRow > preferredRow) {
							selectedRow = preferredRow;
						}
					}
				}
			}
			else if(repeated & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry != 0) {
					firstEntry -= BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
				}
				else {
					firstEntry = ((djinnCount - 1) / BATTLE_LIST_ROWS) *
								 BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry != 0) {
						selectedRow = djinnCount - firstEntry - 1;
						if(selectedRow > preferredRow) {
							selectedRow = preferredRow;
						}
					}
				}
			}
		}

		MENU_PLACE_CURSOR(cursorPos, listWindow, selectedRow, 2, sprite);
		sprite->oam.f.tile =
			syVramCacheReload(cursorSlot, (s32)PointingHandTiles);
		sprite->oam.f.x = cursorPos.x + ((FrameCount & 4) >> 1) - 6;
		sprite->oam.f.y = cursorPos.y - ((FrameCount & 4) >> 2) - 8;
		if(djinnCount != 0) {
			syOamAddSprAtDepth(sprite, OAM_DEPTH_WINDOW + 2);
		}
		/* In the party status window's Djinn column (PARTY_STATUS_SHOW_DJINN),
		 */
		/* the element of a ready standby Djinni under the cursor blinks between
		 */
		/* HIGHLIGHT_PALETTE and WINDOW_PALETTE every four frames; then the
		 * arrow */
		/* tiles' blink. */
		partyStatus = PARTY_STATUS_LAYOUT;
		blinkPhase = FrameCount & 4;
		iconWindow = partyStatus->win;
		if(partyStatus->flg & PARTY_STATUS_SHOW_DJINN) {
			for(i = 0; i < ELEMENT_COUNT; i++) {
				palette = blinkPhase != 0 ? HIGHLIGHT_PALETTE : WINDOW_PALETTE;
				if(((1 << i) & standbyElements) == 0) {
					palette = WINDOW_PALETTE;
				}
				mnWindowHighlightRect(
					iconWindow->x + DjinnMenu_ElementXOffsets[i] + 1,
					iconWindow->y + DjinnMenu_ElementYOffsets[i] + 1, 2, 2,
					palette);
			}
		}
		if(FrameCount & 4) {
			syWordsCopy((void *)DJINN_MENU_ARROW_VRAM, DjinnMenu_BlinkTile,
						TILE_SIZE_4BPP);
			syWordsCopy((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
						DjinnMenu_BlinkTile, TILE_SIZE_4BPP);
		}
		else {
			syWordsFill((void *)DJINN_MENU_ARROW_VRAM, TILE_SIZE_4BPP,
						TEXT_CANVAS_BACKGROUND_WORD);
			syWordsCopy((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
						DjinnMenu_BlinkTile, TILE_SIZE_4BPP);
		}
		sySynch(1);
	}

	/* Leave the Djinn column in WINDOW_PALETTE. */
	if(PARTY_STATUS_LAYOUT->flg & PARTY_STATUS_SHOW_DJINN) {
		mnWindowHighlightRect(PARTY_STATUS_LAYOUT->win->x + 1,
							  PARTY_STATUS_LAYOUT->win->y + 1, 4, 4,
							  WINDOW_PALETTE);
	}
	/* Restore the party strip before releasing the Djinn menu's resources. */
	syVramCacheFreeEntry(cursorSlot);
	mnWindowClose(messageWindow, 1);
	mnWindowClose(previewWindow, 1);
	mnWindowClose(listWindow, 1);
	mnWindowMarkVisibleTextTiles();
	mnTextUseExtraTiles(0);
	syHeapRelease(djinnEntries);
	sySynch(1);
	return (s32)encodedDjinn;
}
