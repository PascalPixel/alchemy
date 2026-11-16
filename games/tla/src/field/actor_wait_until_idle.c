/// @file
/// Waiting for an actor to finish its script.

#include "types.h"
#include "system/system.h"
#include "field/actor.h"

/// Waits until the actor stops rising or falling, a second at most.
void OverlayObject_WaitUntilIdle(struct Actor *actor) {
  s32 count;
  s32 busy;

  count = 60;
  while (count != 0) {
    wait(1);
    busy = actor->velocityY;
    count--;
    if (busy == 0) {
      break;
    }
  }
}
