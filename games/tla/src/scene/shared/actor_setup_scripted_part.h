/* Stops an actor and starts its script, its sprite drawn at `priority`. */
void fdActorSetupScriptedPart(ACTOR *actor, s32 priority) {
	actor->motionFlg = 0;
	actor->collisionFlg = 0;

	((SPR_ATTR2_HIGH *)actor->spr)->pri = priority;
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, ScriptedPart_Script);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
