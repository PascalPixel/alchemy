/// Lets an actor fall until it lands, at most a second, then stops it.
void Actor_WaitToLand(s32 id) {
  s32 n = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (n != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    n--;
  }
  System_Wait(10);
  object->motionFlags = 0;
}
