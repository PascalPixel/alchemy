#include "script_object_runtime.h"

void Object_SetMoveTarget(void *, s32, s32, s32);

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Object_SetMoveTarget(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}
