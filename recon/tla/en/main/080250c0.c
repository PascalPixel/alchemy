/*
 * Draft: Script_ApplyRelativePosition does not yet match; 3 halfwords differ from ☀️'s C, first at +0x2a (movs r0, #1).
 * Links as recon/tla/raw/08025038.s.
 */
#include "SCRIPT_OBJECT_RUNTIME.H"

void Object_SetMoveTarget(void *, s32, s32, s32);

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
