/* mnPsynergyListDiff, the same in both games. Both games' menu/menu_djinn.c
   include it where the function stands. */

/* Lists the Psynergy a party member has after a Djinn change, marking with
   DJINN_PREVIEW_GAINED those they lacked before, then appends what they had and
   lose, marked DJINN_PREVIEW_LOST. Returns how many it wrote, and the gains and
   losses in the two counts. */
Sint32 mnPsynergyListDiff(PSYNERGY_ENTRY *before, PSYNERGY_ENTRY *after,
	Uint16 *actionsOut, Sint32 *gainedCntOut, Sint32 *lostCntOut)
{
	Sint32 actionIdx;
	Sint32 otherIdx;
	Sint32 actionCnt;
	Sint32 gainedCnt;
	Sint32 lostCnt;

	actionCnt = 0;
	gainedCnt = 0;
	lostCnt = 0;
	for(actionIdx = 0;
		actionIdx < PSYNERGY_SLOTS && after[actionIdx].encodedAction != 0;
		actionIdx++) {
		actionsOut[actionCnt] =
			after[actionIdx].encodedAction & PSYNERGY_ABILITY_MASK;
		actionCnt++;
		for(otherIdx = 0; otherIdx < PSYNERGY_SLOTS; otherIdx++) {
			if(((after[actionIdx].encodedAction ^
					before[otherIdx].encodedAction) &
				   PSYNERGY_ABILITY_MASK) == 0)
				break;
		}
		if(otherIdx == PSYNERGY_SLOTS) {
			gainedCnt++;
			actionsOut[actionCnt - 1] |= DJINN_PREVIEW_GAINED;
		}
	}
	for(actionIdx = 0;
		actionIdx < PSYNERGY_SLOTS && before[actionIdx].encodedAction != 0;
		actionIdx++) {
		for(otherIdx = 0; otherIdx < PSYNERGY_SLOTS; otherIdx++) {
			if(((before[actionIdx].encodedAction ^
					after[otherIdx].encodedAction) &
				   PSYNERGY_ABILITY_MASK) == 0)
				break;
		}
		if(otherIdx == PSYNERGY_SLOTS) {
			lostCnt++;
			actionsOut[actionCnt] =
				(before[actionIdx].encodedAction & PSYNERGY_ABILITY_MASK) |
				DJINN_PREVIEW_LOST;
			actionCnt++;
		}
	}
	*gainedCntOut = gainedCnt;
	*lostCntOut = lostCnt;
	return actionCnt;
}
