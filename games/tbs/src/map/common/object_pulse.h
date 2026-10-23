#include "action_script.h"
#include "types.h"
#include "object_event.h"

// Tolbi's ship deck and Vale's storm.

/// An object script call, each frame: every 60 to 149 frames the actor squashes
/// and stretches back to its own shape over six frames.
static s32 Scene_SquashAndStretch(struct ScriptInterpreter *interpreter) {
  struct FieldActor *state = (struct FieldActor *)interpreter;
  switch (state->timer) {
  case 6:
    state->scaleX -= 0x4000;
    state->scaleY += 0x2000;
    break;
  case 4:
    state->scaleX += 0x2000;
    state->scaleY += -0x1000;
    break;
  case 2:
    state->scaleX += 0x1000;
    state->scaleY -= 0x800;
    break;
  case 0:
    state->scaleX = 0x10000;
    state->scaleY = 0x10000;
    state->timer = (Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60);
    break;
  }
  state->timer--;
  return 1;
}
