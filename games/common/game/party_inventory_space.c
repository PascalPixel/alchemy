/* gmPartyInventoryHasSpace and gmPartyInventoryCountFreeSlots, the same in
   both games. Both games' game/game_inventory.c include it where the
   functions stand. */

/* Returns 1 when the leader or any party member has a free inventory slot. */
Bool gmPartyInventoryHasSpace(void)
{
	Sint16	unitIds[10];
	Sint32	unitCnt;
	Sint16 *unitId;
	Sint32	i;

	if(gmInventoryCount(MainRoutineStateWork.leaderActorId) != INVENTORY_SLOTS)
		return TRUE;
	unitCnt = gmPartyListMembers(unitIds);
	unitId = unitIds;
	for(i = 0; i < unitCnt; i++) {
		if(gmInventoryCount(*unitId++) != INVENTORY_SLOTS)
			return TRUE;
	}
	return FALSE;
}

/* Returns the free inventory slots of the whole party. */
Sint32 gmPartyInventoryCountFreeSlots(void)
{
	Sint16	unitIds[10];
	Sint32	unitCnt = gmPartyListMembers(unitIds);
	Sint32	freeSlots = 0;
	Sint16 *unitId = unitIds;
	Sint32	i;

	for(i = 0; i < unitCnt; i++)
		freeSlots = freeSlots - gmInventoryCount(*unitId++) + INVENTORY_SLOTS;
	return freeSlots;
}
