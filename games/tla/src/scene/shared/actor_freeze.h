/* Stops an actor where it stands. */
void fdActorFreeze(s32 id) {
	ACTOR *actor = fdActorGet(id);

	if(actor != NULL) {
		fdActorSetSprFlags(actor, 0);
		actor->collisionFlg = 0;
		actor->motionFlg = 0;
	}
}
