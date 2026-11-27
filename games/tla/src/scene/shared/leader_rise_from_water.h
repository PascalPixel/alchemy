/* Unless the party returns to where it was, the leader appears in the */
/* waterfall 16 pixels north of the entrance, tinted blue, as the screen opens,
 */
/* then comes south out of it in a trail of mist and takes his own colours and
 */
/* shadow back on the way. */
void fdLeaderRiseFromWater(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdCameraMoveToFX(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		leader->motionFlg = 0;
		fdActorSetPosFX(MainRoutineStateWork.leaderActorId,
						INTtoFIXED(leader->x.part[0].integer),
						INTtoFIXED(leader->z.part[0].integer) - toFIXED(16));
		scFieldActorSetSprCol(fdActorGet(MainRoutineStateWork.leaderActorId),
							  SPRITE_COLOR_BLUE_TINT);
		fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
		fdEventStartScrTransition();
		fdEventPlaySnd(SE_SPARKLING_PASSAGE);
		leader->update = (OBJECT_UPDATE)fdActorEmitMist;
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.4);
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 8);
		scFieldActorSetSprCol(fdActorGet(MainRoutineStateWork.leaderActorId),
							  SPRITE_COLOR_NORMAL);
		fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId),
						   SPRITE_FLAG_SHADOW);
		((FIELD_SPR *)leader->spr)->pri = 1;
		fdActorWalkByAndWait(MainRoutineStateWork.leaderActorId, 0, 10);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->update = NULL;
		fdEventNoOp();
		fdEventWaitTransitionFrames();
		fdEventEnd();
	}
}
