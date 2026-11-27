/* Whether vent index is blowing now. */
Sint32 scVentIsBlowing(Sint32 idx)
{
	VENT_WORK *state;

	state = VENT_STATE;
	return VENT_AT(state, idx)->entries[0].active;
}
