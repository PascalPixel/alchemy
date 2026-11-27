// Sets the speed of vent index's gust, in 16.16 pixels a tick.
void scVentSetGust(Sint32 index, Sint32 value)
{
	VENT_WORK *state;

	state                                   = VENT_STATE;
	VENT_AT(state, index)->entries[0].value = value;
}
