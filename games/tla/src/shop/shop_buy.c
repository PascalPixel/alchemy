/* Buying: the stock list, the money and party windows, quantities and prices.
   Part of shop.c. */

#include "field/stubs.h"

/* The Select window is wider outside English and narrower in Japanese, the
   price window wider in German, French and Italian and narrower in Japanese. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELD_WINDOW_X	  10
#define HELD_WINDOW_WIDTH 20
#elif LANGUAGE == LANGUAGE_JAPANESE
#define HELD_WINDOW_X	  14
#define HELD_WINDOW_WIDTH 13
#else
#define HELD_WINDOW_X	  13
#define HELD_WINDOW_WIDTH 17
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) || \
	(LANGUAGE == LANGUAGE_ITALIAN)
#define BUY_PRICE_WINDOW_WIDTH 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define BUY_PRICE_WINDOW_WIDTH 14
#else
#define BUY_PRICE_WINDOW_WIDTH 15
#endif

/* Runs the Buy and Artifacts counters: the player browses the stock seven items
   a page, picks one, chooses who gets it and how many, and pays. Checks room in
   the bag and coins first, and warns when the party member cannot equip it. An
   artifact bought takes its stock counter down, and the list is rebuilt.
   Holding Select shows the item's details. Repeats until B. */
Sint32 mnShopSelectBuy(void)
{
	SHOP_STATE		*shop = Ram_HeapSlots->menuState;
	WINDOW			*priceWin = 0;
	WINDOW			*stockWin;
	WINDOW			*descriptionWin;
	WINDOW			*partyWin;
	Sint32			 selected = 0;
	Sint32			 quantity;
	Sint32			 stockCnt;
	Sint16			*stockIds;
	ITEM_DEFINITION *heldDefinition;
	ITEM_DEFINITION *selectedDefinition;
	Bool			 repromptRecipient;
	WINDOW			*itemWin;
	Sint32			 redraw;
	Sint32			 selectionIdx;
	Sint32			 itemId;
	ITEM_DEFINITION *stockDefinition;
	Sint32			 selectionResult;
	Sint32			 purchaseResult;
	Sint32			 unitId;
	Sint32			 capacitySlot;
	Sint32			 removedCnt;
	/* FAKEMATCH(dead): both A-button tests also store into key. */
	Sint32 key;

	shop->itemWin = 0;
	shop->moneyWin = mnWindowCreateStub(30 - SHOP_COIN_WINDOW_WIDTH, 7,
		SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();
	priceWin = mnWindowCreateStub(
		0, 8, BUY_PRICE_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
	selectionIdx = selected;
	stockWin = mnWindowCreateStub(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	descriptionWin = mnWindowCreateStub(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
	redraw = 1;
	stockIds = shop->stockItemIds;

	while(1) {
		stockCnt = shop->stockCnt;
		if(redraw != 0) {
			itemId = stockIds[selectionIdx];
			stockDefinition = gmItemGetDirectStub(itemId);
			mnCursorPlace(
				stockWin, selectionIdx % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
			shop->cursorFrm = 4;
			mnShopDrawStock(stockWin, selectionIdx);
			mnShopDrawMessage(
				descriptionWin, itemId + (Sint32)MsgItemDescription);
			mnWindowSprRedrawSavedRectStub(priceWin);
			mnShopDrawItemPrice(priceWin, itemId,
				(Uint16)stockDefinition->price, SHOP_PRICE_KIND_BUY);
			redraw = 0;
		}
		if(key = PerWork.push & KEY_A) {
			selected = selectionIdx;
			sySndPlayStub(SE_MENU_CONFIRM);
			selectionResult = 0;
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			selectionResult = -1;
			break;
		}
		if(PerWork.data & KEY_SELECT) {
			Sint32	held;
			WINDOW *win;
#if LANGUAGE == LANGUAGE_JAPANESE
			Sint32 heldItem;
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
			/* stockCnt, which the loop's top reloads, holds the item
			   shown. */
			stockCnt = stockIds[selectionIdx];
			held = gmPartyInventoryCountItemStub(stockCnt);
			heldDefinition = gmItemGetDirectStub(stockCnt);
#else
			heldItem = stockIds[selectionIdx];
			held = gmPartyInventoryCountItemStub(heldItem);
#endif
			sySndPlayStub(SE_RECOVERY);
			win = mnWindowCreateStub(
				HELD_WINDOW_X, 3, HELD_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
#if LANGUAGE != LANGUAGE_JAPANESE
			mnItemMenuDrawItemDetailsWithCountStub(win, stockCnt, held);
#else
			mnItemMenuDrawItemDetailsWithCountStub(win, heldItem, held);
#endif
			while(PerWork.data & KEY_SELECT)
				sySynch(1);
			mnWindowCloseStub(win, 2);
			mnShopDrawStock(stockWin, selectionIdx);
			mnShopDrawMoney();
#if LANGUAGE != LANGUAGE_JAPANESE
			/* Japanese does not redraw the price window here. */
			mnWindowSprRedrawSavedRectStub(priceWin);
			mnShopDrawItemPrice(priceWin, stockCnt,
				(Uint16)heldDefinition->price, SHOP_PRICE_KIND_BUY);
#endif
		} else {
			redraw = mnGridMoveCursor(
				&selectionIdx, stockCnt, SHOP_STOCK_PAGE_ITEMS);
		}
		sySynch(1);
	}

	mnWindowCloseStub(descriptionWin, 2);
	mnWindowCloseStub(stockWin, 2);
	sySynch(1);
	if(selectionResult == -1)
		goto close;

	shop->selectedItem = shop->stockItemIds[selected];
	mnShopShowKeeperMessage((Sint32)MsgWhoGetsThis);
	selectedDefinition = gmItemGetDirectStub(shop->selectedItem);
	quantity = 1;
	unitId = 0;
	repromptRecipient = FALSE;
	selectionIdx = 0;
	partyWin = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	mnShowPartyFiguresStub(partyWin, 2, 0);
	itemWin = mnWindowCreateStub(16, 11, 14, 9, WINDOW_FLAG_INSTANT);
	redraw = MENU_GRID_CHANGED_PAGE;

	while(1) {
		if(repromptRecipient != 0) {
			repromptRecipient = FALSE;
			mnShopShowKeeperMessage((Sint32)MsgWhoGetsThis);
			redraw = 1;
		}
		if(redraw != 0) {
			unitId = shop->partyMemberIds[selectionIdx];
			mnCursorPlace(
				partyWin, selectionIdx % SHOP_PARTY_PAGE_MEMBERS * 24 - 12, 0);
			shop->cursorFrm = 3;
			if(redraw == MENU_GRID_CHANGED_PAGE) {
				mnCreatePartyFiguresStub(selectionIdx /
					SHOP_PARTY_PAGE_MEMBERS * SHOP_PARTY_PAGE_MEMBERS);
				sySynch(1);
			}
			mnShopDrawParty(partyWin, selectionIdx, shop->selectedItem);
			if(gmItemGetEquipmentGroupStub(shop->selectedItem) ==
				EQUIPMENT_GROUP_NONE)
				mnShopDrawCarriedItem(itemWin, unitId, shop->selectedItem);
			else
				mnShopDrawEquipComparison(itemWin, unitId, shop->selectedItem);
		}
		if(key = PerWork.push & KEY_A) {
			redraw = MENU_GRID_CHANGED_PAGE;
			mnWindowSprPrepareForRedrawStub(partyWin);
			capacitySlot = gmInventoryAddItemStub(unitId, shop->selectedItem);
			if(capacitySlot < 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(
					shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
				if(gmInventoryCountStub(unitId) == INVENTORY_SLOTS)
					mnShopShowKeeperMessage((Sint32)MsgPackFull);
				else
					mnShopShowKeeperMessage((Sint32)MsgCannotCarryItem);
				continue;
			}
			gmInventoryRemoveStub(unitId, capacitySlot);
			if((Uint16)selectedDefinition->price > MainRoutineStateWork.coins) {
				sySndPlayStub(SE_MENU_CANCEL);
				mnShopShowKeeperMessageHidingCursor(
					(Sint32)MsgBuyNotEnoughMoney);
				purchaseResult = -1;
				break;
			}
			if(gmItemIsCompatibleWithUnitStub(unitId, shop->selectedItem) ==
				0) {
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				mnShopShowKeeperMessage((Sint32)MsgCannotEquipStillWant);
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
			purchaseResult = 0;
			break;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			purchaseResult = -1;
			break;
		}
		redraw = mnGridMoveCursor(
			&selectionIdx, shop->partyMemberCnt, SHOP_PARTY_PAGE_MEMBERS);
		sySynch(1);
	}

	mnItemIconGridSetVisible(0);
	mnReleasePartyFiguresStub();
	mnWindowCloseStub(itemWin, 2);
	mnWindowCloseStub(partyWin, 2);
	sySynch(1);
	if(purchaseResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS) {
		for(removedCnt = 0; removedCnt < quantity; removedCnt++)
			gmItemAdjustRareItemStockStub(shop->selectedItem, -1);
		if(mnShopListAvailableArtifacts() == 0)
			goto close;
		if(selected > shop->stockCnt - 1)
			selected = shop->stockCnt - 1;
	}
	mnShopShowKeeperMessage((Sint32)MsgBuyAnythingElse);
	goto next_page;

close:
	mnWindowCloseStub(priceWin, 2);
	mnWindowCloseStub(shop->moneyWin, 2);
	sySynch(1);
	return 0;
}

#undef HELD_WINDOW_X
#undef HELD_WINDOW_WIDTH
#undef BUY_PRICE_WINDOW_WIDTH

/* Draws a page of seven stock items with their prices, the page arrows, and the
   chosen item lifted and pulsing. */
void mnShopDrawStock(WINDOW *win, Sint32 selected)
{
	SHOP_STATE		*shop;
	Sint16			*itemIds;
	Sint32			 itemCnt;
	Sint32			 stockIdx;
	Sint32			 pageSlot;
	Sint16			 itemId;
	ITEM_DEFINITION *item;
	WINDOW_SPR		*icon;

	shop = Ram_HeapSlots->menuState;
	itemIds = shop->stockItemIds;
	itemCnt = shop->stockCnt;
	stockIdx = selected / SHOP_STOCK_PAGE_ITEMS * SHOP_STOCK_PAGE_ITEMS;
	if(win != 0) {
		mnWindowSprPrepareForRedrawStub(win);
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
			for(; (Uint32)pageSlot < SHOP_STOCK_PAGE_ITEMS &&
				stockIdx < (Uint32)itemCnt;
				pageSlot++, stockIdx++) {
				itemId = itemIds[stockIdx];
				item = gmItemGetDirectStub(itemId);
				icon = mnWindowSprCreateLoadedStub(
					itemId, 1, win, pageSlot * 32, 0);
				icon->depth = OAM_DEPTH_FRONT - 3;
				if(stockIdx == selected) {
					icon->motion = WINDOW_SPRITE_MOTION_SWELL;
					icon->anmStep = 10;
					icon->depth = OAM_DEPTH_FRONT - 2;
				}
				icon = mnShopCreatePriceSpr(
					(Uint16)item->price, win, pageSlot * 32 + 16, 0);
				icon->depth = OAM_DEPTH_FRONT - 4;
			}
		}
	}
}

/* Draws the party's coins in the coins window, in the edition's layout. */
void mnShopDrawMoney(void)
{
	WINDOW *win = ((SHOP_STATE *)Ram_HeapSlots->menuState)->moneyWin;
#if LANGUAGE == LANGUAGE_JAPANESE
	Sint32 msg;
#endif

	if(win != NULL) {
		mnWindowSprPrepareForRedrawStub(win);
#if LANGUAGE == LANGUAGE_JAPANESE
		/* "Coins" (MsgCoins) after the count, two messages before
		   MsgYourCoins. */
		msg = (Sint32)MsgYourCoins;
		mnTextDrawMessageAsTilesStub(msg, win, 0, 0);
		msg -= 2;
		mnTextDrawNumInWindowStub(
			MainRoutineStateWork.coins, 6, (Sint32)win, 0, 8);
		mnTextDrawMessageAsTilesStub(msg, win, 48, 8);
#else
		mnTextDrawMessageAsTilesStub((Sint32)MsgYourCoins, win, 0, 0);
		mnTextDrawNumInWindowStub(
			MainRoutineStateWork.coins, 6, (Sint32)win, 32, 8);
#endif
	}
}

/* The price line reads "Price", the price, then "Coins". Japanese indents the
   line and its notes by 8 pixels, French draws the price 8 pixels nearer its
   label, Spanish and Italian the coins label 8 pixels further right. */
#define SHOP_ITEM_NOTE_X	JAPANESE_OR(8, 0)
#define SHOP_PRICE_NUMBER_X FRENCH_OR(24, 32)
#define SHOP_PRICE_LABEL_X	BY_LANGUAGE(72, 72, 72, 80, 72, 80)

/* Draws the item's name and below it its price. With a price of 0 it says
   instead that the shop does not buy it (SHOP_PRICE_KIND_SELL) or that it is
   not broken (SHOP_PRICE_KIND_REPAIR); at the forge (SHOP_PRICE_KIND_FORGE)
   `price` is the item's condition, drawn as its word. */
void mnShopDrawItemPrice(
	WINDOW *win, Sint32 itemId, Sint32 price, Sint32 priceKind)
{
	Sint32 labelMsg;

	if(win == 0)
		return;
	goto draw;
cannot_buy:
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgCannotBuyThat, win, SHOP_ITEM_NOTE_X, 8);
	return;
not_broken:
	mnTextDrawMessageAsTilesStub(
		(Sint32)MsgNotBroken, win, SHOP_ITEM_NOTE_X, 8);
	return;
show_price:
	labelMsg = (Sint32)MsgPrice;
	mnTextDrawMessageAsTilesStub(labelMsg, win, SHOP_ITEM_NOTE_X, 8);
	/* "Coins", three messages before "Price". */
	labelMsg -= 3;
	mnTextDrawNumInWindowStub(price, 5, (Sint32)win, SHOP_PRICE_NUMBER_X, 8);
	mnTextDrawMessageAsTilesStub(labelMsg, win, SHOP_PRICE_LABEL_X, 8);
	return;
draw:
	mnWindowSprRedrawSavedRectStub(win);
	mnTextDrawMessageAsTilesStub(itemId + (Sint32)MsgItemName, win, 0, 0);
	if(price == 0) {
		switch(priceKind) {
		case SHOP_PRICE_KIND_SELL:
			goto cannot_buy;
		case SHOP_PRICE_KIND_REPAIR:
			goto not_broken;
		}
	}
	/* MsgItemCondition starts a list: "----------", "Processed", "Rusted". */
	if(priceKind == SHOP_PRICE_KIND_FORGE)
		mnTextDrawMessageAsTilesStub(
			price + (Sint32)MsgItemCondition, win, 8, 8);
	else
		goto show_price;
}

#undef SHOP_ITEM_NOTE_X
#undef SHOP_PRICE_NUMBER_X
#undef SHOP_PRICE_LABEL_X

void mnWindowSprClearListStub(void *win);

/* Shows `msg`, an item's description, in the help window; a NULL window is
   ignored. The Japanese release clears the window and waits a tick; the others
   redraw the window's saved rectangle. */
void mnShopDrawMessage(WINDOW *win, Sint32 msg)
{
	if(win != 0) {
#if LANGUAGE != LANGUAGE_JAPANESE
		mnWindowSprRedrawSavedRectStub(win);
#else
		mnWindowSprClearListStub(win);
		sySynch(1);
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
		mnTextDrawMessageAsTilesStub(msg, win, 0, 0);
#else
		mnTextDrawMessageStub(msg, (Sint32)win, 0, 0);
#endif
	}
}

/* Shows the chosen party member running in place and the others standing, four
   a page with its arrows, and draws at four fifths of their size the party
   members who cannot equip `itemId`. */
void mnShopDrawParty(WINDOW *win, Sint32 selected, Sint32 itemId)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	Sint32		page = selected / SHOP_PARTY_PAGE_MEMBERS;
	Sint32		first = page * SHOP_PARTY_PAGE_MEMBERS;
	Sint32		cnt = shop->partyMemberCnt;
	WINDOW_SPR *icon;
	Sint32		i;

	if(win != 0) {
		mnWindowSprPrepareForRedrawStub(win);
		if(first != 0) {
			icon = mnWindowSprCreateStub(
				shop->prevPageIcon, WINDOW_SPRITE_SIZE_16X16, win, 88, -12);
			icon->kind = WINDOW_SPRITE_PLAIN;
			icon->motion = WINDOW_SPRITE_MOTION_HOP_UP;
			icon->anmStep = 0;
		}
		if(first + SHOP_PARTY_PAGE_MEMBERS < cnt) {
			icon = mnWindowSprCreateStub(
				shop->nextPageIcon, WINDOW_SPRITE_SIZE_16X16, win, 88, 0);
			icon->kind = WINDOW_SPRITE_PLAIN;
			icon->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
			icon->anmStep = 0;
		}
		for(i = 0; i < SHOP_PARTY_PAGE_MEMBERS; i++) {
			Sint32 unitId = shop->partyMemberIds[first + i];
			void  *figure = shop->partyMemberIcons[i];

			if(figure != NULL) {
				if(first + i == selected)
					fdSprStartAnimationStub(figure, ANIM_PARTY_RUN);
				else
					fdSprStartAnimationStub(figure, ANIM_STAND);
				shop->partyMemberScale[i] = toFIXED(1);
				if(!gmItemIsCompatibleWithUnitStub(unitId, itemId))
					shop->partyMemberScale[i] = toFIXED(4) / 5;
			}
		}
	}
}

/* Shows how Attack, Defense and Agility would change if the party member wore
   the item: equips it in place of what it would replace, recomputes, puts the
   slot back and recomputes again, then draws both values with an arrow and the
   name of the item it would replace. Called by mnShopSelectBuy for weapons and
   armor. */
void mnShopDrawEquipComparison(WINDOW *win, Sint32 unitId, Sint32 itemId)
{
	SHOP_STATE		*shop = Ram_HeapSlots->menuState;
	UNIT			*unit = syUnitGet(unitId);
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
	/* Borrow a live slot so gmUnitCalc includes every equipment effect; restore
	   both the slot and the stats before returning to the counter. */
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

/* Shows how many of the item the party member already carries and the party
   member's inventory icons. */
void mnShopDrawCarriedItem(WINDOW *win, Sint32 unitId, Sint32 itemId)
{
	UNIT  *unit;
	Sint32 slot;

	unit = syUnitGet(unitId);
	if(win != 0) {
		mnWindowSprPrepareForRedrawStub(win);
		slot = gmInventoryFindStub(unitId, itemId);
		if(slot != -1) {
			mnMessageArgumentPushStub(
				(unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1,
				MESSAGE_ARGUMENT_NUMBER);
			mnTextDrawMessageAsTilesStub((Sint32)MsgYouHave, win, 0, 0);
		} else {
			mnTextDrawMessageAsTilesStub((Sint32)MsgShopNoneInStock, win, 0, 0);
		}
		mnItemIconGridShowInventory(win, 8, 8, unitId);
	}
}

/* Asks how many to buy when the item stacks, limited by what the coins buy, the
   artifact's stock and a stack of 30 less what the party member carries.
   Returns the count (1 for other items), or -1 when cancelled. */
Sint32 mnShopSelectBuyCount(Sint32 unitId, Sint32 itemId)
{
	SHOP_STATE		*shop;
	UNIT			*unit;
	ITEM_DEFINITION *item;
	Sint32			 carriedQuantity;
	Sint32			 quantityLimit;
	Sint32			 slot;
	Sint32			 artifactsInStock;
	Sint32			 result;

	shop = Ram_HeapSlots->menuState;
	unit = syUnitGet(unitId);
	item = gmItemGetDirectStub(itemId);
	result = 1;
	if(item->flg & ITEM_STACKABLE) {
		mnShopShowKeeperMessage((Sint32)MsgHowManyToBuy);
		slot = gmInventoryFindStub(unitId, itemId);
		if(slot != -1)
			carriedQuantity =
				(unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
		else
			carriedQuantity = 0;

		quantityLimit = SHOP_STACK_LIMIT;
		if((Uint16)item->price != 0)
			quantityLimit = MainRoutineStateWork.coins / (Uint16)item->price;

		if(shop->action == SHOP_ACTION_ARTIFACTS) {
			artifactsInStock = gmItemAdjustRareItemStockStub(itemId, 0);
			if(quantityLimit > artifactsInStock)
				artifactsInStock = gmItemAdjustRareItemStockStub(itemId, 0);
			else
				artifactsInStock = quantityLimit;
			quantityLimit = artifactsInStock;
		}

		quantityLimit += carriedQuantity;
		if(quantityLimit > SHOP_STACK_LIMIT)
			quantityLimit = SHOP_STACK_LIMIT;

		shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
		mnCursorPlace(0, SHOP_QUANTITY_CURSOR_X, 48);
		result = mnShopSelectQuantity(
			carriedQuantity, quantityLimit, (Uint16)item->price);
	}
	return result;
}

/* The quantity window: outside the English edition the count sits further left
   and the price and coins label move with their words; the Spanish, French and
   Italian windows are wider; Japanese draws the counter コ after the count. */
#if LANGUAGE == LANGUAGE_ENGLISH
#define QUANTITY_WINDOW_X	  7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X	  72
#define QUANTITY_PRICE_X	  88
#define QUANTITY_COINS_X	  136
#elif LANGUAGE == LANGUAGE_JAPANESE
#define QUANTITY_WINDOW_X	  7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X	  64
#define QUANTITY_PIECE_X	  80
#define QUANTITY_PRICE_X	  96
#define QUANTITY_COINS_X	  144
#elif LANGUAGE == LANGUAGE_GERMAN
#define QUANTITY_WINDOW_X	  7
#define QUANTITY_WINDOW_WIDTH 23
#define QUANTITY_COUNT_X	  64
#define QUANTITY_PRICE_X	  80
#define QUANTITY_COINS_X	  128
#elif LANGUAGE == LANGUAGE_SPANISH
#define QUANTITY_WINDOW_X	  5
#define QUANTITY_WINDOW_WIDTH 25
#define QUANTITY_COUNT_X	  64
#define QUANTITY_PRICE_X	  104
#define QUANTITY_COINS_X	  160
#else
#define QUANTITY_WINDOW_X	  5
#define QUANTITY_WINDOW_WIDTH 25
#define QUANTITY_COUNT_X	  64
#define QUANTITY_PRICE_X	  88
#define QUANTITY_COINS_X	  144
#endif

/* Lets the player pick a quantity with left and right in a small window that
   shows it and its price in coins: a bar of 30 segments, the ones already
   carried in one colour and the ones chosen in another. Returns the count above
   `minimum`, the items already carried, or -1 when B cancels. */
Sint32 mnShopSelectQuantity(Sint32 minimum, Sint32 maximum, Sint32 price)
{
	Uint8	   *tileBuf;
	WINDOW	   *win;
	Bool		changed;
	Sint32		resourceSlot;
	Sint32		cnt;
	Sint32		result;
	WINDOW_SPR *rightHalf;
	Sint32		selected;

	tileBuf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	changed = TRUE;
	maximum -= minimum;
	result = -1;
	cnt = 0;
	win = mnWindowCreateStub(
		QUANTITY_WINDOW_X, 4, QUANTITY_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);
	if(win != 0) {
		resourceSlot = syVRAMCacheFindFreeEntry();
		if(resourceSlot != VRAM_CACHE_ENTRY_COUNT) {
			syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, 0);
			/* The bar is two 32x8 sprites, the right one four tiles on. */
			mnWindowSprCreateStub(
				resourceSlot, WINDOW_SPRITE_SIZE_32X8, win, 0, 0);
			rightHalf = mnWindowSprCreateStub(
				resourceSlot, WINDOW_SPRITE_SIZE_32X8, win, 32, 0);
			rightHalf->spr.oam.idx += 4;
			for(;;) {
				if(PerWork.push & KEY_A) {
					sySndPlayStub(SE_MENU_CONFIRM);
					result = cnt + 1;
					break;
				}
				if(PerWork.push & KEY_B) {
					sySndPlayStub(SE_MENU_CANCEL);
					result = -1;
					break;
				}
				if(PerWork.repeat & KEY_LEFT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					changed = TRUE;
					cnt--;
				}
				if(PerWork.repeat & KEY_RIGHT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					changed = TRUE;
					cnt++;
				}
				if(changed) {
					changed = FALSE;
					cnt = (cnt + maximum) % maximum;
					syDMAXCopy(Shop_QuantityTiles, tileBuf,
						DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4), REG_DMA3);
					/* Paint capacity, available stock, selection and carried
					   items in that order; each shorter prefix overwrites the
					   previous colour. */
					mnQuantityBarFill(
						SHOP_STACK_LIMIT, QUANTITY_BAR_OUT_OF_REACH, tileBuf);
					mnQuantityBarFill(
						minimum + maximum, QUANTITY_BAR_OPEN, tileBuf);
					mnQuantityBarFill(
						minimum + cnt + 1, QUANTITY_BAR_CHOSEN, tileBuf);
					selected = cnt + 1;
					mnQuantityBarFill(minimum, QUANTITY_BAR_HELD, tileBuf);
					syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuf);
					result = selected;
					mnTextDrawNumInWindowStub(
						result, 2, (Sint32)win, QUANTITY_COUNT_X, 0);
#if LANGUAGE == LANGUAGE_JAPANESE
					/* The counter コ after the count: MsgPiece, which the ROM
					   reaches from MsgCoins, the message before it. */
					mnTextDrawMessageAsTilesStub(
						(Sint32)MsgCoins + 1, win, QUANTITY_PIECE_X, 0);
#endif
					mnTextDrawNumInWindowStub(
						price * result, 6, (Sint32)win, QUANTITY_PRICE_X, 0);
					mnTextDrawMessageAsTilesStub(
						(Sint32)MsgCoins, win, QUANTITY_COINS_X, 0);
				}
				sySynch(1);
			}
			sySynch(1);
			mnWindowCloseStub(win, 2);
		}
	}
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	return result;
}

#undef QUANTITY_WINDOW_X
#undef QUANTITY_WINDOW_WIDTH
#undef QUANTITY_COUNT_X
#undef QUANTITY_PIECE_X
#undef QUANTITY_PRICE_X
#undef QUANTITY_COINS_X

/* Gives the party member each item bought, one at a time, taking its price and
   counting it toward the next Game Ticket; then offers to equip it and to sell
   what it replaced. */
void mnShopBuyDone(Sint32 unitId, Sint32 itemId, Sint32 quantity)
{
	Sint32			 replacedSlot;
	Sint32			 i;
	Sint32			 lastAddedSlot;
	ITEM_DEFINITION *item;

	item = gmItemGetDirectStub(itemId);
	lastAddedSlot = 0;
	replacedSlot = gmInventoryFindEquippedStub(unitId, item->type);
	sySndPlayStub(SE_SHOP_PURCHASE);
	for(i = 0; i < quantity; i++) {
		lastAddedSlot = gmInventoryAddItemStub(unitId, itemId);
		gmPartyAdjustCoinsStub(0 - item->price);
		gmPartyAdjustShopCreditStub(item->price);
		mnShopDrawMoney();
	}
	mnShopShowKeeperMessageHidingCursor(MsgHereYouGo);
	if(mnShopConfirmEquip(unitId, lastAddedSlot) != 0) {
		mnShopSellOld(unitId, replacedSlot);
	}
}

/* Offers to equip the item just bought, and warns of a curse. Returns 1 when it
   was equipped. */
Bool mnShopConfirmEquip(Sint32 unitId, Sint32 slot)
{
	SHOP_STATE		*shop = Ram_HeapSlots->menuState;
	UNIT			*unit = syUnitGet(unitId);
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
		ITEM_DEFINITION *oldInfo =
			gmItemGetDirectStub(unit->inventory[replacedSlot]);

		if(oldInfo->flg & ITEM_CANNOT_UNEQUIP)
			return FALSE;
	}

	mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
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
		mnMessageOpenWindowStub(MsgBecameCursed, 8, 4, MESSAGE_WINDOW_UNREAD);
		while(mnMessageAllTypedStub() == 0) {
			sySynch(1);
		}
	}

	mnShopShowKeeperMessageHidingCursor(MsgLookBolder);
	return TRUE;
}

/* Offers to sell the item the new one replaced, unless there is none or it is a
   Psynergy item or indispensable. Returns 1 when it offered. */
Bool mnShopSellOld(Sint32 unitId, Sint32 slot)
{
	UNIT  *unit;
	Sint32 itemId;

	unit = syUnitGet(unitId);
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
