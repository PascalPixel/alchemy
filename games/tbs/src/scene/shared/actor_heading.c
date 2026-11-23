/// @file
/// Kalay and Venus Lighthouse's aerie: people who look about.

/// An actor script call, each frame: when the actor's countdown (userValue)
/// runs out, it turns by a random amount up to half a turn and waits a random 0
/// to 79 frames.
static s32 Scene_LookAboutRandomly(struct Actor *actor) {
  u32 nextTimer;

  if ((s16)actor->userValue == 0) {
    actor->facing += Random_Below(32768);
    nextTimer = Random_Below(80);
    actor->userValue = nextTimer;
  }
  if ((s16)actor->userValue != 0) {
    actor->userValue = (s16)actor->userValue - 1;
  }
  return 1;
}
