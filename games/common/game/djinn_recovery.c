/* gmDjinnRecoveryRemove and gmDjinnRecoveryAddStandby, the same in both
   games. Both games' game/game_djinn.c include it where the functions stand. */

/* Takes the Djinni off its side's standby and recovery list, closing the gap.
   Returns 1 when it was there. */
Sint32 gmDjinnRecoveryRemove(Sint32 unitId, Sint32 element, Sint32 djinnIdx)
{
	DJINN_RECOVERY_LIST *list;
	Bool				 removed = FALSE;
	Sint32				 entryIdx;

	list = &gmDjinnGetRecoveryTable(unitId < 0 || unitId > PIERS)->list;
	for(entryIdx = 0; entryIdx < list->cnt; entryIdx++) {
		if(element == list->entries[entryIdx].element &&
			djinnIdx == list->entries[entryIdx].idx) {
			list->cnt--;
			removed = TRUE;
			break;
		}
	}
	for(; entryIdx < list->cnt; entryIdx++) {
		list->entries[entryIdx] = list->entries[entryIdx + 1];
	}
	return removed;
}

/* Puts a unit's Djinni at the end of its side's list on standby. Nothing checks
   the list's capacity. */
Uint32 *gmDjinnRecoveryAddStandby(
	Uint32 unitId, Uint32 element, Uint32 djinnIdx)
{
	DJINN_RECOVERY_LIST *list;
	Sint32				 cnt;

	gmDjinnRecoveryRemove(unitId, element, djinnIdx);
	list = &gmDjinnGetRecoveryTable(unitId > PIERS)->list;
	cnt = list->cnt;
	list->entries[cnt].element = element;
	list->entries[cnt].idx = djinnIdx;
	list->entries[cnt].unitId = unitId;
	list->entries[cnt].turns = DJINN_RECOVERY_AFTER_BATTLE;
	list->cnt = cnt + 1;
	return (Uint32 *)&list->cnt;
}
