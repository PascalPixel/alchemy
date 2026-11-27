/* The forge's counter, and the item picker it and scripts use to choose one of
   a party member's items. Part of shop.c. */

/* Runs the forge: the smith's line, then a party member's materials to forge,
   until the player leaves. `actorId` is the actor whose portrait the messages
   show. Called through the stub table; returns 0. */
Sint32 mnForgeRun(Sint32 actorId)
{
	SHOP_STATE *shop;
	WINDOW_SPR *icon;
	WINDOW	   *window;
	Sint32		messageBase;
	Sint32		result;

	mnShopInitState();
	shop = Ram_HeapSlots->menuState;
	shop->shopType = SHOP_TYPE_WEAPONS;
	shop->action = SHOP_ACTION_FORGE;
	shop->keeperGraphics =
		((SPR *)fdActorGetByIdStub(actorId)->spr)->entries[0]->anmId;
	window = mnPortraitOpenWindowStub(shop->keeperGraphics, 0, 0, 0);
	icon = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, window, 0, 0);
	icon->depth = OAM_DEPTH_FRONT;
	icon->motion = WINDOW_SPRITE_MOTION_STILL;
	icon->kind = WINDOW_SPRITE_PLAIN;
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = icon;
	messageBase = (Sint32)MsgRightWhenI;
	/* "Right when I saw those materials, I had to work!" */
	mnShopShowKeeperMessage(messageBase);
	/* "Which item needs my special attention?" */
	mnShopShowKeeperMessage(messageBase + 1);
	result = mnForgeChooseMember();
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	if(result == -1)
		/* "Aren't you the ones mucking about earlier?" */
		mnShopShowKeeperMessage(messageBase + 2);
	mnWindowCloseStub(window, 2);
	mnShopReleaseState();
	return 0;
}

/* Runs the forge's party member choice, the sell counter's. */
Sint32 mnForgeChooseMember(void)
{
	return mnShopChooseMember();
}

/* Lets the player choose the party member's item to forge, on the sell
   counter's grid. */
Sint32 mnForgeSelectItem(Sint32 unitId)
{
	return mnShopSelectSell(unitId);
}

/* The forge's offer for one inventory item: refuses an item it has no recipe
   for, otherwise asks (a rusted weapon takes its own lines), and on yes takes
   the item, redraws the grid and stores the rolled result. Returns 0 when the
   item was taken, else -1. */
Sint32 mnForgeOfferItem(Sint32 unitId, Sint32 slot)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	UNIT	   *unit = syUnitGet(unitId);
	Uint32		inventoryEntry = unit->inventory[slot];
	Sint32		itemId;
	Sint32		condition;
	Sint32		result;

	itemId = INVENTORY_ITEM_ID_MASK;
	itemId &= inventoryEntry;
	condition = mnForgeGetItemCondition(itemId);
	result = -1;

	if(mnForgeGetItemCondition(itemId) == 0) {
		mnShopShowKeeperMessageHidingCursor((Sint32)MsgNoThatSNoGood);
		return result;
	}
	mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
	mnShopShowKeeperMessageHidingCursor(
		condition == FORGE_CONDITION_PROCESSED + 1
			? (Sint32)MsgItSOkIfIWork
			: (Sint32)MsgYouWantItToShine);
	if(mnShopAskYesNo(0) != 0) {
		mnShopShowKeeperMessageHidingCursor(
			condition == FORGE_CONDITION_PROCESSED + 1
				? (Sint32)MsgHmThatSNotRight
				: (Sint32)MsgWellHmIWonder);
	} else {
		mnShopShowKeeperMessageHidingCursor(
			condition == FORGE_CONDITION_PROCESSED + 1
				? (Sint32)MsgWellAllRightIM
				: (Sint32)MsgALittleElbowGrease);
		gmInventoryRemoveStub(unitId, slot);
		mnShopDrawMemberItems(shop->itemWin, unitId);
		itemId = mnForgeRollResult(itemId);
		mnForgeSetItem(itemId);
		result = 0;
	}
	return result;
}

/* Runs the item picker for an item to show, refusing one that cannot be
   unequipped: Naribwe's fortune teller reads the party's fortune from it. */
Sint32 mnItemPickerRunToShow(Sint32 *selectedUnit, Sint32 *selectedSlot)
{
	return mnItemPickerRun(selectedUnit, selectedSlot, ITEM_PICKER_SHOW);
}

/* Runs the item picker for an item to drop, showing each item's sale price. */
Sint32 mnItemPickerRunToDrop(Sint32 *selectedUnit, Sint32 *selectedSlot)
{
	return mnItemPickerRun(selectedUnit, selectedSlot, ITEM_PICKER_DROP);
}

/* Lets the player choose a party member, then one of its items, four party
   members a page, returning the unit and slot through the pointers (both -1
   when cancelled). `itemMode` is what the item is for (ITEM_PICKER_MODE). Holds
   the shop's state while it runs. */
Sint32 mnItemPickerRun(
	Sint32 *selectedUnit, Sint32 *selectedSlot, Sint32 itemMode)
{
	SHOP_STATE *shop;
	Sint32		unitId = 0;
	Sint32		redraw = 1;
	Sint32		memberIndex = 0;
	WINDOW	   *partyWindow;
	WINDOW	   *cursorWindow;
	WINDOW_SPR *cursorAnchor;
	Sint32		itemSlot;
	Sint32		result = 0;

	mnShopInitState();
	shop = Ram_HeapSlots->menuState;
	shop->itemMode = itemMode;
	shop->itemWin = mnWindowCreateStub(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
	partyWindow = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	shop->listWin = partyWindow;
	cursorWindow = mnWindowCreateStub(30, 0, 1, 1, WINDOW_FLAG_INSTANT);
	cursorAnchor = mnWindowSprCreateStub(
		shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, cursorWindow, 0, 0);
	cursorAnchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	cursorAnchor->kind = WINDOW_SPRITE_PLAIN;
	mnCursorSetPosImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursorAnchor;
	shop->cursorFrm = 1;
	mnShowPartyFiguresStub(partyWindow, 2, 0);
	while(1) {
		if(redraw != 0) {
			unitId = shop->partyMemberIds[memberIndex];
			mnCursorPlace(partyWindow,
				(memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
			shop->cursorFrm = 3;
			if(redraw == MENU_GRID_CHANGED_PAGE) {
				mnCreatePartyFiguresStub(memberIndex / SHOP_PARTY_PAGE_MEMBERS *
					SHOP_PARTY_PAGE_MEMBERS);
				sySynch(1);
			}
			mnShopDrawParty(partyWindow, memberIndex, 0);
			mnShopDrawMemberItems(shop->itemWin, unitId);
			redraw = 0;
		}
		sySynch(1);
		if(PerWork.push & KEY_A) {
			if(gmInventoryCountStub(unitId) == 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}
			mnWindowSprPrepareForRedrawStub(partyWindow);
			sySndPlayStub(SE_MENU_CONFIRM);
			itemSlot = mnItemPickerSelectItem(unitId);
			if(itemSlot == -1) {
				shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
				shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
				redraw = MENU_GRID_CHANGED_PAGE;
				continue;
			}
			*selectedUnit = unitId;
			*selectedSlot = itemSlot;
			result = 0;
			break;
		} else if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			*selectedUnit = -1;
			*selectedSlot = -1;
			result = -1;
			break;
		} else {
			redraw = mnGridMoveCursor(
				&memberIndex, shop->partyMemberCnt, SHOP_PARTY_PAGE_MEMBERS);
		}
	}
	mnReleasePartyFiguresStub();
	mnWindowCloseStub(cursorWindow, 2);
	mnWindowCloseStub(partyWindow, 2);
	mnWindowCloseStub(shop->itemWin, 2);
	sySynch(1);
	mnShopReleaseState();
	return result;
}

/* The price window and the item details window fit their edition's text. */
#define ITEM_PICKER_PRICE_WINDOW_X	   BY_LANGUAGE(16, 14, 13, 14, 14, 14)
#define ITEM_PICKER_PRICE_WINDOW_WIDTH BY_LANGUAGE(14, 16, 17, 16, 16, 16)

/* The item details window's width, in tiles, fitted to its edition's text. */
#define ITEM_PICKER_DETAILS_WINDOW_WIDTH BY_LANGUAGE(13, 16, 18, 18, 16, 18)

/* Lets the player choose one of the party member's items, refusing with a
   message one that cannot be dropped or removed (gmInventoryCheckDiscard or
   gmInventoryCheckUnequip, by the picker's mode). Returns the slot, or -1 when
   cancelled. */
Sint32 mnItemPickerSelectItem(Sint32 unitId)
{
	WINDOW	   *descriptionWindow;
	SHOP_STATE *shop;
	WINDOW	   *priceWindow;
	UNIT	   *unit;
	Sint32		slot;
	Sint32		inventoryCount;
	Bool		redraw;
	Sint32		result;
	Sint32		itemId;
	Sint32		refusal;
	WINDOW	   *window;
	Sint32		column;
	Sint32		row;

	shop = Ram_HeapSlots->menuState;
	unit = syUnitGet(unitId);
	inventoryCount = 1;
	redraw = TRUE;
	priceWindow = mnWindowCreateStub(ITEM_PICKER_PRICE_WINDOW_X, 8,
		ITEM_PICKER_PRICE_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	descriptionWindow = mnWindowCreateStub(0, 5, 30, 3, WINDOW_FLAG_INSTANT);

	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
	shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
	slot = 0;
	while(1) {
		if(redraw != 0) {
			redraw = FALSE;
			inventoryCount = gmInventoryCountStub(unitId);
			if(slot > inventoryCount - 1)
				slot = inventoryCount - 1;
			itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
			window = shop->itemWin;
			column = slot % SHOP_INVENTORY_COLUMNS;
			row = slot / SHOP_INVENTORY_COLUMNS;
			mnCursorPlace(window, column * 16, row * 16 + 8);
			shop->cursorFrm = 3;
			mnItemPickerDrawItem(priceWindow, unitId, slot);
			mnShopDrawMessage(
				descriptionWindow, itemId + (Sint32)MsgItemDescription);
		}
		sySynch(1);
		if(PerWork.push & KEY_A) {
			sySynch(1);
			if(shop->itemMode == ITEM_PICKER_DROP)
				refusal = gmInventoryCheckDiscardStub(unitId, slot);
			else
				refusal = gmInventoryCheckUnequipStub(unitId, slot);
			if(refusal == 0) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = slot;
				break;
			}
			if(refusal & INVENTORY_REFUSE_CANNOT_UNEQUIP)
				mnMessageOpenWindowStub((Sint32)MsgCannotRemoveIt, 8, 1, 0);
			else if(refusal & INVENTORY_REFUSE_INDISPENSABLE)
				mnMessageOpenWindowStub((Sint32)MsgCannotDropIt, 8, 1, 0);
			sySndPlayStub(SE_MENU_CANCEL);
			while(mnMessageAllTypedStub() == 0)
				sySynch(1);
			mnMessageCloseFinishedWindowsStub();
			continue;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(PerWork.data & KEY_SELECT) {
			Uint32	inventoryEntry = unit->inventory[slot];
			WINDOW *detailsWindow;

			sySndPlayStub(SE_RECOVERY);
			detailsWindow = mnWindowCreateStub(0, 9,
				ITEM_PICKER_DETAILS_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
			mnItemMenuDrawItemDetailsStub(detailsWindow, inventoryEntry);
			shop->memberSprsShown = 0;
			while(PerWork.data & KEY_SELECT)
				sySynch(1);
			shop->memberSprsShown = 1;
			mnWindowCloseStub(detailsWindow, 2);
			mnWindowSprPrepareForRedrawStub(shop->listWin);
			sySynch(1);
			/* German, Spanish and Italian also redraw the item window here. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_ITALIAN
			mnShopDrawMemberItems(shop->itemWin, unitId);
			sySynch(1);
#endif
			/* Japanese does not redraw the cursor or the two windows here. */
#if LANGUAGE != LANGUAGE_JAPANESE
			redraw = TRUE;
#endif
			continue;
		}
		if(PerWork.repeat & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			slot--;
			slot = (slot + inventoryCount) % inventoryCount;
			redraw = TRUE;
		}
		if(PerWork.repeat & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			slot++;
			slot = (slot + inventoryCount) % inventoryCount;
			redraw = TRUE;
		}
		if(PerWork.repeat & KEY_UP) {
			slot -= SHOP_INVENTORY_COLUMNS;
			if(slot < 0)
				slot += INVENTORY_SLOTS;
			while(slot >= inventoryCount)
				slot -= SHOP_INVENTORY_COLUMNS;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = TRUE;
		}
		if(PerWork.repeat & KEY_DOWN) {
			slot += SHOP_INVENTORY_COLUMNS;
			if(slot >= inventoryCount)
				slot -= INVENTORY_SLOTS;
			while(slot < 0)
				slot += SHOP_INVENTORY_COLUMNS;
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			redraw = TRUE;
		}
	}
	mnWindowCloseStub(descriptionWindow, 2);
	mnWindowCloseStub(priceWindow, 2);
	sySynch(1);
	return result;
}

#undef ITEM_PICKER_PRICE_WINDOW_X
#undef ITEM_PICKER_PRICE_WINDOW_WIDTH
#undef ITEM_PICKER_DETAILS_WINDOW_WIDTH

/* The note sits under the item's name; the price line reads "Price", the price,
   then "Coins". Japanese indents the note by 8 pixels; Japanese and French draw
   the price and the coins label 8 pixels further left, Spanish the coins label
   8 pixels further right. */
#define SHOP_PRICE_NOTE_X		   JAPANESE_OR(8, 0)
#define ITEM_PICKER_PRICE_NUMBER_X BY_LANGUAGE(32, 40, 40, 40, 32, 40)
#define SHOP_PRICE_COINS_X		   BY_LANGUAGE(72, 80, 80, 88, 72, 80)

/* Draws the slot's item name and, below it, why it cannot be dropped or
   removed, or, for an item to drop, what the whole stack would sell for. Called
   by mnItemPickerSelectItem each time the cursor moves. */
void mnItemPickerDrawItem(WINDOW *window, Sint32 unitId, Sint32 slot)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	UNIT	   *unit = syUnitGet(unitId);
	Sint32		itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	Uint32		inventoryEntry = unit->inventory[slot];
	Sint32		quantity = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;
	Sint32		refusal;
	Sint32		price;
	Sint32		labelMessage;

	if(window == 0)
		return;
	mnWindowSprRedrawSavedRectStub(window);
	mnTextDrawMessageAsTilesStub(itemId + (Sint32)MsgItemName, window, 0, 0);
	if(shop->itemMode == ITEM_PICKER_DROP)
		refusal = gmInventoryCheckDiscardStub(unitId, slot);
	else
		refusal = gmInventoryCheckUnequipStub(unitId, slot);
	if(refusal & INVENTORY_REFUSE_INDISPENSABLE) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgCannotDrop, window, SHOP_PRICE_NOTE_X, 8);
	} else if(refusal & INVENTORY_REFUSE_CANNOT_UNEQUIP) {
		mnTextDrawMessageAsTilesStub(
			(Sint32)MsgCannotRemove, window, SHOP_PRICE_NOTE_X, 8);
	} else if(shop->itemMode == ITEM_PICKER_DROP) {
		price = mnShopSalePrice(unit->inventory[slot]) * quantity;
		labelMessage = (Sint32)MsgPriceHeading;
		mnTextDrawMessageAsTilesStub(labelMessage, window, 8, 8);
		mnTextDrawNumInWindowStub(
			price, 5, (Sint32)window, ITEM_PICKER_PRICE_NUMBER_X, 8);
		/* Five messages before the price heading: MsgCoins. */
		mnTextDrawMessageAsTilesStub(
			labelMessage - 5, window, SHOP_PRICE_COINS_X, 8);
	}
}

#undef SHOP_PRICE_NOTE_X
#undef ITEM_PICKER_PRICE_NUMBER_X
#undef SHOP_PRICE_COINS_X

/* Queues each loaded icon's sprite while the grid is visible. Runs every frame
   as an event. */
void mnItemIconGridDraw(void)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;

	if(shop->iconsVisible) {
		ICON_SPR *sprite = shop->iconSprs;
		Uint16	 *iconSlot = shop->iconSlots;
		Sint32	  i;

		for(i = 0; i < INVENTORY_SLOTS; i++) {
			if(*iconSlot++ != ICON_NONE)
				syOAMAddSprAtDepth(sprite, OAM_DEPTH_WINDOW);
			sprite++;
		}
	}
}

/* Empties the grid, hides it and starts its drawing event. */
void mnItemIconGridInit(void)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	Sint32		i;

	for(i = 0; i < INVENTORY_SLOTS; i++)
		shop->iconSlots[i] = ICON_NONE;
	{
		Sint32 hidden = 0;
		shop->iconsVisible = hidden;
	}
	sySetEvent(mnItemIconGridDraw, EVENT_PASS_FRAME + 10);
}

/* Stops the drawing event and frees the icons' VRAM cache entries. */
void mnItemIconGridRelease(void)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	Sint32		i;

	syCloseEvent(mnItemIconGridDraw);
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		if(shop->iconSlots[i] != ICON_NONE)
			syVRAMCacheFreeEntry(shop->iconSlots[i]);
	}
}

/* Places icon `index` at (x, y) screen pixels. */
void mnItemIconGridSetPos(Sint32 index, Sint32 x, Sint32 y)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;

	shop->iconSprs[index].attr.x = x;
	shop->iconSprs[index].attr.y = y;
}

/* Shows the grid (1) or hides it (0). */
void mnItemIconGridSetVisible(Sint32 visible)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;

	shop->iconsVisible = visible;
}

/* Releases the item icons' VRAM entries, then loads the icons of up to count
   items with the ICON_ layers in `mode` (an empty item keeps its old entry) and
   gives each its sprite attributes and tile. */
void mnItemIconGridLoadItems(Uint16 *items, Sint32 count, Sint32 mode)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	ICON_SPR   *sprite;
	Sint32		entry;
	Sint32		i;

	for(i = 0; i < INVENTORY_SLOTS; i++) {
		if(shop->iconSlots[i] != ICON_NONE) {
			syVRAMCacheFreeEntry(shop->iconSlots[i]);
			shop->iconSlots[i] = ICON_NONE;
		}
	}
	sprite = shop->iconSprs;
	for(i = 0; i < INVENTORY_SLOTS && i != count; i++, sprite++) {
		if(items[i] != 0) {
			entry = syVRAMCacheFindFreeEntry();
			if(entry < VRAM_CACHE_ENTRY_COUNT) {
				Uint32 *attr;

				mnItemIconLoadLayersIntoSlotStub(items[i], mode, entry);
				attr = (Uint32 *)&sprite->attr;
				*attr++ = WINDOW_SPRITE_SIZE_16X16;
				*attr = 0;
				sprite->attr.tile = VramCacheEntries[entry].ofs >> 5;
			}
			shop->iconSlots[i] = entry;
		}
	}
}

/* Shows a party member's INVENTORY_SLOTS inventory icons at (x, y) pixels
   inside `window`, SHOP_INVENTORY_COLUMNS to a row, 16 pixels apart. */
void mnItemIconGridShowInventory(
	WINDOW *window, Sint32 x, Sint32 y, Sint32 unitId)
{
	UNIT  *unit = syUnitGet(unitId);
	Sint32 left = window->x * 8 + x + 8;
	Sint32 top = window->y * 8 + y + 8;
	Sint32 i;

	mnItemIconGridLoadItems(unit->inventory, INVENTORY_SLOTS,
		ICON_BASE | ICON_COUNT_ABOVE_ONE | ICON_BROKEN | ICON_EQUIPPED);
	for(i = 0; i < INVENTORY_SLOTS; i++)
		mnItemIconGridSetPos(i, left + i % SHOP_INVENTORY_COLUMNS * 16,
			top + i / SHOP_INVENTORY_COLUMNS * 16);
	mnItemIconGridSetVisible(1);
}
