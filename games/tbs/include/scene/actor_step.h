#ifndef ALCHEMY_ACTOR_STEP_H
#define ALCHEMY_ACTOR_STEP_H

/* @file */
/* One step from an actor, for the scenes whose statues and pillars are pushed
 */
/* (Bilibin, its cave, Kalay, Sol Sanctum's entrance, the Vault's cave, Venus */
/* Lighthouse) and those of pushed_actor.c. */

#include "macros.h"
#include "field/pushed_actor.h"

/* Sets `position` to one step from `actor`: `step` is a DIRECTION_STEP, as */
/* the push tables hold them. */
static inline void fdActorStepFrom(FIELD_POS *position, ACTOR *actor,
								   s32 step) {
	SET_POSITION(position, actor->x.fixed + DIRECTION_STEP_X_FIXED(step),
				 actor->y.fixed, actor->z.fixed + DIRECTION_STEP_Z_FIXED(step));
}

#endif
