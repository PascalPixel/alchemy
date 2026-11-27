/* btPlacementContainsId, the same in both games. Both games'
   battle/battle_placement.c include it where the function stands. */

/* Returns 1 if the unit at place is in list. */
Bool btPlacementContainsId(Sint16 *list, Sint32 place)
{
	Uint32 i;

	if(place > ALLY_MAX)
		place += FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
	i = 0;
	do {
		Uint16 unitId = *list++;

		if(unitId == BATTLE_UNIT_LIST_END)
			break;
		if(unitId == place)
			return TRUE;
	} while(++i < UNIT_MAX);
	return FALSE;
}
