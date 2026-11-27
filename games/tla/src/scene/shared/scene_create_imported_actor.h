/* scCreateActor through the overlay's import of the actor creator. */
ACTOR *scCreateImportedActor(s32 x, s32 y, s32 z, s32 type) {
	ACTOR *actor = syImportActorCreate(type, x, y, z);

	if(actor != NULL) {
		scSetActorSprPriority(actor, 1);
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
