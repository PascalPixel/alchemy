/* Felix walks into a door: it opens, he walks through it, lined up with it, */
/* and the screen closes as he leaves by exit. Eastern Alhafra's ship cabin */
/* and Lemuria's palace doors open so. */
void fdLeaderOpenDoorAndEnter(const MAP_GRID_CELL_ANIMATION *door, s32 exit,
							  s32 unused) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdEventPlaySnd(SE_DOOR_OPEN);
	fdMapGridAnimateCells(door->steps, door->x, door->y);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -8);
	syFieldEventWait(10);
	syFieldEventSetExitRequest(exit);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}
