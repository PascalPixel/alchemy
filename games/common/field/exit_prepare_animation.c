/* fdExitPrepareAnimation, the same in both games. Both games'
   field/script_commands_exit_animations.c include it where the function
   stands. */

/* Starts the leader's animation for leaving through an exit: centres it on its
   metatile, at the metatile's south edge, stops it and plays animation over 18
   ticks, its sprite gaining the doorway (SPRITE_DOORWAY) as a second part. */
void fdExitPrepareAnimation(Sint32 anm)
{
	ACTOR		  *actor;
	SPR			  *spr;
	SPR_ANIMATION *doorway;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	spr = actor->spr;
	doorway = (SPR_ANIMATION *)fdSprAddAnimationStub(spr, SPRITE_DOORWAY);
	{
		Uint32 cleared = 0;
		Uint8  hidden = SPRITE_COLOR_HIDDEN;

		/* No shadow as the leader leaves. */
		spr->flg = cleared;
		doorway->colMode = hidden;
		/* Keep the metatile and put the leader at its middle in x and its south
		   edge in z. */
		actor->x.fixed = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		actor->z.fixed = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(16);
		actor->velX = cleared;
		actor->velZ = cleared;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		fdActorSelectAnimationStub(actor, anm);
		sySynch(18);
	}
}
