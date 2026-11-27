/* Drops the leader through a cracked tile: it gives way and the leader spins */
/* down into it while the screen fades. */
void scCrackedTileFall(CELL_TARGET *table, CELL_TARGET_ENTRY *entry) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	s32 x = (FIELD_CELL(leader->x.fixed)) | (s16)(table->x & 64);
	s32 z = (FIELD_CELL(leader->z.fixed)) | (s16)(table->z & 64);

	fdLeaderCenterInCell();
	fdEventPlaySnd(SE_SPLASH);
	fdMapGridCopyCells(table->x, table->z + 2, 1, 1, x, z);
	syFieldEventWait(12);
	leader->facing = FACING_SOUTH;
	leader->motionFlg = 0;
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	syIoWrite16(WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
					WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
					WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3,
				&REG_WINOUT);
	REG_DISPCNT |= DISPCNT_OBJWIN_ON;
	table->shown = 1;
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
						ANIM_LEADER_ARMS_RAISED);
	syFieldEventWait(16);
	for(x = 0; x <= 39; x++) {
		if(x == 5)
			fdEventPlaySnd(SE_DUST_PUFF);
		leader->scaleX += -(FIXED_ONE / 64);
		leader->scaleY += -(FIXED_ONE * 3 / 256);
		leader->y.fixed += -(toFIXED(3) / 5);
		syWait(1);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind = 0;
	table->shown = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
}
