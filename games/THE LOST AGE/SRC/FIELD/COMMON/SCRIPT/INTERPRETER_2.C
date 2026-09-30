#include "SCRIPT_INTERPRETER.H"

s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor =
        Script_FindLabel(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}

u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result != 0) {
        interpreter->cursor = Script_FindLabel(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result == 0) {
        interpreter->cursor = Script_FindLabel(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}
