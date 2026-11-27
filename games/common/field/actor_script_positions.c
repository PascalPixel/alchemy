/* fdActorScriptApplyRelativePos to fdActorScriptApplyLocalOffsetPos, the
   same in both games. Both games' field/field_script_commands.c include it
   where the functions stand. */

/* SC_MOVE_BY(x, y, z): sends the actor towards its position plus (x, y, z). */
Sint32 fdActorScriptApplyRelativePos(ACTOR *actor)
{

	Uint8  *commandBytes = (Uint8 *)(actor->script + actor->scriptCursor);
	Sint32 *arguments = (Sint32 *)(commandBytes + 4);
	Sint32	deltaX = *arguments++;
	Sint32	deltaY = *arguments++;
	Sint32	deltaZ = *arguments;

	fdActorSetMoveTarget(actor, actor->x.fixed + deltaX,
		actor->y.fixed + deltaY, actor->z.fixed + deltaZ);
	actor->scriptCursor += 4;
	return 1;
}

/* SC_FACE_WATCHED: turns the actor to face its watched actor, on the ground
   plane (x and z). */
Sint32 fdActorScriptStoreAngToWatchedActor(ACTOR *actor)
{

	ACTOR *tgt;

	tgt = (ACTOR *)actor->watchedActor;
	actor->facing = syAtan(
		(tgt->z.fixed - actor->z.fixed), (tgt->x.fixed - actor->x.fixed));
	actor->scriptCursor++;
	return 1;
}

/* SC_MOVE_TO_WATCHED: sends the actor towards its watched actor. */
Sint32 fdActorScriptApplyWatchedActorPos(ACTOR *actor)
{

	ACTOR *tgt;

	tgt = (ACTOR *)actor->watchedActor;
	fdActorSetMoveTarget(actor, tgt->x.fixed, tgt->y.fixed, tgt->z.fixed);
	actor->scriptCursor++;
	return 1;
}

/* SC_MOVE_LOCAL(a, b): sends the actor towards its position plus an offset that
   is never filled in; the operands are skipped unread. */
Sint32 fdActorScriptApplyLocalOffsetPos(ACTOR *actor)
{

	Sint32 localOfs[3];

	/* BUG: offset is never set, so the target is the position plus whatever the
	   stack held. */
#if FIX_UNINITIALIZED_READ
	localOfs[0] = localOfs[1] = localOfs[2] = 0;
#endif
	fdActorSetMoveTarget(actor, actor->x.fixed + localOfs[0],
		actor->y.fixed + localOfs[1], actor->z.fixed + localOfs[2]);
	actor->scriptCursor += 3;
	return 1;
}
