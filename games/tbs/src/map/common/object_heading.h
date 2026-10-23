#include "action_script.h"
#include "types.h"
#include "object_event.h"

// Kalay and Venus Lighthouse's aerie: actors that look about.

/// An object script call, each frame: when the actor's countdown (work) runs
/// out, it turns by a random amount up to half a turn and waits a random 0 to
/// 79 frames.
static s32 Scene_LookAboutRandomly(struct ScriptInterpreter *interpreter) {
  struct FieldActor *obj = (struct FieldActor *)interpreter;
  s32 cnt = obj->work;
  s32 loaded = (s16)obj->work;
  s32 amount;

  if (loaded == 0) {
    obj->facing += (u32)(Engine_RandomNext() << 15) >> 16;
    amount = (u32)(Engine_RandomNext() * 80) >> 16;
    obj->work = amount;
    if (amount == 0) {
      goto done;
    }
    cnt = amount;
  }
  obj->work = cnt - 1;
done:
  return 1;
}
