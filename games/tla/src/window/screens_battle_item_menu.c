/* @file */
/* The battle item menu. */
/* Part of screens.c. */

/* Says whether party member `unit` can use inventory entry `itemId` in battle:
 */
/* 0 yes; 1 no, "Using this will have no effect." (no item, an item that only */
/* teaches Psynergy while equipped, one with no action, or equipment the party
 */
/* member cannot equip); 2 not in battle, "Cannot be used during battle." (its
 */
/* action lacks ABILITY_USABLE_IN_BATTLE). */
s32 mnItemCheckBattleUse(s32 unit, s32 itemId) {
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
	   (gmItemCanUnitEquip(unit, itemId) == 0)) {
		return 1;
	}
	if((ABILITY_USABLE_IN_BATTLE & btAbilityGet(item->abilityId)->flg) == 0) {
		return 2;
	}
	return 0;
}

/* Copies one 4bpp tile of BG character block 0 to another, each given as a */
/* tilemap entry whose palette and flip bits are ignored. */
void mnVramCopyTile(u32 sourceTile, u32 destinationTile) {
	syDMAXCopy(BG_CHAR_BLOCK(0) +
				   (sourceTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
			   BG_CHAR_BLOCK(0) +
				   (destinationTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
			   DMA_COPY_WORDS(8), REG_DMA3);
}

/* How much of a refusal is copied: more in German, Spanish, French and */
/* Italian. */
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_JAPANESE)
#define REFUSAL_TEXT_LENGTH 32
#else
#define REFUSAL_TEXT_LENGTH 52
#endif

/* The battle item list for unit `member`: `count` inventory entries (ended by
 */
/* 0), five to a page, each with its icon and name: plain when usable, */
/* TEXT_COLOR_YELLOW when mnItemCheckBattleUse refuses it, TEXT_COLOR_RED */
/* when broken; the window above names the one under the cursor */
/* (MsgItemDescription), or says it cannot be used in battle. */
s32 mnBattleItemMenuRun(s32 member, u16 *entries, s32 count) {
	WIN_STATE *state;
	s32 drawnFirstEntry;
	s32 drawnRow;
	s32 cursorHandle;
	WINDOW *messageWindow;
	s32 iconCount;
	s32 preferredRow;
	struct {
		s32 x, y;
	} cursorPosition;
	BATTLE_MENU_STATE *navigation;
	WINDOW *window;
	s32 firstEntry;
	s32 selectedRow;
	s32 item;
	s32 iconTile;
	s32 unusableReason;
	s32 result;
	s32 i;
	MENU_SPR cursorStorage;
	MENU_SPR *iconSprite;
	u16 messageText[64];
	s32 iconSlots[BATTLE_LIST_ROWS];
	MENU_SPR sprites[BATTLE_LIST_ROWS];

	state = Ram_HeapSlots->winState;
	drawnFirstEntry = -1;
	drawnRow = -1;
	cursorHandle = syVramCacheAllocEntry(128);
	messageWindow =
		mnWindowCreate(0, 5, 30, 4,
					   WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
						   WINDOW_FLAG_BORDERED_CANVAS);
	iconCount = 0;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	window = mnWindowCreate(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		s32 y = i * 2;

		iconSprite = &sprites[i];
		syMenuSprInit(sprites[i], OBJ_SIZE_16X16);
		iconSprite->oam.f.x = window->x * 8 + 8;
		iconSprite->oam.f.y = (y + window->y) * 8 + 4;
	}
	/* One icon slot a visible row; paging reloads these same slots. */
	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		iconSlots[i] = syVramCacheAllocEntry(128);
		sprites[i].oam.f.tile =
			syVramCacheReload(iconSlots[i], (const void *)-1);
	}
	/* The border's tees go into tiles 0, 1, 16 and 17 of the second text range,
	 */
	/* which nothing shows here. In battle BG1 takes 8bpp tiles from character
	 */
	/* block 0 (BATTLE_BG1CNT), so these are its tiles COMMAND_ROW_ICON_TILE and
	 */
	/* COMMAND_ROW_ICON_TILE + 8, the first command icon's top-left and */
	/* bottom-left; mnBattleCommandRowRun turns BG1 off as it returns, before a
	 */
	/* list opens, and draws its icons there again when it opens next. */
	mnVramCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST);
	mnVramCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST + 1);
	mnVramCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 16);
	mnVramCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 17);

	for(;;) {
		iconSprite = &cursorStorage;

		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
								  window->width - 2, 1, WINDOW_PALETTE);
			mnTextFillCanvasBackground();
			if(count != 0) {
				if(mnItemCheckBattleUse(member,
										entries[firstEntry + selectedRow]) == 2)
					BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemUnavailable,
											 messageText,
											 BATTLE_MENU_MESSAGE_CAPACITY);
				else
					BATTLE_MENU_MESSAGE_COPY(
						(entries[firstEntry + selectedRow] &
						 INVENTORY_ITEM_ID_MASK) +
							(s32)MsgItemDescription,
						messageText, BATTLE_MENU_MESSAGE_CAPACITY);
			}
			else {
				BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleNoItems, messageText,
										 BATTLE_MENU_MESSAGE_CAPACITY);
			}
			mnTextDrawWideStringAtOffset(messageText, messageWindow, 0, 4);
			drawnRow = selectedRow;
			/* Only a page change redraws the rows and their icons. */
			if(firstEntry != drawnFirstEntry) {
				mnWindowSprRedrawSavedRect(window);
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
					mnTextDrawMessageAsTiles((item & INVENTORY_ITEM_ID_MASK) +
												 (s32)MsgItemName,
											 window, 16, i * 16);
					mnTextSetCol(TEXT_COLOR_WHITE);
					sprites[i].oam.f.tile =
						mnItemIconLoadIntoSlot(item, iconSlots[i]);
				}
				iconCount = i;
				drawnFirstEntry = firstEntry;
			}
			if(count > BATTLE_LIST_ROWS) {
				for(i = 0;
					i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
					i++) {
					iconTile = i + PAGE_NUMBER_TILE;
					if(i == firstEntry / BATTLE_LIST_ROWS)
						iconTile = i + PAGE_NUMBER_LIT_TILE;
					mnWindowSetTilemapEntry(window, iconTile,
											window->width -
												(count + BATTLE_LIST_ROWS - 1) /
													BATTLE_LIST_ROWS +
												i - 2,
											-1, 0);
				}
			}
			mnWindowHighlightRect(window->x + 1,
								  window->y + selectedRow * 2 + 1,
								  window->width - 2, 1, HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		/* Each frame: the current page's number lit 12 frames in 16, and the
		 * page */
		/* arrows on the window's top edge (y -1). */
		if(count > BATTLE_LIST_ROWS) {
			for(i = 0; i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
				i++) {
				iconTile = i + PAGE_NUMBER_TILE;
				if((FrameCount & 15) <= 11) {
					if(i == firstEntry / BATTLE_LIST_ROWS)
						iconTile = i + PAGE_NUMBER_LIT_TILE;
				}
				mnWindowSetTilemapEntry(window, iconTile,
										window->width -
											(count + BATTLE_LIST_ROWS - 1) /
												BATTLE_LIST_ROWS +
											i - 2,
										-1, 0);
			}
			mnWindowSetTilemapEntry(
				window, LIST_TILE_PAGES_LEFT,
				window->width -
					(count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS - 3,
				-1, 0);
			mnWindowSetTilemapEntry(window, LIST_TILE_PAGES_RIGHT,
									window->width - 2, -1, 0);
			WINDOW_MARK_TILEMAP_ROW(state, window->y - 1);
		}

		for(i = 0; i < iconCount; i++)
			syOamAddSprAtDepth(&sprites[i], OAM_DEPTH_WINDOW);

		MENU_PLACE_CURSOR(cursorPosition, window, selectedRow, 2, iconSprite);
		iconSprite->oam.f.tile =
			syVramCacheReload(cursorHandle, PointingHandTiles);
		iconSprite->oam.f.x = cursorPosition.x + ((FrameCount & 4) >> 1) - 4;
		iconSprite->oam.f.y = cursorPosition.y - ((FrameCount & 4) >> 2) - 8;
		if(count != 0)
			syOamAddSprAtDepth(&cursorStorage, OAM_DEPTH_WINDOW + 2);

		BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
		navigation = BattleMenuStateWork;
		/* A takes a usable item; a broken or refused one plays the error sound
		 * and */
		/* shows why. */
		if(InputWork.pressed & KEY_A) {
			if(count != 0) {
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
					BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemUnavailable,
											 messageText, REFUSAL_TEXT_LENGTH);
				else if(entries[result] & INVENTORY_BROKEN)
					BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemBroken,
											 messageText, REFUSAL_TEXT_LENGTH);
				else
					BATTLE_MENU_MESSAGE_COPY((s32)MsgBattleItemNoEffect,
											 messageText, REFUSAL_TEXT_LENGTH);
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(messageText, messageWindow, 0, 4);
			}
			else {
				result = -1;
				break;
			}
		}
		else if(navigation->tmr == 0 || (InputWork.pressed & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}

		if(count != 0) {
			if(InputWork.repeat & KEY_DOWN) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow++;
				if(selectedRow == BATTLE_LIST_ROWS ||
				   firstEntry + selectedRow == count)
					selectedRow = 0;
				preferredRow = selectedRow;
			}
			else if(InputWork.repeat & KEY_UP) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow--;
				if(selectedRow < 0) {
					if(firstEntry ==
					   (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
						selectedRow = count - firstEntry - 1;
					else
						selectedRow = BATTLE_LIST_ROWS - 1;
				}
				preferredRow = selectedRow;
			}
			else if(InputWork.repeat & KEY_RIGHT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry + BATTLE_LIST_ROWS >= count) {
					if(firstEntry != 0) {
						firstEntry = 0;
						selectedRow = preferredRow;
					}
				}
				else {
					firstEntry += BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry ==
					   (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
						selectedRow = count - firstEntry - 1;
						if(selectedRow > preferredRow)
							selectedRow = preferredRow;
					}
				}
			}
			else if(InputWork.repeat & KEY_LEFT) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry != 0) {
					firstEntry -= BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
				}
				else {
					firstEntry =
						(count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry != 0) {
						selectedRow = count - firstEntry - 1;
						if(selectedRow > preferredRow)
							selectedRow = preferredRow;
					}
				}
			}
		}
		sySynch(1);
	}

	mnWindowClose(messageWindow, 1);
	mnWindowClose(window, 1);
	sySynch(1);
	syVramCacheFreeEntry(cursorHandle);
	for(i = 0; i < BATTLE_LIST_ROWS; i++)
		syVramCacheFreeEntry(iconSlots[i]);
	sySynch(1);
	return result;
}

#undef REFUSAL_TEXT_LENGTH
