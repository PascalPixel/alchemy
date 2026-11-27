/* scCreateActor through the overlay's import of the actor creator. */
ACTOR *scCreateImportedActor(Sint32 x, Sint32 y, Sint32 z, Sint32 type)
{
	ACTOR *actor = syImportActorCreate(type, x, y, z);

	if(actor != NULL) {
		fdSetActorSpritePriority(actor, 1);
		actor->motionFlags = 0;
		actor->collisionFlags = ACTOR_NOT_TARGETED;
		fdActorSetSpriteFlags(actor, 0);
		fdActorSetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
		actor->priorityFlags |=
			ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
		return actor;
	}
	return NULL;
}
