#include "object_commands.h"
#include "script_interpreter.h"

s32 Func_0800d8c4(struct ScriptInterpreter *interpreter)
{
    Func_0800c300(interpreter,
        interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
