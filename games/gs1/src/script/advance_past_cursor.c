#include "scene.h"
#include "script_interpreter.h"
#include "script_operands.h"
#include "types.h"
#include "object_runtime.h"
#include "object_commands.h"

/* script/advance_past_cursor.c */
/* script/interpreter/cmd/advance.c */
s32 Script_AdvancePastCursor(struct ScriptInterpreter *interpreter)
{
    s32 zero;

    interpreter->script = interpreter->script + interpreter->cursor + 1;
    zero = 0;
    interpreter->cursor = zero;
    return 1;
}

/* script/operands/byte/clear_byte54.c */
s32 Script_ClearByte54(struct ScriptOperands *work)
{
    work->byte_54 = 0;
    work->cursor++;
    return 1;
}

/* script/operands/byte/set_byte54.c */
s32 Script_SetByte54(struct ScriptOperands *work)
{
    work->byte_54 = 1;
    work->cursor++;
    return 1;
}

/* script/interpreter/wait/timeout.c */
void WaitFrames(s32);

void Script_WaitForEventTimeout(s32 arg0)
{
    s32 cnt;

    cnt = 0;
    while (cnt <= 0x257 && Script_Check(arg0) == 0) {
        WaitFrames(1);
        cnt++;
    }
}

/* object/motion/is_target_unset.c */
/* object/motion/aim/is_target_unset.c */
s32 Object_IsTargetUnset(struct ObjectRuntime *object)
{
    s32 first;
    s32 second;

    if (object->flags == 0) {
        second = object->target_x;
        if (second == 0x80000000) {
            first = object->target_y;
            goto block_4;
        }
        goto block_6;
    }
    first = object->target_x;
    second = 0x80000000;
block_4:
    if ((first != second) || (object->target_z != first)) {
block_6:
        return 0;
    }
    return 1;
}

/* object/motion/pos/set_position_and_reset_motion.c */
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    object->z = z;
    object->target_x = 0x80000000;
    object->target_y = 0x80000000;
    object->target_z = 0x80000000;
    object->x = x;
    object->y = y;
    object->velocity_x = 0;
    object->velocity_y = 0;
    object->velocity_z = 0;
}

/* script/interpreter/lookup.c */
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
s32 GameFlag_IsSet(s32);

s32 Script_TestFlag(struct ScriptInterpreter *interpreter)
{
    interpreter->condition_result =
        GameFlag_IsSet(interpreter->script[interpreter->cursor + 1]);
    interpreter->cursor = (u16)interpreter->cursor + 2;
    return 1;
}

/* script/interpreter/flag/set_test.c */
s32 GameFlag_IsSet(s32);
void GameFlag_Set(s32);

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
s32 GameFlag_IsSet(s32);
void GameFlag_Clear(s32);

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
