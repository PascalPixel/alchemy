#include "types.h"
#include "actor.h"

// Goma Cave Entrance and Goma Cave, and the Lamakan Desert.

/// Waits until the object has come down to height (16.16), a second at most.
static void Actor_WaitBelowHeight(struct Actor *actor, s32 height) {
  s32 frames = 60;

  while (frames != 0) {
    WaitFrames(1);
    frames--;
    if (actor->y.fixed <= height)
      break;
  }
}
