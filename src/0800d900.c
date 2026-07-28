#include "script_interpreter.h"

s32 Func_080f9010(s32);

s32 Func_0800d900(struct ScriptInterpreter *interpreter)
{
    Func_080f9010(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
