/* Lifts the leader by `height` and has the camera hold still, unless the party
 */
/* keeps its place. */
void fdLeaderLift(FIXED height) {
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	if(!fdEventGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		actor->motionFlg = 0;
		fdActorSetSprFlags(actor, 0);
		actor->y.fixed += height;
		actor->speedLimit = toFIXED(1);
		actor->velY = 0;
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
		actor->steeringFlg = ACTOR_FACES_MOTION;
		syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
		fdMapGridCenterOnFollowedPoint();
		syWait(1);
	}
}
