/* fdActorRunRepeatedMotion, the same in both games. The Broken Seal's
   field/script_commands_actor_commands.c and The Lost Age's
   field/field_mode_actor_commands.c include it where the function stands. */

/* The script command "run repeated motion": as fdActorStartRepeatedMotion, then
   waits for it to end. */
void fdActorRunRepeatedMotion(Uint32 actorId, Sint32 cnt)
{
	fdActorStartRepeatedMotion(actorId, cnt);
	fdActorWaitForScriptEndById(actorId);
}
