/* scActorResetScripted as a two-argument callback. */
void scActorResetScriptedFor(ACTOR *actor, Sint32 unused)
{
	actor->motionFlags = 0;
	actor->collisionFlags = 0;
	fdActorSetSpriteFlags(actor, 0);
	fdActorStartScript(actor, ScriptedPartScript);
	actor->timer = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
