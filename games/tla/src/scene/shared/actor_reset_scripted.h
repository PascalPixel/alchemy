/* Stops an actor and starts its script. */
void fdActorResetScripted(struct ACTOR *actor) {
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, ScriptedPart_Script);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
