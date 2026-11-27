// Stops an actor where it stands.
void scActorFreeze(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	if (actor != NULL)
	{
		fdActorSetSpriteFlags(actor, 0);
		actor->collisionFlags = 0;
		actor->motionFlags    = 0;
	}
}
