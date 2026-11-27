/* Unused: steps the flame throwers' counter while the party walks, wrapping */
/* at `limit`, as the vents' scVentAdvanceCounter does. */
UNUSED s32 scFlameThrowersAdvanceCounter(s32 limit) {
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	if(!fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
		if(++state->cnt >= limit)
			state->cnt = 0;
	}
	return state->cnt;
}
