#include "types.h"
#include "object_event.h"

// Goma Cave Entrance and Goma Cave, and the Lamakan Desert.

/// Waits until the object has come down to height (16.16), a second at most.
static void ObjectEvent_WaitBelowHeight(struct ObjectEvent *objectEvent,
                                        s32 height) {
  s32 frames = 60;

  while (frames != 0) {
    Engine_TaskWait(1);
    frames--;
    if (objectEvent->y.fixed <= height)
      break;
  }
}
