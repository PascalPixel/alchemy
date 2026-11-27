/* Stops an actor and starts its script, its sprite drawn at `pri`. */
void scActorSetupScriptedPart(ACTOR *actor, Sint32 pri)
{
	actor->motionFlg = 0;
	actor->collisionFlg = 0;

	((SPR_ATTR2_HIGH *)actor->spr)->pri = pri;
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, ScriptedPartScript);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
