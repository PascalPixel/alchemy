/* Repairing broken items. Part of shop.c. */

/* Returns what a repair costs: a quarter of the price for a broken item,
   nothing for one that is not broken. */
Sint32 mnShopRepairPrice(Sint32 inventoryEntry)
{
	Sint32 price = (Uint16)gmItemGetDirectStub(inventoryEntry)->price >> 2;

	if((inventoryEntry & INVENTORY_BROKEN) == 0)
		price = 0;
	return price;
}

/* Runs the repair counter's party member choice. Called by mnShopRun for
   Repair. */
Sint32 mnShopRunRepairMemberChoice(void)
{
	return mnShopChooseMember();
}

/* Lets the player choose items to repair on the sell counter's grid, which
   serves repairs too by shop->action. */
Sint32 mnShopSelectRepair(Sint32 unitId)
{
	return mnShopSelectSell(unitId);
}

/* Repairs the item in `slot`: refuses one that cannot break, is not broken or
   is equipped and cursed on, or that the coins do not cover; asks, then takes
   the item away for the smith's three hammer blows, gives it back mended,
   charges the price and offers to equip it. */
void mnShopRepairItem(Sint32 unitId, Sint32 slot)
{
	SHOP_STATE		*shop = RamHeapSlots->menuState;
	UNIT			*unit = syUnitGet(unitId);
	Sint32			 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
	ITEM_DEFINITION *item = gmItemGetDirectStub(itemId);
	Sint32			 equipped = gmInventoryFindEquippedStub(unitId, item->type);
	Uint32			 repairCost;
	Sint32			 msgBase;
	Uint8			 useType;
	Uint32			 savedEntry;

	repairCost = mnShopRepairPrice(unit->inventory[slot]);
	useType = item->useType;
	if(useType != ITEM_USE_MAY_BREAK) {
		mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
		mnShopShowKeeperMessageHidingCursor((Sint32)MsgRepairNotHandled);
		return;
	}
	if(!(unit->inventory[slot] & INVENTORY_BROKEN)) {
		mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
		mnShopShowKeeperMessageHidingCursor((Sint32)MsgRepairNotDamaged);
		return;
	}
	/* useType is ITEM_USE_MAY_BREAK here, so this tests ITEM_CANNOT_UNEQUIP. */
	if((unit->inventory[slot] & INVENTORY_EQUIPPED) && (useType & item->flg)) {
		mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
		mnShopShowKeeperMessageHidingCursor((Sint32)MsgRepairStuckOn);
		return;
	}
	if(repairCost > MainRoutineStateWork.coins) {
		mnShopShowKeeperMessageHidingCursor((Sint32)MsgRepairNotEnoughMoney);
		return;
	}
	mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
	mnMessageArgumentPushStub(repairCost, MESSAGE_ARGUMENT_NUMBER);
	msgBase = (Sint32)MsgRepairOffer;
	/* "I'll fix your <item> for <n> coins, OK?" */
	mnShopShowKeeperMessageHidingCursor(msgBase);
	if(mnShopAskYesNo(0) != 0) {
		/* "Changed your mind? Whatever you say!" */
		mnShopShowKeeperMessageHidingCursor(msgBase + 1);
		return;
	}
	/* Hide the item while it is repaired, without compacting its slot. */
	savedEntry = unit->inventory[slot];
	unit->inventory[slot] = 0;
	mnShopDrawMemberItems(shop->itemWin, unitId);
	mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
	/* "Yes, sir! I'll take your <item>, then." */
	mnShopShowKeeperMessageHidingCursor(msgBase + 2);
	mnMessageCloseFinishedWindowsStub();
	/* The hammer blows are 110 ticks apart; the item stays hidden
	   throughout. */
	sySynch(10);
	sySndPlayStub(SE_BELL);
	sySynch(110);
	sySndPlayStub(SE_BELL);
	sySynch(110);
	sySndPlayStub(SE_BELL);
	sySynch(110);
	sySndPlayStub(SE_MENU_CONFIRM);
	sySynch(20);
	unit->inventory[slot] = savedEntry;
	gmInventoryRepairStub(unitId, slot);
	gmPartyAdjustCoinsStub(-repairCost);
	mnShopDrawMoney();
	mnShopDrawMemberItems(shop->itemWin, unitId);
	mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
	/* "I've repaired your <item>. Here it is." */
	mnShopShowKeeperMessageHidingCursor(msgBase + 3);
	if(mnShopConfirmEquip(unitId, slot))
		mnShopSellOld(unitId, equipped);
}
