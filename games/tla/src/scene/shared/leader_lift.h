/// Lifts the leader by `height` and has the camera hold still, unless the party
/// keeps its place.
void Leader_Lift(s32 height) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    object->motionFlags = 0;
    Actor_SetSpriteFlags(object, 0);
    object->y += height;
    object->speedLimit = px(1);
    object->velocityY = 0;
    gMainRoutineWork.renderMode = 2;
    object->actionFlags = 1;
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Map_CenterOnFollowedPoint();
    System_Wait(1);
  }
}
