/* scCreateActor through the overlay's import of the actor creator. */
ACTOR *scCreateImportedActor(Sint32 x, Sint32 y, Sint32 z, Sint32 type)
{
	ACTOR *actor = syImportActorCreate(type, x, y, z);

	if(actor != NULL) {
		fdSetActorSprPriority(actor, 1);
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(actor, 0);
		fdActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
		actor->priFlg |=
			ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
		return actor;
	}
	return NULL;
}
