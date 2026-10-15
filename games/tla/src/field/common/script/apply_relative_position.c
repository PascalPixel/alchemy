#include "script_object_runtime.h"

void Object_SetMoveTarget(void *, s32, s32, s32);
void Object_SetPositionAndResetMotion(void *, s32, s32, s32);
s32  Func_0802d45c(s32 terrainId, s32 x, s32 z);

s32 Func_08024fe0(struct ScriptObjectRuntime *object)
{
    const s32 *argument = object->script + (s16)object->scriptCursor + 1;
    s32        x = *argument++;
    s32        y = *argument++;
    s32        z = *argument;
    s32        height;

    Object_SetPositionAndResetMotion(object, x, y, z);
    if (y == -1 && (object->flags & 1))
    {
        height = Func_0802d45c(object->terrainId, object->x, object->z);
        object->y = object->y - object->terrainHeight + height;
        object->terrainHeight = height;
    }
    object->scriptCursor += 4;
    return 1;
}

s32 Func_08025038(struct ScriptObjectRuntime *object)
{
    s32 height;

    Object_SetPositionAndResetMotion(object, 0, object->y, 0);
    if (object->flags & 1)
    {
        height = Func_0802d45c(object->terrainId, object->x, object->z);
        object->y = object->y - object->terrainHeight + height;
        object->terrainHeight = height;
    }
    object->scriptCursor += 1;
    return 1;
}

s32 Func_08025078(struct ScriptObjectRuntime *object)
{
    const s32 *argument = object->script + (s16)object->scriptCursor + 1;
    s32        x = *argument++;
    s32        y = *argument++;
    s32        z = *argument;

    Object_SetMoveTarget(object, x, y, z);
    object->scriptCursor += 4;
    return 1;
}

s32 Func_0802509c(struct ScriptObjectRuntime *object)
{
    const s32 *argument = object->script + (s16)object->scriptCursor + 1;
    s32        x = *argument++;
    s32        z = *argument;

    Object_SetMoveTarget(object, x, object->y, z);
    object->scriptCursor += 3;
    return 1;
}

s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    const s32 *cursor = object->script + (s16)object->scriptCursor + 1;
    s32        first = *cursor++;
    s32        second = *cursor++;
    s32        third = *cursor;

    Object_SetMoveTarget(object, object->x + first, object->y + second, object->z + third);
    object->scriptCursor += 4;
    return 1;
}

u16 ArcTan2(s32, s32);

s32 Script_StoreAngleToLinkedObject(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linkedObject;
    object->scriptValue =
        ArcTan2((s32)((u32)target->z - (u32)object->z), (s32)((u32)target->x - (u32)object->x));
    object->scriptCursor++;
    return 1;
}

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linkedObject;
    Object_SetMoveTarget(object, target->x, target->y, target->z);
    object->scriptCursor++;
    return 1;
}
