#include "action_script.h"
#include "types.h"
#include "object_event.h"

// Kalay and Venus Lighthouse's aerie: object events that look about.

/// An object script call, each frame: when the object event's countdown (work)
/// runs out, it turns by a random amount up to half a turn and waits a random 0
/// to 79 frames.
static s32 Map_LookAboutRandomly(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s32 cnt = objectEvent->work;
  s32 loaded = (s16)objectEvent->work;
  s32 amount;

  if (loaded == 0) {
    objectEvent->facing += (u32)(Engine_RandomNext() << 15) >> 16;
    amount = (u32)(Engine_RandomNext() * 80) >> 16;
    objectEvent->work = amount;
    if (amount == 0) {
      goto done;
    }
    cnt = amount;
  }
  objectEvent->work = cnt - 1;
done:
  return 1;
}
