/// Lifts the leader by `height` and has the camera hold still, unless the party
/// keeps its place.
void Leader_Lift(s32 height) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    object->motionFlags = 0;
    Actor_SetSpriteFlags(object, 0);
    object->y += height;
    object->speedLimit = px(1);
    object->velocityY = 0;
    gMainRoutineWork.movementMode = 2;
    object->steeringFlags = 1;
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    Map_CenterOnFollowedPoint();
    System_Wait(1);
  }
}
