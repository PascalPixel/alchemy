/* gmDjinnRecoveryCountStandby, the same in both games. Both games'
   game/game_djinn.c include it where the function stands. */

/* Counts the party's Djinn on standby, and by element into `cnts` when it is
   given. */
Sint32 gmDjinnRecoveryCountStandby(Uint8 *cnts)
{
	Sint32				  standbyCnt = 0;
	DJINN_RECOVERY_TABLE *recoveryTbl =
		gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY);
	DJINN_RECOVERY_ENTRY *entry = recoveryTbl->list.entries;
	Sint32				  entryIdx;

	if(cnts != 0) {
		Sint32 element;

		/* Each element's count starts at 0. */
		for(element = 0; element < ELEMENT_COUNT; element++)
			cnts[element] = 0;
	}
	for(entryIdx = 0; entryIdx != recoveryTbl->list.cnt; entryIdx++) {
		if(entry->turns == DJINN_RECOVERY_AFTER_BATTLE) {
			if(cnts != 0)
				cnts[entry->element]++;
			standbyCnt++;
		}
		entry++;
	}
	return standbyCnt;
}
