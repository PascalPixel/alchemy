/* Readies a flame thrower's spark (FLAME_SPARK): it stops, shadowless and */
/* tinted SPRITE_COLOR_RED_TINT, and breaks up (the dust cloud's animation 2) */
/* without spinning or growing until ScriptedPart_Script releases it, half a */
/* second on. */
void fdActorSetupScripted(ACTOR *actor) {
	actor->motionFlg = 0;
	actor->collisionFlg = 0;
	fdActorSetSprFlags(actor, 0);
	fdActorSelectAnimation(actor, 2);
	fdActorStartScript(actor, ScriptedPart_Script);
	scFieldActorSetSprCol(actor, SPRITE_COLOR_RED_TINT);
	actor->tmr = 0;
	actor->speedLimit = 0;
	actor->acceleration = 0;
}
