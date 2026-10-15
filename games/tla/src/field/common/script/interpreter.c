#include "script.h"

s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter)
{
    s16           initial = interpreter->cursor;
    ScriptCommand callback = (ScriptCommand)interpreter->script[initial + 1];

    if (callback(interpreter) != 0)
        return 0;
    if (interpreter->cursor == initial)
        interpreter->cursor += 2;
    return 1;
}

s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 key)
{
    u32 *entries;
    s32  index;
    s16 *field = &interpreter->lookupResult;
    s32  zero = 0;

    *field = zero;
    if (key == 0)
    {
        return 0;
    }

    key &= 0xBFFFFFFF;
    entries = (u32 *)interpreter->script;
    index = 0;
    do
    {
        if (*entries++ == key)
        {
            return index + 1;
        }
        index++;
    } while (index <= 0x3FF);
    return 0;
}

s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
    const s32 *arguments;
    s32        repeatLimit;
    s32        jumpKey;
    u8        *repeatCount;
    s32        nextCount;

    arguments = &interpreter->script[interpreter->cursor + 1];
    repeatLimit = *arguments++;
    jumpKey = *arguments;
    if (repeatLimit == 0xFFFF)
    {
        interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
    }
    else
    {
        repeatCount = &interpreter->repeatCount;
        nextCount = *repeatCount + 1;
        *repeatCount = nextCount;
        if ((s32)(u8)nextCount < (s32)(s16)repeatLimit)
        {
            interpreter->cursor = Script_FindLabel(interpreter, jumpKey);
        }
        else
        {
            *repeatCount = 0;
            interpreter->cursor = interpreter->cursor + 3;
        }
    }
    return 1;
}
