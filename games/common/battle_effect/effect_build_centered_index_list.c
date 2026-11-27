/* btEffectBuildCenteredIndexList, the same in both games. Both games'
   battle_effect/battle_effect_flare.c include it where the function stands. */

/* Lists the indices within idxRadius of where curValue sits in values, up to
   entryLimit, and returns how many. */
UNUSED Sint32 btEffectBuildCenteredIndexList(
	BATTLE_EFFECT_INDEX_WINDOW *win, Sint16 *output)
{
	Sint32 winLength = win->idxRadius * 2 + 1;
	Sint32 outputCnt = 0;
	Sint32 centerIdx = 0;
	Sint32 winIdx;

	for(winIdx = 0; winIdx != winLength; winIdx++) {
		if(win->curValue == win->values[winIdx]) {
			centerIdx = winIdx;
			break;
		}
	}
	for(winIdx = 0; winIdx != winLength; winIdx++) {
		Sint32 idx = centerIdx + winIdx - win->idxRadius;

		if(idx >= 0 && idx < win->entryLimit) {
			output[outputCnt++] = idx;
		}
	}
	return outputCnt;
}
