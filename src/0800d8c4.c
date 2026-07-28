#include "script_interpreter.h"

void Func_0800c300(struct ScriptInterpreter *, s32);

s32 Func_0800d8c4(struct ScriptInterpreter *interpreter)
{
    Func_0800c300(interpreter,
        interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
