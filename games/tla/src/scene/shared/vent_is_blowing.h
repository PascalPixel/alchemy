/* Whether vent index is blowing now. */
Sint32 scVentIsBlowing(Sint32 index)
{
	VENT_WORK *state;

	state = VENT_STATE;
	return VENT_AT(state, index)->entries[0].active;
}
