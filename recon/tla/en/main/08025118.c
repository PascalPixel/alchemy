/*
 * Draft: Script_ApplyLinkedObjectPosition does not yet match; 3 halfwords differ from ☀️'s C, first at +0x12 (movs r0, #1).
 * Links as recon/tla/raw/08025118.s.
 */
#include "SCRIPT_OBJECT_RUNTIME.H"

void Object_SetMoveTarget(void *, s32, s32, s32);

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Object_SetMoveTarget(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}
