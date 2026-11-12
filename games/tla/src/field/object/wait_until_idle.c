#include "types.h"
#include "system.h"
#include "actor.h"

/// Waits until the actor stops rising or falling, a second at most.
void OverlayObject_WaitUntilIdle(struct Actor *object) {
  s32 count;
  s32 busy;

  count = 60;
  while (count != 0) {
    wait(1);
    busy = object->velocityY;
    count--;
    if (busy == 0) {
      break;
    }
  }
}
