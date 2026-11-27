/* Walks the leader to the middle of the cell he stands on, as he does before */
/* dropping through a cracked tile, sets him there and lowers the camera's */
/* point 8 pixels. */
void fdLeaderCenterInCell(void) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = actor->x.fixed;
	s32 z = actor->z.fixed;

	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 4);

	x = FIELD_CELL(x);
	z = FIELD_CELL(z);
	fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId,
							 x * MAPGRID_CELL_PIXELS + MAPGRID_CELL_PIXELS / 2,
							 z * MAPGRID_CELL_PIXELS + MAPGRID_CELL_PIXELS / 2);
	syWait(1);
	scFieldActorSetPosAndResetMotion(actor, FIELD_CELL_CENTER(x),
									 actor->y.fixed, FIELD_CELL_CENTER(z));
	syFieldEventWait(4);
	syEventCameraGetActor()->y.fixed -= toFIXED(8);
}
