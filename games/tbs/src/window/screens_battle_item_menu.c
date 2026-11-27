/* The battle item menu. Part of screens.c. */

/* Says whether party member `unit` can use inventory entry `itemId` in battle
   (BATTLE_ITEM_USE). */
Sint32 mnItemCheckBattleUse(Sint32 unit, Sint32 itemId)
{
	ITEM_DEFINITION *item;

	if(itemId == 0) {
		return BATTLE_ITEM_NO_EFFECT;
	}
	item = gmItemGetDirectStub(itemId);
	if(item->useType == ITEM_USE_TEACHES) {
		return BATTLE_ITEM_NO_EFFECT;
	}
	if(item->abilityId == 0) {
		return BATTLE_ITEM_NO_EFFECT;
	}
	if((item->type != ITEM_TYPE_NONE) &&
		(gmItemCanUnitEquipDirectStub(unit, itemId) == 0)) {
		return BATTLE_ITEM_NO_EFFECT;
	}
	if((ABILITY_USABLE_IN_BATTLE &
		   gmAbilityGetDirectStub(item->abilityId)->flags) == 0) {
		return BATTLE_ITEM_NOT_IN_BATTLE;
	}
	return BATTLE_ITEM_USABLE;
}

/* Copies one 4bpp tile of BG character block 0 to another, each given as a
   tilemap entry whose palette and flip bits are ignored. */
void mnVRAMCopyTile(Uint32 srcTile, Uint32 destTile)
{
	syDMAXCopy(BG_CHAR_BLOCK(0) + (srcTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
		BG_CHAR_BLOCK(0) + (destTile & TILE_NUMBER_MASK) * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(8), REG_DMA3);
}

/* The battle item list for unit `member`: `count` inventory entries (ended by
   0), five to a page, each with its icon and name: plain when usable,
   TEXT_COLOR_YELLOW when mnItemCheckBattleUse refuses it, TEXT_COLOR_RED when
   broken; the window above names the one under the cursor (MsgItemPlainName),
   or says it cannot be used in battle. */
Sint32 mnBattleItemMenuRun(Sint32 member, Uint16 *entries, Sint32 count)
{
	WIN_STATE		  *state;
	Sint32			   drawnFirstEntry;
	Sint32			   drawnRow;
	Sint32			   cursorHandle;
	WINDOW			  *msgWindow;
	Sint32			   iconCount;
	Sint32			   preferredRow;
	MENU_POINT		   cursorPos;
	BATTLE_MENU_STATE *navigation;
	WINDOW			  *window;
	Sint32			   firstEntry;
	Sint32			   selectedRow;
	Sint32			   item;
	Sint32			   iconTile;
	Sint32			   unusableReason;
	Sint32			   result;
	Sint32			   i;
	MENU_SPR		   cursorStorage;
	MENU_SPR		  *iconSprite;
	Uint16			   msgText[64];
	Sint32			   iconSlots[BATTLE_LIST_ROWS];
	MENU_SPR		   sprites[BATTLE_LIST_ROWS];

	state = WINDOW_STATE;
	drawnFirstEntry = -1;
	drawnRow = -1;
	cursorHandle = syVRAMCacheAllocateEntry(128);
	msgWindow = mnWindowCreate(0, 5, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	iconCount = 0;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	window =
		mnWindowCreate(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, MENU_WINDOW_FLAGS);

	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		Sint32 y = i * 2;

		iconSprite = &sprites[i];
		MENU_SPR_INIT(sprites[i], OBJ_SIZE_16X16);
		iconSprite->oam.f.x = window->x * 8 + 8;
		iconSprite->oam.f.y = (y + window->y) * 8 + 4;
	}

	/* Reserve one glyph slot per visible row; paging reloads these same
	   slots. */
	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		iconSlots[i] = syVRAMCacheAllocateEntry(128);
		sprites[i].oam.f.tile = syVRAMCacheReload(iconSlots[i], -1);
	}

	/* The border's tees go into tiles 0, 1, 16 and 17 of the second text range,
	   which nothing shows here: in battle they are BG1's 8bpp tiles
	   COMMAND_ROW_ICON_TILE and COMMAND_ROW_ICON_TILE + 8, the first command
	   icon's top-left and bottom-left, and BG1 is off while a list is open. The
	   command row draws its icons there again when it opens next. */
	mnVRAMCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST);
	mnVRAMCopyTile(WINDOW_TILE_BORDER_TOP_TEE, TEXT_EXTRA_TILE_FIRST + 1);
	mnVRAMCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 16);
	mnVRAMCopyTile(WINDOW_TILE_BORDER_BOTTOM_TEE, TEXT_EXTRA_TILE_FIRST + 17);

	for(;;) {
		/* FAKEMATCH(temporary): sprite holds &cursorStorage; as &cursorStorage
		 * at each use. */
		MENU_SPR *sprite = &cursorStorage;

		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
				window->width - 2, 1, WINDOW_PALETTE);
			mnTextFillCanvasBackground();
			if(count != 0) {
				if(mnItemCheckBattleUse(
					   member, entries[firstEntry + selectedRow]) ==
					BATTLE_ITEM_NOT_IN_BATTLE) {
					BATTLE_MENU_MESSAGE_COPY(MsgBattleItemUnavailable, msgText,
						BATTLE_MENU_MESSAGE_CAPACITY);
				} else {
					BATTLE_MENU_MESSAGE_COPY(
						(entries[firstEntry + selectedRow] &
							INVENTORY_ITEM_ID_MASK) +
							MsgItemPlainName,
						msgText, BATTLE_MENU_MESSAGE_CAPACITY);
				}
			} else {
				BATTLE_MENU_MESSAGE_COPY(
					MsgBattleNoItems, msgText, BATTLE_MENU_MESSAGE_CAPACITY);
			}
			mnTextDrawWideStringAtOffset(msgText, msgWindow, 0, 4);
			drawnRow = selectedRow;

			/* Cursor moves update help; only a page change reloads the row
			   icons. */
			if(firstEntry != drawnFirstEntry) {
				mnWindowSpriteRedrawSavedRect(window);
				for(i = 0; i < BATTLE_LIST_ROWS; i++) {
					item = entries[firstEntry + i];
					if(item == 0) {
						break;
					}
					gmItemGetDirectStub(item);
					mnTextSetColor(TEXT_COLOR_WHITE);
					if(mnItemCheckBattleUse(member, item) !=
						BATTLE_ITEM_USABLE) {
						mnTextSetColor(TEXT_COLOR_YELLOW);
					} else if((item & INVENTORY_BROKEN) != 0) {
						mnTextSetColor(TEXT_COLOR_RED);
					}
					mnTextDrawMessageAsTiles(
						(item & INVENTORY_ITEM_ID_MASK) + MsgItemName, window,
						16, i * 16);
					mnTextSetColor(TEXT_COLOR_WHITE);
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
					if(i == firstEntry / BATTLE_LIST_ROWS) {
						iconTile = i + PAGE_NUMBER_LIT_TILE;
					}
					mnWindowSetTilemapEntry(window, iconTile,
						window->width -
							(count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS +
							i - 2,
						-1, 0);
				}
			}

			mnWindowHighlightRect(window->x + 1,
				window->y + selectedRow * 2 + 1, window->width - 2, 1,
				HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		/* Each tick: the current page's number lit 12 ticks in 16, and the
		   page arrows on the window's top edge (y -1). */
		if(count > BATTLE_LIST_ROWS) {
			for(i = 0; i < (count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS;
				i++) {
				iconTile = i + PAGE_NUMBER_TILE;
				if((TickCount & 15) <= 11) {
					if(i == firstEntry / BATTLE_LIST_ROWS) {
						iconTile = i + PAGE_NUMBER_LIT_TILE;
					}
				}
				mnWindowSetTilemapEntry(window, iconTile,
					window->width -
						(count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS + i -
						2,
					-1, 0);
			}
			mnWindowSetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
				window->width -
					(count + BATTLE_LIST_ROWS - 1) / BATTLE_LIST_ROWS - 3,
				-1, 0);
			mnWindowSetTilemapEntry(
				window, LIST_TILE_PAGES_RIGHT, window->width - 2, -1, 0);
			mnWindowMarkTilemapRow(state, window->y - 1);
		}

		for(i = 0; i < iconCount; i++) {
			iconSprite = &sprites[i];
			syOAMAddSpriteAtDepth(iconSprite, OAM_DEPTH_WINDOW);
		}

		MENU_PLACE_CURSOR(cursorPos, window, selectedRow, 2, sprite);
		sprite->oam.f.tile =
			syVRAMCacheReload(cursorHandle, (Sint32)PointingHandTiles);
		sprite->oam.f.x = cursorPos.x + ((TickCount & 4) >> 1) - 4;
		sprite->oam.f.y = cursorPos.y - ((TickCount & 4) >> 2) - 8;
		if(count != 0) {
			syOAMAddSpriteAtDepth(sprite, OAM_DEPTH_WINDOW + 2);
		}

		BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
		navigation = BattleMenuStateWork;

		/* A takes a usable item; a broken or refused one plays the error sound
		   and shows why. */
		if((PerPush & KEY_A) != 0) {
			if(count != 0) {
				result = firstEntry + selectedRow;
				gmItemGetDirectStub(entries[result]);
				unusableReason = BATTLE_ITEM_USABLE;
				if((entries[result] & INVENTORY_BROKEN) == 0) {
					unusableReason =
						mnItemCheckBattleUse(member, entries[result]);
					if(unusableReason == BATTLE_ITEM_USABLE) {
						break;
					}
				}
				sySoundPlayStub(SE_MENU_ERROR);
				if(unusableReason == BATTLE_ITEM_NOT_IN_BATTLE) {
					BATTLE_MENU_MESSAGE_COPY(MsgBattleItemUnavailable, msgText,
						BATTLE_MENU_MESSAGE_CAPACITY);
				} else if((entries[result] & INVENTORY_BROKEN) != 0) {
					BATTLE_MENU_MESSAGE_COPY(MsgBattleItemBroken, msgText,
						BATTLE_MENU_MESSAGE_CAPACITY);
				} else {
					BATTLE_MENU_MESSAGE_COPY(MsgBattleItemNoEffect, msgText,
						BATTLE_MENU_MESSAGE_CAPACITY);
				}
				mnTextFillCanvasBackground();
				mnTextDrawWideStringAtOffset(msgText, msgWindow, 0, 4);
			} else {
				result = -1;
				break;
			}
		} else if(navigation->timer == 0 || (PerPush & KEY_B) != 0) {
			sySoundPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}

		if(count != 0) {
			if((PerRepeat & KEY_DOWN) != 0) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow++;
				if(selectedRow == BATTLE_LIST_ROWS ||
					firstEntry + selectedRow == count) {
					selectedRow = 0;
				}
				preferredRow = selectedRow;
			} else if((PerRepeat & KEY_UP) != 0) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				selectedRow--;
				if(selectedRow < 0) {
					if(firstEntry ==
						(count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
						selectedRow = count - firstEntry - 1;
					} else {
						selectedRow = BATTLE_LIST_ROWS - 1;
					}
				}
				preferredRow = selectedRow;
			} else if((PerRepeat & KEY_RIGHT) != 0) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry + BATTLE_LIST_ROWS >= count) {
					if(firstEntry != 0) {
						firstEntry = 0;
						selectedRow = preferredRow;
					}
				} else {
					firstEntry += BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry ==
						(count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
						selectedRow = count - firstEntry - 1;
						if(selectedRow > preferredRow) {
							selectedRow = preferredRow;
						}
					}
				}
			} else if((PerRepeat & KEY_LEFT) != 0) {
				sySoundPlayStub(SE_MENU_CURSOR_MOVE);
				syInputResetKeyRepeat();
				if(firstEntry != 0) {
					firstEntry -= BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
				} else {
					firstEntry =
						(count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
					selectedRow = preferredRow;
					if(firstEntry != 0) {
						selectedRow = count - firstEntry - 1;
						if(selectedRow > preferredRow) {
							selectedRow = preferredRow;
						}
					}
				}
			}
		}
		sySynch(1);
	}

	mnWindowClose(msgWindow, 1);
	mnWindowClose(window, 1);
	sySynch(1);
	syVRAMCacheFreeEntry(cursorHandle);
	for(i = 0; i < BATTLE_LIST_ROWS; i++) {
		syVRAMCacheFreeEntry(iconSlots[i]);
	}
	sySynch(1);
	return result;
}
