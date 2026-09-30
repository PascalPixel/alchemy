/*
 * Draft: Script_StoreAngleToLinkedObject does not yet match; 3 halfwords differ from ☀️'s C, first at +0x18 (strh r0, [r5, #6]).
 * Links as recon/tla/raw/080250f4.s.
 */
#include "SCRIPT_OBJECT_RUNTIME.H"

u16 ArcTan2(s32, s32);

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
