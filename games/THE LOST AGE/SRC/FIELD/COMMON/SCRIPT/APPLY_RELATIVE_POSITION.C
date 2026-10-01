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
    s32 step;
    s32 done;

    Object_SetMoveTarget(object, object->x + first,
        object->y + second, object->z + third);
    step = (u16)object->script_cursor;
    done = 1;
    asm volatile("" : "+l"(done)); /* FAKEMATCH: ⚓️ sets the result between the cursor load and store */
    object->script_cursor = step + 4;
    return done;
}
