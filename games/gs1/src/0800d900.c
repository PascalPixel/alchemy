#include "script_interpreter.h"

s32 Audio_PlayCue(s32);

s32 Func_0800d900(struct ScriptInterpreter *interpreter)
{
    Audio_PlayCue(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
