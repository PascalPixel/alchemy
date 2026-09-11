#include "scene.h"
#include "script_interpreter.h"
#include "object_commands.h"

/* script/interpreter/cmd/lookup.c */
s32 Script_StoreLookupResult(struct ScriptInterpreter *interpreter)
{
    s16 cursor = interpreter->cursor;

    interpreter->lookup_result = interpreter->script[cursor + 1] - 1;
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 0;
}

/* script/interpreter/wait/event.c */
s32 Script_WaitForEvent(struct ScriptInterpreter *interpreter)
{
    if ((u32)interpreter->delay > 0x3B) {
        interpreter->delay = 0;
        goto block_3;
    }
    if (Script_Check(interpreter)!= 0) {
block_3:
        interpreter->cursor = (u16)interpreter->cursor + 1;
        return 1;
    }
    return 0;
}

/* script/interpreter/cmd/invoke.c */
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

/* script/interpreter/jump/find_label.c */
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

/* script/interpreter/jump/repeat.c */
s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter)
{
    const s32 *arguments;
    s32 repeat_limit;
    s32 jump_key;
    u8 *repeat_count;
    s32 next_count;

    arguments = &interpreter->script[interpreter->cursor + 1];
    repeat_limit = *arguments++;
    jump_key = *arguments;
    if (repeat_limit == 0xFFFF) {
        interpreter->cursor = Script_Run(interpreter, jump_key);
    } else {
        repeat_count = &interpreter->repeat_count;
        next_count = *repeat_count + 1;
        *repeat_count = next_count;
        if ((s32)(u8)next_count < (s32)(s16)repeat_limit) {
            interpreter->cursor = Script_Run(interpreter, jump_key);
        } else {
            *repeat_count = 0;
            interpreter->cursor = interpreter->cursor + 3;
        }
    }
    return 1;
}

/* script/interpreter/jump/to_label.c */
s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter)
{
    interpreter->cursor =
        Script_Run(interpreter, interpreter->script[interpreter->cursor + 1]);
    return 1;
}

/* script/interpreter/jump/if_true.c */
u32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result != 0) {
        interpreter->cursor = Script_Run(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

/* script/interpreter/jump/if_false.c */
u32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter)
{
    s32 index = interpreter->cursor;
    const s32 *table = interpreter->script;
    u32 value = table[index + 1];

    if (interpreter->condition_result == 0) {
        interpreter->cursor = Script_Run(interpreter, value);
    } else {
        interpreter->cursor = (u16)interpreter->cursor + 2;
    }
    return 1;
}

/* script/interpreter/cmd/load.c */
extern const s32 gRom[];

s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = gRom;
    interpreter->cursor = (result = 0);
    return result;
}

/* script/interpreter/flag/test.c */
s8 GameFlag_IsSet(s32);

s32 Script_TestFlag(struct ScriptInterpreter *interpreter)
{
    interpreter->condition_result =
        GameFlag_IsSet(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/flag/set_test.c */
s8 GameFlag_IsSet(s32);
s32 GameFlag_Set(s32);

s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_IsSet(value);
    GameFlag_Set(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/flag/clear_test.c */
s8 GameFlag_IsSet(s32);
s32 GameFlag_Clear(s32);

s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;

    value = interpreter->script[interpreter->cursor + 1];
    interpreter->condition_result = GameFlag_IsSet(value);
    GameFlag_Clear(value);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/flag/toggle_test.c */
s32 GameFlag_IsSet(s32 arg0);
void GameFlag_Set(s32 arg0);
void GameFlag_Clear(s32 arg0);

s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter)
{
    s32 value;
    s32 result;

    value = interpreter->script[interpreter->cursor + 1];
    result = GameFlag_IsSet(value);
    interpreter->condition_result = result;
    if (((u32)result << 0x18) == 0x01000000) {
        GameFlag_Clear(value);
    } else {
        GameFlag_Set(value);
    }
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/cmd/object_arg.c */
s32 Script_ApplyObjectArgument(struct ScriptInterpreter *interpreter)
{
    ObjectDispatch_ApplyArgumentToChildren(interpreter,
        interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}
