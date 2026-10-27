#include "types.h"
#include "actor.h"
#include "event_layer.h"

// Babi Lighthouse and Venus Lighthouse.

/// Waits until the actor stops rising or falling, a second at most.
static void Actor_WaitUntilIdle(struct Actor *actor) {
  s32 cnt;
  s32 busy;

  cnt = 60;
  while (cnt != 0) {
    WaitTicks(1);
    busy = actor->velocityY;
    cnt--;
    if (busy == 0)
      break;
  }
}
