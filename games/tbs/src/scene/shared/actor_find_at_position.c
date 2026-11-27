// Colosseum Final 1, 2 and 3, and Venus Lighthouse: who stands at a place.

// Returns the placed actor (8 to 65) whose metatile, and 16-pixel step of height, is that of
// the 16.16 position, or 0. self is not read.
static void *scActorFindAtPos(const FIELD_POS *pos, void *self)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR      **slots = state->actors;
	Uint32       i;

	for (i = ACTOR_FIRST_PLACED; i < sizeof(state->actors) / sizeof(state->actors[0]); i++)
	{
		ACTOR *actor = slots[i];

		// UB: fdActorClearAll leaves unused slots null.
#if FIX_NULL_DEREFERENCE
		if (actor == NULL)
			continue;
#endif
		if (FIELD_METATILE(pos->x) == FIELD_METATILE(actor->x.fixed)
		    && FIELD_METATILE(pos->y) == FIELD_METATILE(actor->y.fixed)
		    && FIELD_METATILE(pos->z) == FIELD_METATILE(actor->z.fixed))
		{
			return actor;
		}
	}
	return 0;
}
