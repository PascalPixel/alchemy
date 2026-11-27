/* Run each tick: when the leader steps onto a whole tile it cracks, and onto a
   cracked one other than the one it cracked last, the leader falls through it;
   1 when it falls. */
Bool scCrackedTileStep(void)
{
	ACTOR			*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32			 x = FIELD_METATILE(actor->x.fixed);
	Sint32			 z = FIELD_METATILE(actor->z.fixed);
	METATILE_TARGET *state = &CrackedTiles;
	METATILE_TARGET_ENTRY *entry = state->entries;
	Sint32				   i;

	for(i = 0; i < state->count; i++, entry++) {
		if(entry->x == x && entry->z == z) {
			if(fdScriptGetFlag(entry->flags) == 0) {
				scCrackedTileCrack(state, entry);
				fdScriptSetFlag(entry->flags);
				state->current = i;
				break;
			}
			if(i == state->current)
				break;
			state->current = i;
			scCrackedTileFall(state, entry);
			syGameFlagSetByte(state->flagByte, x);
			syGameFlagSetByte(state->flagByte + 8, z);
			return TRUE;
		}
	}
	return FALSE;
}
