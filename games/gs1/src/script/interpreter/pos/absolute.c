#include "scene.h"
#include "abi/script/interpreter/pos/absolute.h"
#include "object_commands.h"
#include "script_interpreter.h"

s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter)
{
    s32 first;
    s32 second;
    s32 third;
    const s32 *argument;

    argument = interpreter->script + interpreter->cursor;
    argument++;
    first = *argument;
    argument++;
    second = *argument;
    argument++;
    third = *argument;
    Script_Run(interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}
