/// Lets an actor fall to the ground, with the landing sound.
void Actor_DropAndWait(struct Actor *actor) {
  u8 *flags = &actor->motionFlags;

  *flags = 3;
  do {
    wait(1);
  } while (actor->velocityY != 0);
  Sound_Play(SE_THUMP);
  wait(10);
  *flags = 0;
}
