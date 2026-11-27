/* The Game Ticket counter and every shop's stock. Part of shop.c. */

/* Gives a Game Ticket once the party's spending in shops reaches the next
   ticket's cost, and lets the player choose who takes it. */
void mnShopGiveGameTicket(WINDOW *win, WINDOW *itemWin)
{
	SHOP_STATE *shop = RamHeapSlots->menuState;
	Uint32		savedSelectedItem;
	Sint32		memberIdx;
	Sint32		unitId;
	Sint32		creditCost;
	Sint32		redraw;
	Sint32		slot;
	Sint32		msgBase;

	memberIdx = 0;
	savedSelectedItem = shop->selectedItem;
	unitId = 0;
	creditCost = ShopGameTicketCreditCosts[MainRoutineStateWork.shopGifts];

	/* A ticket is paid from the coins spent in shops, not from the party's
	   coins. */
	if(creditCost > MainRoutineStateWork.shopCredit)
		return;
	shop->selectedItem = ITEM_GAME_TICKET;
	mnMessageArgumentPushStub(ITEM_GAME_TICKET, MESSAGE_ARGUMENT_ITEM);
	msgBase = (Sint32)MsgLetMeGiveYouThis;
	/* "Let me give you this <item> as a token of my gratitude." */
	mnShopShowKeeperMessageHidingCursor(msgBase);
	mnMessageArgumentPushStub(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
	/* "So who gets this <item>?" */
	mnShopShowKeeperMessageHidingCursor(msgBase + 1);
	redraw = MENU_GRID_CHANGED_PAGE;
	while(1) {
		if(redraw != 0) {
			unitId = shop->partyMemberIds[memberIdx];
			mnCursorPlace(
				win, (memberIdx % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
			shop->cursorFrm = 3;
			if(redraw == MENU_GRID_CHANGED_PAGE) {
				mnCreatePartyFiguresStub(memberIdx / SHOP_PARTY_PAGE_MEMBERS *
					SHOP_PARTY_PAGE_MEMBERS);
				sySynch(1);
			}
			mnShopDrawParty(win, memberIdx, shop->selectedItem);
			mnShopDrawCarriedItem(itemWin, unitId, shop->selectedItem);
			redraw = 0;
		}
		if(PerWork.push & KEY_A) {
			/* Try the ticket in the bag to see that it fits; it is given only
			   after "Then here you go!". */
			slot = gmInventoryAddItemStub(unitId, shop->selectedItem);
			if(slot < 0) {
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
			gmInventoryRemoveStub(unitId, slot);
			sySndPlayStub(SE_SHOP_PURCHASE);
			mnShopShowKeeperMessageHidingCursor((Sint32)MsgHereYouGo);
			gmInventoryAddItemStub(unitId, shop->selectedItem);
			gmPartyAdjustShopCreditStub(-creditCost);
			gmPartyAdjustShopGiftsStub(1);
			break;
		} else if(PerWork.push & KEY_B) {
			mnShopShowKeeperMessageHidingCursor((Sint32)MsgDontWantIt);
			sySndPlayStub(SE_MENU_CANCEL);
			break;
		} else {
			redraw = mnGridMoveCursor(
				&memberIdx, shop->partyMemberCnt, SHOP_PARTY_PAGE_MEMBERS);
			sySynch(1);
		}
	}
	shop->selectedItem = savedSelectedItem;
}

#include "shop/shop_stock_get_row_count.c"

/* Adds a shop's artifacts to the shared stock on its first visit, so an item
   two shops list can be bought twice. */
void mnShopStockAddArtifacts(Sint32 stockRow)
{
	Sint32 stockedFlag = stockRow + FLAG_SHOP_ARTIFACTS_STOCKED;
	Sint32 artifactIdx;

	if(syGameFlagGet(stockedFlag) == 0) {
		syGameFlagSet(stockedFlag);
#if ARTIFACT_STOCK_SKIPS_EMPTY
		/* In every edition but the Japanese an empty entry is skipped; the
		   Japanese release stops at the row's first one. */
		for(artifactIdx = 0; artifactIdx < SHOP_STOCK_ARTIFACT_COUNT;
			artifactIdx++) {
			Sint16 itemId =
				ShopStockTable[stockRow][SHOP_STOCK_ARTIFACTS + artifactIdx];

			if(itemId != 0)
				gmItemAdjustRareItemStockStub(itemId, 1);
		}
#else
		for(artifactIdx = 0; artifactIdx < SHOP_STOCK_ARTIFACT_COUNT;
			artifactIdx++) {
			Sint16 itemId =
				ShopStockTable[stockRow][SHOP_STOCK_ARTIFACTS + artifactIdx];

			if(itemId == 0)
				break;
			gmItemAdjustRareItemStockStub(itemId, 1);
		}
#endif
	}
}

/* Copies the shop's ordinary stock, up to 24 items, to `output` with a 0 after,
   and returns how many. Called by mnShopRun for Buy. */
Sint32 mnShopStockCopyItems(Sint32 stockRow, Sint16 *output)
{
	const Sint16 *src;
	Sint16		 *dst;
	Sint32		  cnt;
	Uint32		  item;

	cnt = 0;
	if(ShopStockTable[stockRow][SHOP_STOCK_ITEMS] != 0) {
		dst = output;
		src = ShopStockTable[stockRow];
		do {
			item = (Uint16)*src;
			*dst = item;
			cnt++;
			src++;
			dst++;
			if(cnt > SHOP_STOCK_ITEM_COUNT - 1)
				break;
		} while(*src != 0);
	}
	output[cnt] = 0;
	return cnt;
}

/* Returns the shop's type from the last column of its stock row (enum
   SHOP_TYPE). */
Sint32 mnShopStockGetType(Sint32 stockRow)
{
	return ShopStockTable[stockRow][SHOP_STOCK_TYPE];
}
