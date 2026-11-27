/* gmPartyListMembers, the same in both games. Both games' game/game_party.c
   include it where the function stands. */

/* Writes the party members to `unitIds`, ended by BATTLE_UNIT_LIST_END, and
   returns how many there are. */
Sint32 gmPartyListMembers(Sint16 *unitIds)
{
	Sint32 cnt = 0;

	if(unitIds != NULL) {
		Sint32 idx;

		cnt = gmPartyCountMembers();
		for(idx = 0; idx != cnt; idx++) {
			*unitIds++ = MainRoutineStateWork.party[idx];
		}
		*unitIds = BATTLE_UNIT_LIST_END;
	}
	return cnt;
}
