/* Sets a vent's actor. */
void scVentSetId(s32 i, s32 id) {
	VENT_WORK *state = VENT_STATE;

	if((u32)i <= 3)
		state->ids[i] = id;
}
