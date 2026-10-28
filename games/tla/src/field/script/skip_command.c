#include "script.h"
#include "types.h"

void Actor_Release();

s32 Actor_ReleaseAndClear(void *object) {
  Actor_Release(object);
  return 0;
}

s32 Script_Nop(struct ScriptInterpreter *interpreter) {
  interpreter->cursor += 2;
  return 1;
}
