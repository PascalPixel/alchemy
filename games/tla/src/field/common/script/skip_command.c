#include "script.h"
#include "types.h"

void ObjectDispatch_Release();

s32 ObjectDispatch_ReleaseAndClear(void *object) {
  ObjectDispatch_Release(object);
  return 0;
}

s32 Script_SkipCommand(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}
