/* mnShopBuyDone, the same in both games. Both games' shop/shop_buy.c include
   it where the function stands. */

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
