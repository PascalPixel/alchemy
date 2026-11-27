#include "scene/actor_step.h"

/* @file */
/* Bilibin, its cave, Kalay and Sol Sanctum's entrance: who stands on a cell. */

/* Returns the placed actor (8 to 65) standing on cell (x, z), or 0. */
static ACTOR *scActorFindAtCell(s32 x, s32 z) {
	ACTOR **actors = FieldStateWork->actors;
	u32 i;

	for(i = ACTOR_FIRST_PLACED; i <= ACTOR_LAST_ID; i++) {
		ACTOR *actor = actors[i];

		if(x == FIELD_CELL(actor->x.fixed) && z == FIELD_CELL(actor->z.fixed)) {
			return actor;
		}
	}
	return 0;
}
