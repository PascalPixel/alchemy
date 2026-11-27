/* Runs the room's hook, then steps the vents: each active one puffs every ten
   frames. */
void scVentUpdate(void)
{
	VENT_WORK  *state = VENT_STATE;
	VENT_ENTRY *entry = state->entries;
	Sint32		i;

	state->hook();
	for(i = sizeof(state->entries) / sizeof(*entry) - 1; i >= 0; i--, entry++) {
		if(entry->active != 0) {
			if(entry->tmr <= 0) {
				scVentPuff(&entry->x, entry->value);
				entry->tmr = 9;
			} else {
				entry->tmr--;
			}
		}
	}
}
