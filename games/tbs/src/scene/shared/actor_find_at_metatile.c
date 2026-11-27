#include "scene/actor_step.h"

/* Bilibin, its cave, Kalay and Sol Sanctum's entrance: who stands on a
   metatile. */

/* Returns the placed actor (8 to 65) standing on metatile (x, z), or 0. */
static ACTOR *scActorFindAtMetatile(Sint32 x, Sint32 z)
{
	ACTOR **actors = FieldStateWork->actors;
	Uint32	i;

	for(i = ACTOR_FIRST_PLACED; i <= ACTOR_LAST_ID; i++) {
		ACTOR *actor = actors[i];

		if(x == FIELD_METATILE(actor->x.fixed) &&
			z == FIELD_METATILE(actor->z.fixed)) {
			return actor;
		}
	}
	return 0;
}
