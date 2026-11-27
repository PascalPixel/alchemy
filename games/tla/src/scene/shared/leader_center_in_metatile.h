/* Walks the leader to the middle of the metatile he stands on, as he does
   before dropping through a cracked tile, sets him there and lowers the
   camera's point 8 pixels. */
void scLeaderCenterInMetatile(void)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x = actor->x.fixed;
	Sint32 z = actor->z.fixed;

	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 4);

	x = FIELD_METATILE(x);
	z = FIELD_METATILE(z);
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId,
		x * MAPGRID_METATILE_PIXELS + MAPGRID_METATILE_PIXELS / 2,
		z * MAPGRID_METATILE_PIXELS + MAPGRID_METATILE_PIXELS / 2);
	syWait(1);
	scFieldActorSetPosAndResetMotion(actor, FIELD_METATILE_CENTER(x),
		actor->y.fixed, FIELD_METATILE_CENTER(z));
	syFieldScriptWait(4);
	syScriptCameraGetActor()->y.fixed -= toFIXED(8);
}
