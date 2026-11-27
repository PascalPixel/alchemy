/* Steps the vents' counter, wrapping at `limit`. */
Sint32 scVentAdvanceCounter(Sint32 limit)
{
	VENT_WORK *state = VENT_STATE;

	if(fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
		if(++state->cnt >= limit)
			state->cnt = 0;
	}
	return state->cnt;
}
