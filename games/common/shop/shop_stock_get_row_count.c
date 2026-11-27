/* mnShopStockGetRowCount, the same in both games. Both games'
   shop/shop_stock.c include it where the function stands. */

/* Returns how many stock rows there are; mnShopRun refuses a row past them. */
Sint32 mnShopStockGetRowCount(void)
{
	return SHOP_STOCK_ROWS;
}
