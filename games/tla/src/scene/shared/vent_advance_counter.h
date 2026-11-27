/* Steps the vents' counter, wrapping at `limit`. */
s32 scVentAdvanceCounter(s32 limit) {
	VENT_WORK *state = VENT_STATE;

	if(fdEventGetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
		if(++state->cnt >= limit)
			state->cnt = 0;
	}
	return state->cnt;
}
