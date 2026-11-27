/* btUnitIsListed, the same in both games. Both games' battle/battle_action.c
   include it where the function stands. */

/* Returns 1 when the unit is in its side's list of units in the battle
   (btUnitListIds), 0 otherwise. */
Uint32 btUnitIsListed(Uint32 unitId)
{
	Uint16 list[8];
	Sint32 side;
	Sint32 unitCnt;
	Sint32 unitIdx;

	side = BATTLE_SIDE_MASK_PARTY;
	if(unitId > ALLY_MAX)
		side = BATTLE_SIDE_MASK_ENEMIES;
	unitCnt = btUnitListIds(side, list);

	for(unitIdx = 0; unitIdx < unitCnt; unitIdx++) {
		if(list[unitIdx] == unitId)
			break;
	}

	return unitIdx != unitCnt;
}
