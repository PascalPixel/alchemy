#include "types.h"
#include "object_event.h"
#include "script_layer.h"

// Babi Lighthouse and Venus Lighthouse.

/// Waits until the object stops rising or falling, a second at most.
static void ObjectEvent_WaitUntilIdle(struct ObjectEvent *objectEvent) {
  s32 cnt;
  s32 busy;

  cnt = 60;
  while (cnt != 0) {
    Engine_TaskWait(1);
    busy = objectEvent->velocityY;
    cnt--;
    if (busy == 0)
      break;
  }
}
