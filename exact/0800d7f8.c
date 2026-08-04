#include "script_interpreter.h"

s8 Func_080770c0(s32);

s32 Func_0800d7f8(struct ScriptInterpreter *interpreter) {
    interpreter->condition_result =
        Func_080770c0(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
