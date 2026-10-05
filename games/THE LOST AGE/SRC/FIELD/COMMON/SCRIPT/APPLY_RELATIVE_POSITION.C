#include "SCRIPT_OBJECT_RUNTIME.H"

void Object_SetMoveTarget(void *, s32, s32, s32);

/* Moves the object by the three offsets the command carries. */
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
