#include "object_commands.h"
#include "script_object_runtime.h"

#define Script_ApplyLinkedObjectPosition Func_0800daa0

s32 Script_ApplyLinkedObjectPosition(struct ScriptObjectRuntime *object)
{
    struct ScriptObjectRuntime *target;

    target = object->linked_object;
    Func_0800d14c(object, target->x, target->y, target->z);
    object->script_cursor++;
    return 1;
}
