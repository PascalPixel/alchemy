#include "script_object_runtime.h"

void Object_SetMoveTarget(void *, s32, s32, s32);
void Object_SetPositionAndResetMotion(void *, s32, s32, s32);
s32 Func_0802d45c(s32 terrain_id, s32 x, s32 z);

s32 Func_08024fe0(struct ScriptObjectRuntime *object)
{
    const s32 *argument = object->script + (s16)object->script_cursor + 1;
    s32 x = *argument++;
    s32 y = *argument++;
    s32 z = *argument;
    s32 height;

    Object_SetPositionAndResetMotion(object, x, y, z);
    if (y == -1 && (object->flags & 1)) {
        height = Func_0802d45c(object->terrain_id, object->x, object->z);
        object->y = object->y - object->terrain_height + height;
        object->terrain_height = height;
    }
    object->script_cursor += 4;
    return 1;
}

s32 Func_08025038(struct ScriptObjectRuntime *object)
{
    s32 height;

    Object_SetPositionAndResetMotion(object, 0, object->y, 0);
    if (object->flags & 1) {
        height = Func_0802d45c(object->terrain_id, object->x, object->z);
        object->y = object->y - object->terrain_height + height;
        object->terrain_height = height;
    }
    object->script_cursor += 1;
    return 1;
}

s32 Func_08025078(struct ScriptObjectRuntime *object)
{
    const s32 *argument = object->script + (s16)object->script_cursor + 1;
    s32 x = *argument++;
    s32 y = *argument++;
    s32 z = *argument;

    Object_SetMoveTarget(object, x, y, z);
    object->script_cursor += 4;
    return 1;
}

s32 Func_0802509c(struct ScriptObjectRuntime *object)
{
    const s32 *argument = object->script + (s16)object->script_cursor + 1;
    s32 x = *argument++;
    s32 z = *argument;

    Object_SetMoveTarget(object, x, object->y, z);
    object->script_cursor += 3;
    return 1;
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    const s32 *cursor = object->script + (s16)object->script_cursor + 1;
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Object_SetMoveTarget(object, object->x + first,
        object->y + second, object->z + third);
    object->script_cursor += 4;
    return 1;
}
