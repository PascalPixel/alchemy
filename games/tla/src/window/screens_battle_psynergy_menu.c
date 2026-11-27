/* The battle Psynergy menu. Part of screens.c. */

/* The list is a tile wider in Spanish and French, and its columns move with
   it. */
#define LIST_WINDOW_X	  BY_LANGUAGE(9, 9, 9, 8, 8, 9)
#define LIST_WINDOW_WIDTH BY_LANGUAGE(21, 21, 21, 22, 22, 21)
#define COST_COLUMN		  BY_LANGUAGE(11, 11, 11, 12, 12, 11)
#define COST_X			  BY_LANGUAGE(104, 104, 104, 112, 112, 104)
#define RANGE_COLUMN	  BY_LANGUAGE(15, 15, 15, 16, 16, 15)

/* The battle Psynergy list for unit `unitId`: `cnt` abilities of `actions`
   (ended by 0), five to a page, each with its icon, name, PP cost, element and
   range icon, in TEXT_COLOR_YELLOW when not usable in battle, TEXT_COLOR_RED
   when the unit's PP fall short and TEXT_COLOR_PINK while its Psynergy is
   sealed; the window above describes the one under the cursor. Returns the
   chosen ability's place in `actions`, or -1 when cancelled, when the link
   battle's time runs out or when the list is empty. Only an ability not usable
   in battle is refused here, silently: one the PP cannot pay for, or a sealed
   party member's, can be chosen. The Psynergy command runs it
   (mnBattleCollectPartyCommands), listing only abilities usable in battle. */
Sint32 mnBattlePsynergyMenuRun(Sint32 unitId, Uint16 *actions, Sint32 cnt)
{
	WIN_STATE *state;
	Sint32	   drawnFirstEntry;
	Sint32	   firstEntry;
	Sint32	   drawnRow;
	Sint32	   cursorHandle;
	UNIT	  *unit;
	WINDOW	  *msgWin;
	Sint32	   iconCnt;
	Sint32	   preferredRow;

	struct {
		Sint32 x, y;
	} cursorPos;

	BATTLE_MENU_STATE *navigation;
	WINDOW			  *win;
	Sint32			   selectedRow;
	Sint32			   encodedAction;
	Sint32			   iconTile;
	Sint32			   pageTile;
	Sint32			   rangePattern;
	Sint32			   result;
	Sint32			   i;
	MENU_SPR		   cursor;
	MENU_SPR		  *iconSpr;
	Uint16			   msgText[64];
	Sint32			   iconSlots[BATTLE_LIST_ROWS];
	MENU_SPR		   sprs[BATTLE_LIST_ROWS];
	Sint32			   glyph;

	state = (WIN_STATE *)RamHeapSlots->winState;
	drawnRow = -1;
	drawnFirstEntry = -1;
	cursorHandle = syVRAMCacheAllocEntry(128);
	unit = syUnitGet(unitId);
	msgWin = mnWindowCreate(0, 5, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	iconCnt = BATTLE_LIST_ROWS;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	win = mnWindowCreate(LIST_WINDOW_X, 9, LIST_WINDOW_WIDTH,
		BATTLE_LIST_ROWS * 2 + 1, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		Sint32 y = i * 2;

		iconSpr = &sprs[i];
		MENU_SPR_INIT(sprs[i], OBJ_SIZE_16X16);
		iconSpr->oam.f.x = win->x * 8 + 8;
		iconSpr->oam.f.y = (y + win->y) * 8 + 4;
	}
	/* Reserve one glyph slot per visible row; paging reloads these same
	   slots. */
	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		iconSlots[i] = syVRAMCacheAllocEntry(128);
		sprs[i].oam.f.tile = syVRAMCacheReload(iconSlots[i], (const void *)-1);
	}
	/* The border's tees go into tiles 0, 1, 16 and 17 of the second text range,
	   which nothing shows here. In battle BG1 takes 8bpp tiles from character
	   block 0 (BATTLE_BG1CNT), so these are its tiles COMMAND_ROW_ICON_TILE and
	   COMMAND_ROW_ICON_TILE + 8, the first command icon's top-left and
	   bottom-left; mnBattleCommandRowRun turns BG1 off as it returns, before a
	   list opens, and draws its icons there again when it opens next. */
	mnVRAMCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST);
	mnVRAMCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST + 1);
	mnVRAMCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 16);
	mnVRAMCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 17);

	for(;;) {
		iconSpr = &cursor;
		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(win->x + 1, win->y + drawnRow * 2 + 1,
				win->width - 2, 1, WINDOW_PALETTE);
			mnTextFillCanvasBackground();
			if(cnt != 0)
				BATTLE_MENU_MESSAGE_COPY(
					(actions[firstEntry + selectedRow] & ACTION_ID_MASK) +
						(Sint32)MsgAbilityDescription,
					msgText, BATTLE_MENU_MESSAGE_CAPACITY);
			else
				BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleNoPsynergy, msgText,
					BATTLE_MENU_MESSAGE_CAPACITY);
			mnTextDrawWideStringAtOffset(msgText, msgWin, 0, 4);
			drawnRow = selectedRow;
			/* Cursor moves update help; only a page change reloads the row
			   icons. */
			if(firstEntry != drawnFirstEntry) {
				mnWindowSprRedrawSavedRect(win);
				for(i = 0; i < BATTLE_LIST_ROWS; i++) {
					ABILITY_DEFINITION *action;

					encodedAction = actions[firstEntry + i];
					if(encodedAction == 0)
						break;
					action = gmAbilityGetDirectStub(encodedAction);
					mnWindowSetTilemapEntry(
						win, WINDOW_TILE_PP_LABEL, COST_COLUMN, i * 2, 0);
					mnWindowSetTilemapEntry(win, WINDOW_TILE_PP_LABEL_END,
						COST_COLUMN + 1, i * 2, 0);
					mnAbilityBuildIconTiles(
						encodedAction & PSYNERGY_ABILITY_MASK, 0, &iconSlots[i],
						&glyph, 1);
					sprs[i].oam.f.tile = glyph;
					if((action->flg & ABILITY_USABLE_IN_BATTLE) == 0)
						mnTextSetCol(TEXT_COLOR_YELLOW);
					else if(action->ppCost > unit->pp)
						mnTextSetCol(TEXT_COLOR_RED);
					else if(unit->psySeal != 0)
						mnTextSetCol(TEXT_COLOR_PINK);
					state->textPal = BASIC_COLORS_PALETTE;
					mnTextDrawMessageAsTiles(
						encodedAction + (Sint32)MsgAbilityName, win, 16,
						i * 16);
					mnTextDrawNumAtOffset(
						action->ppCost, 2, win, COST_X, i * 16);
					mnTextSetCol(TEXT_COLOR_WHITE);
					state->textPal = WINDOW_PALETTE;
					if(action->element != ELEMENT_NONE)
						mnWindowSetTilemapEntry(win,
							action->element + ELEMENT_ICON_TILE, RANGE_COLUMN,
							i * 2, 0);
					rangePattern = action->range;
					if(rangePattern == ABILITY_RANGE_ALL)
						rangePattern = RANGE_ICON_ALL;
					else
						rangePattern--;
					mnRangeIconDraw(
						win, RANGE_COLUMN + 1, i * 2, rangePattern, 0);
				}
				iconCnt = i;
				drawnFirstEntry = firstEntry;
			}
			if(cnt > BATTLE_LIST_ROWS) {
				for(i = 0; i < (cnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
					i++) {
					iconTile = i + PAGE_NUMBER_TILE;
					if(i == firstEntry / BATTLE_LIST_ROWS)
						iconTile = i + PAGE_NUMBER_LIT_TILE;
					mnWindowSetTilemapEntry(win, iconTile,
						win->width -
							(cnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS +
							i - 2,
						-1, 0);
				}
			}
			mnWindowHighlightRect(win->x + 1, win->y + selectedRow * 2 + 1,
				win->width - 2, 1, HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		/* Each tick: the current page's number lit 12 ticks in 16, and the
		   page arrows on the window's top edge. */
		if(cnt > BATTLE_LIST_ROWS) {
			for(i = 0; i < (cnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
				i++) {
				iconTile = i + PAGE_NUMBER_TILE;
				if((TickCount & 15) <= 11) {
					if(i == firstEntry / BATTLE_LIST_ROWS)
						iconTile = i + PAGE_NUMBER_LIT_TILE;
				}
				mnWindowSetTilemapEntry(win, iconTile,
					win->width -
						(cnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS + i - 2,
					-1, 0);
			}
			pageTile = -1;
			mnWindowSetTilemapEntry(win, LIST_TILE_PAGES_LEFT,
				win->width - (cnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS -
					3,
				pageTile, 0);
			mnWindowSetTilemapEntry(
				win, LIST_TILE_PAGES_RIGHT, win->width - 2, pageTile, 0);
			WINDOW_MARK_TILEMAP_ROW(state, win->y - 1);
		}

		for(i = 0; i < iconCnt; i++)
			syOAMAddSprAtDepth(&sprs[i], OAM_DEPTH_WINDOW);

		MENU_PLACE_CURSOR(cursorPos, win, selectedRow, 4, iconSpr);
		iconSpr->oam.f.tile =
			syVRAMCacheReload(cursorHandle, PointingHandTiles);
		iconSpr->oam.f.x = cursorPos.x + ((TickCount & 4) >> 1) - 4;
		iconSpr->oam.f.y = cursorPos.y - ((TickCount & 4) >> 2) - 8;
		if(cnt != 0)
			syOAMAddSprAtDepth(iconSpr, OAM_DEPTH_WINDOW + 2);

		BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
		navigation = BattleMenuStateWork;
		if(PerWork.push & KEY_A) {
			if(cnt != 0) {
				result = firstEntry + selectedRow;
				if((gmAbilityGetDirectStub(actions[result])->flg &
					   ABILITY_USABLE_IN_BATTLE) == 0) {
					/* Not usable in battle: the menu stays open. */
				} else {
					break;
				}
			} else {
				result = -1;
				break;
			}
		} else if(navigation->tmr == 0 || (PerWork.push & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}

		if(cnt != 0) {
			if(PerWork.repeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow++;
				if(selectedRow == BATTLE_LIST_ROWS ||
					firstEntry + selectedRow == cnt)
					selectedRow = 0;
				preferredRow = selectedRow;
			} else if(PerWork.repeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow--;
				if(selectedRow < 0) {
					if(firstEntry ==
						(cnt - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
						selectedRow = cnt - firstEntry - 1;
					else
						selectedRow = BATTLE_LIST_ROWS - 1;
				}
				preferredRow = selectedRow;
			} else if(PerWork.repeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry + BATTLE_LIST_ROWS >= cnt) {
					if(firstEntry != 0) {
						firstEntry = 0;
						selectedRow = preferredRow;
					}
				} else {
					firstEntry += BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry ==
						(cnt - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
						selectedRow = cnt - firstEntry - 1;
						if(selectedRow > preferredRow)
							selectedRow = preferredRow;
					}
				}
			} else if(PerWork.repeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry != 0) {
					firstEntry -= BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
				} else {
					firstEntry =
						(cnt - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry != 0) {
						selectedRow = cnt - firstEntry - 1;
						if(selectedRow > preferredRow)
							selectedRow = preferredRow;
					}
				}
			}
		}
		sySynch(1);
	}

	mnWindowClose(msgWin, 1);
	mnWindowClose(win, 1);
	sySynch(1);
	for(i = 0; i < BATTLE_LIST_ROWS; i++)
		syVRAMCacheFreeEntry(iconSlots[i]);
	syVRAMCacheFreeEntry(cursorHandle);
	sySynch(1);
	return result;
}

#undef LIST_WINDOW_X
#undef LIST_WINDOW_WIDTH
#undef COST_COLUMN
#undef COST_X
#undef RANGE_COLUMN
