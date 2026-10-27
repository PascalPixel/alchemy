#include "action_script.h"
#include "types.h"
#include "actor.h"

// Kalay and Venus Lighthouse's aerie: actors that look about.

/// An object script call, each frame: when the actor's countdown (work)
/// runs out, it turns by a random amount up to half a turn and waits a random 0
/// to 79 frames.
static s32 Scene_LookAboutRandomly(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s32 cnt = actor->userValue;
  s32 loaded = (s16)actor->userValue;
  s32 amount;

  if (loaded == 0) {
    actor->facing += (u32)(Import_RandomNext() << 15) >> 16;
    amount = (u32)(Import_RandomNext() * 80) >> 16;
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
