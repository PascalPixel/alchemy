// Felix walks into a door: it opens, he walks through it, lined up with it, and the screen closes
// as he leaves by exit. Eastern Alhafra's ship cabin and Lemuria's palace doors open so.
void scLeaderOpenDoorAndEnter(const MAP_GRID_METATILE_ANIMATION *door, Sint32 exit, Sint32 unused)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySound(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(door->steps, door->x, door->y);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlags = ACTOR_FALLS;
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -8);
	syFieldScriptWait(10);
	syFieldScriptSetExitRequest(exit);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
}
