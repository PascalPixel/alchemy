/// Lifts the leader by `height` and has the camera hold still, unless the party
/// keeps its place.
void Leader_Lift(FIXED height) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    actor->motionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    actor->y.fixed += height;
    actor->speedLimit = toFIXED(1);
    actor->velocityY = 0;
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_KEYS;
    actor->steeringFlags = ACTOR_FACES_MOTION;
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    System_Wait(1);
  }
}
