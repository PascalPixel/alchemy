/* Cracks the tile under the leader, swapping in its cracked picture. */
void scCrackedTileCrack(METATILE_TARGET *tbl, METATILE_TARGET_ENTRY *entry)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x;
	Sint32 z;

	x = (FIELD_METATILE(leader->x.fixed)) | (Sint16)(tbl->x & 64);
	z = (FIELD_METATILE(leader->z.fixed)) | (Sint16)(tbl->z & 64);
	scLeaderCenterInMetatile();
	fdScriptPlaySnd(SE_RATTLE);
	fdMapGridCopyMetatiles(tbl->x, tbl->z + 1, 1, 1, x, z);
	syFieldScriptWait(12);
}
