/* gmPartyInventoryRemove and gmPartyInventoryDiscard, the same in both
   games. Both games' game/game_inventory.c include it where the functions
   stand. */

/* Removes one `itemId` from whoever carries it first; always returns 0. */
Sint32 gmPartyInventoryRemove(Sint32 itemId)
{
	Sint32 unitId = gmPartyInventoryFindMember(itemId);

	if(unitId == -1)
		return 0;
	gmInventoryRemove(unitId, gmInventoryFind(unitId, itemId));
	return 0;
}

/* Discards one `itemId` from whoever carries it first, as gmInventoryDiscard
   does; always returns 0. */
Sint32 gmPartyInventoryDiscard(Sint32 itemId)
{
	Sint32 unitId = gmPartyInventoryFindMember(itemId);

	if(unitId == -1)
		return 0;
	gmInventoryDiscard(unitId, gmInventoryFind(unitId, itemId));
	return 0;
}
