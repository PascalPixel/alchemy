/* gmPartyInventoryAdd, the same in both games. Both games'
   game/game_inventory.c include it where the function stands. */

/* Gives the item to the first party member with room. Returns who took it, or
   -1. */
Sint32 gmPartyInventoryAdd(Sint32 itemId)
{
	Sint16	unitIds[10];
	Sint32	unitCnt = gmPartyListMembers(unitIds);
	Sint16 *cursor = unitIds;
	Sint32	i;

	for(i = 0; i < unitCnt; i++) {
		Sint16 unitId = *cursor++;

		if(gmInventoryAddItem(unitId, itemId) >= 0)
			return unitId;
	}
	return -1;
}
