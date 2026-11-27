/* Cracks the tile under the leader, swapping in its cracked picture. */
void scCrackedTileCrack(CELL_TARGET *table, CELL_TARGET_ENTRY *entry) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x;
	s32 z;

	x = (FIELD_CELL(leader->x.fixed)) | (s16)(table->x & 64);
	z = (FIELD_CELL(leader->z.fixed)) | (s16)(table->z & 64);
	fdLeaderCenterInCell();
	fdEventPlaySnd(SE_RATTLE);
	fdMapGridCopyCells(table->x, table->z + 1, 1, 1, x, z);
	syFieldEventWait(12);
}
