/* @file */
/* Creating an actor as a black silhouette. */

#include "field/field_sequence.h"
#include "field/field.h"
#include "field/field_event.h"

/* Creates an actor of sprite type at a 16.16 position to stand as a */
/* silhouette: at drawing priority 0, still, skipped by target searches, with */
/* no outline or shadow, all black and translucent. */
ACTOR *fdActorCreateSilhouette(FIXED x, FIXED y, FIXED z, s32 type) {
	ACTOR *actor = syImportActorCreate(type, x, y, z);

	if(actor != NULL) {
		scSetActorSprPriority(actor, 0);
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		fdActorSetSprCol(actor, SPRITE_COLOR_BLACK);
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
		return actor;
	}
	return NULL;
}
