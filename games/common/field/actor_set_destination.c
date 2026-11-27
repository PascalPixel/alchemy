/* fdActorSetDestinationById and fdActorMoveToAndWaitById, the same in both
   games. The Broken Seal's field/script_commands_actor_commands.c and The
   Lost Age's field/field_mode_actor_commands.c include it where the
   functions stand. */

/* The script command "set actor destination": the actor heads for (x, z), in
   pixels, keeping its current animation. */
void fdActorSetDestinationById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
	}
}

/* The script command "move actor and wait": as fdActorSetDestinationById, then
   waits (at most 600 ticks) for the actor to arrive. */
void fdActorMoveToAndWaitById(Uint32 actorId, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL) {
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSetMoveTargetStub(actor, toFIXED(x), actor->y.fixed, toFIXED(z));
		fdActorWaitForMoveStub(actor);
	}
}
