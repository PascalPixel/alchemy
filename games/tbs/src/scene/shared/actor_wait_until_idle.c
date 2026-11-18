/// @file
/// Babi Lighthouse and Venus Lighthouse.

/// Waits until the actor stops rising or falling, a second at most.
static void Actor_WaitUntilIdle(struct Actor *actor) {
  s32 frames;
  s32 busy;

  frames = FRAMES_PER_SECOND;
  while (frames != 0) {
    wait(1);
    busy = actor->velocityY;
    frames--;
    if (busy == 0)
      break;
  }
}
