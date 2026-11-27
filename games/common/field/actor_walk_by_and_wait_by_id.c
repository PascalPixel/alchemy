/* fdActorWalkByAndWaitById, the same in both games. The Broken Seal's
   field/script_commands_actor_commands.c and The Lost Age's
   field/field_mode_actor_commands.c include it where the function stands. */

/* Walks the actor (xOfs, zOfs) pixels, waits for it to arrive and stands
   it still. */
void fdActorWalkByAndWaitById(Uint32 actorId, Sint32 xOfs, Sint32 zOfs)
{
	ACTOR *actor = fdActorLookup(actorId);

	fdActorWalkByById(actorId, xOfs, zOfs);
	if(actor != NULL) {
		fdActorWaitForMoveStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_STAND);
	}
}
