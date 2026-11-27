/* mnShopSalePrice, the same in both games. Both games' shop/shop_buy.c
   include it where the function stands. */

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
