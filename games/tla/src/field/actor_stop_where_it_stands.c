/* @file */
/* Stopping an actor where it stands. */

#include "field/field_event.h"
#include "macros.h"

/* Stops an actor where it stands: no movement target, no speed, and its timer
 */
/* back to 0. */
void fdActorStopWhereItStands(ACTOR *actor) {
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;

	SET_VELOCITY(actor, 0, 0, 0);

	actor->tmr = 0;
}
