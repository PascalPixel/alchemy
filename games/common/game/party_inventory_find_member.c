/* gmPartyInventoryFindMember, the same in both games. Both games'
   game/game_inventory.c include it where the function stands. */

/* Returns who carries `itemId`, asking the leader first, or -1. */
Sint32 gmPartyInventoryFindMember(Sint32 itemId)
{
	Sint16	unitIds[10];
	Sint32	unitCnt;
	Sint16 *cursor;
	Sint32	i;

	if(gmInventoryFind(MainRoutineStateWork.leaderActorId, itemId) != -1)
		return MainRoutineStateWork.leaderActorId;
	unitCnt = gmPartyListMembers(unitIds);
	cursor = unitIds;
	for(i = 0; i < unitCnt; i++) {
		Sint16 unitId = *cursor++;

		if(gmInventoryFind(unitId, itemId) != -1)
			return unitId;
	}
	return -1;
}
