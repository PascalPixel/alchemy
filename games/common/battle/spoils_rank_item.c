/* btSpoilsRankItem, the same in both games. Both games'
   battle/battle_spoils.c include it where the function stands. */

/* Returns an item's rank among the spoils: its item id
   (INVENTORY_ITEM_ID_MASK's nine bits), with ITEM_RARE's bit above it and
   ITEM_INDISPENSABLE's above that, so an indispensable item ranks first, then a
   rare one, then the higher item id; 0 for none. */
Uint32 btSpoilsRankItem(Uint32 inventoryEntry)
{
	Uint32 pri = 0;
	Uint32 itemId = inventoryEntry & INVENTORY_ITEM_ID_MASK;
	if(itemId == 0)
		return 0;
	{
		Uint8 flg = gmItemGetDirectStub(itemId)->flg;

		if(flg & ITEM_INDISPENSABLE)
			pri = 1;
		pri <<= 1;
		if(flg & ITEM_RARE)
			pri++;
		pri <<= 9;
		pri += itemId;
	}
	return pri;
}
