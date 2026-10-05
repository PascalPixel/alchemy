#include "SCRIPT_OBJECT_RUNTIME.H"

void Object_SetMoveTarget(void *, s32, s32, s32);

/* Moves the object by the three offsets the command carries. */
s32 Script_ApplyRelativePosition(struct ScriptObjectRuntime *object)
{
    u8 *entry = (u8 *)(object->script + (s16)object->script_cursor);
    s32 *cursor = (s32 *)(entry + 4);
    s32 first = *cursor++;
    s32 second = *cursor++;
    s32 third = *cursor;

    Object_SetMoveTarget(object, object->x + first,
        object->y + second, object->z + third);
    object->script_cursor += 4;
    return 1;
}
