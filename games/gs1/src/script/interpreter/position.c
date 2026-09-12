#include "script_interpreter.h"
#include "object_commands.h"
#include "script_object_runtime.h"

void Func_0800d14c(void *, s32, s32, s32);
u16 ArcTan2(s32, s32);

void Func_0800d130(struct ScriptInterpreter *, s32, s32, s32);

s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter)
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
    Func_0800d130(interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

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
    Func_0800d14c(interpreter, first, second, third);
    interpreter->cursor = (u16)interpreter->cursor + 4;
    return 1;
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    u8 *entry = (u8 *)(object->script + (s16)object->script_cursor);
    s32 *cursor = (s32 *)(entry + 4);
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Func_0800d14c(object, object->x + first,
        object->y + second, object->z + third);
    object->script_cursor += 4;
    return 1;
}

s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    object->script_value = ArcTan2(
        (s32)((u32)target->z - (u32)object->z),
        (s32)((u32)target->x - (u32)object->x));
    object->script_cursor++;
    return 1;
}

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Func_0800d14c(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}
