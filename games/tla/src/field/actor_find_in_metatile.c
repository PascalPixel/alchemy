/* Finding the actor that stands in a metatile. */

#include "field/actor.h"
#include "field/field_state.h"

/* Returns the first placed actor (ACTOR_FIRST_PLACED up to the treasure spots,
   FIELD_TREASURE_ACTOR) standing in the same 16-pixel metatile as position (x,
   y and z), or NULL. Callers pass the actor asking; the search does not use
   it. */
ACTOR *fdActorFindInMetatile(Sint32 *position, ACTOR *self)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Uint32		 i;
	ACTOR	   **objects;
	ACTOR		*actor;

	i = ACTOR_FIRST_PLACED;
	objects = &state->actors[ACTOR_FIRST_PLACED];
	for(; i < FIELD_TREASURE_ACTOR; i++) {
		actor = *objects++;
		/* UB: fdActorTableClear leaves unused slots null. */
#if FIX_NULL_DEREFERENCE
		if(actor == NULL)
			continue;
#endif
		if(FIELD_METATILE(position[0]) == FIELD_METATILE(actor->x.fixed) &&
			FIELD_METATILE(position[1]) == FIELD_METATILE(actor->y.fixed) &&
			FIELD_METATILE(position[2]) == FIELD_METATILE(actor->z.fixed))
			return actor;
	}
	return NULL;
}
