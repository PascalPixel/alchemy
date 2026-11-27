/* Steps the vents' counter, wrapping at `limit`. */
Sint32 scVentAdvanceCounter(Sint32 limit)
{
	VENT_WORK *state = VENT_STATE;

	if(fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
		if(++state->count >= limit)
			state->count = 0;
	}
	return state->count;
}
