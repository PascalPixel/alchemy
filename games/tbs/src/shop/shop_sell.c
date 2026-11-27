/* Selling: choosing a party member, the item grid and the sale. Part of
   shop.c. */

#include "shop_internal.h"

/* Lets the player choose whose items to sell or repair, until B. */
Sint32 mnShopChooseMember(void)
{
	SHOP_STATE *shop = MenuState;
	WINDOW	   *listWin;
	Sint32		memberIdx = 0;
	Bool		redraw = TRUE;
	Sint32		unitId = 0;

	shop->moneyWin = mnWindowCreateStub(
		0, 9, SHOP_MONEY_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
	mnShopDrawMoney();
	shop->itemWin = mnWindowCreateStub(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
	listWin = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
	shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
	shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
	mnCreatePartyFiguresStub(listWin, 2, 0, 8, 0);

	for(;;) {
		if(redraw != 0) {
			redraw = FALSE;
			memberIdx =
				(memberIdx + shop->partyMemberCnt) % shop->partyMemberCnt;
			unitId = shop->partyMemberIds[memberIdx];
			mnCursorPlace(listWin, memberIdx * 24 - 12, 0);
			shop->cursorFrm = 3;
			mnShopDrawParty(listWin, memberIdx, 0);
			mnShopDrawMemberItems(shop->itemWin, unitId);
		}

		if((PerPush & KEY_A) != 0) {
			sySynch(1);
			if(gmInventoryCountStub(unitId) == 0) {
				sySndPlayStub(SE_MENU_CANCEL);
			} else {
				sySndPlayStub(SE_MENU_CONFIRM);
				if(shop->action == SHOP_ACTION_SELL)
					mnShopSelectSell(unitId);
				else
					mnShopSelectRepair(unitId);
				shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
				shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
				redraw = TRUE;
			}
			continue;
		}

		if((PerPush & KEY_B) != 0) {
			sySndPlayStub(SE_MENU_CANCEL);
			mnReleasePartyFiguresStub();
			mnWindowCloseStub(listWin, 2);
			mnWindowCloseStub(shop->itemWin, 2);
			mnWindowCloseStub(shop->moneyWin, 2);
			sySynch(1);
			return 0;
		}

		if((PerRepeat & KEY_LEFT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIdx--;
			redraw = TRUE;
		}
		if((PerRepeat & KEY_RIGHT) != 0) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			memberIdx++;
			redraw = TRUE;
		}
		sySynch(1);
	}
}

/* Lets the player choose items to sell from the party member's grid, one at a
   time, with each one's price and description, until B or the inventory is
   empty. Returns -1 when cancelled, 0 when the inventory ran out. */
Sint32 mnShopSelectSell(Sint32 unitId)
{
	WINDOW	   *descriptionWin;
	SHOP_STATE *shop;
	WINDOW	   *priceWin;
	UNIT	   *unit;
	Sint32		slot;
	Sint32		inventoryCnt;
	Bool		redraw;
	Sint32		result;
	Sint32		itemId;
	Sint32		quantity;
	WINDOW	   *win;
	Sint32		x;

	shop = MenuState;
	unit = gmUnitGetStub(unitId);
	inventoryCnt = 1;
	priceWin = mnWindowCreateStub(
		SHOP_SELL_PRICE_X, 8, SHOP_SELL_PRICE_WIDTH, 4, WINDOW_FLAG_INSTANT);
	slot = 0;

	for(;;) {
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
				win = shop->itemWin;
				x = slot % SHOP_INVENTORY_COLUMNS * 16;
				mnCursorPlace(win, x, slot / SHOP_INVENTORY_COLUMNS * 16 + 8);
				shop->cursorFrm = 3;
				mnShopDrawItemPrice(priceWin, itemId,
					mnShopSalePrice(unit->inventory[slot]),
					SHOP_PRICE_KIND_SELL);
				mnShopDrawMessage(descriptionWin, itemId + MsgItemPlainName);
			}

			if((PerPush & KEY_A) != 0) {
				sySndPlayStub(SE_MENU_CONFIRM);
				result = 0;
				break;
			}
			if((PerPush & KEY_B) != 0) {
				sySndPlayStub(SE_MENU_CANCEL);
				result = -1;
				break;
			}
			if((PerRepeat & KEY_LEFT) != 0) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				slot--;
				slot = (slot + inventoryCnt) % inventoryCnt;
				redraw = TRUE;
			}
			if((PerRepeat & KEY_RIGHT) != 0) {
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				slot++;
				slot = (slot + inventoryCnt) % inventoryCnt;
				redraw = TRUE;
			}
			/* Up and down wrap round the 15-slot grid onto filled slots. */
			if((PerRepeat & KEY_UP) != 0) {
				slot -= SHOP_INVENTORY_COLUMNS;
				if(slot < 0)
					slot += INVENTORY_SLOTS;
				while(slot >= inventoryCnt)
					slot -= SHOP_INVENTORY_COLUMNS;
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				redraw = TRUE;
			}
			if((PerRepeat & KEY_DOWN) != 0) {
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

		mnWindowCloseStub(descriptionWin, 2);
		sySynch(1);
		if(result != 0)
			break;

		quantity = mnShopSelectSellCount(unitId, slot);
		if(quantity != -1)
			mnShopSellItem(unitId, slot, quantity);
		/* "Want to sell anything else?" */
		mnShopShowKeeperMessage(MsgSellAnythingElse);
		if(gmInventoryCountStub(unitId) == 0)
			break;
	}

	mnWindowCloseStub(priceWin, 2);
	return result;
}

/* Draws the party member's items as icons in rows of five, or a line saying it
   has none. */
void mnShopDrawMemberItems(WINDOW *win, Sint32 unitId)
{
	UNIT	   *unit;
	Sint32		x;
	Sint32		y;
	Sint32		slot;
	WINDOW_SPR *icon;

	unit = gmUnitGetStub(unitId);
	x = 8;
	y = 0;
	if(win != 0) {
		mnWindowSprPrepareForRedrawStub(win);
		if(gmInventoryCountStub(unitId) == 0) {
			mnTextDrawMessageStub(
				MsgNoItems, win, SHOP_EMPTY_GRID_X, SHOP_EMPTY_GRID_Y);
		} else {
			for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
				if(unit->inventory[slot] != 0) {
					icon = mnWindowSprCreateLoadedStub(unit->inventory[slot],
						ICON_BASE | ICON_COUNT_ABOVE_ONE | ICON_BROKEN |
							ICON_EQUIPPED,
						win, x, y);
					icon->depth = OAM_DEPTH_FRONT - 3;
				}
				x += 16;
				if(slot == SHOP_INVENTORY_COLUMNS - 1) {
					x = 8;
					y += 16;
				}
				if(slot == SHOP_INVENTORY_COLUMNS * 2 - 1) {
					x = 8;
					y += 16;
				}
			}
		}
	}
}

/* Asks how many to sell of a stack, and returns the count, -1 when
   cancelled. */
Sint32 mnShopSelectSellCount(Sint32 unitId, Sint32 slot)
{
	Sint32			 result;
	Sint16			 savedX;
	Sint16			 savedY;
	Sint32			 unitSalePrice;
	Sint32			 stackQuantity;
	ITEM_DEFINITION *item;
	SHOP_STATE		*shop;
	UNIT			*unit;

	shop = MenuState;
	unit = gmUnitGetStub(unitId);
	item = gmItemGetDirectStub(unit->inventory[slot]);
	result = 1;
	unitSalePrice = mnShopSalePrice(unit->inventory[slot]);
	/* gmInventoryGetQuantityStub is game.s's name for gmInventoryGetQuantity */
	stackQuantity = gmInventoryGetQuantityStub(unitId, slot);
	if((item->flg & ITEM_STACKABLE) && stackQuantity > 1) {
		/* "How many will you sell?" */
		mnShopShowKeeperMessage(MsgHowManyToSell);
		savedX = shop->cursor.tgtX;
		savedY = shop->cursor.tgtY;
		shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
		shop->cursorFrm = SHOP_CURSOR_ENTER_TICKS;
		mnCursorPlace(NULL, SHOP_QUANTITY_CURSOR_X, 48);
		result = mnShopSelectQuantity(0, stackQuantity, unitSalePrice);
		sySynch(1);
		mnIconShowStub(shop->cursor.anchor);
		mnCursorPlace(NULL, savedX, savedY);
	}
	return result;
}

/* Sells `cnt` of the item in `slot`, after the shopkeeper's offer that fits
   it and a Yes; whatever the shops keep a stock of goes back on sale. A count
   of -1 sells the one old piece of equipment a purchase replaced. */
void mnShopSellItem(Sint32 unitId, Sint32 slot, Sint32 cnt)
{
	SHOP_STATE		*shop = MenuState;
	UNIT			*unit = gmUnitGetStub(unitId);
	Sint32			 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
	Uint8			 rare = item->flg & ITEM_RARE;
	Sint32			 sellingReplacedItem = 0;
	Sint32			 total;
	Sint32			 msg;
	Sint32			 soldCnt;

	if(cnt == -1) {
		sellingReplacedItem = 1;
		cnt = 1;
	}
	total = mnShopSalePrice(unit->inventory[slot]) * cnt;
	if(total == 0) {
		mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
		/* "Sorry, but we don't buy those." */
		mnShopShowKeeperMessageHidingCursor(MsgWeDoNotBuy);
		return;
	}
	if((unit->inventory[slot] & INVENTORY_EQUIPPED) &&
		(item->flg & ITEM_CANNOT_UNEQUIP)) {
		mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
		/* "You can't remove your (item). Come back once it's off." */
		mnShopShowKeeperMessageHidingCursor(MsgCannotSellEquipped);
		return;
	}
	/* "How about (total) coins for your old (item)?", "Your (item) is damaged,
	   so...maybe (total) coins?", "That comes to (total) coins! Sound good?",
	   "What rare stuff! How about (total) coins?" or "How about (total) coins
	   for your (item)?" */
	if(sellingReplacedItem)
		msg = MsgOldItemSellOffer;
	else if(unit->inventory[slot] & INVENTORY_BROKEN)
		msg = MsgDamagedSellOffer;
	else if(cnt > 1)
		msg = MsgSellTotal;
	else if(rare)
		msg = MsgRareSellOffer;
	else
		msg = MsgSellOffer;
	mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
	mnMessageArgumentPushStub(total, MESSAGE_ARGUMENT_NUMBER);
	mnShopShowKeeperMessageHidingCursor(msg);
	if(mnShopAskYesNo(0) != 0) {
		/* "You don't want to let go of it, do you?", or "What? Changed your
		   mind? Well, too bad for you." */
		if(rare || sellingReplacedItem)
			msg = MsgKeepIt;
		else
			msg = MsgChangedMind;
		mnShopShowKeeperMessageHidingCursor(msg);
		return;
	}
	sySndPlayStub(SE_SHOP_SELL);
	for(soldCnt = 0; soldCnt < cnt; soldCnt++)
		gmInventoryDiscardStub(unitId, slot);
	gmPartyAdjustCoinsStub(total);
	mnShopDrawMoney();
	mnShopDrawMemberItems(shop->itemWin, unitId);
	/* "All right, I'll buy it! Here are your coins.", or "OK! It's a deal!" */
	if(rare || sellingReplacedItem)
		msg = MsgSold;
	else
		msg = MsgDeal;
	mnShopShowKeeperMessageHidingCursor(msg);
}
