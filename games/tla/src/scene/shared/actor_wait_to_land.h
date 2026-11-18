/// Lets an actor fall, waits up to sixty frames for it to land and ten more,
/// then holds it still.
void Actor_WaitToLand(s32 id) {
  s32 remainingFrames = 60;
  struct Actor *actor = Actor_Get(id);

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (remainingFrames != 0) {
    System_Wait(1);
    if (actor->velocityY == 0)
      break;
    remainingFrames--;
  }
  System_Wait(10);
  actor->motionFlags = 0;
}
