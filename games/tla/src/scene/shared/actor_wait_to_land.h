/// Lets an actor fall until it lands, at most a second, then stops it.
void Actor_WaitToLand(s32 id) {
  s32 n = 60;
  struct Actor *actor = Actor_Get(id);

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (n != 0) {
    System_Wait(1);
    if (actor->velocityY == 0)
      break;
    n--;
  }
  System_Wait(10);
  actor->motionFlags = 0;
}
