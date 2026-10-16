#include "script.h"
#include "types.h"
#include "field_obj.h"

static s32
SceneEffect_AdvanceAngleUntilIdle(struct ScriptInterpreter *interpreter) {
  struct FieldActor *p = (struct FieldActor *)interpreter;
  p->scaleX += 0x1eb8;
  if (p->targetX == 0x80000000 && p->targetY == 0x80000000 &&
      p->targetZ == 0x80000000) {
    Engine_ObjectDispatchRelease(p);
  }
  return 1;
}
