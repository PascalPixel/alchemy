/* fdActorResetScripted as a two-argument callback. */
void fdActorResetScriptedFor(ACTOR *actor, s32 unused) {
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	fdActorSetSprFlags(actor, 0);
	fdActorStartScript(actor, ScriptedPart_Script);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
