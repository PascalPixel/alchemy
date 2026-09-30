#include "SCRIPT_INTERPRETER.H"

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter)
{
    s16 initial = interpreter->cursor;
    ScriptCommand callback =
        (ScriptCommand)interpreter->script[initial + 1];

    if (callback(interpreter)!= 0)
        return 0;
    if (interpreter->cursor == initial)
        interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key)
{
    u32 *entries;
    s32 index;
    s16 *field = &interpreter->lookup_result;
    s32 zero = 0;

    *field = zero;
    if (key == 0) {
        return 0;
    }

    key &= 0xBFFFFFFF;
    entries = (u32 *)interpreter->script;
    index = 0;
    do {
        if (*entries++ == key) {
            return index + 1;
        }
        index++;
    } while (index <= 0x3FF);
    return 0;
}
