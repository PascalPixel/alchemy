/* fdActorWaitForScriptEndById, the same in both games. The Broken Seal's
   field/script_commands_actor_commands.c and The Lost Age's
   field/field_mode_actor_commands.c include it where the function stands. */

/* Waits, at most 300 ticks, for an actor's script to reach its end
   (SC_END). */
void fdActorWaitForScriptEndById(Uint32 actorId)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL)
		fdActorWaitForScriptEndStub(actor);
}
