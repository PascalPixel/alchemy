#include "script_interpreter.h"

s32 Func_0800d8f4(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
