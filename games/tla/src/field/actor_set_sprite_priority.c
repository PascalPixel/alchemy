/* @file */
/* Setting an actor's sprite priority by hand. */

#include "field/field.h"

/* Fixes the priority of the actor's sprite and its shadow, which then no */
/* longer follows the map cell under it (clears ACTOR_PRIORITY_AUTOMATIC). */
void scSetActorSprPriority(ACTOR *actor, s32 priority) {
	SPR *sprite;

	if(actor == NULL ||
	   (actor->sprStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_NONE)
		return;
	sprite = actor->spr;
	sprite->image.pri = priority;
	sprite->shadow.pri = priority;
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
}
