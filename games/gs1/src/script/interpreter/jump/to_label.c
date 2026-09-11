#include "scene.h"
#include "abi/script/interpreter/jump/to_label.h"
#include "script_interpreter.h"

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor =
        Script_Run(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}
