/* Lifts the leader by `height` and has the camera hold still, unless the party
   keeps its place. */
void scLeaderLift(FIXED height)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		actor->motionFlags = 0;
		fdActorSetSpriteFlags(actor, 0);
		actor->y.fixed += height;
		actor->speedLimit = toFIXED(1);
		actor->velocityY = 0;
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
		actor->steeringFlags = ACTOR_FACES_MOTION;
		syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 0);
		fdMapGridCenterOnFollowedPoint();
		syWait(1);
	}
}
