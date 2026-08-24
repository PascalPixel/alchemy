#include "object_commands.h"
#include "script_object_runtime.h"

s32 Func_0800da40(struct ScriptObjectRuntime *object)
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
