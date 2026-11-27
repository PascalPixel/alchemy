/* Buying: the stock list, the money and party windows, quantities and prices.
   Part of shop.c. */

#include "shop_internal.h"

/* Runs the Buy and Artifacts counters: the player browses the stock seven items
   a page, picks one, chooses who gets it and how many, and pays. Checks room in
   the pack and coins first, and warns when the party member cannot equip it. An
   artifact bought takes its stock counter down, and the list is rebuilt.
   Repeats until B. */
Sint32 mnShopSelectBuy(void)
{
	SHOP_STATE		*shop = MenuState;
	WINDOW			*priceWin = 0;
	WINDOW			*stockWin;
	WINDOW			*msgWin;
	WINDOW			*partyWin;
	Sint32			 selected = 0;
	Sint32			 quantity;
	ITEM_DEFINITION *selectedDefinition;
	Bool			 repromptRecipient;
	WINDOW			*itemWin;
	Bool			 redraw;
	Sint32			 selectionIdx;
	Sint32			 stockCnt;
	Sint32			 itemId;
	ITEM_DEFINITION *stockDefinition;
	Sint32			 selectionResult;
	Sint32			 prevIdx;
	Sint32			 unitId;
	Sint32			 capacitySlot;
	Sint32			 removedCnt;
	Sint16			*stockIds;

	shop->itemWin = 0;
	shop->moneyWin = mnWindowCreateStub(30 - SHOP_MONEY_WINDOW_WIDTH, 7,
		SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();
	priceWin =
		mnWindowCreateStub(0, 8, SHOP_PRICE_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
	selectionIdx = selected;
	stockWin = mnWindowCreateStub(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	msgWin = mnWindowCreateStub(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
	redraw = TRUE;

	while(1) {
		stockCnt = shop->stockCnt;
		if(redraw != 0) {
			stockIds = shop->stockItemIds;
			itemId = stockIds[selectionIdx];
			stockDefinition = gmItemGetDirectStub(itemId);
			redraw = FALSE;
			mnCursorPlace(
				stockWin, selectionIdx % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
			shop->cursorFrm = 4;
			mnShopDrawStock(stockWin, selectionIdx);
			mnShopDrawMessage(msgWin, itemId + MsgItemPlainName);
			mnWindowSprRedrawSavedRectStub(priceWin);
			mnShopDrawItemPrice(
				priceWin, itemId, stockDefinition->price, SHOP_PRICE_KIND_BUY);
		}
		if((PerPush & KEY_A) != 0) {
			selected = selectionIdx;
			sySndPlayStub(SE_MENU_CONFIRM);
			selectionResult = 0;
			break;
		}
		if((PerPush & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}
		if((PerRepeat & KEY_LEFT) != 0) {
			prevIdx = selectionIdx;
			selectionIdx--;
			selectionIdx = (selectionIdx + stockCnt) % stockCnt;
			if(prevIdx != selectionIdx) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
			}
		}
		if((PerRepeat & KEY_RIGHT) != 0) {
			prevIdx = selectionIdx;
			selectionIdx++;
			selectionIdx = (selectionIdx + stockCnt) % stockCnt;
			if(prevIdx != selectionIdx) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
			}
		}
		if((PerRepeat & KEY_UP) != 0 &&
			selectionIdx - SHOP_STOCK_PAGE_ITEMS >= 0) {
			selectionIdx -= SHOP_STOCK_PAGE_ITEMS;
			redraw = TRUE;
		}
		if((PerRepeat & KEY_DOWN) != 0) {
			if(selectionIdx + SHOP_STOCK_PAGE_ITEMS <
				(stockCnt + (SHOP_STOCK_PAGE_ITEMS - 1)) /
					SHOP_STOCK_PAGE_ITEMS * SHOP_STOCK_PAGE_ITEMS) {
				selectionIdx += SHOP_STOCK_PAGE_ITEMS;
				redraw = TRUE;
			}
			if(selectionIdx > stockCnt - 1)
				selectionIdx = stockCnt - 1;
		}
		sySynch(1);
	}

	mnWindowCloseStub(msgWin, 2);
	mnWindowCloseStub(stockWin, 2);
	sySynch(1);
	if(selectionResult != 0)
		goto close;

	shop->selectedItem = shop->stockItemIds[selected];
	/* "Which one of you gets this?" */
	mnShopShowKeeperMessage(MsgWhoGetsThis);
	selectedDefinition = gmItemGetDirectStub(shop->selectedItem);
	quantity = 1;
	unitId = 0;
	repromptRecipient = FALSE;
	selectionIdx = 0;
	partyWin = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	mnCreatePartyFiguresStub(partyWin, 2, 0, 8, 0);
	itemWin = mnWindowCreateStub(16, 11, 14, 9, WINDOW_FLAG_INSTANT);
	redraw = TRUE;

	while(1) {
		if(repromptRecipient != 0) {
			repromptRecipient = FALSE;
			mnShopShowKeeperMessage(MsgWhoGetsThis);
			redraw = TRUE;
		}
		if(redraw != 0) {
			redraw = FALSE;
			selectionIdx =
				(selectionIdx + shop->partyMemberCnt) % shop->partyMemberCnt;
			unitId = shop->partyMemberIds[selectionIdx];
			mnCursorPlace(partyWin, selectionIdx * 24 - 12, 0);
			shop->cursorFrm = 3;
			mnShopDrawParty(partyWin, selectionIdx, shop->selectedItem);
			if(gmItemGetEquipmentGroupStub(shop->selectedItem) ==
				EQUIPMENT_GROUP_NONE)
				mnShopDrawCarriedItem(itemWin, unitId, shop->selectedItem);
			else
				mnShopDrawEquipComparison(itemWin, unitId, shop->selectedItem);
		}
		if((PerPush & KEY_A) != 0) {
			/* Test for room by adding one and taking it out again, before the
			   coins are checked; a full bag and a full stack get different
			   lines */
			capacitySlot = gmInventoryAddItemStub(unitId, shop->selectedItem);
			if(capacitySlot < 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(
					shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
				if(gmInventoryCountStub(unitId) == INVENTORY_SLOTS)
					/* "Your pack is full, (the member). Who will you give this
					   to?" */
					mnShopShowKeeperMessage(MsgPackFull);
				else
					/* "You can't carry any more of this item, (the member), so
					   who gets it?" */
					mnShopShowKeeperMessage(MsgCannotCarryItem);
				continue;
			}
			gmInventoryRemoveStub(unitId, capacitySlot);
			if((Uint32)selectedDefinition->price >
				MainRoutineStateWork.holdings.coins) {
				sySndPlayStub(SE_MENU_CANCEL);
				/* "Oh! That's a fine choice, but you don't have enough
				   money." */
				mnShopShowKeeperMessageHidingCursor(MsgBuyNotEnoughMoney);
				selectionResult = -1;
				break;
			}
			if(gmItemIsCompatibleWithUnitStub(unitId, shop->selectedItem) ==
				0) {
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				/* "You can't equip this, (the member). Do you still want
				   it?" */
				mnShopShowKeeperMessage(MsgCannotEquipStillWant);
				if(mnShopAskYesNo(0) != 0) {
					repromptRecipient = TRUE;
					continue;
				}
			}
			sySndPlayStub(SE_MENU_CONFIRM);
			sySynch(1);
			quantity = mnShopSelectBuyCount(unitId, shop->selectedItem);
			repromptRecipient = TRUE;
			if(quantity == -1)
				continue;
			mnShopBuyDone(unitId, shop->selectedItem, quantity);
			mnShopGiveGameTicket(partyWin, itemWin);
			selectionResult = 0;
			break;
		}
		if((PerPush & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}
		if((PerRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectionIdx--;
			redraw = TRUE;
		}
		if((PerRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectionIdx++;
			redraw = TRUE;
		}
		sySynch(1);
	}

	mnReleasePartyFiguresStub();
	mnWindowCloseStub(itemWin, 2);
	mnWindowCloseStub(partyWin, 2);
	sySynch(1);
	/* An artifact bought leaves the world's stock, one for each bought, and the
	   list is made again without what is gone. */
	if(selectionResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS) {
		for(removedCnt = 0; removedCnt < quantity; removedCnt++)
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
	mnWindowCloseStub(priceWin, 2);
	mnWindowCloseStub(shop->moneyWin, 2);
	sySynch(1);
	return 0;
}

/* Draws a page of seven stock items with their prices, the page arrows, and the
   chosen item lifted and pulsing. */
void mnShopDrawStock(WINDOW *win, Sint32 selected)
{
	SHOP_STATE		*shop;
	Sint16			*itemIds;
	Sint32			 itemCnt;
	Sint32			 stockIdx;
	Uint32			 pageSlot;
	Sint16			 itemId;
	ITEM_DEFINITION *item;
	WINDOW_SPR		*icon;

	shop = MenuState;
	itemIds = shop->stockItemIds;
	itemCnt = shop->stockCnt;
	stockIdx = selected - selected % SHOP_STOCK_PAGE_ITEMS;
	if(win != 0) {
		mnWindowSprPrepareForRedrawStub(win);
		/* The arrows show the shop's own icons, which closing the window
		   leaves. */
		if(stockIdx != 0) {
			icon = mnWindowSprCreateStub(
				shop->prevPageIcon, WINDOW_SPRITE_SIZE_16X16, win, 216, -16);
			icon->kind = WINDOW_SPRITE_PLAIN;
			icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
			icon->anmStep = 0;
		}
		if(stockIdx + SHOP_STOCK_PAGE_ITEMS < itemCnt) {
			icon = mnWindowSprCreateStub(
				shop->nextPageIcon, WINDOW_SPRITE_SIZE_16X16, win, 216, 24);
			icon->kind = WINDOW_SPRITE_PLAIN;
			icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
			icon->anmStep = 0;
		}
		pageSlot = 0;
		if(stockIdx < (Uint32)itemCnt) {
			/* Each price sits 16 pixels right of its item. */
			for(;
				pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIdx < (Uint32)itemCnt;
				pageSlot++, stockIdx++) {
				itemId = itemIds[stockIdx];
				item = gmItemGetDirectStub(itemId);
				icon = mnWindowSprCreateLoadedStub(
					itemId, ICON_BASE, win, pageSlot * 32, 0);
				icon->depth = OAM_DEPTH_FRONT - 3;
				if(stockIdx == selected) {
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 10;
					icon->depth = OAM_DEPTH_FRONT - 2;
				}
				icon = mnShopCreatePriceSpr(
					item->price, win, pageSlot * 32 + 16, 0);
				icon->depth = OAM_DEPTH_FRONT - 4;
			}
		}
	}
}

/* Draws the party's coins in the coins window, in the edition's layout. */
void mnShopDrawMoney(void)
{
	SHOP_STATE *shop;
	WINDOW	   *win;

	shop = MenuState;
	win = shop->moneyWin;
	if(win != 0) {
#if LANGUAGE != LANGUAGE_JAPANESE
		mnTextDrawMessageAsTilesStub(MsgYourCoins, win, 0, 0);
		mnTextDrawNumInWindowStub(
			MainRoutineStateWork.holdings.coins, 6, win, 32, 8);
#else
		Sint32 msg = MsgYourCoins;

		mnTextDrawMessageAsTilesStub(msg, win, 0, 0);
		mnTextDrawNumInWindowStub(
			MainRoutineStateWork.holdings.coins, 6, win, 0, 8);
		mnTextDrawMessageAsTilesStub(msg - 2, win, 48, 8); /* MsgCoins */
#endif
	}
}

/* Draws the item's name and below it its price. With a price of 0 it says
   instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or that it is
   not broken (SHOP_PRICE_KIND_REPAIR). */
void mnShopDrawItemPrice(
	WINDOW *win, Sint32 itemId, Sint32 price, Sint32 priceKind)
{
	Sint32 labelMsg;

	if(win == 0)
		return;
	goto draw;
cannot_buy:
	mnTextDrawMessageAsTilesStub(MsgCannotBuyThat, win, SHOP_ITEM_NOTE_X, 8);
	return;
show_price:
	labelMsg = MsgPrice;
	mnTextDrawMessageAsTilesStub(labelMsg, win, SHOP_ITEM_NOTE_X, 8);
	labelMsg -= 3; /* MsgCoins */
	mnTextDrawNumInWindowStub(price, 5, win, SHOP_PRICE_NUMBER_X, 8);
	mnTextDrawMessageAsTilesStub(labelMsg, win, SHOP_PRICE_LABEL_X, 8);
	return;
draw:
	mnWindowSprRedrawSavedRectStub(win);
	mnTextDrawMessageAsTilesStub(itemId + MsgItemName, win, 0, 0);
	if(price != 0)
		goto show_price;
	switch(priceKind) {
	case SHOP_PRICE_KIND_SELL:
		goto cannot_buy;
	case SHOP_PRICE_KIND_REPAIR:
		mnTextDrawMessageAsTilesStub(MsgNotBroken, win, SHOP_ITEM_NOTE_X, 8);
		break;
	default:
		goto show_price;
	}
}

/* Shows `msg`, an item's description, in the help window; a NULL window is
   ignored. */
void mnShopDrawMessage(WINDOW *win, Sint32 msg)
{
	if(win != 0) {
#if LANGUAGE != LANGUAGE_JAPANESE
		mnWindowSprRedrawSavedRectStub(win);
#else
		mnWindowSprClearListStub(win);
		sySynch(1);
#endif
		SHOP_DRAW_MESSAGE(msg, win, 0, 0);
	}
}

/* Shows the chosen party member running in place and the others standing, and
   draws at four fifths of their size the party members who cannot equip
   `itemId`. */
void mnShopDrawParty(WINDOW *win, Sint32 selected, Sint32 itemId)
{
	SHOP_STATE *shop = MenuState;
	Sint32		memberIdx;

	if(win != 0) {
		for(memberIdx = 0; memberIdx < shop->partyMemberCnt; memberIdx++) {
			if(memberIdx == selected)
				fdSprStartAnimationStub(
					shop->partyMemberIcons[memberIdx], ANIM_PARTY_RUN);
			else
				fdSprStartAnimationStub(
					shop->partyMemberIcons[memberIdx], ANIM_STAND);
			shop->partyMemberScale[memberIdx] = toFIXED(1);
			if(!gmItemIsCompatibleWithUnitStub(
				   shop->partyMemberIds[memberIdx], itemId))
				shop->partyMemberScale[memberIdx] = toFIXED(4) / 5;
		}
	}
}

/* Shows how Attack, Defense and Agility would change if the party member wore
   the item, with an arrow up or down beside each that changes and the name of
   the item it would replace. The item is tried on in that item's slot and the
   slot put back. */
void mnShopDrawEquipComparison(WINDOW *win, Sint32 unitId, Sint32 itemId)
{
	SHOP_STATE		*shop = MenuState;
	UNIT			*unit = gmUnitGetStub(unitId);
	ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
	Sint32			 replacedItemId = -1;
	Sint32			 previewSlot;
	Sint32			 savedEntry;
	Sint32			 idx;
	Sint32			 baseStats[4];
	Sint32			 previewStats[4];

	if(win == 0)
		return;
	mnWindowSprPrepareForRedrawStub(win);
	if(!gmItemCanUnitEquipDirectStub(unitId, itemId)) {
		mnTextDrawMessageStub(MsgShopCannotEquip, win, 8, 24);
		return;
	}
	previewSlot = gmInventoryFindEquippedStub(unitId, item->type);
	/* With nothing of the type equipped, borrow the first slot that is not
	   equipped, else the first Psynergy item's, else slot 0. */
	if(previewSlot == -1) {
		for(idx = 0; idx < INVENTORY_SLOTS; idx++) {
			if(!(unit->inventory[idx] & INVENTORY_EQUIPPED))
				break;
		}
		if(idx == INVENTORY_SLOTS) {
			for(idx = 0; idx < INVENTORY_SLOTS; idx++) {
				if(gmItemGetDirectStub(unit->inventory[idx])->type ==
					ITEM_TYPE_PSYNERGY)
					break;
			}
			if(idx == INVENTORY_SLOTS)
				idx = 0;
		}
		previewSlot = idx;
	} else {
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
	for(idx = 0; idx <= 2; idx++) {
		if(baseStats[idx] > previewStats[idx])
			mnWindowSprCreateStub(shop->statDownIcon, WINDOW_SPRITE_SIZE_16X16,
				win, 56, idx * 16 - 4)
				->kind = WINDOW_SPRITE_PLAIN;
		else if(baseStats[idx] < previewStats[idx])
			mnWindowSprCreateStub(shop->statUpIcon, WINDOW_SPRITE_SIZE_16X16,
				win, 56, idx * 16 - 4)
				->kind = WINDOW_SPRITE_PLAIN;
		mnTextDrawNumInWindowStub(baseStats[idx], 3, win, 32, idx * 16);
		if(baseStats[idx] != previewStats[idx])
			mnTextDrawNumInWindowStub(previewStats[idx], 3, win, 72, idx * 16);
		mnTextDrawMessageAsTilesStub(MsgShopStatLabel + idx, win, 0, idx * 16);
		mnWindowDrawDividerLineStub(win, 0, idx * 2 + 2, 13, idx * 2 + 2);
	}
	if(replacedItemId != -1)
		mnTextDrawMessageAsTilesStub(replacedItemId + MsgItemName, win, 0, 48);
}

/* Shows the quantity already carried and the party member's inventory icons. */
void mnShopDrawCarriedItem(WINDOW *win, Sint32 unitId, Sint32 itemId)
{
	UNIT	   *unit;
	Sint32		x;
	Sint32		y;
	Sint32		itemIdx;
	Sint32		slot;
	WINDOW_SPR *icon;

	unit = gmUnitGetStub(unitId);
	x = 8;
	y = 8;
	if(win != 0) {
		mnWindowSprPrepareForRedrawStub(win);
		slot = gmInventoryFindStub(unitId, itemId);
		if(slot != -1) {
			mnMessageArgumentPushStub(
				(unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1,
				MESSAGE_ARGUMENT_NUMBER);
			mnTextDrawMessageAsTilesStub(MsgYouHave, win, 0, 0);
		} else {
			mnTextDrawMessageAsTilesStub(MsgShopNoneInStock, win, 0, 0);
		}
		itemIdx = 0;
		if(unit->inventory[itemIdx] != 0) {
			for(;;) {
				icon = mnWindowSprCreateLoadedStub(unit->inventory[itemIdx],
					ICON_BASE | ICON_COUNT_ABOVE_ONE | ICON_BROKEN |
						ICON_EQUIPPED,
					win, x, y);
				icon->depth = OAM_DEPTH_FRONT - 3;
				x += 16;
				if(itemIdx == SHOP_INVENTORY_COLUMNS - 1) {
					x = 8;
					y += 16;
				}
				if(itemIdx == SHOP_INVENTORY_COLUMNS * 2 - 1) {
					x = 8;
					y += 16;
				}
				itemIdx++;
				if(itemIdx >= INVENTORY_SLOTS || unit->inventory[itemIdx] == 0)
					break;
			}
		}
	}
}

/* The smaller of `a` and `b`. It reads `b` again when that is the smaller, so a
   call passed as `b` is made a second time. */
#define SHOP_MIN(a, b) ((a) > (b) ? (b) : (a))

/* Asks how many to buy when the item stacks, limited by what the coins buy, the
   artifact's stock and a full stack less what the party member carries. Returns
   the count (1 for other items), or -1 when cancelled. */
Sint32 mnShopSelectBuyCount(Sint32 unitId, Sint32 itemId)
{
	SHOP_STATE		*shop;
	UNIT			*unit;
	ITEM_DEFINITION *item;
	Sint32			 carriedQuantity;
	Sint32			 quantityLimit;
	Sint32			 slot;
	Sint32			 selectedQuantity;

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
		} else {
			carriedQuantity = 0;
		}

		quantityLimit = INVENTORY_STACK_MAX;
		if(item->price != 0)
			quantityLimit = MainRoutineStateWork.holdings.coins / item->price;

		if(shop->action == SHOP_ACTION_ARTIFACTS) {
			quantityLimit = SHOP_MIN(
				quantityLimit, gmItemAdjustRareItemStockStub(itemId, 0));
		}

		quantityLimit += carriedQuantity;
		if(quantityLimit > INVENTORY_STACK_MAX)
			quantityLimit = INVENTORY_STACK_MAX;

		shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
		mnCursorPlace(0, SHOP_QUANTITY_CURSOR_X, 48);
		selectedQuantity =
			mnShopSelectQuantity(carriedQuantity, quantityLimit, item->price);
	}
	return selectedQuantity;
}

/* The quantity window's count and the coins word after its total, in pixels:
   German and French move both 8 left, Spanish the coins word 8 right. */
#define SHOP_QUANTITY_NUMBER_X	   BY_LANGUAGE(72, 72, 64, 72, 64, 72)
#define SHOP_QUANTITY_COIN_LABEL_X BY_LANGUAGE(136, 136, 128, 144, 128, 136)

/* Lets the player choose how many to buy or sell on the quantity bar, from 1 to
   `maximum` less the `minimum` already carried, with the coins they come to.
   Returns the number, or -1 for B. */
Sint32 mnShopSelectQuantity(Sint32 minimum, Sint32 maximum, Sint32 price)
{
	void	   *tileBuf;
	WINDOW	   *win;
	Bool		redraw;
	Sint32		resourceSlot;
	Sint32		quantityIdx;
	Sint32		selectedQuantity;
	WINDOW_SPR *quantitySpr;
#if LANGUAGE == LANGUAGE_JAPANESE
	Sint32 unitMsgBase;
#endif

	tileBuf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	redraw = TRUE;
	maximum -= minimum;
	selectedQuantity = -1;
	quantityIdx = 0;
	win = mnWindowCreateStub(7, 4, 23, 3, WINDOW_FLAG_INSTANT);
	if(win != 0) {
		resourceSlot = syVRAMCacheFindFreeEntry();
		if(resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
			/* The bar is one 8-tile entry shown as two 32x8 sprites
			   (OBJ_SIZE_32X8); the second starts at the entry's tile 4 */
			syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, 0);
			mnWindowSprCreateStub(resourceSlot, OUTPUT_SIZE_32X8, win, 0, 0);
			quantitySpr = mnWindowSprCreateStub(
				resourceSlot, OUTPUT_SIZE_32X8, win, 32, 0);
			quantitySpr->spr.oam.idx += 4;
			for(;;) {
				if(PerPush & KEY_A) {
					sySndPlayStub(SE_MENU_CONFIRM);
					selectedQuantity = quantityIdx + 1;
					break;
				}
				if(PerPush & KEY_B) {
					sySndPlayStub(SE_MENU_CANCEL);
					selectedQuantity = -1;
					break;
				}
				if(PerRepeat & KEY_LEFT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw = TRUE;
					quantityIdx--;
				}
				if(PerRepeat & KEY_RIGHT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw = TRUE;
					quantityIdx++;
				}
				if(redraw) {
					redraw = FALSE;
					quantityIdx = (quantityIdx + maximum) % maximum;
					syDMAXCopy(Shop_QuantityTiles, tileBuf,
						DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4), REG_DMA3);
					mnQuantityBarFill(INVENTORY_STACK_MAX,
						QUANTITY_BAR_OUT_OF_REACH, tileBuf);
					mnQuantityBarFill(
						minimum + maximum, QUANTITY_BAR_CHOOSABLE, tileBuf);
					mnQuantityBarFill(minimum + quantityIdx + 1,
						QUANTITY_BAR_CHOSEN, tileBuf);
					mnQuantityBarFill(minimum, QUANTITY_BAR_HELD, tileBuf);
					syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuf);
					selectedQuantity = quantityIdx + 1;
#if LANGUAGE != LANGUAGE_JAPANESE
					mnTextDrawNumInWindowStub(
						selectedQuantity, 2, win, SHOP_QUANTITY_NUMBER_X, 0);
					mnTextDrawNumInWindowStub(price * selectedQuantity, 6, win,
						SHOP_QUANTITY_NUMBER_X + 16, 0);
					mnTextDrawMessageAsTilesStub(
						MsgCoins, win, SHOP_QUANTITY_COIN_LABEL_X, 0);
#else
					mnTextDrawNumInWindowStub(selectedQuantity, 2, win, 64, 0);
					unitMsgBase = MsgPiece;
					mnTextDrawMessageAsTilesStub(unitMsgBase, win, 80, 0);
					mnTextDrawNumInWindowStub(
						price * selectedQuantity, 6, win, 96, 0);
					/* MsgCoins */
					mnTextDrawMessageAsTilesStub(unitMsgBase - 1, win, 144, 0);
#endif
				}
				sySynch(1);
			}
			sySynch(1);
			mnWindowCloseStub(win, 2);
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return selectedQuantity;
}

/* Gives the party member each item bought and takes its price, then offers to
   equip it and to sell what it replaced. */
void mnShopBuyDone(Sint32 unitId, Sint32 itemId, Sint32 quantity)
{
	Sint32			 replacedSlot;
	Sint32			 i;
	Sint32			 lastAddedSlot;
	ITEM_DEFINITION *item;

	item = gmItemGetDirectStub(itemId);
	lastAddedSlot = 0;
	replacedSlot = gmInventoryFindEquippedStub(unitId, item->type);
	/* Each one is paid for on its own, and its price counts toward the next
	   Game Ticket. */
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

/* Offers to equip the item just bought, and warns of a curse. Returns 1 when it
   was equipped. */
Bool mnShopConfirmEquip(Sint32 unitId, Sint32 slot)
{
	SHOP_STATE		*shop = MenuState;
	UNIT			*unit = gmUnitGetStub(unitId);
	Sint32			 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
	Sint32			 replacedSlot;
	WINDOW			*itemWin;

	if(unit->inventory[slot] & INVENTORY_EQUIPPED)
		return FALSE;

	if(gmItemCanUnitEquipDirectStub(unitId, itemId) == 0)
		return FALSE;

	replacedSlot = gmInventoryFindEquippedStub(unitId, item->type);
	if(replacedSlot != -1) {
		ITEM_DEFINITION *equippedItem =
			gmItemGetDirectStub(unit->inventory[replacedSlot]);

		if(equippedItem->flg & ITEM_CANNOT_UNEQUIP)
			return FALSE;
	}

	mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
	/* "Would you like to equip it now, (the member)?" */
	mnShopShowKeeperMessage(MsgEquipNowPrompt);
	if(mnShopAskYesNo(0) != 0)
		return FALSE;

	gmInventoryEquipStub(unitId, slot);
	itemWin = shop->itemWin;
	if(itemWin != 0)
		mnShopDrawMemberItems(itemWin, unitId);

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
	return TRUE;
}

/* Offers to sell the item the new one replaced, unless there is none or it is a
   Psynergy item or indispensable. Returns 1 when it offered. */
Bool mnShopSellOld(Sint32 unitId, Sint32 slot)
{
	UNIT  *unit;
	Sint32 itemId;

	unit = gmUnitGetStub(unitId);
	if(slot == -1) {
		return FALSE;
	}
	itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	if(gmItemGetDirectStub(itemId)->type == ITEM_TYPE_PSYNERGY) {
		return FALSE;
	}
	if(gmItemGetDirectStub(itemId)->flg & ITEM_INDISPENSABLE) {
		return FALSE;
	}
	mnShopSellItem(unitId, slot, -1);
	return TRUE;
}

/* Returns what the shops pay for an inventory entry: nothing for an
   indispensable item, half the price for a broken one, three quarters
   otherwise. */
Sint32 mnShopSalePrice(Sint32 inventoryEntry)
{
	Sint32 salePrice;

	salePrice = gmItemGetDirectStub(inventoryEntry)->price;

	if(gmItemGetDirectStub(inventoryEntry)->flg & ITEM_INDISPENSABLE) {
		salePrice = 0;
	} else if(inventoryEntry & INVENTORY_BROKEN) {
		salePrice = salePrice / 2;
	} else {
		salePrice = salePrice * 3 / 4;
	}
	return salePrice;
}

#undef SHOP_QUANTITY_NUMBER_X
#undef SHOP_QUANTITY_COIN_LABEL_X
