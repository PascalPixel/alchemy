
// Kalay and Venus Lighthouse's aerie: actors that look about.

/// An actor script call, each frame: when the actor's countdown (work)
/// runs out, it turns by a random amount up to half a turn and waits a random 0
/// to 79 frames.
static s32 Scene_LookAboutRandomly(struct Actor *actor) {
  s32 cnt = actor->userValue;
  s32 loaded = (s16)actor->userValue;
  s32 amount;

  if (loaded == 0) {
    actor->facing += Random_Below(32768);
    amount = Random_Below(80);
    actor->userValue = amount;
    if (amount == 0) {
      goto done;
    }
    cnt = amount;
  }
  actor->userValue = cnt - 1;
done:
  return 1;
}
