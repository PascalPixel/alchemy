/* fdActorClearDestinationAndStand and fdActorFaceDirectionById, the same in
   both games. Both games' field/script_commands_actor_commands.c include it
   where the functions stand. */

/* Clears an actor's destination, starts its idle script and stands it still. */
void fdActorClearDestinationAndStand(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}

/* The script command "actor face direction": the actor turns toward angle
   (TRIG_TURN a full turn; fdActorStepAng), then the script waits the given
   number of ticks. */
void fdActorFaceDirectionById(Sint32 actorId, Sint32 ang, Sint32 ticks)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->tmr = ang;
		fdActorStartScriptStub(actor, (const Sint32 *)ActorStepAngleScript);
		fdScriptWait(ticks);
	}
}
