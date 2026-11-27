/* gmPartyInventoryCountItem, the same in both games. Both games'
   game/game_inventory.c include it where the function stands. */

/* Counts `itemId` across the party. */
Sint32 gmPartyInventoryCountItem(Sint32 itemId)
{
	Uint16 unitIds[16];
	Sint32 cnt = 0;
	Sint32 unitCnt = gmPartyListMembers(unitIds);
	Sint32 i;

	for(i = 0; i < unitCnt; i++)
		cnt += gmInventoryCountItem(unitIds[i], itemId);
	return cnt;
}
