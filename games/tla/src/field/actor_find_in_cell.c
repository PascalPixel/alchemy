/* @file */
/* Finding the actor that stands in a cell. */

#include "field/actor.h"
#include "field/field_state.h"

/* Returns the first placed actor (ACTOR_FIRST_PLACED up to the treasure spots,
 */
/* FIELD_TREASURE_ACTOR) standing in the same 16-pixel cell as position (x, y */
/* and z), or NULL. Callers pass the actor asking; the search does not use it.
 */
ACTOR *fdActorFindInCell(s32 *position, ACTOR *self) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	u32 i;
	ACTOR **objects;
	ACTOR *actor;

	i = ACTOR_FIRST_PLACED;
	objects = &state->actors[ACTOR_FIRST_PLACED];
	for(; i < FIELD_TREASURE_ACTOR; i++) {
		actor = *objects++;
		/* UB: fdActorTableClear leaves unused slots null. */
#ifdef UBFIX
		if(actor == NULL)
			continue;
#endif
		if(FIELD_CELL(position[0]) == FIELD_CELL(actor->x.fixed) &&
		   FIELD_CELL(position[1]) == FIELD_CELL(actor->y.fixed) &&
		   FIELD_CELL(position[2]) == FIELD_CELL(actor->z.fixed))
			return actor;
	}
	return NULL;
}
