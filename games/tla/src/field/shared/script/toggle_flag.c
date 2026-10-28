#include "script.h"

s32 GameFlag_Test(s32);
void GameFlag_ClearBit(s32);
void GameFlag_SetBit(s32);

s32 Script_ToggleFlag(struct ScriptInterpreter *interpreter) {
  s32 value;

  value = interpreter->script[interpreter->cursor + 1];
  interpreter->conditionResult = GameFlag_Test(value);
  if (interpreter->conditionResult == 1)
    GameFlag_ClearBit(value);
  else
    GameFlag_SetBit(value);
  interpreter->cursor += 2;
  return 1;
}

struct DispatchObject;
void Actor_SelectAnimation(struct DispatchObject *object,
                                            s32 argument);

s32 Script_SetAnimation(struct ScriptInterpreter *interpreter) {
  Actor_SelectAnimation(
      (struct DispatchObject *)interpreter,
      interpreter->script[interpreter->cursor + 1]);
  interpreter->cursor += 2;
  return 1;
}
