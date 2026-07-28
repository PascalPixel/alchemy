#include "script_interpreter.h"

s32 Func_0800d760(struct ScriptInterpreter *interpreter) {
    interpreter->cursor =
        Func_0800d6d8(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}
