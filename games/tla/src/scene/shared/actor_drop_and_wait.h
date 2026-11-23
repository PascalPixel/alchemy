/// Lets an actor fall to the ground and waits there: a thump as it lands, ten
/// frames, and it stands still again.
void Actor_DropAndWait(struct Actor *actor) {
  u8 *flags = &actor->motionFlags;

  *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  do {
    wait(1);
  } while (actor->velocityY != 0);
  Sound_Play(SE_THUMP);
  wait(10);
  *flags = 0;
}
