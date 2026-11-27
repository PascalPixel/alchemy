/* The battle Djinn menu. Part of screens.c. */

/* A recovering Djinni's turns left: its digits and their x in the list, in
   pixels. */
#define DJINN_RECOVERY_DIGITS JAPANESE_OR(2, 1)
#define DJINN_RECOVERY_X	  JAPANESE_OR(40, 48)

/* The party status window's layout, whose Djinn column the menu blinks. */
#define PARTY_STATUS_LAYOUT (*(PARTY_STAT_WINDOW **)PartyStatusState)

/* A battle Djinn entry packs a Djinni's DJINN_COMMAND (bits 11..8 its element,
   7..0 its number in the element, DJINN_COMMAND_INDEX), its standby bit and its
   recovery turns. */
#define BATTLE_DJINN_ENTRY_ELEMENT(entry) (((entry) & 0xf00) >> 8)
/* The Djinni is on standby or recovering, not Set. */
#define BATTLE_DJINN_ENTRY_STANDBY		  0x10000
/* bits 21..17: turns until a recovering Djinni is back; 0 when ready. */
#define BATTLE_DJINN_ENTRY_RECOVERY_MASK  0x3e0000
#define BATTLE_DJINN_ENTRY_RECOVERY_SHIFT 17
#define BATTLE_DJINN_ENTRY_END			  0x80000000 /* ends the list */

/* The battle Djinn menu for unit `member`: its Djinn in element and number
   order, five to a page, set ones plain, standby ones in TEXT_COLOR_RED and
   recovering ones in TEXT_COLOR_YELLOW with their turns, the one under the
   cursor described above the list with its change preview beside it
   (mnBattleDjinnMenuShowChangePreview; holding R shows the Psynergy pages, Left
   and Right turn them). */
Sint32 mnBattleDjinnMenuRun(Sint32 member, Sint32 unused)
{
	MENU_SPR			 cursorStorage;
	Uint16				 msgText[64];
	WIN_STATE			*state;
	WINDOW				*previewWin;
	Sint32				 drawnRow;
	Sint32				 cursorSlot;
	Uint32				*djinnEntries;
	Sint32				 djinnCnt;
	Sint32				 drawnFirstEntry;
	WINDOW				*msgWin;
	Sint32				 standbyElements;
	Sint32				 preferredRow;
	Sint32				 previewPage;
	Bool				 redraw;
	Bool				 alreadySelected;
	Sint32				 previewPages;
	BATTLE_MENU_STATE	*navigation;
	UNIT				*unit;
	DJINN_RECOVERY_LIST *list;
	WINDOW				*listWin;
	WINDOW				*iconWin;
	PARTY_STAT_WINDOW	*partyStat;
	Uint32				 selectedDjinn;
	Uint32				 blinkPhase;
	Sint32				 firstEntry;
	Sint32				 selectedRow;
	Uint32				 encodedDjinn;
	Sint32				 listPages;
	Sint32				 palette;
	MENU_POINT			 cursorPos;
	Sint32				 pressed;
	Sint32				 repeated;
	Sint32				 held;
	Sint32				 element;
	Sint32				 djinnIdx;
	Sint32				 i;
	Sint32				 j;

	state = WINDOW_STATE;
	previewWin = 0;
	drawnRow = -1;
	cursorSlot = syVRAMCacheAllocEntry(128);
	/* 84 words: room for 4 elements of 20 Djinn and the end. */
	djinnEntries = syHeapAllocEWRAM(84 * 4);
	djinnCnt = 0;
	drawnFirstEntry = -1;
	/* Default: the chosen entry, or -1, is set as the loop ends. */
	encodedDjinn = 0;
	standbyElements = 0;
	previewPage = 0;
	previewPages = 0;
	redraw = FALSE;
	alreadySelected = FALSE;
	msgWin = mnWindowCreate(0, 4, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	mnTextUseExtraTiles(1);
	listWin = mnWindowCreate(21, 9, 9, 11, MENU_WINDOW_FLAGS);
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	unit = gmUnitGetStub(member);

	/* Pack Djinn state and recovery turns before paging the selection list. */
	for(element = 0; element < ELEMENT_COUNT; element++) {
		for(djinnIdx = 0; djinnIdx < DJINN_PER_ELEMENT; djinnIdx++) {
			if(unit->djinnSet[element] & (Uint32)(1 << djinnIdx)) {
				djinnEntries[djinnCnt] =
					(Uint32)DJINN_COMMAND(element, djinnIdx);
				djinnCnt++;
			} else if(unit->djinnHeld[element] & (Uint32)(1 << djinnIdx)) {
				Uint32 v;
				Sint32 k;
				/* Owned but not set: look for its recovery in its side's
				   list. */
				list = &((DJINN_RECOVERY_TABLE *)gmDjinnGetRecoveryTableStub(
							 (Uint32)member > ALLY_MAX ? DJINN_RECOVERY_OPPONENT
													   : DJINN_RECOVERY_PARTY))
							->list;
				for(k = 0; k < list->cnt; k++) {
					if(list->entries[k].unitId == member &&
						list->entries[k].element == element &&
						list->entries[k].idx == djinnIdx) {
						break;
					}
				}
				v = (Uint32)(DJINN_COMMAND(element, djinnIdx) |
					BATTLE_DJINN_ENTRY_STANDBY);
				djinnEntries[djinnCnt] = v;
				if(list->entries[k].turns > 0) {
					v |= (Uint32)(list->entries[k].turns
						<< BATTLE_DJINN_ENTRY_RECOVERY_SHIFT);
					djinnEntries[djinnCnt] = v;
				}
				djinnCnt++;
			}
		}
	}
	djinnEntries[djinnCnt] = BATTLE_DJINN_ENTRY_END;
	state->dirty = 1;

	for(;;) {
		/* FAKEMATCH(temporary): sprite holds &cursorStorage; as &cursorStorage
		 * at each use. */
		MENU_SPR *spr = &cursorStorage;

		/* Rebuild descriptions only when the selection or preview page changes:
		   the message ("Cannot use the same Djinni twice.", "Ready to summon."
		   for a ready standby Djinni, the Djinni's effect otherwise, or "You
		   have no Djinn."), the preview, the list page, the page numbers and
		   the row's highlight. */
		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
			redraw != 0) {
			selectedDjinn = djinnEntries[firstEntry + selectedRow];
			standbyElements = 0;
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(listWin->x + 1, listWin->y + drawnRow * 2 + 1,
				listWin->width - 2, 1, WINDOW_PALETTE);
			if(previewPage != 0) {
				mnWindowClose(msgWin, 1);
				msgWin = mnWindowCreate(0, 4, 30, 4,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
						WINDOW_FLAG_BORDERED_CANVAS);
				mnTextFillCanvasBackground();
			}
			redraw = FALSE;
			if(djinnCnt != 0) {
				/* Default: 0, though the loop below sets it again. */
				Sint32 k = 0;
				mnMessageArgumentClearAll();
				alreadySelected = FALSE;
				for(k = 0; k <= 7; k++) {
					if((Uint32)BattleMenuStateWork->chosenDjinn[k] ==
						selectedDjinn) {
						alreadySelected = TRUE;
						break;
					}
				}
				if(alreadySelected != 0) {
					BATTLE_MENU_MESSAGE_COPY(MsgBattleDjinnTwice, msgText,
						BATTLE_MENU_MESSAGE_CAPACITY);
					if(previewWin != 0) {
						mnWindowClose(previewWin, 1);
						previewWin = 0;
						previewPage = 0;
					}
				} else if(selectedDjinn & BATTLE_DJINN_ENTRY_STANDBY) {
					/* A recovering Djinni: its effect, with the turns as the
					   argument (MESSAGE_ARGUMENT_NUMBER), and no preview. */
					if(selectedDjinn & BATTLE_DJINN_ENTRY_RECOVERY_MASK) {
						mnMessageArgumentPush(
							(Sint32)((selectedDjinn &
										 BATTLE_DJINN_ENTRY_RECOVERY_MASK) >>
								BATTLE_DJINN_ENTRY_RECOVERY_SHIFT),
							MESSAGE_ARGUMENT_NUMBER);
						BATTLE_MENU_MESSAGE_COPY(
							(Sint32)(BATTLE_DJINN_ENTRY_ELEMENT(selectedDjinn) *
									DJINN_PER_ELEMENT +
								DJINN_COMMAND_INDEX(selectedDjinn)) +
								MsgDjinnEffect,
							msgText, BATTLE_MENU_MESSAGE_CAPACITY);
						if(previewWin != 0) {
							mnWindowClose(previewWin, 1);
							previewWin = 0;
							previewPage = 0;
						}
					} else {
						previewWin =
							(WINDOW *)mnBattleDjinnMenuShowChangePreview(
								previewWin, member, (Sint32)selectedDjinn,
								previewPage, &previewPages);
						mnMessageCopyExpanded(MsgBattleSummonStandby, msgText,
							BATTLE_MENU_MESSAGE_CAPACITY);
						standbyElements = 1
							<< BATTLE_DJINN_ENTRY_ELEMENT(selectedDjinn);
					}
				} else {
					previewWin = (WINDOW *)mnBattleDjinnMenuShowChangePreview(
						previewWin, member, (Sint32)selectedDjinn, previewPage,
						&previewPages);
					BATTLE_MENU_MESSAGE_COPY(
						(Sint32)(BATTLE_DJINN_ENTRY_ELEMENT(selectedDjinn) *
								DJINN_PER_ELEMENT +
							DJINN_COMMAND_INDEX(selectedDjinn)) +
							MsgDjinnEffect,
						msgText, BATTLE_MENU_MESSAGE_CAPACITY);
				}
			} else {
				BATTLE_MENU_MESSAGE_COPY(
					MsgBattleNoDjinn, msgText, BATTLE_MENU_MESSAGE_CAPACITY);
			}
			state->holdTilemapUpload = 0;
			if(previewPage == 0) {
				state->holdTilemapUpload = 1;
				mnWindowClose(msgWin, 1);
				msgWin = mnWindowCreate(0, 4, 30, 4,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
						WINDOW_FLAG_BORDERED_CANVAS);
				state->holdTilemapUpload = (Uint8)previewPage;
			}
			mnTextDrawWideStringAtOffset(msgText, msgWin, 0, 4);
			drawnRow = selectedRow;
			if(firstEntry != drawnFirstEntry) {
				mnWindowSprRedrawSavedRect(listWin);
				j = 0;
				while((encodedDjinn = djinnEntries[firstEntry + j]) !=
					BATTLE_DJINN_ENTRY_END) {
					mnWindowSetTilemapEntry(listWin,
						(Sint32)BATTLE_DJINN_ENTRY_ELEMENT(encodedDjinn) +
							LIST_TILE_SUMMON_ELEMENT,
						0, j * 2, 0);
					if(encodedDjinn & BATTLE_DJINN_ENTRY_RECOVERY_MASK) {
						mnTextSetCol(TEXT_COLOR_YELLOW);
					} else if(encodedDjinn & BATTLE_DJINN_ENTRY_STANDBY) {
						mnTextSetCol(TEXT_COLOR_RED);
					}
					mnTextDrawMessageAsTiles(
						(Sint32)(BATTLE_DJINN_ENTRY_ELEMENT(encodedDjinn) *
								DJINN_PER_ELEMENT +
							DJINN_COMMAND_INDEX(encodedDjinn)) +
							MsgDjinnName,
						listWin, 8, j * 16);
					if(encodedDjinn & BATTLE_DJINN_ENTRY_RECOVERY_MASK) {
						mnTextDrawNumAtOffset(
							(Sint32)((encodedDjinn &
										 BATTLE_DJINN_ENTRY_RECOVERY_MASK) >>
								BATTLE_DJINN_ENTRY_RECOVERY_SHIFT),
							DJINN_RECOVERY_DIGITS, listWin, DJINN_RECOVERY_X,
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
			if(djinnCnt > BATTLE_LIST_ROWS) {
				for(j = 0; j < (listPages = ((djinnCnt + BATTLE_LIST_ROWS - 1) /
									BATTLE_LIST_ROWS));
					j++) {
					Sint32 tile = j + PAGE_NUMBER_TILE;
					if(j == (firstEntry / BATTLE_LIST_ROWS)) {
						tile = j + PAGE_NUMBER_LIT_TILE;
					}
					mnWindowSetTilemapEntry(listWin, tile,
						listWin->width - listPages + j - 2, -1, 0);
				}
			}
			mnWindowHighlightRect(listWin->x + 1,
				listWin->y + selectedRow * 2 + 1, listWin->width - 2, 1,
				HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		/* Each tick: the current page's number lit 12 ticks in 16 (steady
		   while R is held), and the page arrows, which give way to the plain
		   tick while R turns the preview's pages instead. */
		if(djinnCnt > BATTLE_LIST_ROWS) {
			for(j = 0; j < (djinnCnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
				j++) {
				Sint32 tile = j + PAGE_NUMBER_TILE;
				if((PerData & KEY_R) || (TickCount & 15) <= 11) {
					if(j == (firstEntry / BATTLE_LIST_ROWS)) {
						tile = j + PAGE_NUMBER_LIT_TILE;
					}
				}
				mnWindowSetTilemapEntry(listWin, tile,
					listWin->width -
						((djinnCnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS) +
						j - 2,
					-1, 0);
			}
			if((PerData & KEY_R) == 0) {
				mnWindowSetTilemapEntry(listWin, LIST_TILE_PAGES_LEFT,
					listWin->width -
						(djinnCnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS -
						3,
					-1, 0);
				mnWindowSetTilemapEntry(
					listWin, LIST_TILE_PAGES_RIGHT, listWin->width - 2, -1, 0);
			} else {
				mnWindowSetTilemapEntry(listWin, WINDOW_TILE_BORDER_TOP,
					listWin->width -
						(djinnCnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS -
						3,
					-1, 0);
				mnWindowSetTilemapEntry(listWin, WINDOW_TILE_BORDER_TOP_RIGHT,
					listWin->width - 2, -1, 0);
			}
			mnWindowMarkTilemapRow(state, listWin->y - 1);
		}

		/* Tutorial input replaces live keys but uses the same navigation
		   below. */
		pressed = PerPush;
		repeated = PerRepeat;
		held = PerData;
		navigation = BattleMenuStateWork;
		if(navigation->tutorialStep != 0) {
			repeated = 0;
			pressed = 0;
			held = 0;
			if(navigation->tutorialDelay == 0) {
				navigation->tutorialDelay = 60;
				repeated = KEY_A;
				pressed = KEY_A;
			} else {
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
		   one is refused with "Your Djinni is still recovering.". */
		if(pressed & KEY_A) {
			Uint32 selection;
			Uint32 turns;
			if(djinnCnt != 0) {
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
				} else if(alreadySelected == 0) {
					mnTextFillCanvasBackground();
					mnMessageArgumentClearAll();
					mnMessageArgumentPush(
						(Sint32)((turns & BATTLE_DJINN_ENTRY_RECOVERY_MASK) >>
							BATTLE_DJINN_ENTRY_RECOVERY_SHIFT),
						MESSAGE_ARGUMENT_NUMBER);
					mnMessageCopyExpanded(MsgBattleDjinnRecovering, msgText,
						BATTLE_MENU_MESSAGE_CAPACITY);
					mnTextDrawWideStringAtOffset(msgText, msgWin, 0, 4);
				}
				sySndPlayStub(SE_MENU_ERROR);
			} else {
				encodedDjinn = -1;
				break;
			}
		}

		if(djinnCnt != 0) {
			if(repeated & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow++;
				if(selectedRow == BATTLE_LIST_ROWS ||
					firstEntry + selectedRow == djinnCnt) {
					selectedRow = 0;
				}
				preferredRow = selectedRow;
			} else if(repeated & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow--;
				if(selectedRow < 0) {
					if(firstEntry ==
						((djinnCnt - 1) / BATTLE_LIST_ROWS) *
							BATTLE_LIST_ROWS) {
						selectedRow = djinnCnt - firstEntry - 1;
					} else {
						selectedRow = BATTLE_LIST_ROWS - 1;
					}
				}
				preferredRow = selectedRow;
			} else if(held & KEY_R) {
				/* R held: the preview's page numbers and arrows on its top
				   edge; the first tick turns to page 1, then Left and Right
				   turn the pages. */
				if(previewWin != 0) {
					for(i = 0; i < previewPages; i++) {
						Sint32 tile = i + PAGE_NUMBER_TILE;
						if((TickCount & 15) <= 11 && i == previewPage - 1) {
							tile = previewPage - 1 + PAGE_NUMBER_LIT_TILE;
						}
						mnWindowSetTilemapEntry(previewWin, tile,
							previewWin->width - previewPages + i - 2, -1, 0);
					}
					mnWindowSetTilemapEntry(previewWin, LIST_TILE_PAGES_LEFT,
						previewWin->width - previewPages - 3, -1, 0);
					mnWindowSetTilemapEntry(previewWin, LIST_TILE_PAGES_RIGHT,
						previewWin->width - 2, -1, 0);
					mnWindowMarkTilemapRow(state, previewWin->y);
				}
				if(previewPage == 0 && previewPages != 0) {
					if(previewWin != 0) {
						mnWindowSprClearList(previewWin);
					}
					previewPage = 1;
					redraw = TRUE;
				} else {
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
							redraw = TRUE;
						} else if(repeated & KEY_LEFT) {
							sySndPlayStub(SE_MENU_CURSOR_MOVE);
							previewPage--;
							if(previewPage <= 0) {
								previewPage = previewPages;
							}
							redraw = TRUE;
						}
					}
				}
			} else if(previewPage != 0) {
				/* R let go: back to the stats page. */
				if(previewWin != 0) {
					mnWindowSprClearList(previewWin);
				}
				previewPage = 0;
				redraw = TRUE;
			} else if(repeated & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry + BATTLE_LIST_ROWS >= djinnCnt) {
					if(firstEntry != 0) {
						firstEntry = 0;
						selectedRow = preferredRow;
					}
				} else {
					firstEntry += BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry ==
						((djinnCnt - 1) / BATTLE_LIST_ROWS) *
							BATTLE_LIST_ROWS) {
						selectedRow = djinnCnt - firstEntry - 1;
						if(selectedRow > preferredRow) {
							selectedRow = preferredRow;
						}
					}
				}
			} else if(repeated & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry != 0) {
					firstEntry -= BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
				} else {
					firstEntry =
						((djinnCnt - 1) / BATTLE_LIST_ROWS) * BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry != 0) {
						selectedRow = djinnCnt - firstEntry - 1;
						if(selectedRow > preferredRow) {
							selectedRow = preferredRow;
						}
					}
				}
			}
		}

		MENU_PLACE_CURSOR(cursorPos, listWin, selectedRow, 2, spr);
		spr->oam.f.tile =
			syVRAMCacheReload(cursorSlot, (Sint32)PointingHandTiles);
		spr->oam.f.x = cursorPos.x + ((TickCount & 4) >> 1) - 6;
		spr->oam.f.y = cursorPos.y - ((TickCount & 4) >> 2) - 8;
		if(djinnCnt != 0) {
			syOAMAddSprAtDepth(spr, OAM_DEPTH_WINDOW + 2);
		}
		/* In the party status window's Djinn column (PARTY_STATUS_SHOW_DJINN),
		   the element of a ready standby Djinni under the cursor blinks between
		   HIGHLIGHT_PALETTE and WINDOW_PALETTE every four ticks; then the
		   arrow tiles' blink. */
		partyStat = PARTY_STATUS_LAYOUT;
		blinkPhase = TickCount & 4;
		iconWin = partyStat->win;
		if(partyStat->flg & PARTY_STATUS_SHOW_DJINN) {
			for(i = 0; i < ELEMENT_COUNT; i++) {
				palette = blinkPhase != 0 ? HIGHLIGHT_PALETTE : WINDOW_PALETTE;
				if(((1 << i) & standbyElements) == 0) {
					palette = WINDOW_PALETTE;
				}
				mnWindowHighlightRect(
					iconWin->x + DjinnMenuElementXOffsets[i] + 1,
					iconWin->y + DjinnMenuElementYOffsets[i] + 1, 2, 2,
					palette);
			}
		}
		if(TickCount & 4) {
			syWordsCopy((void *)DJINN_MENU_ARROW_VRAM, DjinnMenu_BlinkTile,
				TILE_SIZE_4BPP);
			syWordsCopy((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
				DjinnMenu_BlinkTile, TILE_SIZE_4BPP);
		} else {
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
			PARTY_STATUS_LAYOUT->win->y + 1, 4, 4, WINDOW_PALETTE);
	}
	/* Restore the party strip before releasing the Djinn menu's resources. */
	syVRAMCacheFreeEntry(cursorSlot);
	mnWindowClose(msgWin, 1);
	mnWindowClose(previewWin, 1);
	mnWindowClose(listWin, 1);
	mnWindowMarkVisibleTextTiles();
	mnTextUseExtraTiles(0);
	syHeapRelease(djinnEntries);
	sySynch(1);
	return (Sint32)encodedDjinn;
}
