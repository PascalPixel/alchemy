/* The battle item menu. Part of screens.c. */

/* Says whether party member `unit` can use inventory entry `itemId` in battle:
   0 yes; 1 no, "Using this will have no effect." (no item, an item that only
   teaches Psynergy while equipped, one with no action, or equipment the party
   member cannot equip); 2 not in battle, "Cannot be used during battle." (its
   action lacks ABILITY_USABLE_IN_BATTLE). */
Sint32 mnItemCheckBattleUse(Sint32 unit, Sint32 itemId)
{
	ITEM_DEFINITION *item;

	if(itemId == 0) {
		return 1;
	}
	item = gmItemGetDirectStub(itemId);
	if(item->useType == ITEM_USE_TEACHES) {
		return 1;
	}
	if(item->abilityId == 0) {
		return 1;
	}
	if((item->type != ITEM_TYPE_NONE) &&
		(gmItemCanUnitEquipDirectStub(unit, itemId) == 0)) {
		return 1;
	}
	if((ABILITY_USABLE_IN_BATTLE &
		   gmAbilityGetDirectStub(item->abilityId)->flg) == 0) {
		return 2;
	}
	return 0;
}

#include "window/vram_copy_tile.c"

/* How much of a refusal is copied: more in German, Spanish, French and
   Italian. */
#define REFUSAL_TEXT_LENGTH WHEN(LONG_BATTLE_ITEM_REFUSAL, 52, 32)

/* The battle item list for unit `member`: `cnt` inventory entries (ended by
   0), five to a page, each with its icon and name: plain when usable,
   TEXT_COLOR_YELLOW when mnItemCheckBattleUse refuses it, TEXT_COLOR_RED when
   broken; the window above names the one under the cursor (MsgItemDescription),
   or says it cannot be used in battle. */
Sint32 mnBattleItemMenuRun(Sint32 member, Uint16 *entries, Sint32 cnt)
{
	WIN_STATE *state;
	Sint32	   drawnFirstEntry;
	Sint32	   drawnRow;
	Sint32	   cursorHandle;
	WINDOW	  *msgWin;
	Sint32	   iconCnt;
	Sint32	   preferredRow;

	struct {
		Sint32 x, y;
	} cursorPos;

	BATTLE_MENU_STATE *navigation;
	WINDOW			  *win;
	Sint32			   firstEntry;
	Sint32			   selectedRow;
	Sint32			   item;
	Sint32			   iconTile;
	Sint32			   unusableReason;
	Sint32			   result;
	Sint32			   i;
	MENU_SPR		   cursorStorage;
	MENU_SPR		  *iconSpr;
	Uint16			   msgText[64];
	Sint32			   iconSlots[BATTLE_LIST_ROWS];
	MENU_SPR		   sprs[BATTLE_LIST_ROWS];

	state = RamHeapSlots->winState;
	drawnFirstEntry = -1;
	drawnRow = -1;
	cursorHandle = syVRAMCacheAllocEntry(128);
	msgWin = mnWindowCreate(0, 5, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	iconCnt = 0;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	win = mnWindowCreate(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		Sint32 y = i * 2;

		iconSpr = &sprs[i];
		MENU_SPR_INIT(sprs[i], OBJ_SIZE_16X16);
		iconSpr->oam.f.x = win->x * 8 + 8;
		iconSpr->oam.f.y = (y + win->y) * 8 + 4;
	}
	/* One icon slot a visible row; paging reloads these same slots. */
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
		iconSpr = &cursorStorage;

		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(win->x + 1, win->y + drawnRow * 2 + 1,
				win->width - 2, 1, WINDOW_PALETTE);
			mnTextFillCanvasBackground();
			if(cnt != 0) {
				if(mnItemCheckBattleUse(
					   member, entries[firstEntry + selectedRow]) == 2)
					BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleItemUnavailable,
						msgText, BATTLE_MENU_MESSAGE_CAPACITY);
				else
					BATTLE_MENU_MESSAGE_COPY(
						(entries[firstEntry + selectedRow] &
							INVENTORY_ITEM_ID_MASK) +
							(Sint32)MsgItemDescription,
						msgText, BATTLE_MENU_MESSAGE_CAPACITY);
			} else {
				BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleNoItems, msgText,
					BATTLE_MENU_MESSAGE_CAPACITY);
			}
			mnTextDrawWideStringAtOffset(msgText, msgWin, 0, 4);
			drawnRow = selectedRow;
			/* Only a page change redraws the rows and their icons. */
			if(firstEntry != drawnFirstEntry) {
				mnWindowSprRedrawSavedRect(win);
				for(i = 0; i < BATTLE_LIST_ROWS; i++) {
					item = entries[firstEntry + i];
					if(item == 0)
						break;
					gmItemGetDirectStub(item);
					mnTextSetCol(TEXT_COLOR_WHITE);
					if(mnItemCheckBattleUse(member, item) != 0)
						mnTextSetCol(TEXT_COLOR_YELLOW);
					else if(item & INVENTORY_BROKEN)
						mnTextSetCol(TEXT_COLOR_RED);
					mnTextDrawMessageAsTiles(
						(item & INVENTORY_ITEM_ID_MASK) + (Sint32)MsgItemName,
						win, 16, i * 16);
					mnTextSetCol(TEXT_COLOR_WHITE);
					sprs[i].oam.f.tile =
						mnItemIconLoadIntoSlot(item, iconSlots[i]);
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
		   page arrows on the window's top edge (y -1). */
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
			mnWindowSetTilemapEntry(win, LIST_TILE_PAGES_LEFT,
				win->width - (cnt + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS -
					3,
				-1, 0);
			mnWindowSetTilemapEntry(
				win, LIST_TILE_PAGES_RIGHT, win->width - 2, -1, 0);
			WINDOW_MARK_TILEMAP_ROW(state, win->y - 1);
		}

		for(i = 0; i < iconCnt; i++)
			syOAMAddSprAtDepth(&sprs[i], OAM_DEPTH_WINDOW);

		MENU_PLACE_CURSOR(cursorPos, win, selectedRow, 2, iconSpr);
		iconSpr->oam.f.tile =
			syVRAMCacheReload(cursorHandle, PointingHandTiles);
		iconSpr->oam.f.x = cursorPos.x + ((TickCount & 4) >> 1) - 4;
		iconSpr->oam.f.y = cursorPos.y - ((TickCount & 4) >> 2) - 8;
		if(cnt != 0)
			syOAMAddSprAtDepth(&cursorStorage, OAM_DEPTH_WINDOW + 2);

		BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
		navigation = BattleMenuStateWork;
		/* A takes a usable item; a broken or refused one plays the error sound
		   and shows why. */
		if(PerWork.push & KEY_A) {
			if(cnt != 0) {
				result = firstEntry + selectedRow;
				gmItemGetDirectStub(entries[result]);
				unusableReason = 0;
				if((entries[result] & INVENTORY_BROKEN) == 0) {
					unusableReason =
						mnItemCheckBattleUse(member, entries[result]);
					if(unusableReason == 0)
						break;
				}
				sySndPlayStub(SE_MENU_ERROR);
				if(unusableReason == 2)
					BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleItemUnavailable,
						msgText, REFUSAL_TEXT_LENGTH);
				else if(entries[result] & INVENTORY_BROKEN)
					BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleItemBroken,
						msgText, REFUSAL_TEXT_LENGTH);
				else
					BATTLE_MENU_MESSAGE_COPY((Sint32)MsgBattleItemNoEffect,
						msgText, REFUSAL_TEXT_LENGTH);
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(msgText, msgWin, 0, 4);
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
	syVRAMCacheFreeEntry(cursorHandle);
	for(i = 0; i < BATTLE_LIST_ROWS; i++)
		syVRAMCacheFreeEntry(iconSlots[i]);
	sySynch(1);
	return result;
}

#undef REFUSAL_TEXT_LENGTH
