/* Drops the leader through a cracked tile: it gives way and the leader spins
   down into it while the screen fades. */
void scCrackedTileFall(METATILE_TARGET *tbl, METATILE_TARGET_ENTRY *entry)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 x = (FIELD_METATILE(leader->x.fixed)) | (Sint16)(tbl->x & 64);
	Sint32 z = (FIELD_METATILE(leader->z.fixed)) | (Sint16)(tbl->z & 64);

	scLeaderCenterInMetatile();
	fdScriptPlaySnd(SE_SPLASH);
	fdMapGridCopyMetatiles(tbl->x, tbl->z + 2, 1, 1, x, z);
	syFieldScriptWait(12);
	leader->facing = FACING_SOUTH;
	leader->motionFlg = 0;
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	syIOWrite16(WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
			WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
			WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3,
		&REG_WINOUT);
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	tbl->shown = 1;
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	syFieldScriptWait(16);
	for(x = 0; x <= 39; x++) {
		if(x == 5)
			fdScriptPlaySnd(SE_DUST_PUFF);
		leader->scaleX += -(toFIXED(1) / 64);
		leader->scaleY += -(toFIXED(3) / 256);
		leader->y.fixed += -(toFIXED(3) / 5);
		syWait(1);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	tbl->shown = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
}
