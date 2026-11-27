/* @file */
/* Buying: the stock list, the money and party windows, quantities and prices.
 */
/* Part of shop.c. */

#include "shop_internal.h"

/* Runs the Buy and Artifacts counters: the player browses the stock seven */
/* items a page, picks one, chooses who gets it and how many, and pays. Checks
 */
/* room in the pack and coins first, and warns when the party member cannot */
/* equip it. An artifact bought takes its stock counter down, and the list is */
/* rebuilt. Repeats until B. */
s32 mnShopSelectBuy(void) {
	SHOP_STATE *shop = MenuState;
	WINDOW *priceWindow = 0;
	WINDOW *stockWindow;
	WINDOW *messageWindow;
	WINDOW *partyWindow;
	s32 selected = 0;
	s32 quantity;
	ITEM_DEFINITION *selectedDefinition;
	s32 repromptRecipient;
	WINDOW *itemWindow;
	s32 redraw;
	s32 selectionIndex;
	s32 stockCount;
	s32 itemId;
	ITEM_DEFINITION *stockDefinition;
	s32 selectionResult;
	s32 previousIndex;
	s32 unitId;
	s32 capacitySlot;
	s32 removedCount;
	s16 *stockIds;

	shop->itemWin = 0;
	shop->moneyWin =
		mnWindowCreateStub(30 - SHOP_MONEY_WINDOW_WIDTH, 7,
						   SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();
	priceWindow =
		mnWindowCreateStub(0, 8, SHOP_PRICE_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
	selectionIndex = selected;
	stockWindow = mnWindowCreateStub(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
	shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
	messageWindow = mnWindowCreateStub(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
	redraw = 1;

	while(1) {
		stockCount = shop->stockCnt;
		if(redraw != 0) {
			stockIds = shop->stockItemIds;
			itemId = stockIds[selectionIndex];
			stockDefinition = gmItemGetDirectStub(itemId);
			redraw = 0;
			mnCursorPlace(stockWindow,
						  selectionIndex % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
			shop->cursorFrm = 4;
			mnShopDrawStock(stockWindow, selectionIndex);
			mnShopDrawMessage(messageWindow, itemId + MsgItemPlainName);
			mnWindowSprRedrawSavedRectStub(priceWindow);
			mnShopDrawItemPrice(priceWindow, itemId, stockDefinition->price,
								SHOP_PRICE_KIND_BUY);
		}
		if((KeysPressed & KEY_A) != 0) {
			selected = selectionIndex;
			sySndPlayStub(SE_MENU_CONFIRM);
			selectionResult = 0;
			break;
		}
		if((KeysPressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}
		if((KeysRepeat & KEY_LEFT) != 0) {
			previousIndex = selectionIndex;
			selectionIndex--;
			selectionIndex = (selectionIndex + stockCount) % stockCount;
			if(previousIndex != selectionIndex) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = 1;
			}
		}
		if((KeysRepeat & KEY_RIGHT) != 0) {
			previousIndex = selectionIndex;
			selectionIndex++;
			selectionIndex = (selectionIndex + stockCount) % stockCount;
			if(previousIndex != selectionIndex) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = 1;
			}
		}
		if((KeysRepeat & KEY_UP) != 0 &&
		   selectionIndex - SHOP_STOCK_PAGE_ITEMS >= 0) {
			selectionIndex -= SHOP_STOCK_PAGE_ITEMS;
			redraw = 1;
		}
		if((KeysRepeat & KEY_DOWN) != 0) {
			if(selectionIndex + SHOP_STOCK_PAGE_ITEMS <
			   (stockCount + (SHOP_STOCK_PAGE_ITEMS - 1)) /
				   SHOP_STOCK_PAGE_ITEMS * SHOP_STOCK_PAGE_ITEMS) {
				selectionIndex += SHOP_STOCK_PAGE_ITEMS;
				redraw = 1;
			}
			if(selectionIndex > stockCount - 1)
				selectionIndex = stockCount - 1;
		}
		sySynch(1);
	}

	mnWindowCloseStub(messageWindow, 2);
	mnWindowCloseStub(stockWindow, 2);
	sySynch(1);
	if(selectionResult != 0)
		goto close;

	shop->selectedItem = shop->stockItemIds[selected];
	/* "Which one of you gets this?" */
	mnShopShowKeeperMessage(MsgWhoGetsThis);
	selectedDefinition = gmItemGetDirectStub(shop->selectedItem);
	quantity = 1;
	unitId = 0;
	repromptRecipient = 0;
	selectionIndex = 0;
	partyWindow = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
	mnCreatePartyFiguresStub(partyWindow, 2, 0, 8, 0);
	itemWindow = mnWindowCreateStub(16, 11, 14, 9, WINDOW_FLAG_INSTANT);
	redraw = 1;

	while(1) {
		if(repromptRecipient != 0) {
			repromptRecipient = 0;
			mnShopShowKeeperMessage(MsgWhoGetsThis);
			redraw = 1;
		}
		if(redraw != 0) {
			redraw = 0;
			selectionIndex =
				(selectionIndex + shop->partyMemberCnt) % shop->partyMemberCnt;
			unitId = shop->partyMemberIds[selectionIndex];
			mnCursorPlace(partyWindow, selectionIndex * 24 - 12, 0);
			shop->cursorFrm = 3;
			mnShopDrawParty(partyWindow, selectionIndex, shop->selectedItem);
			if(gmItemGetEquipmentGroupStub(shop->selectedItem) ==
			   EQUIPMENT_GROUP_NONE)
				mnShopDrawCarriedItem(itemWindow, unitId, shop->selectedItem);
			else
				mnShopDrawEquipComparison(itemWindow, unitId,
										  shop->selectedItem);
		}
		if((KeysPressed & KEY_A) != 0) {
			/* Test for room by adding one and taking it out again, before the
			 */
			/* coins are checked; a full bag and a full stack get different
			 * lines */
			capacitySlot = gmInventoryAddItemStub(unitId, shop->selectedItem);
			if(capacitySlot < 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(shop->selectedItem,
										  MESSAGE_ARGUMENT_ITEM);
				if(gmInventoryCountStub(unitId) == INVENTORY_SLOTS)
					/* "Your pack is full, (the member). Who will you give this
					 * to?" */
					mnShopShowKeeperMessage(MsgPackFull);
				else
					/* "You can't carry any more of this item, (the member), so
					 * who */
					/* gets it?" */
					mnShopShowKeeperMessage(MsgCannotCarryItem);
				continue;
			}
			gmInventoryRemoveStub(unitId, capacitySlot);
			if((u32)selectedDefinition->price >
			   MainRoutineStateWork.holdings.coins) {
				sySndPlayStub(SE_MENU_CANCEL);
				/* "Oh! That's a fine choice, but you don't have enough money."
				 */
				mnShopShowKeeperMessageHidingCursor(MsgBuyNotEnoughMoney);
				selectionResult = -1;
				break;
			}
			if(gmItemIsCompatibleWithUnitStub(unitId, shop->selectedItem) ==
			   0) {
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				/* "You can't equip this, (the member). Do you still want it?"
				 */
				mnShopShowKeeperMessage(MsgCannotEquipStillWant);
				if(mnShopAskYesNo(0) != 0) {
					repromptRecipient = 1;
					continue;
				}
			}
			sySndPlayStub(SE_MENU_CONFIRM);
			sySynch(1);
			quantity = mnShopSelectBuyCount(unitId, shop->selectedItem);
			repromptRecipient = 1;
			if(quantity == -1)
				continue;
			mnShopBuyDone(unitId, shop->selectedItem, quantity);
			mnShopGiveGameTicket(partyWindow, itemWindow);
			selectionResult = 0;
			break;
		}
		if((KeysPressed & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}
		if((KeysRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectionIndex--;
			redraw = 1;
		}
		if((KeysRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectionIndex++;
			redraw = 1;
		}
		sySynch(1);
	}

	mnReleasePartyFiguresStub();
	mnWindowCloseStub(itemWindow, 2);
	mnWindowCloseStub(partyWindow, 2);
	sySynch(1);
	/* An artifact bought leaves the world's stock, one for each bought, and the
	 */
	/* list is made again without what is gone. */
	if(selectionResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS) {
		for(removedCount = 0; removedCount < quantity; removedCount++)
			gmItemAdjustRareItemStockStub(shop->selectedItem, -1);
		if(mnShopListAvailableArtifacts() == 0)
			goto close;
		if(selected > shop->stockCnt - 1)
			selected = shop->stockCnt - 1;
	}
	/* "Anything else?" */
	mnShopShowKeeperMessage(MsgBuyAnythingElse);
	goto next_page;

close:
	mnWindowCloseStub(priceWindow, 2);
	mnWindowCloseStub(shop->moneyWin, 2);
	sySynch(1);
	return 0;
}

/* Draws a page of seven stock items with their prices, the page arrows, and */
/* the chosen item lifted and pulsing. */
void mnShopDrawStock(WINDOW *window, s32 selected) {
	SHOP_STATE *shop;
	s16 *itemIds;
	s32 itemCount;
	s32 stockIndex;
	u32 pageSlot;
	s16 itemId;
	ITEM_DEFINITION *item;
	WINDOW_SPR *icon;

	shop = MenuState;
	itemIds = shop->stockItemIds;
	itemCount = shop->stockCnt;
	stockIndex = selected - selected % SHOP_STOCK_PAGE_ITEMS;
	if(window != 0) {
		mnWindowSprPrepareForRedrawStub(window);
		/* The arrows show the shop's own icons, which closing the window
		 * leaves. */
		if(stockIndex != 0) {
			icon = mnWindowSprCreateStub(
				shop->prevPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 216, -16);
			icon->kind = WINDOW_SPRITE_PLAIN;
			icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
			icon->anmStep = 0;
		}
		if(stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount) {
			icon = mnWindowSprCreateStub(
				shop->nextPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 216, 24);
			icon->kind = WINDOW_SPRITE_PLAIN;
			icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
			icon->anmStep = 0;
		}
		pageSlot = 0;
		if(stockIndex < (u32)itemCount) {
			/* Each price sits 16 pixels right of its item. */
			for(;
				pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (u32)itemCount;
				pageSlot++, stockIndex++) {
				itemId = itemIds[stockIndex];
				item = gmItemGetDirectStub(itemId);
				icon = mnWindowSprCreateLoadedStub(itemId, ICON_BASE, window,
												   pageSlot * 32, 0);
				icon->depth = OAM_DEPTH_FRONT - 3;
				if(stockIndex == selected) {
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 10;
					icon->depth = OAM_DEPTH_FRONT - 2;
				}
				icon = mnShopCreatePriceSpr(item->price, window,
											pageSlot * 32 + 16, 0);
				icon->depth = OAM_DEPTH_FRONT - 4;
			}
		}
	}
}

/* Draws the party's coins in the coins window, in the edition's layout. */
void mnShopDrawMoney(void) {
	SHOP_STATE *shop;
	WINDOW *window;

	shop = MenuState;
	window = shop->moneyWin;
	if(window != 0) {
#if LANGUAGE != LANGUAGE_JAPANESE
		mnTextDrawMessageAsTilesStub(MsgYourCoins, window, 0, 0);
		mnTextDrawNumInWindowStub(MainRoutineStateWork.holdings.coins, 6,
								  window, 32, 8);
#else
		s32 msg = MsgYourCoins;

		mnTextDrawMessageAsTilesStub(msg, window, 0, 0);
		mnTextDrawNumInWindowStub(MainRoutineStateWork.holdings.coins, 6,
								  window, 0, 8);
		mnTextDrawMessageAsTilesStub(msg - 2, window, 48, 8); /* MsgCoins */
#endif
	}
}

/* Draws the item's name and below it its price. With a price of 0 it says */
/* instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or that it is */
/* not broken (SHOP_PRICE_KIND_REPAIR). */
void mnShopDrawItemPrice(WINDOW *window, s32 itemId, s32 price, s32 priceKind) {
	s32 labelMessage;

	if(window == 0)
		return;
	goto draw;
cannot_buy:
	mnTextDrawMessageAsTilesStub(MsgCannotBuyThat, window, SHOP_ITEM_NOTE_X, 8);
	return;
show_price:
	labelMessage = MsgPrice;
	mnTextDrawMessageAsTilesStub(labelMessage, window, SHOP_ITEM_NOTE_X, 8);
	labelMessage -= 3; /* MsgCoins */
	mnTextDrawNumInWindowStub(price, 5, window, SHOP_PRICE_NUMBER_X, 8);
	mnTextDrawMessageAsTilesStub(labelMessage, window, SHOP_PRICE_LABEL_X, 8);
	return;
draw:
	mnWindowSprRedrawSavedRectStub(window);
	mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window, 0, 0);
	if(price != 0)
		goto show_price;
	switch(priceKind) {
	case SHOP_PRICE_KIND_SELL:
		goto cannot_buy;
	case SHOP_PRICE_KIND_REPAIR:
		mnTextDrawMessageAsTilesStub(MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
		break;
	default:
		goto show_price;
	}
}

/* Shows `message`, an item's description, in the help window; a NULL window is
 */
/* ignored. */
void mnShopDrawMessage(WINDOW *window, s32 message) {
	if(window != 0) {
#if TBS_PATCH_1
		mnWindowSprRedrawSavedRectStub(window);
#else
		mnWindowSprClearListStub(window);
		sySynch(1);
#endif
		SHOP_DRAW_MESSAGE(message, window, 0, 0);
	}
}

/* Shows the chosen party member running in place and the others standing, and
 */
/* draws at four fifths of their size the party members who cannot equip */
/* `itemId`. */
void mnShopDrawParty(WINDOW *window, s32 selected, s32 itemId) {
	SHOP_STATE *shop = MenuState;
	s32 memberIndex;

	if(window != 0) {
		for(memberIndex = 0; memberIndex < shop->partyMemberCnt;
			memberIndex++) {
			if(memberIndex == selected)
				fdSprStartAnimationStub(shop->partyMemberIcons[memberIndex],
										ANIM_PARTY_RUN);
			else
				fdSprStartAnimationStub(shop->partyMemberIcons[memberIndex],
										ANIM_STAND);
			shop->partyMemberScale[memberIndex] = FIXED_ONE;
			if(!gmItemIsCompatibleWithUnitStub(
				   shop->partyMemberIds[memberIndex], itemId))
				shop->partyMemberScale[memberIndex] = FIXED_ONE * 4 / 5;
		}
	}
}

/* Shows how Attack, Defense and Agility would change if the party member wore
 */
/* the item, with an arrow up or down beside each that changes and the name of
 */
/* the item it would replace. The item is tried on in that item's slot and the
 */
/* slot put back. */
void mnShopDrawEquipComparison(WINDOW *window, s32 unitId, s32 itemId) {
	SHOP_STATE *shop = MenuState;
	UNIT *unit = gmUnitGetStub(unitId);
	ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
	s32 replacedItemId = -1;
	s32 previewSlot;
	s32 savedEntry;
	s32 index;
	s32 baseStats[4];
	s32 previewStats[4];

	if(window == 0)
		return;
	mnWindowSprPrepareForRedrawStub(window);
	if(!gmItemCanUnitEquip(unitId, itemId)) {
		mnTextDrawMessageStub(MsgShopCannotEquip, window, 8, 24);
		return;
	}
	previewSlot = gmInventoryFindEquippedStub(unitId, item->type);
	/* With nothing of the type equipped, borrow the first slot that is not */
	/* equipped, else the first Psynergy item's, else slot 0. */
	if(previewSlot == -1) {
		for(index = 0; index < INVENTORY_SLOTS; index++) {
			if(!(unit->inventory[index] & INVENTORY_EQUIPPED))
				break;
		}
		if(index == INVENTORY_SLOTS) {
			for(index = 0; index < INVENTORY_SLOTS; index++) {
				if(gmItemGetDirectStub(unit->inventory[index])->type ==
				   ITEM_TYPE_PSYNERGY)
					break;
			}
			if(index == INVENTORY_SLOTS)
				index = 0;
		}
		previewSlot = index;
	}
	else {
		replacedItemId = unit->inventory[previewSlot] & INVENTORY_ITEM_ID_MASK;
	}
	/* Luck is worked out with the rest but not shown. */
	savedEntry = unit->inventory[previewSlot];
	unit->inventory[previewSlot] = itemId | INVENTORY_EQUIPPED;
	gmUnitCalcStub(unitId);
	previewStats[0] = unit->attack;
	previewStats[1] = unit->defense;
	previewStats[2] = unit->agility;
	previewStats[3] = unit->luck;
	unit->inventory[previewSlot] = savedEntry;
	gmUnitCalcStub(unitId);
	baseStats[0] = unit->attack;
	baseStats[1] = unit->defense;
	baseStats[2] = unit->agility;
	baseStats[3] = unit->luck;
	/* Attack, Defense and Agility, each with the shop's own arrow icon. */
	for(index = 0; index <= 2; index++) {
		if(baseStats[index] > previewStats[index])
			mnWindowSprCreateStub(shop->statDownIcon, WINDOW_SPRITE_SIZE_16X16,
								  window, 56, index * 16 - 4)
				->kind = WINDOW_SPRITE_PLAIN;
		else if(baseStats[index] < previewStats[index])
			mnWindowSprCreateStub(shop->statUpIcon, WINDOW_SPRITE_SIZE_16X16,
								  window, 56, index * 16 - 4)
				->kind = WINDOW_SPRITE_PLAIN;
		mnTextDrawNumInWindowStub(baseStats[index], 3, window, 32, index * 16);
		if(baseStats[index] != previewStats[index])
			mnTextDrawNumInWindowStub(previewStats[index], 3, window, 72,
									  index * 16);
		mnTextDrawMessageAsTilesStub(MsgShopStatLabel + index, window, 0,
									 index * 16);
		mnWindowDrawDividerLineStub(window, 0, index * 2 + 2, 13,
									index * 2 + 2);
	}
	if(replacedItemId != -1)
		mnTextDrawMessageAsTilesStub(replacedItemId + MsgItemName, window, 0,
									 48);
}
/* Shows the quantity already carried and the party member's inventory icons. */
void mnShopDrawCarriedItem(WINDOW *window, s32 unitId, s32 itemId) {
	UNIT *unit;
	s32 x;
	s32 y;
	s32 itemIndex;
	s32 slot;
	WINDOW_SPR *icon;

	unit = gmUnitGetStub(unitId);
	x = 8;
	y = 8;
	if(window != 0) {
		mnWindowSprPrepareForRedrawStub(window);
		slot = gmInventoryFindStub(unitId, itemId);
		if(slot != -1) {
			mnMessageArgumentPushStub(
				(unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1,
				MESSAGE_ARGUMENT_NUMBER);
			mnTextDrawMessageAsTilesStub(MsgYouHave, window, 0, 0);
		}
		else {
			mnTextDrawMessageAsTilesStub(MsgShopNoneInStock, window, 0, 0);
		}
		itemIndex = 0;
		if(unit->inventory[itemIndex] != 0) {
			for(;;) {
				icon = mnWindowSprCreateLoadedStub(
					unit->inventory[itemIndex],
					ICON_BASE | ICON_COUNT_ABOVE_ONE | ICON_BROKEN |
						ICON_EQUIPPED,
					window, x, y);
				icon->depth = OAM_DEPTH_FRONT - 3;
				x += 16;
				if(itemIndex == SHOP_INVENTORY_COLUMNS - 1) {
					x = 8;
					y += 16;
				}
				if(itemIndex == SHOP_INVENTORY_COLUMNS * 2 - 1) {
					x = 8;
					y += 16;
				}
				itemIndex++;
				if(itemIndex >= INVENTORY_SLOTS ||
				   unit->inventory[itemIndex] == 0)
					break;
			}
		}
	}
}

/* The smaller of `a` and `b`. It reads `b` again when that is the smaller, so
 */
/* a call passed as `b` is made a second time. */
#define SHOP_MIN(a, b) ((a) > (b) ? (b) : (a))

/* Asks how many to buy when the item stacks, limited by what the coins buy, */
/* the artifact's stock and a full stack less what the party member carries. */
/* Returns the count (1 for other items), or -1 when cancelled. */
s32 mnShopSelectBuyCount(s32 unitId, s32 itemId) {
	SHOP_STATE *shop;
	UNIT *unit;
	ITEM_DEFINITION *item;
	s32 carriedQuantity;
	s32 quantityLimit;
	s32 slot;
	s32 selectedQuantity;

	shop = MenuState;
	unit = gmUnitGetStub(unitId);
	item = gmItemGetDirectStub(itemId);
	selectedQuantity = 1;
	if(item->flg & ITEM_STACKABLE) {
		/* "How many do you want?" */
		mnShopShowKeeperMessage(MsgHowManyToBuy);
		slot = gmInventoryFindStub(unitId, itemId);
		if(slot != -1) {
			carriedQuantity =
				(unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
		}
		else {
			carriedQuantity = 0;
		}

		quantityLimit = INVENTORY_STACK_MAX;
		if(item->price != 0)
			quantityLimit = MainRoutineStateWork.holdings.coins / item->price;

		if(shop->action == SHOP_ACTION_ARTIFACTS) {
			quantityLimit = SHOP_MIN(quantityLimit,
									 gmItemAdjustRareItemStockStub(itemId, 0));
		}

		quantityLimit += carriedQuantity;
		if(quantityLimit > INVENTORY_STACK_MAX)
			quantityLimit = INVENTORY_STACK_MAX;

		shop->cursorFrm = SHOP_CURSOR_ENTER_FRAMES;
		mnCursorPlace(0, SHOP_QUANTITY_CURSOR_X, 48);
		selectedQuantity =
			mnShopSelectQuantity(carriedQuantity, quantityLimit, item->price);
	}
	return selectedQuantity;
}

/* The quantity window's count and the coins word after its total, in pixels: */
/* German and French move both 8 left, Spanish the coins word 8 right. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
#define SHOP_QUANTITY_NUMBER_X	   64
#define SHOP_QUANTITY_COIN_LABEL_X 128
#elif LANGUAGE == LANGUAGE_SPANISH
#define SHOP_QUANTITY_NUMBER_X	   72
#define SHOP_QUANTITY_COIN_LABEL_X 144
#else
#define SHOP_QUANTITY_NUMBER_X	   72
#define SHOP_QUANTITY_COIN_LABEL_X 136
#endif

/* Lets the player choose how many to buy or sell on the quantity bar, from 1 */
/* to `maximum` less the `minimum` already carried, with the coins they come */
/* to. Returns the number, or -1 for B. */
s32 mnShopSelectQuantity(s32 minimum, s32 maximum, s32 price) {
	void *tileBuffer;
	WINDOW *window;
	s32 redraw;
	s32 resourceSlot;
	s32 quantityIndex;
	s32 selectedQuantity;
	WINDOW_SPR *quantitySprite;
#if LANGUAGE == LANGUAGE_JAPANESE
	s32 unitMessageBase;
#endif

	tileBuffer = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	redraw = 1;
	maximum -= minimum;
	selectedQuantity = -1;
	quantityIndex = 0;
	window = mnWindowCreateStub(7, 4, 23, 3, WINDOW_FLAG_INSTANT);
	if(window != 0) {
		resourceSlot = syVramCacheFindFreeEntry();
		if(resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
			/* The bar is one 8-tile entry shown as two 32x8 sprites
			 * (OBJ_SIZE_32X8); */
			/* the second starts at the entry's tile 4 */
			syVramCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, 0);
			mnWindowSprCreateStub(resourceSlot, OUTPUT_SIZE_32X8, window, 0, 0);
			quantitySprite = mnWindowSprCreateStub(
				resourceSlot, OUTPUT_SIZE_32X8, window, 32, 0);
			quantitySprite->spr.oam.idx += 4;
			for(;;) {
				if(KeysPressed & KEY_A) {
					sySndPlayStub(SE_MENU_CONFIRM);
					selectedQuantity = quantityIndex + 1;
					break;
				}
				if(KeysPressed & KEY_B) {
					sySndPlayStub(SE_MENU_CANCEL);
					selectedQuantity = -1;
					break;
				}
				if(KeysRepeat & KEY_LEFT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw = 1;
					quantityIndex--;
				}
				if(KeysRepeat & KEY_RIGHT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw = 1;
					quantityIndex++;
				}
				if(redraw) {
					redraw = 0;
					quantityIndex = (quantityIndex + maximum) % maximum;
					syDMAXCopy(Shop_QuantityTiles, tileBuffer,
							   DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4),
							   REG_DMA3);
					mnQuantityBarFill(INVENTORY_STACK_MAX,
									  QUANTITY_BAR_OUT_OF_REACH, tileBuffer);
					mnQuantityBarFill(minimum + maximum, QUANTITY_BAR_CHOOSABLE,
									  tileBuffer);
					mnQuantityBarFill(minimum + quantityIndex + 1,
									  QUANTITY_BAR_CHOSEN, tileBuffer);
					mnQuantityBarFill(minimum, QUANTITY_BAR_HELD, tileBuffer);
					syVramCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP,
									tileBuffer);
					selectedQuantity = quantityIndex + 1;
#if LANGUAGE != LANGUAGE_JAPANESE
					mnTextDrawNumInWindowStub(selectedQuantity, 2, window,
											  SHOP_QUANTITY_NUMBER_X, 0);
					mnTextDrawNumInWindowStub(price * selectedQuantity, 6,
											  window,
											  SHOP_QUANTITY_NUMBER_X + 16, 0);
					mnTextDrawMessageAsTilesStub(MsgCoins, window,
												 SHOP_QUANTITY_COIN_LABEL_X, 0);
#else
					mnTextDrawNumInWindowStub(selectedQuantity, 2, window, 64,
											  0);
					unitMessageBase = MsgPiece;
					mnTextDrawMessageAsTilesStub(unitMessageBase, window, 80,
												 0);
					mnTextDrawNumInWindowStub(price * selectedQuantity, 6,
											  window, 96, 0);
					mnTextDrawMessageAsTilesStub(unitMessageBase - 1, window,
												 144, 0); /* MsgCoins */
#endif
				}
				sySynch(1);
			}
			sySynch(1);
			mnWindowCloseStub(window, 2);
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return selectedQuantity;
}

/* Gives the party member each item bought and takes its price, then offers to
 */
/* equip it and to sell what it replaced. */
void mnShopBuyDone(s32 unitId, s32 itemId, s32 quantity) {
	s32 replacedSlot;
	s32 i;
	s32 lastAddedSlot;
	ITEM_DEFINITION *item;

	item = gmItemGetDirectStub(itemId);
	lastAddedSlot = 0;
	replacedSlot = gmInventoryFindEquippedStub(unitId, item->type);
	/* Each one is paid for on its own, and its price counts toward the next */
	/* Game Ticket. */
	sySndPlayStub(SE_SHOP_PURCHASE);
	for(i = 0; i < quantity; i++) {
		lastAddedSlot = gmInventoryAddItemStub(unitId, itemId);
		gmPartyAdjustCoinsStub(0 - item->price);
		gmPartyAdjustShopCreditStub(item->price);
		mnShopDrawMoney();
	}
	/* "Then here you go!" */
	mnShopShowKeeperMessageHidingCursor(MsgHereYouGo);
	if(mnShopConfirmEquip(unitId, lastAddedSlot) != 0) {
		mnShopSellOld(unitId, replacedSlot);
	}
}

/* Offers to equip the item just bought, and warns of a curse. Returns 1 when */
/* it was equipped. */
s32 mnShopConfirmEquip(s32 unitId, s32 slot) {
	SHOP_STATE *shop = MenuState;
	UNIT *unit = gmUnitGetStub(unitId);
	s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
	s32 replacedSlot;
	WINDOW *itemWindow;

	if(unit->inventory[slot] & INVENTORY_EQUIPPED)
		return 0;

	if(gmItemCanUnitEquip(unitId, itemId) == 0)
		return 0;

	replacedSlot = gmInventoryFindEquippedStub(unitId, item->type);
	if(replacedSlot != -1) {
		ITEM_DEFINITION *equippedItem =
			gmItemGetDirectStub(unit->inventory[replacedSlot]);

		if(equippedItem->flg & ITEM_CANNOT_UNEQUIP)
			return 0;
	}

	mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
	/* "Would you like to equip it now, (the member)?" */
	mnShopShowKeeperMessage(MsgEquipNowPrompt);
	if(mnShopAskYesNo(0) != 0)
		return 0;

	gmInventoryEquipStub(unitId, slot);
	itemWindow = shop->itemWin;
	if(itemWindow != 0)
		mnShopDrawMemberItems(itemWindow, unitId);

	if(item->flg & ITEM_CURSED) {
		sySndPlayStub(SE_CURSE);
		mnMessageCloseFinishedWindowsStub();
		/* "You've been cursed!" */
		mnMessageOpenWindowStub(MsgBecameCursed, 8, 4, MESSAGE_WINDOW_UNREAD);
		while(mnMessageAllTypedStub() == 0) {
			sySynch(1);
		}
	}

	/* "You look bolder already!" */
	mnShopShowKeeperMessageHidingCursor(MsgLookBolder);
	return 1;
}

/* Offers to sell the item the new one replaced, unless there is none or it is
 */
/* a Psynergy item or indispensable. Returns 1 when it offered. */
s32 mnShopSellOld(s32 unitId, s32 slot) {
	UNIT *unit;
	s32 itemId;

	unit = gmUnitGetStub(unitId);
	if(slot == -1) {
		return 0;
	}
	itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	if(gmItemGetDirectStub(itemId)->type == ITEM_TYPE_PSYNERGY) {
		return 0;
	}
	if(gmItemGetDirectStub(itemId)->flg & ITEM_INDISPENSABLE) {
		return 0;
	}
	mnShopSellItem(unitId, slot, -1);
	return 1;
}

/* Returns what the shops pay for an inventory entry: nothing for an */
/* indispensable item, half the price for a broken one, three quarters */
/* otherwise. */
s32 mnShopSalePrice(s32 inventoryEntry) {
	s32 salePrice;

	salePrice = gmItemGetDirectStub(inventoryEntry)->price;

	if(gmItemGetDirectStub(inventoryEntry)->flg & ITEM_INDISPENSABLE) {
		salePrice = 0;
	}
	else if(inventoryEntry & INVENTORY_BROKEN) {
		salePrice = salePrice / 2;
	}
	else {
		salePrice = salePrice * 3 / 4;
	}
	return salePrice;
}

#undef SHOP_QUANTITY_NUMBER_X
#undef SHOP_QUANTITY_COIN_LABEL_X
