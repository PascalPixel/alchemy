// The Game Ticket counter and every shop's stock. Part of shop.c.

// Gives a Game Ticket once the party's spending in shops reaches the next ticket's cost, and lets
// the player choose who takes it.
void mnShopGiveGameTicket(WINDOW *window, WINDOW *itemWindow)
{
	SHOP_STATE *shop = RamHeapSlots->menuState;
	Uint32      savedSelectedItem;
	Sint32      memberIndex;
	Sint32      unitId;
	Sint32      creditCost;
	Sint32      redraw;
	Sint32      slot;
	Sint32      msgBase;

	memberIndex       = 0;
	savedSelectedItem = shop->selectedItem;
	unitId            = 0;
	creditCost        = ShopGameTicketCreditCosts[MainRoutineStateWork.shopGifts];

	// A ticket is paid from the coins spent in shops, not from the party's coins.
	if (creditCost > MainRoutineStateWork.shopCredit)
		return;
	shop->selectedItem = ITEM_GAME_TICKET;
	mnMessageArgumentPushStub(ITEM_GAME_TICKET, MESSAGE_ARGUMENT_ITEM);
	msgBase = (Sint32)MsgLetMeGiveYouThis;
	// "Let me give you this <item> as a token of my gratitude."
	mnShopShowKeeperMessageHidingCursor(msgBase);
	mnMessageArgumentPushStub(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
	// "So who gets this <item>?"
	mnShopShowKeeperMessageHidingCursor(msgBase + 1);
	redraw = MENU_GRID_CHANGED_PAGE;
	while (1)
	{
		if (redraw != 0)
		{
			unitId = shop->partyMemberIds[memberIndex];
			mnCursorPlace(window, (memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
			shop->cursorFrame = 3;
			if (redraw == MENU_GRID_CHANGED_PAGE)
			{
				mnCreatePartyFiguresStub(memberIndex / SHOP_PARTY_PAGE_MEMBERS
				                         * SHOP_PARTY_PAGE_MEMBERS);
				sySynch(1);
			}
			mnShopDrawParty(window, memberIndex, shop->selectedItem);
			mnShopDrawCarriedItem(itemWindow, unitId, shop->selectedItem);
			redraw = 0;
		}
		if (PerWork.push & KEY_A)
		{
			// Try the ticket in the bag to see that it fits; it is given only after "Then here you
			// go!".
			slot = gmInventoryAddItemStub(unitId, shop->selectedItem);
			if (slot < 0)
			{
				sySoundPlayStub(SE_MENU_CANCEL);
				mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
				mnMessageArgumentPushStub(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
				if (gmInventoryCountStub(unitId) == INVENTORY_SLOTS)
					mnShopShowKeeperMessage((Sint32)MsgPackFull);
				else
					mnShopShowKeeperMessage((Sint32)MsgCannotCarryItem);
				continue;
			}
			gmInventoryRemoveStub(unitId, slot);
			sySoundPlayStub(SE_SHOP_PURCHASE);
			mnShopShowKeeperMessageHidingCursor((Sint32)MsgHereYouGo);
			gmInventoryAddItemStub(unitId, shop->selectedItem);
			gmPartyAdjustShopCreditStub(-creditCost);
			gmPartyAdjustShopGiftsStub(1);
			break;
		}
		else if (PerWork.push & KEY_B)
		{
			mnShopShowKeeperMessageHidingCursor((Sint32)MsgDontWantIt);
			sySoundPlayStub(SE_MENU_CANCEL);
			break;
		}
		else
		{
			redraw =
				mnGridMoveCursor(&memberIndex, shop->partyMemberCount, SHOP_PARTY_PAGE_MEMBERS);
			sySynch(1);
		}
	}
	shop->selectedItem = savedSelectedItem;
}

// Returns how many stock rows there are; mnShopRun refuses a row past them.
Sint32 mnShopStockGetRowCount(void)
{
	return SHOP_STOCK_ROWS;
}

// Adds a shop's artifacts to the shared stock on its first visit, so an item two shops list can be
// bought twice.
void mnShopStockAddArtifacts(Sint32 stockRow)
{
	Sint32 stockedFlag = stockRow + FLAG_SHOP_ARTIFACTS_STOCKED;
	Sint32 artifactIndex;

	if (syGameFlagGet(stockedFlag) == 0)
	{
		syGameFlagSet(stockedFlag);
#if ARTIFACT_STOCK_SKIPS_EMPTY
		// In every edition but the Japanese an empty entry is skipped; the Japanese release stops
		// at the row's first one.
		for (artifactIndex = 0; artifactIndex < SHOP_STOCK_ARTIFACT_COUNT; artifactIndex++)
		{
			Sint16 itemId = ShopStockTable[stockRow][SHOP_STOCK_ARTIFACTS + artifactIndex];

			if (itemId != 0)
				gmItemAdjustRareItemStockStub(itemId, 1);
		}
#else
		for (artifactIndex = 0; artifactIndex < SHOP_STOCK_ARTIFACT_COUNT; artifactIndex++)
		{
			Sint16 itemId = ShopStockTable[stockRow][SHOP_STOCK_ARTIFACTS + artifactIndex];

			if (itemId == 0)
				break;
			gmItemAdjustRareItemStockStub(itemId, 1);
		}
#endif
	}
}

// Copies the shop's ordinary stock, up to 24 items, to `output` with a 0 after, and returns how
// many. Called by mnShopRun for Buy.
Sint32 mnShopStockCopyItems(Sint32 stockRow, Sint16 *output)
{
	const Sint16 *src;
	Sint16       *dest;
	Sint32        count;
	Uint32        item;

	count = 0;
	if (ShopStockTable[stockRow][SHOP_STOCK_ITEMS] != 0)
	{
		dest = output;
		src  = ShopStockTable[stockRow];
		do
		{
			item  = (Uint16)*src;
			*dest = item;
			count++;
			src++;
			dest++;
			if (count > SHOP_STOCK_ITEM_COUNT - 1)
				break;
		} while (*src != 0);
	}
	output[count] = 0;
	return count;
}

// Returns the shop's type from the last column of its stock row (enum SHOP_TYPE).
Sint32 mnShopStockGetType(Sint32 stockRow)
{
	return ShopStockTable[stockRow][SHOP_STOCK_TYPE];
}
