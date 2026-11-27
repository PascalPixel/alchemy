/* @file */
/* Colosseum Final 1, 2 and 3, and Venus Lighthouse: who stands at a place. */

/* Returns the placed actor (8 to 65) whose cell, and 16-pixel step of */
/* height, is that of the 16.16 position, or 0. self is not read. */
static void *scActorFindAtPos(const FIELD_POS *position, void *self) {
	FIELD_STATE *state = FieldStateWork;
	ACTOR **slots = state->actors;
	u32 i;

	for(i = ACTOR_FIRST_PLACED;
		i < sizeof(state->actors) / sizeof(state->actors[0]); i++) {
		ACTOR *actor = slots[i];

		/* UB: fdActorClearAll leaves unused slots null. */
#ifdef UBFIX
		if(actor == NULL)
			continue;
#endif
		if(FIELD_CELL(position->x) == FIELD_CELL(actor->x.fixed) &&
		   FIELD_CELL(position->y) == FIELD_CELL(actor->y.fixed) &&
		   FIELD_CELL(position->z) == FIELD_CELL(actor->z.fixed)) {
			return actor;
		}
	}
	return 0;
}
