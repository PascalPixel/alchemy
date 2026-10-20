/// @file
/// Kolima (scene 36) and Kolima Forest (scenes 40 and 44): an effect (object
/// 26) that stretches as it travels.
#include "script.h"
#include "types.h"
#include "field_obj.h"

/// An object script call: widen the object by 0x1eb8 (about 0.12) a frame, and
/// release it once it has reached its destination.
static s32
SceneEffect_AdvanceAngleUntilIdle(struct ScriptInterpreter *interpreter) {
  struct FieldActor *streak = (struct FieldActor *)interpreter;
  streak->scaleX += 0x1eb8;
  if (streak->targetX == ACTOR_NO_TARGET &&
      streak->targetY == ACTOR_NO_TARGET &&
      streak->targetZ == ACTOR_NO_TARGET) {
    Engine_ObjectDispatchRelease(streak);
  }
  return 1;
}
