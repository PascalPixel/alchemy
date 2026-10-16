#include "script.h"
#include "types.h"
#include "field_obj.h"
#include "field_event.h"

static s32
SceneEffect_SetModeByFrameBit1(struct ScriptInterpreter *interpreter) {
  struct FieldActor *arg0 = (struct FieldActor *)interpreter;
  if ((gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue(arg0, 10);
  } else {
    ObjectGroup_SetChildValue(arg0, 7);
  }
  return 0;
}
