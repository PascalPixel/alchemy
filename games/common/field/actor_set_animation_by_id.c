/* fdActorSetAnimationById, the same in both games. The Broken Seal's
   field/script_commands_actor_commands.c and The Lost Age's
   field/field_mode_actor_commands.c include it where the function stands. */

/* The script command "set actor animation" (an ACTOR_ANIMATION: ANIM_STAND,
   ANIM_WALK, ANIM_NOD, ANIM_SHAKE_HEAD and so on). */
void fdActorSetAnimationById(Uint32 actorId, Sint32 anm)
{
	ACTOR *actor = fdActorLookup(actorId);

	if(actor != NULL)
		fdActorSelectAnimationStub(actor, anm);
}
