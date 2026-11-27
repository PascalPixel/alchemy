/* fdActorCenterOnColumn, the same in both games. The Broken Seal's
   field/script_commands_actor_commands.c and The Lost Age's
   field/field_mode_actor_commands.c include it where the function stands. */

/* The script command "center actor and walk": walks the actor sideways to the
   middle of its 16-pixel column, sets its sprite priority to priority
   (fdActorSetSprPriority), then sends it zOfs pixels along z. */
void fdActorCenterOnColumn(Uint32 actorId, Sint32 pri, Sint32 zOfs)
{
	ACTOR *actor;
	Sint16 pixelX;
	Sint32 columnX;
	Sint16 ofsInColumn;

	actor = fdActorLookup(actorId);
	if(actor != NULL) {
		/* The column's left edge, from a division by 16 that rounds toward zero
		   (left of x = 0 it finds the edge to the right); its middle is 8
		   pixels in. */
		pixelX = actor->x.part[0].integer;
		columnX = pixelX / 16 * 16;
		ofsInColumn = pixelX - columnX;
		actor->paused = 0;
		fdActorStartIdleStub(actor);
		fdActorSelectAnimationStub(actor, ANIM_WALK);
		fdActorSetMoveTargetStub(actor,
			actor->x.fixed + toFIXED(8 - ofsInColumn), actor->y.fixed,
			actor->z.fixed);
		fdActorWaitForMoveStub(actor);
		fdActorSetSprPriority(actorId, pri);
		fdActorSetMoveTargetStub(actor, actor->x.fixed, actor->y.fixed,
			actor->z.fixed + toFIXED(zOfs));
	}
}
