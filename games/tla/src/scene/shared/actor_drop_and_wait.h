/// Lets an actor fall to the ground, with the landing sound.
void Actor_DropAndWait(struct Actor *object) {
  u8 *flags = &object->motionFlags;

  *flags = 3;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(SE_THUMP);
  wait(10);
  *flags = 0;
}
