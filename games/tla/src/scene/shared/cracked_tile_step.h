/* Run each frame: when the leader steps onto a whole tile it cracks, */
/* and onto a cracked one other than the one it cracked last, the leader falls
 */
/* through it; 1 when it falls. */
s32 scCrackedTileStep(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = FIELD_CELL(actor->x.fixed);
	s32 z = FIELD_CELL(actor->z.fixed);
	CELL_TARGET *state = &CrackedTiles;
	CELL_TARGET_ENTRY *entry = state->entries;
	s32 i;

	for(i = 0; i < state->cnt; i++, entry++) {
		if(entry->x == x && entry->z == z) {
			if(fdEventGetFlag(entry->flg) == 0) {
				scCrackedTileCrack(state, entry);
				fdEventSetFlag(entry->flg);
				state->cur = i;
				break;
			}
			if(i == state->cur)
				break;
			state->cur = i;
			scCrackedTileFall(state, entry);
			syGameFlagSetByte(state->flgByte, x);
			syGameFlagSetByte(state->flgByte + 8, z);
			return 1;
		}
	}
	return 0;
}
