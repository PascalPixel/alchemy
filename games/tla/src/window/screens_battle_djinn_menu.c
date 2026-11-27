/* The battle Djinn menu. Part of screens.c. */

/* The Japanese list window is 9 tiles wide from column 21 and a recovering
   Djinni's turns stand at x 40. */
#define DJINN_LIST_X	 JAPANESE_OR(21, 20)
#define DJINN_LIST_WIDTH JAPANESE_OR(9, 10)
#define DJINN_TURNS_X	 JAPANESE_OR(40, 48)

#define DJINN_MENU_ARROW_VRAM \
	(BG_CHAR_BLOCK(0) + DJINN_MENU_ARROW_TILE * TILE_SIZE_4BPP)

/* The battle Djinn menu for unit `member`: its Djinn in element and number
   order, five to a page, set ones plain, standby ones in TEXT_COLOR_RED and
   recovering ones in TEXT_COLOR_YELLOW with their turns, the one under the
   cursor described above the list with its change preview beside it
   (mnBattleDjinnMenuShowChangePreview; holding R shows the Psynergy pages, Left
   and Right turn them). The battle commands pass 1 as unused, which no code
   reads. */
Sint32 mnBattleDjinnMenuRun(Sint32 member, Sint32 unused)
{
	MENU_SPR   cursorSprite;
	Uint16	   messageText[64];
	WIN_STATE *state;
	WINDOW	  *previewWindow;
	Sint32	   drawnRow;
	Sint32	   cursorSlot;
	Uint32	  *djinnEntries;
	Sint32	   djinnCount;
	Sint32	   drawnFirstEntry;
	WINDOW	  *messageWindow;
	Sint32	   standbyElements;
	Sint32	   preferredRow;
	Sint32	   previewPage;
	Bool	   redraw;
	Bool	   alreadySelected;
	Sint32	   previewPages;
#if BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES
	Bool redrawList;
#endif
	BATTLE_MENU_STATE	*navigation;
	UNIT				*unit;
	DJINN_RECOVERY_LIST *list;
	WINDOW				*listWindow;
	WINDOW				*iconWindow;
	PARTY_STAT_WINDOW	*partyStatus;
	Sint32				 listEntry;
	Uint32				 selectedDjinn;
	Uint32				 blinkPhase;
	Sint32				 firstEntry;
	Sint32				 selectedRow;
	Uint32				 encodedDjinn;

	struct {
		Sint32 x, y;
	} cursorPos;

	Sint32 pressed;
	Sint32 repeated;
	Sint32 held;
	Sint32 element;
	Sint32 djinnIndex;
	Sint32 i;
	Sint32 j;

	state = (WIN_STATE *)Ram_HeapSlots->winState;
	previewWindow = NULL;
	drawnRow = -1;
	cursorSlot = syVRAMCacheAllocEntry(128);
	djinnEntries = syHeapAllocEWRAM(84 * sizeof(*djinnEntries));
	djinnCount = 0;
	drawnFirstEntry = drawnRow;
	encodedDjinn = 0;
	standbyElements = 0;
	previewPage = 0;
	previewPages = 0;
	redraw = FALSE;
	alreadySelected = FALSE;
#if BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES
	redrawList = FALSE;
#endif
	messageWindow = mnWindowCreate(0, 4, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	mnTextUseExtraTiles(1);
	listWindow = mnWindowCreate(DJINN_LIST_X, 9, DJINN_LIST_WIDTH, 11,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	unit = syUnitGet(member);

	for(element = 0; element <= 3; element++) {
		for(djinnIndex = 0; djinnIndex < DJINN_PER_ELEMENT; djinnIndex++) {
			if(unit->djinnSet[element] & (Uint32)(1 << djinnIndex)) {
				djinnEntries[djinnCount] =
					(Uint32)DJINN_COMMAND(element, djinnIndex);
				djinnCount++;
			} else if(unit->djinnHeld[element] & (Uint32)(1 << djinnIndex)) {
				Uint32 v;
				Sint32 k;

				list = &(gmDjinnGetRecoveryTableStub(
							 (Uint32)member > ALLY_MAX ? 1 : 0))
							->list;
				for(k = 0; k < list->cnt; k++) {
					if(list->entries[k].unitId == member &&
						list->entries[k].element == element &&
						list->entries[k].idx == djinnIndex) {
						break;
					}
				}
				v = (Uint32)(DJINN_COMMAND(element, djinnIndex) |
					BATTLE_DJINN_UNSET);
				djinnEntries[djinnCount] = v;
				if(list->entries[k].turns > 0) {
					v |= (Uint32)(list->entries[k].turns
						<< BATTLE_DJINN_TURNS_SHIFT);
					djinnEntries[djinnCount] = v;
				}
				djinnCount++;
			}
		}
	}
	djinnEntries[djinnCount] = BATTLE_DJINN_END;
	state->dirty = 1;

	for(;;) {
		/* FAKEMATCH(temporary): sprite holds &cursorSprite; as &cursorSprite at
		 * each use. */
		MENU_SPR *sprite = &cursorSprite;

		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow ||
			redraw != 0) {
			selectedDjinn = djinnEntries[firstEntry + selectedRow];
			standbyElements = 0;
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(listWindow->x + 1,
				listWindow->y + drawnRow * 2 + 1, listWindow->width - 2, 1,
				WINDOW_PALETTE);
			if(previewPage != 0) {
				mnWindowClose(messageWindow, 1);
				messageWindow = mnWindowCreate(0, 4, 30, 4,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
						WINDOW_FLAG_BORDERED_CANVAS);
				mnTextFillCanvasBackground();
			}
			redraw = FALSE;
			if(djinnCount != 0) {
				Sint32 k = 0;

				mnMessageArgumentClearAll();
				alreadySelected = FALSE;
				for(k = 0; k <= 7; k++) {
					if(BattleMenuStateWork->chosenDjinn[k] == selectedDjinn) {
						alreadySelected = TRUE;
						break;
					}
				}
				if(alreadySelected != 0) {
					BATTLE_MENU_MESSAGE_COPY(
						(Sint32)MsgCannotUseSameDjinniTwice, messageText,
						BATTLE_MENU_MESSAGE_CAPACITY);
					if(previewWindow != NULL) {
						mnWindowClose(previewWindow, 1);
						previewWindow = NULL;
#if BATTLE_DJINN_LIST_REDRAW_ON_UNAVAILABLE_DJINNI
						redrawList = TRUE;
#endif
						previewPage = 0;
					}
				} else if(selectedDjinn & BATTLE_DJINN_UNSET) {
					if(selectedDjinn & BATTLE_DJINN_TURNS) {
						mnMessageArgumentPush(
							(selectedDjinn & BATTLE_DJINN_TURNS) >>
								BATTLE_DJINN_TURNS_SHIFT,
							MESSAGE_ARGUMENT_NUMBER);
						BATTLE_MENU_MESSAGE_COPY(
							BATTLE_DJINN_ELEMENT_OF(selectedDjinn) *
									DJINN_PER_ELEMENT +
								DJINN_COMMAND_INDEX(selectedDjinn) +
								(Sint32)MsgDjinnEffect,
							messageText, BATTLE_MENU_MESSAGE_CAPACITY);
						if(previewWindow != NULL) {
							mnWindowClose(previewWindow, 1);
							previewWindow = NULL;
#if BATTLE_DJINN_LIST_REDRAW_ON_UNAVAILABLE_DJINNI
							redrawList = TRUE;
#endif
							previewPage = 0;
						}
					} else {
						previewWindow = mnBattleDjinnMenuShowChangePreview(
							previewWindow, member, selectedDjinn, previewPage,
							&previewPages);
						mnMessageCopyExpanded((Sint32)MsgReadyToSummon,
							messageText, BATTLE_MENU_MESSAGE_CAPACITY);
						standbyElements = 1
							<< BATTLE_DJINN_ELEMENT_OF(selectedDjinn);
					}
				} else {
					previewWindow =
						mnBattleDjinnMenuShowChangePreview(previewWindow,
							member, selectedDjinn, previewPage, &previewPages);
					BATTLE_MENU_MESSAGE_COPY(
						BATTLE_DJINN_ELEMENT_OF(selectedDjinn) *
								DJINN_PER_ELEMENT +
							DJINN_COMMAND_INDEX(selectedDjinn) +
							(Sint32)MsgDjinnEffect,
						messageText, BATTLE_MENU_MESSAGE_CAPACITY);
				}
			} else {
				BATTLE_MENU_MESSAGE_COPY((Sint32)MsgNoDjinn, messageText,
					BATTLE_MENU_MESSAGE_CAPACITY);
			}
			state->holdTilemapUpload = 0;
			if(previewPage == 0) {
				state->holdTilemapUpload = 1;
				mnWindowClose(messageWindow, 1);
				messageWindow = mnWindowCreate(0, 4, 30, 4,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
						WINDOW_FLAG_BORDERED_CANVAS);
				state->holdTilemapUpload = previewPage;
			}
			mnTextDrawWideStringAtOffset(messageText, messageWindow, 0, 4);
			drawnRow = selectedRow;
#if BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES
			if(firstEntry != drawnFirstEntry || redrawList == 1) {
#else
			if(firstEntry != drawnFirstEntry) {
#endif
				mnWindowSprRedrawSavedRect(listWindow);
				j = 0;
				for(listEntry = firstEntry;
					(encodedDjinn = djinnEntries[listEntry]) !=
					BATTLE_DJINN_END;
					listEntry++) {
					mnWindowSetTilemapEntry(listWindow,
						BATTLE_DJINN_ELEMENT_OF(encodedDjinn) +
							ELEMENT_ICON_TILE,
						0, j * 2, 0);
					if(encodedDjinn & BATTLE_DJINN_TURNS)
						mnTextSetCol(TEXT_COLOR_YELLOW);
					else if(encodedDjinn & BATTLE_DJINN_UNSET)
						mnTextSetCol(TEXT_COLOR_RED);
					mnTextDrawMessageAsTiles(
						BATTLE_DJINN_ELEMENT_OF(encodedDjinn) *
								DJINN_PER_ELEMENT +
							DJINN_COMMAND_INDEX(encodedDjinn) +
							(Sint32)MsgDjinnName,
						listWindow, 8, j * 16);
					if(encodedDjinn & BATTLE_DJINN_TURNS)
						mnTextDrawNumAtOffset(
							(encodedDjinn & BATTLE_DJINN_TURNS) >>
								BATTLE_DJINN_TURNS_SHIFT,
							2, listWindow, DJINN_TURNS_X, j * 16);
					mnTextSetCol(TEXT_COLOR_WHITE);
					j++;
					if(j > 4)
						break;
				}
				drawnFirstEntry = firstEntry;
#if BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES
				redrawList = FALSE;
#endif
			}
			if(djinnCount > BATTLE_LIST_ROWS) {
				Sint32 listPages;

				for(j = 0;
					j < (listPages = (djinnCount + 4) / BATTLE_LIST_ROWS);
					j++) {
					Sint32 tile = j + PAGE_NUMBER_TILE;

					if(j == firstEntry / BATTLE_LIST_ROWS)
						tile = j + PAGE_NUMBER_LIT_TILE;
					mnWindowSetTilemapEntry(listWindow, tile,
						listWindow->width - listPages + j - 2, -1, 0);
				}
			}
			mnWindowHighlightRect(listWindow->x + 1,
				listWindow->y + selectedRow * 2 + 1, listWindow->width - 2, 1,
				HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		if(djinnCount > BATTLE_LIST_ROWS) {
			for(j = 0; j < (djinnCount + 4) / BATTLE_LIST_ROWS; j++) {
				Sint32 tile = j + PAGE_NUMBER_TILE;

				if((PerWork.data & KEY_R) || (FrameCount & 15) <= 11) {
					if(j == firstEntry / BATTLE_LIST_ROWS)
						tile = j + PAGE_NUMBER_LIT_TILE;
				}
				mnWindowSetTilemapEntry(listWindow, tile,
					listWindow->width - (djinnCount + 4) / 5 + j - 2, -1, 0);
			}
			if((PerWork.data & KEY_R) == 0) {
				mnWindowSetTilemapEntry(listWindow, LIST_TILE_PAGES_LEFT,
					listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3,
					-1, 0);
				mnWindowSetTilemapEntry(listWindow, LIST_TILE_PAGES_RIGHT,
					listWindow->width - 2, -1, 0);
			} else {
				mnWindowSetTilemapEntry(listWindow, WINDOW_TILE_BORDER_TOP,
					listWindow->width - (djinnCount + 4) / BATTLE_LIST_ROWS - 3,
					-1, 0);
				mnWindowSetTilemapEntry(listWindow,
					WINDOW_TILE_BORDER_TOP_RIGHT, listWindow->width - 2, -1, 0);
			}
			WINDOW_MARK_TILEMAP_ROW(state, listWindow->y - 1);
		}

		pressed = PerWork.push;
		repeated = PerWork.repeat;
		held = PerWork.data;
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
		if(pressed & KEY_A) {
			if(djinnCount != 0) {
				Uint32 selection = djinnEntries[firstEntry + selectedRow];
				Uint32 turns = selection & BATTLE_DJINN_TURNS;

				if(turns == 0) {
					if(alreadySelected == 0) {
						encodedDjinn = selection;
						navigation->page = firstEntry;
						navigation->row = selectedRow;
						navigation->preferredRow = preferredRow;
						break;
					}
				} else if(alreadySelected == 0) {
					mnTextFillCanvasBackground();
					mnMessageArgumentClearAll();
					mnMessageArgumentPush((turns & BATTLE_DJINN_TURNS) >>
							BATTLE_DJINN_TURNS_SHIFT,
						MESSAGE_ARGUMENT_NUMBER);
					mnMessageCopyExpanded((Sint32)MsgDjinniStillRecovering,
						messageText, BATTLE_MENU_MESSAGE_CAPACITY);
					mnTextDrawWideStringAtOffset(
						messageText, messageWindow, 0, 4);
				}
				sySndPlayStub(SE_MENU_ERROR);
			} else {
				encodedDjinn = -1;
				break;
			}
		}

		if(djinnCount != 0) {
			if(repeated & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow++;
				if(selectedRow == BATTLE_LIST_ROWS ||
					firstEntry + selectedRow == djinnCount)
					selectedRow = 0;
				preferredRow = selectedRow;
			} else if(repeated & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow--;
				if(selectedRow < 0) {
					if(firstEntry == (djinnCount - 1) / 5 * BATTLE_LIST_ROWS)
						selectedRow = djinnCount - firstEntry - 1;
					else
						selectedRow = 4;
				}
				preferredRow = selectedRow;
			} else if(held & KEY_R) {
				if(previewWindow != NULL) {
					for(i = 0; i < previewPages; i++) {
						Sint32 tile = i + PAGE_NUMBER_TILE;

						if((FrameCount & 15) <= 11 && i == previewPage - 1)
							tile = previewPage + PAGE_NUMBER_LIT_TILE - 1;
						mnWindowSetTilemapEntry(previewWindow, tile,
							previewWindow->width - previewPages + i - 2, -1, 0);
					}
					mnWindowSetTilemapEntry(previewWindow, LIST_TILE_PAGES_LEFT,
						previewWindow->width - previewPages - 3, -1, 0);
					mnWindowSetTilemapEntry(previewWindow,
						LIST_TILE_PAGES_RIGHT, previewWindow->width - 2, -1, 0);
					WINDOW_MARK_TILEMAP_ROW(state, previewWindow->y);
				}
				if(previewPage == 0 && previewPages != 0) {
					if(previewWindow != NULL)
						mnWindowSprClearList(previewWindow);
					previewPage = 1;
					redraw = TRUE;
				} else {
					if(previewPage > previewPages)
						previewPage = previewPages;
					if(previewPage != 0) {
						if(repeated & KEY_RIGHT) {
							sySndPlayStub(SE_MENU_CURSOR_MOVE);
							previewPage++;
							if(previewPage > previewPages)
								previewPage = 1;
							redraw = TRUE;
						} else if(repeated & KEY_LEFT) {
							sySndPlayStub(SE_MENU_CURSOR_MOVE);
							previewPage--;
							if(previewPage <= 0)
								previewPage = previewPages;
							redraw = TRUE;
						}
					}
				}
			} else if(previewPage != 0) {
				if(previewWindow != NULL)
					mnWindowSprClearList(previewWindow);
				previewPage = 0;
				redraw = TRUE;
#if BATTLE_DJINN_LIST_REDRAW_AFTER_PSYNERGY_PAGES
				redrawList = TRUE;
#endif
			} else if(repeated & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry + BATTLE_LIST_ROWS >= djinnCount) {
					if(firstEntry != 0) {
						firstEntry = 0;
						selectedRow = preferredRow;
					}
				} else {
					firstEntry += BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry == (djinnCount - 1) / 5 * BATTLE_LIST_ROWS) {
						selectedRow = djinnCount - firstEntry - 1;
						if(selectedRow > preferredRow)
							selectedRow = preferredRow;
					}
				}
			} else if(repeated & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry != 0) {
					firstEntry -= BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
				} else {
					firstEntry = (djinnCount - 1) / 5 * BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry != 0) {
						selectedRow = djinnCount - firstEntry - 1;
						if(selectedRow > preferredRow)
							selectedRow = preferredRow;
					}
				}
			}
		}

		MENU_PLACE_CURSOR(cursorPos, listWindow, selectedRow, 2, sprite);
		sprite->oam.f.tile = syVRAMCacheReload(cursorSlot, PointingHandTiles);
		sprite->oam.f.x = cursorPos.x + ((FrameCount & 4) >> 1) - 6;
		sprite->oam.f.y = cursorPos.y - ((FrameCount & 4) >> 2) - 8;
		if(djinnCount != 0)
			syOAMAddSprAtDepth(sprite, OAM_DEPTH_WINDOW + 2);

		partyStatus = Ram_HeapSlots->partyStat;
		blinkPhase = FrameCount & 4;
		iconWindow = (WINDOW *)partyStatus->win;
		if(partyStatus->flg & 2) {
			for(i = 0; i <= 3; i++) {
				/* HIGHLIGHT_PALETTE, one below, while the blink is on. */
				Sint32 palette = WINDOW_PALETTE - (blinkPhase != 0);

				if(((1 << i) & standbyElements) == 0)
					palette = WINDOW_PALETTE;
				mnWindowHighlightRect(
					iconWindow->x + DjinnMenu_ElementXOffsets[i] + 1,
					iconWindow->y + DjinnMenu_ElementYOffsets[i] + 1, 2, 1,
					palette);
			}
		}
		if(FrameCount & 4) {
			syWordsCopyEntry((void *)DJINN_MENU_ARROW_VRAM, DjinnMenu_BlinkTile,
				TILE_SIZE_4BPP);
			syWordsCopyEntry((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
				DjinnMenu_BlinkTile, TILE_SIZE_4BPP);
		} else {
			syWordsFillEntry((void *)DJINN_MENU_ARROW_VRAM, TILE_SIZE_4BPP,
				TEXT_CANVAS_BACKGROUND_WORD);
			syWordsCopyEntry((void *)(DJINN_MENU_ARROW_VRAM + TILE_SIZE_4BPP),
				DjinnMenu_BlinkTile, TILE_SIZE_4BPP);
		}
		sySynch(1);
	}

	{
		PARTY_STAT_WINDOW *owner = Ram_HeapSlots->partyStat;

		if(owner->flg & 2)
			mnWindowHighlightRect(
				owner->win->x + 1, owner->win->y + 1, 4, 4, WINDOW_PALETTE);
	}
	syVRAMCacheFreeEntry(cursorSlot);
	mnWindowClose(messageWindow, 1);
	mnWindowClose(previewWindow, 1);
	mnWindowClose(listWindow, 1);
	mnWindowMarkVisibleTextTiles();
	mnTextUseExtraTiles(0);
	syHeapRelease(djinnEntries);
	sySynch(1);
	return encodedDjinn;
}

#undef DJINN_LIST_X
#undef DJINN_LIST_WIDTH
#undef DJINN_TURNS_X
