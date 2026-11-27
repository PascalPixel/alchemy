/* Selling: choosing a party member, the item grid and the sale. Part of
   shop.c. */

/* Lets the player choose whose items to sell, repair or forge, by shop->action,
   until B or, at the forge, an item is taken. */
Sint32 mnShopChooseMember(void)
{
	SHOP_STATE *shop = Ram_HeapSlots->menuState;
	Sint32		unitId = 0;
	Sint32		memberIdx = 0;
	WINDOW	   *listWin;
	Sint32		redraw = 1;
	Sint32		result;

	shop->moneyWin = mnWindowCreateStub(
		0, 9, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();
	shop->itemWin = mnWindowCreateStub(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
	listWin = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	shop->listWin = listWin;
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	mnShowPartyFiguresStub(listWin, 2, 0);
	while(1) {
		/* FAKEMATCH(dead): updates of unitId that cancel out. */
		unitId++;
		unitId--;
		if(redraw != 0) {
			unitId = shop->partyMemberIds[memberIdx];
			mnCursorPlace(
				listWin, (memberIdx % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
			shop->cursorFrm = 3;
			if(redraw == MENU_GRID_CHANGED_PAGE) {
				mnCreatePartyFiguresStub(memberIdx / SHOP_PARTY_PAGE_MEMBERS *
					SHOP_PARTY_PAGE_MEMBERS);
				sySynch(1);
			}
			mnShopDrawParty(listWin, memberIdx, 0);
			mnShopDrawMemberItems(shop->itemWin, unitId);
			redraw = 0;
		}
		if(PerWork.push & KEY_A) {
			sySynch(1);
			if(gmInventoryCountStub(unitId) == 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				continue;
			}
			mnWindowSprPrepareForRedrawStub(listWin);
			sySndPlayStub(SE_MENU_CONFIRM);
			if(shop->action == SHOP_ACTION_SELL) {
				mnShopSelectSell(unitId);
			} else if(shop->action == SHOP_ACTION_REPAIR) {
				mnShopSelectRepair(unitId);
			} else {
				result = mnForgeSelectItem(unitId);
				if(result == 0)
					break;
			}
			shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
			shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
			redraw = MENU_GRID_CHANGED_PAGE;
		} else if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		} else {
			redraw = mnGridMoveCursor(
				&memberIdx, shop->partyMemberCnt, SHOP_PARTY_PAGE_MEMBERS);
			sySynch(1);
		}
	}
	mnItemIconGridSetVisible(0);
	mnReleasePartyFiguresStub();
	mnWindowCloseStub(listWin, 2);
	mnWindowCloseStub(shop->itemWin, 2);
	mnWindowCloseStub(shop->moneyWin, 2);
	sySynch(1);
	return result;
}

/* The price window and the item details window are sized to their text. */
#define PRICE_WINDOW_X		 BY_LANGUAGE(16, 15, 14, 15, 14, 15)
#define PRICE_WINDOW_WIDTH	 BY_LANGUAGE(14, 15, 16, 15, 16, 15)
#define DETAILS_WINDOW_WIDTH BY_LANGUAGE(13, 16, 18, 18, 18, 18)

/* Lets the player choose items to sell, repair or forge from the party member's
   grid, one at a time, with each one's price and description, until B or the
   inventory is empty. Returns -1 when cancelled, 0 when the inventory ran out
   or, at the forge, an item was taken. */
Sint32 mnShopSelectSell(Sint32 unitId)
{
	SHOP_STATE *shop;
	WINDOW	   *descriptionWin;
	WINDOW	   *priceWin;
	UNIT	   *unit;
	Sint32		inventoryCnt;
	Sint32		slot;
	WINDOW	   *win;
	Bool		redraw;
	Sint32		itemId;
	Sint32		result;
	Sint32		cnt;

	shop = Ram_HeapSlots->menuState;
	unit = syUnitGet(unitId);
	slot = 0;
	inventoryCnt = 1;
	priceWin = mnWindowCreateStub(
		PRICE_WINDOW_X, 8, PRICE_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	do {
		descriptionWin = mnWindowCreateStub(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
		shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
		shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
		redraw = TRUE;
		while(1) {
			if(redraw != 0) {
				redraw = FALSE;
				inventoryCnt = gmInventoryCountStub(unitId);
				if(slot > inventoryCnt - 1)
					slot = inventoryCnt - 1;
				itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
				mnCursorPlace(shop->itemWin, slot % SHOP_INVENTORY_COLUMNS * 16,
					slot / SHOP_INVENTORY_COLUMNS * 16 + 8);
				shop->cursorFrm = 3;
				if(shop->action == SHOP_ACTION_SELL)
					mnShopDrawItemPrice(priceWin, itemId,
						mnShopSalePrice(unit->inventory[slot]),
						SHOP_PRICE_KIND_SELL);
				else if(shop->action == SHOP_ACTION_REPAIR)
					mnShopDrawItemPrice(priceWin, itemId,
						mnShopRepairPrice(unit->inventory[slot]),
						SHOP_PRICE_KIND_REPAIR);
				else
					mnShopDrawItemPrice(priceWin, itemId,
						mnForgeGetItemCondition(itemId), SHOP_PRICE_KIND_FORGE);
				mnShopDrawMessage(
					descriptionWin, itemId + (Sint32)MsgItemDescription);
			}
			if(PerWork.push & KEY_A) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = 0;
				break;
			}
			if(PerWork.push & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				break;
			}
			if(PerWork.data & KEY_SELECT) {
				Uint32 inventoryEntry = unit->inventory[slot];

				sySndPlayStub(SE_RECOVERY);
				win = mnWindowCreateStub(
					0, 9, DETAILS_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
				mnItemMenuDrawItemDetailsStub(win, inventoryEntry);
				*(Uint8 *)&shop->memberSprsShown = 0;
				while(PerWork.data & KEY_SELECT)
					sySynch(1);
				shop->memberSprsShown = 1;
				mnWindowCloseStub(win, 2);
				mnShopDrawMoney();
				mnWindowSprPrepareForRedrawStub(shop->listWin);
#if LANGUAGE != LANGUAGE_JAPANESE && LANGUAGE != LANGUAGE_ENGLISH
				mnShopDrawMemberItems(shop->itemWin, unitId);
#endif
				sySynch(1);
				/* The Japanese release goes on to the next tick without
				   redrawing the cursor, price and name. */
#if LANGUAGE != LANGUAGE_JAPANESE
				redraw = TRUE;
				continue;
#endif
			} else {
				if(PerWork.repeat & KEY_LEFT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					slot--;
					slot = (inventoryCnt + slot) % inventoryCnt;
					redraw = TRUE;
				}
				if(PerWork.repeat & KEY_RIGHT) {
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					slot++;
					slot = (inventoryCnt + slot) % inventoryCnt;
					redraw = TRUE;
				}
				if(PerWork.repeat & KEY_UP) {
					slot -= SHOP_INVENTORY_COLUMNS;
					if(slot < 0)
						slot += INVENTORY_SLOTS;
					while(slot >= inventoryCnt)
						slot -= SHOP_INVENTORY_COLUMNS;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw = TRUE;
				}
				if(PerWork.repeat & KEY_DOWN) {
					slot += SHOP_INVENTORY_COLUMNS;
					if(slot >= inventoryCnt)
						slot -= INVENTORY_SLOTS;
					while(slot < 0)
						slot += SHOP_INVENTORY_COLUMNS;
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					redraw = TRUE;
				}
				sySynch(1);
			}
		}
		mnWindowCloseStub(descriptionWin, 2);
		sySynch(1);
		if(result != 0)
			break;
		if(shop->action == SHOP_ACTION_SELL) {
			cnt = mnShopSelectSellCount(unitId, slot);
			if(cnt != -1)
				mnShopSellItem(unitId, slot, cnt);
			mnShopShowKeeperMessage((Sint32)MsgSellAnythingElse);
		} else if(shop->action == SHOP_ACTION_REPAIR) {
			mnShopRepairItem(unitId, slot);
			mnShopShowKeeperMessage((Sint32)MsgRepairAnythingElse);
		} else {
			result = mnForgeOfferItem(unitId, slot);
			if(result == 0)
				break;
			mnShopShowKeeperMessage((Sint32)MsgWhichItemNeedsAttention);
		}
	} while(gmInventoryCountStub(unitId) != 0);
	mnWindowCloseStub(priceWin, 2);
	return result;
}

#undef PRICE_WINDOW_X
#undef PRICE_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH

/* Where the item grid writes MsgNoItems for a party member with no items, in
   pixels. */
#define SHOP_EMPTY_GRID_X ITALIAN_OR(20, 8)
#define SHOP_EMPTY_GRID_Y ITALIAN_OR(12, 20)

/* Draws the party member's items as icons in rows of five, or a line saying it
   has none. */
void mnShopDrawMemberItems(WINDOW *win, Sint32 unitId)
{
	if(win != NULL) {
		mnWindowSprPrepareForRedrawStub(win);
		mnItemIconGridShowInventory(win, 8, 0, unitId);
		if(gmInventoryCountStub(unitId) == 0)
			mnTextDrawMessageStub((Sint32)MsgNoItems, (Sint32)win,
				SHOP_EMPTY_GRID_X, SHOP_EMPTY_GRID_Y);
	}
}

#undef SHOP_EMPTY_GRID_X
#undef SHOP_EMPTY_GRID_Y

/* Asks how many of a stack to sell, when the party member holds more than one,
   and returns the count, -1 when cancelled; 1 otherwise. */
Sint32 mnShopSelectSellCount(Sint32 unitId, Sint32 slot)
{
	Sint32			 result;
	Sint16			 savedX;
	Sint16			 savedY;
	Sint32			 price;
	Sint32			 cnt;
	Sint32			 selection;
	ITEM_DEFINITION *item;
	SHOP_STATE		*shop;
	UNIT			*unit;

	shop = Ram_HeapSlots->menuState;
	unit = syUnitGet(unitId);
	item = gmItemGetDirectStub(unit->inventory[slot]);
	result = 1;
	price = mnShopSalePrice(unit->inventory[slot]);
	cnt = gmInventoryGetQuantityStub(unitId, slot);
	selection = cnt;
	if((item->flg & ITEM_STACKABLE) && cnt > 1) {
		mnShopShowKeeperMessage((Sint32)MsgHowManyToSell);
		savedX = shop->cursor.tgtX;
		savedY = shop->cursor.tgtY;
		shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
		shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
		mnCursorPlace(NULL, SHOP_QUANTITY_CURSOR_X, 48);
		result = mnShopSelectQuantity(0, selection, price);
		sySynch(1);
		mnIconShowStub(shop->cursor.anchor);
		mnCursorPlace(NULL, savedX, savedY);
	}
	return result;
}

/* Sells `cnt` of the item in `slot` once the player accepts the shopkeeper's
   offer, worded for a broken item, for several or for a rare one. A `cnt` of
   -1 is the offer for the equipment a purchase replaced, and sells one. The
   shop refuses an item it pays nothing for and one its wearer cannot take
   off. */
void mnShopSellItem(Sint32 unitId, Sint32 slot, Sint32 cnt)
{
	SHOP_STATE *shop;

	shop = Ram_HeapSlots->menuState;
	{
		UNIT  *unit = syUnitGet(unitId);
		Sint32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
		ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
		Uint8			 rare = item->flg & ITEM_RARE;
		Sint32			 all = 0;
		Sint32			 total;
		Sint32			 msg;
		Sint32			 i;

		if(cnt == -1) {
			all = 1;
			cnt = 1;
		}
		total = mnShopSalePrice(unit->inventory[slot]) * cnt;
		if(total == 0) {
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			mnShopShowKeeperMessageHidingCursor((Sint32)MsgWeDoNotBuy);
			return;
		}
		if((unit->inventory[slot] & INVENTORY_EQUIPPED) &&
			(item->flg & ITEM_CANNOT_UNEQUIP)) {
			mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
			mnShopShowKeeperMessageHidingCursor((Sint32)MsgCannotSellEquipped);
			return;
		}
		if(all)
			msg = (Sint32)MsgOldItemSellOffer;
		else if(unit->inventory[slot] & INVENTORY_BROKEN)
			msg = (Sint32)MsgDamagedSellOffer;
		else if(cnt > 1)
			msg = (Sint32)MsgSellTotal;
		else if(rare)
			msg = (Sint32)MsgRareSellOffer;
		else
			msg = (Sint32)MsgSellOffer;
		mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
		mnMessageArgumentPushStub(total, MESSAGE_ARGUMENT_NUMBER);
		mnShopShowKeeperMessageHidingCursor(msg);
		if(mnShopAskYesNo(0) != 0) {
			if(rare || all)
				msg = (Sint32)MsgKeepIt;
			else
				msg = (Sint32)MsgChangedMind;
			mnShopShowKeeperMessageHidingCursor(msg);
			return;
		}
		sySndPlayStub(SE_SHOP_SELL);
		for(i = 0; i < cnt; i++)
			gmInventoryDiscardStub(unitId, slot);
		gmUnitRefreshPsynergyStub(unitId);
		gmUnitCalcStub(unitId);
		gmPartyAdjustCoinsStub(total);
		mnShopDrawMoney();
		mnShopDrawMemberItems(shop->itemWin, unitId);
		if(rare || all)
			msg = (Sint32)MsgSold;
		else
			msg = (Sint32)MsgDeal;
		mnShopShowKeeperMessageHidingCursor(msg);
	}
}
