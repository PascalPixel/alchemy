/* scActorResetScripted as a two-argument callback. */
void scActorResetScriptedFor(ACTOR *actor, Sint32 unused)
{
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, ScriptedPartScript);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
