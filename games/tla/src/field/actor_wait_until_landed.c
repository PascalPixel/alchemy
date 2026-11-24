/// @file
/// Waiting for an actor to stop rising or falling.

#include "types.h"
#include "system/system.h"
#include "field/actor.h"

/// Waits until the actor stops rising or falling, sixty frames at most.
void Actor_WaitUntilLanded(struct Actor *actor) {
  s32 remainingFrames = 60;
  s32 busy;

  while (remainingFrames != 0) {
    wait(1);
    busy = actor->velocityY;
    remainingFrames--;
    if (busy == 0)
      break;
  }
}
