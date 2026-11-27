/* mnItemRemoveFromInventory, the same in both games. Both games'
   window/window_shortcuts.c include it where the function stands. */

/* Removes itemId from the party's inventory and returns 0. */
Sint32 mnItemRemoveFromInventory(Sint32 unused, Sint32 itemId)
{
	gmPartyInventoryRemoveStub(itemId);
	return 0;
}
